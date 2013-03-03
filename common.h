#ifndef COMMON_H
#define COMMON_H

#include "itkImage.h"

#define __ImageDimension 3

typedef itk::Image< unsigned char, __ImageDimension > ImageType;

#endif /* COMMON_H */
