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

#include "imageinfos.h"

#include <QFileInfo>
#include <QDir>

namespace Piwap {

ImageInfos::ImageInfos(QString originalFileUrl, QObject *parent) :
	QObject(parent),
	_infos(originalFileUrl)
{

}

QString ImageInfos::originalFileName() const
{
	return _infos.baseName();
}
QString ImageInfos::originalFilePath() const {
	return _infos.absoluteFilePath();
}

QString ImageInfos::originalFolderUrl() const
{
	return _infos.dir().path();
}

QString ImageInfos::originalImageType() const
{
	return _infos.completeSuffix();
}

} // namespace Piwap
