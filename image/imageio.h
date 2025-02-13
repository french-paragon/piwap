/*Piwap, or Picture Warping App, is a GUI to batch process images.

Copyright (C) 2019-2024  Paragon<french.paragon@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef IMAGEIO_H
#define IMAGEIO_H

#include "./image.h"

class QObject;

namespace Piwap {

class ImageInfos;

struct ImageWithInfos {
	Image* image;
	ImageInfos* infos;
};

ImageWithInfos openImage(const char* fileName, QObject *imgParent = nullptr, QObject *infoParent = nullptr);
int reloadImage(Image::ImageData & imageDest, ImageInfos* infos);

bool writeImage(const char* fileName,
				Image* image,
				ImageInfos* infos,
				bool saveExif = true,
				bool saveIptc = true,
				bool saveXmp = true);

}// namespace Piwap

#endif // IMAGEIO_H
