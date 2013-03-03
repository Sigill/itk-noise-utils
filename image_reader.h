#ifndef IMAGE_READER_H
#define IMAGE_READER_H

#include <stdexcept>

#include "common.h"

class ImageReadingException : public std::runtime_error
{
public:
  ImageReadingException ( const std::string &err ) : std::runtime_error (err) {}
};


class ImageReader
{
public:
  /**
   * Load an image either as a single file or as a serie of files.
   * @param[in] filename The file to load of the folder containing the files. Must exists.
   */
  static ImageType::Pointer read(const std::string filename);

private:
  /**
   * Load an image as a single file.
   * @param[in] filename The file to load. Must exists.
   */
  static ImageType::Pointer readImage(const std::string filename);

  /**
   * Load an image as a serie of files.
   * @param[in] filename The folder containing the files. Must be a directory.
   */
  static ImageType::Pointer readImageSerie(const std::string filename);

};

#endif /* IMAGE_READER_H */
