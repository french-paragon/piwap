/*Piwap, or Picture Warping App, is a GUI to batch process images.

Copyright (C) 2019  Paragon<french.paragon@gmail.com>

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

#include "imageio.h"
#include "imageinfos.h"

#include <Magick++/Image.h>

namespace Piwap {

ImageInfos* openImage(const char *fileName, Magick::Image & imageDest, QObject *infoParent) {

	imageDest.read(std::string(fileName));

	if (!imageDest.isValid()) {
		return nullptr;
	}

	ImageInfos* infos = new ImageInfos(QString(fileName), infoParent);

	return infos;

}

int reloadImage(Magick::Image & imageDest, ImageInfos* infos) {

	imageDest.read(infos->originalFilePath().toStdString());

	if (imageDest.isValid()) {
		return 0;
	}

	return 1;

}


}// namespace Piwap
