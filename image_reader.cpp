#include "image_reader.h"

#include "itkImageFileReader.h"
#include "itkImageSeriesReader.h"

#include <ostream>

#include <boost/filesystem.hpp>
#include <boost/regex.hpp>

#include "log4cxx/logger.h"

typedef itk::ImageFileReader< ImageType > ITKImageReader;
typedef itk::ImageSeriesReader< ImageType > ITKImageSeriesReader;

ImageType::Pointer ImageReader::read(const std::string filename)
{
	log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("main"));

	LOG4CXX_INFO(logger, "Reading image \"" << filename << "\"");

	try
	{
		boost::filesystem::path path(filename);

		if(boost::filesystem::exists(path)) {
			ImageType::Pointer img;

			if(boost::filesystem::is_directory(path))
			{
				LOG4CXX_DEBUG(logger, path << " is a folder");

				img = readImageSerie(filename);
			} else {
				LOG4CXX_DEBUG(logger, path << " is a file");

				img = readImage(filename);
			}

			LOG4CXX_INFO(logger, "Image " << path << " loaded");

			return img;
		} else {
			std::stringstream err;
			err << "\"" << filename << "\" does not exists";

			LOG4CXX_FATAL(logger, err.str());

			throw ImageReadingException(err.str());
		}
	} catch(boost::filesystem::filesystem_error &ex) {
		std::stringstream err;
		err << filename << " cannot be read (" << ex.what() << ")" << std::endl;
		throw ImageReadingException(err.str());
	}
}

ImageType::Pointer ImageReader::readImage(const std::string filename)
{
	typename ITKImageReader::Pointer reader = ITKImageReader::New();

	reader->SetFileName(filename);

	try {
		reader->Update();
	}
	catch( itk::ExceptionObject &ex )
	{
		std::stringstream err;
		err << "ITK is unable to read the image \"" << filename << "\" (" << ex.what() << ")";

		throw ImageReadingException(err.str());
	}

	return reader->GetOutput();
}

ImageType::Pointer ImageReader::readImageSerie(const std::string filename)
{
	typename ITKImageSeriesReader::Pointer reader = ITKImageSeriesReader::New();

	typename ITKImageSeriesReader::FileNamesContainer filenames;

	log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("main"));

	try
	{
		boost::filesystem::path path(filename);
		boost::filesystem::directory_iterator end_iter;
		boost::regex pattern(".*\\.((?:png)|(?:bmp)|(?:jpe?g))", boost::regex::icase);

		for( boost::filesystem::directory_iterator dir_iter(path) ; dir_iter != end_iter ; ++dir_iter)
		{
			boost::smatch match;
			if( !boost::regex_match( dir_iter->path().filename().string(), match, pattern ) ) continue;

			LOG4CXX_DEBUG(logger, "Reading \"" << boost::filesystem::absolute(dir_iter->path()).string() << "\"");

			filenames.push_back(boost::filesystem::absolute(dir_iter->path()).string());
		}
	}
	catch(boost::filesystem::filesystem_error &ex) {
		std::stringstream err;
		err << filename << " cannot be read (" << ex.what() << ")" << std::endl;

		throw ImageReadingException(err.str());
	}

	std::sort(filenames.begin(), filenames.end());

	reader->SetFileNames(filenames);

	try {
		reader->Update();
	}
	catch( itk::ExceptionObject &ex )
	{
		std::stringstream err;
		err << "ITK is unable to read the image serie located in \"" << filename << "\" (" << ex.what() << ")";

		throw ImageReadingException(err.str());
	}

	return reader->GetOutput();
}

