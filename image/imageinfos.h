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

#ifndef IMAGEINFOS_H
#define IMAGEINFOS_H

#include <QObject>
#include <QFileInfo>

#include <exiv2/exiv2.hpp>

namespace Piwap {

class ImageInfos : public QObject
{
	Q_OBJECT
public:
	explicit ImageInfos(QString originalFileUrl, QObject *parent = nullptr);

	QString originalFileName() const;
	QString originalFilePath() const;

	QString originalFolderUrl() const;

	QString originalImageType() const;

	void copyInfosFromOther(ImageInfos const& other);

	Exiv2::Image* metadataobject() const;

Q_SIGNALS:

public Q_SLOTS:

protected:

	QFileInfo _infos;
	Exiv2::Image::AutoPtr _image;

};

} // namespace Piwap

#endif // IMAGEINFOS_H
