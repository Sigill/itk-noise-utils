#ifndef _CLI_OPTIONS_H
#define _CLI_OPTIONS_H

#include <boost/program_options.hpp>
#include <vector>
#include <iostream>
#include <stdexcept>

#include <boost/regex.hpp>

namespace po = boost::program_options;

template <typename TNumericType>
class NumericTypeWrapper
{
public :
	typedef TNumericType NumericType;

	NumericTypeWrapper() {}
	NumericTypeWrapper(const NumericType v) : value(v) {}

	NumericType value;

	inline operator NumericType() const { return value; }
};

class Double : public NumericTypeWrapper<double> {
public:
	Double() : NumericTypeWrapper< double >() {}
	Double(const double v) : NumericTypeWrapper< double >(v) {}
};

class StrictlyPositiveDouble : public NumericTypeWrapper<double> {
public:
	StrictlyPositiveDouble() : NumericTypeWrapper< double >() {}
	StrictlyPositiveDouble(const double v) : NumericTypeWrapper< double >(v) {}
};

class CliException : public std::runtime_error
{
  public:
      CliException ( const std::string &err ) : std::runtime_error (err) {}
};

class CliParser
{
public:
	enum ParseResult {
		CONTINUE = 0,
		EXIT,
	};

	CliParser();
	ParseResult parse_argv(int argc, char ** argv);

	const std::string get_input_image() const;
	const std::string get_output_image() const;
	const std::string get_noise_type() const;
	const double      get_stddev() const;
	const double      get_amplitude() const;
	const double      get_probability() const;

private:
	std::string            input_image, output_image;
	std::string            noise_type;
	StrictlyPositiveDouble stddev;
	StrictlyPositiveDouble amplitude;
	StrictlyPositiveDouble probability;
};

#endif /* _CLI_OPTIONS_H */
