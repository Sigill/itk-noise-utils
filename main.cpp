#include "common.h"
#include "time_utils.h"

#include "image_reader.h"
#include "image_writer.h"

#include "cli_parser.h"

#include "log4cxx/logger.h"
#include "log4cxx/consoleappender.h"
#include "log4cxx/patternlayout.h"
#include "log4cxx/basicconfigurator.h"

#include "itkAdditiveGaussianNoiseImageFilter.h"
#include "itkSparseAdditiveGaussianNoiseImageFilter.h"
#include "itkImpulseNoiseImageFilter.h"

typedef itk::AdditiveGaussianNoiseImageFilter< ImageType, ImageType > GaussianNoiseGenerator;
typedef itk::SparseAdditiveGaussianNoiseImageFilter< ImageType, ImageType > SparseGaussianNoiseGenerator;
typedef itk::ImpulseNoiseImageFilter< ImageType, ImageType > ImpulseNoiseGenerator;

int main(int argc, char **argv)
{
	log4cxx::BasicConfigurator::configure(
			log4cxx::AppenderPtr(new log4cxx::ConsoleAppender(
					log4cxx::LayoutPtr(new log4cxx::PatternLayout("\%-5p - [%c] - \%m\%n")),
					log4cxx::ConsoleAppender::getSystemErr()
					)
				)
			);

	log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("main"));

	CliParser cli_parser;
	try {
		if(cli_parser.parse_argv(argc, argv) != CliParser::CONTINUE)
			exit(0);
	} catch (CliException &err) {
		LOG4CXX_FATAL(logger, err.what());
		return -1;
	}

	ImageType::Pointer input_image;

	ImageType::Pointer image;
	try {
		image = ImageReader::read(cli_parser.get_input_image());
	} catch (ImageReadingException & ex) {
		LOG4CXX_FATAL(logger, "ITK is unable to read the image \"" << cli_parser.get_input_image() << "\" (" << ex.what() << ")");
		exit(-1);
	}

	const std::string noise_type = cli_parser.get_noise_type();

	typedef itk::ImageToImageFilter< ImageType, ImageType >::Pointer FilterPointer;
	FilterPointer noiseFilter;

	if(0 == noise_type.compare("awg")) {
		GaussianNoiseGenerator::Pointer ng = GaussianNoiseGenerator::New();
		ng->SetInput(image);
		ng->SetMean(cli_parser.get_mean());
		ng->SetStandardDeviation(cli_parser.get_stddev());
		noiseFilter = FilterPointer(ng);
	} else if(0 == noise_type.compare("sawg")) {
		SparseGaussianNoiseGenerator::Pointer ng = SparseGaussianNoiseGenerator::New();
		ng->SetInput(image);
		ng->SetProbability(cli_parser.get_probability());
		ng->SetMean(cli_parser.get_mean());
		ng->SetStandardDeviation(cli_parser.get_stddev());
		noiseFilter = FilterPointer(ng);
	} else if(0 == noise_type.compare("impulse")) {
		ImpulseNoiseGenerator::Pointer ng = ImpulseNoiseGenerator::New();
		ng->SetInput(image);
		ng->SetProbability(cli_parser.get_probability());
		noiseFilter = FilterPointer(ng);
	} else {
		LOG4CXX_FATAL(logger, "No noise called " << noise_type << " found.");
	}

	if(noiseFilter.IsNotNull()) {
		noiseFilter->Update();

		LOG4CXX_DEBUG(logger, "Noise generated");

		ImageWriter::write(noiseFilter->GetOutput(), cli_parser.get_output_image());
	}

	return 0;
}
