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

#include <sys/stat.h>
#include <stdio.h>
#include <zip.h>

namespace Piwap {

int openKraOraFile(const char* fileName, Magick::Image & imageDest) {

	struct zip *f_zip = nullptr;
	int err = 0;

	f_zip = zip_open(fileName, ZIP_RDONLY, &err);

	if(err != ZIP_ER_OK or f_zip == nullptr) {
		return -1;
	}

	zip_int64_t visu = zip_name_locate(f_zip,"mergedimage.png",0);
	if (visu==-1) {
		return -1;
	}

	struct zip_stat file_stat;
	struct zip_file *file_zip=nullptr;
	zip_stat_index(f_zip, static_cast<zip_uint64_t>(visu), 0, &file_stat);

	if(zip_stat(f_zip, file_stat.name, 0, &file_stat) == -1) {
		return -1;
	}

	file_zip=zip_fopen(f_zip, file_stat.name, ZIP_FL_UNCHANGED);

	if(!file_zip) {
		return -1;
	}

	char *str = nullptr;
	try {
		str = new char[static_cast<size_t>(file_stat.size+1)];
		memset(str, 0, static_cast<size_t>(file_stat.size+1));
	} catch(std::bad_alloc& ba){
		Q_UNUSED(ba);
		return -1;
	}

	/* on lit le fichier archivé */
	if(zip_fread(file_zip, str, static_cast<size_t>(file_stat.size)) != static_cast<zip_int64_t>(file_stat.size))
	{
		return -1;
	}

	Magick::Blob bob(str, static_cast<size_t>(file_stat.size+1));
	imageDest.read(bob);

	return 0;
}

ImageInfos* openImage(const char *fileName, Magick::Image & imageDest, QObject *infoParent) {

	QString fName(fileName);

	if (fName.endsWith(".kra", Qt::CaseInsensitive) || fName.endsWith(".ora", Qt::CaseInsensitive)) {

		if (openKraOraFile(fileName, imageDest) != 0) {
			return nullptr;
		}

	} else {
		imageDest.read(std::string(fileName));
	}

	if (!imageDest.isValid()) {
		return nullptr;
	}

	ImageInfos* infos = new ImageInfos(fName, infoParent);

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
