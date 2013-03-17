#include "cli_parser.h"

#include <iostream>
#include <sstream>
#include <vector>

#include "ParseUtils.h"

#include "log4cxx/logger.h"

namespace po = boost::program_options;

void validate(boost::any& v, const std::vector<std::string>& values, StrictlyPositiveDouble*, int)
{
	po::validators::check_first_occurrence(v);
	const std::string& s = po::validators::get_single_string(values);

	double value;
	if( ParseUtils::ParseDouble(value, s.data()) && value > 0 )
	{
		v = boost::any(StrictlyPositiveDouble(value));
	} else {
		throw po::invalid_option_value(s);
	}
}

void validate(boost::any& v, const std::vector<std::string>& values, Double*, int)
{
	po::validators::check_first_occurrence(v);
	const std::string& s = po::validators::get_single_string(values);

	double value;
	if( ParseUtils::ParseDouble(value, s.data()) )
	{
		v = boost::any(Double(value));
	} else {
		throw po::invalid_option_value(s);
	}
}

template< typename TElemType >
std::ostream &operator<<(std::ostream &s, const std::vector< TElemType >& v)
{
	std::stringstream ss;
	if( !v.empty() )
	{

		typename std::vector< TElemType >::const_iterator it = v.begin(), begin = v.begin(), end = v.end();
		for( ; it < end; ++it )
		{
			if(it != begin)
				ss << " ";
			ss << *it;
		}
	}

	return s << ss.str();
}

CliParser::CliParser()
{}

CliParser::ParseResult CliParser::parse_argv(int argc, char ** argv)
{
	log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("main"));

	LOG4CXX_INFO(logger, "Parsing command line options");

	po::options_description desc("Command line parameters");
	desc.add_options()
		("help,h",
			"Produce help message.")
		("input-image,i",
			po::value< std::string >(&(this->input_image))->required(),
			"Input image.")
		("output-image,o",
			po::value< std::string >(&(this->output_image))->required(),
			"Output image.")
		("noise-type,n",
			po::value< std::string >(&(this->noise_type))->required(),
			"Noise type (gaussian, sparse-gaussian, uniform, sparse-uniform, impulse).")
		("mean,m",
			po::value< Double >(&(this->mean))->default_value(0.0),
			"Mean value of the generated noise.")
		("stddev,s",
			po::value< StrictlyPositiveDouble >(&(this->stddev))->default_value(32),
			"Standard deviation of the generated noise (for gaussian noise).")
		("amplitude,a",
			po::value< StrictlyPositiveDouble >(&(this->amplitude))->default_value(32),
			"Amplitude of the generated noise (for uniform noise).")
		("probability,p",
			po::value< StrictlyPositiveDouble >(&(this->probability))->default_value(0.01),
			"Probability of the generated noise.")
		;

	po::variables_map vm;

	try {
		po::store(po::command_line_parser(argc, argv).options(desc).run(), vm);

		// Handling --help before notify() in order to allow ->required()
		// http://stackoverflow.com/questions/5395503/required-and-optional-arguments-using-boost-library-program-options#answer-5517755
		if (vm.count("help")) {
			std::cout << "Usage: " << argv[0] << " [options]" << std::endl;
			std::cout << desc;
			return EXIT;
		}

		po::notify(vm);
	} catch(po::error &err) {
		throw CliException(err.what());
	}

	return CONTINUE;
}

const std::string CliParser::get_input_image() const
{
	return this->input_image;
}

const std::string CliParser::get_output_image() const
{
	return this->output_image;
}

const std::string CliParser::get_noise_type() const
{
	return this->noise_type;
}

const double CliParser::get_mean() const {
	return this->mean;
}

const double CliParser::get_stddev() const {
	return this->stddev;
}

const double CliParser::get_amplitude() const {
	return this->amplitude;
}

const double CliParser::get_probability() const {
	return this->probability;
}
