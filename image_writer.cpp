#include "image_writer.h"

#include <itkImageFileWriter.h>
#include <itkImageSeriesWriter.h>
#include <itkNumericSeriesFileNames.h>

#include <ostream>

#include <boost/filesystem.hpp>
#include <boost/regex.hpp>

#include "log4cxx/logger.h"

typedef itk::ImageFileWriter< ImageType> ITKImageWriter;
typedef itk::ImageSeriesWriter< ImageType, itk::Image< unsigned char, 2 > > ITKImageSeriesWriter;

void ImageWriter::write(const ImageType::Pointer image, const std::string filename)
{
	log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("main"));

	try
	{
		if(std::string::npos == filename.find('%')) {
			LOG4CXX_DEBUG(logger, "Writing image in \"" << filename << "\" as a single file");
			writeImage(image, filename);
		} else {
			LOG4CXX_DEBUG(logger, "Writing image in \"" << filename << "\" as a serie");
			writeImageSerie(image, filename);
		}
	} catch(boost::filesystem::filesystem_error &ex) {
		std::stringstream err;
		err << "Cannot write image in " << filename << " (" << ex.what() << ")" << std::endl;
		throw ImageWritingException(err.str());
	}
}

void ImageWriter::writeImage(const ImageType::Pointer image, const std::string filename)
{
	typename ITKImageWriter::Pointer writer = ITKImageWriter::New();

	writer->SetInput(image);
	writer->SetFileName(filename);

	try {
		writer->Update();
	}
	catch( itk::ExceptionObject &ex )
	{
		throw ImageWritingException(ex.what());
	}
}

void ImageWriter::writeImageSerie(const ImageType::Pointer image, const std::string filename)
{
	typename ITKImageSeriesWriter::Pointer writer = ITKImageSeriesWriter::New();

	try
	{
		ImageType::SizeType imageSize = image->GetLargestPossibleRegion().GetSize();

		itk::NumericSeriesFileNames::Pointer outputNames = itk::NumericSeriesFileNames::New();
		outputNames->SetSeriesFormat(filename);
		outputNames->SetStartIndex(0);
		outputNames->SetEndIndex(imageSize[2] - 1);

		writer->SetInput(image);
		writer->SetFileNames(outputNames->GetFileNames());
		writer->Update();
	}
	catch(boost::filesystem::filesystem_error &ex) {
		throw ImageWritingException(ex.what());
	}
}

