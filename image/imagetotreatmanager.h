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

#ifndef IMAGETOTREATMANAGER_H
#define IMAGETOTREATMANAGER_H

#include <QAbstractListModel>

namespace Piwap {

class ImageToTreatManager : public QAbstractListModel
{
	Q_OBJECT
public:
	explicit ImageToTreatManager(QObject *parent = nullptr);

	virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
	virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

Q_SIGNALS:

public Q_SLOTS:

	void removeImage (int row);
	void insertImage (QString url);

protected:

	QList<QString> _imagesUrls;

private:

	static const int registrationCodes;
};

} // namespace Piwap

#endif // IMAGETOTREATMANAGER_H
