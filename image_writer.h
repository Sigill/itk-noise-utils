#ifndef IMAGE_WRITER_H
#define IMAGE_WRITER_H

#include <stdexcept>

#include "common.h"

class ImageWritingException : public std::runtime_error
{
public:
	ImageWritingException ( const std::string &err ) : std::runtime_error (err) {}
};


class ImageWriter
{
public:
	/**
	 * Write an image either as a single file or as a serie of files.
	 * @param[in] image The image to write.
	 * @param[in] filename The file or folder in which to write the image.
	 */
	static void write(const ImageType::Pointer image, const std::string filename);

private:
	/**
	 * Write an image as a single file.
	 * @param[in] image The image to write.
	 * @param[in] filename The file in which to write the image.
	 */
	static void writeImage(const ImageType::Pointer image, const std::string filename);

	/**
	 * Write an image as a serie of files.
	 * @param[in] image The image to write.
	 * @param[in] filename The folder or file with placeholder in which to write the image.
	 */
	static void writeImageSerie(const ImageType::Pointer image, const std::string filename);

};

#endif /* IMAGE_WRITER_H */
