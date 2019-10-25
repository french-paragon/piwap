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

#include "imagetotreatmanager.h"

#include <QDebug>

namespace Piwap {

const int ImageToTreatManager::registrationCodes = qRegisterMetaType<Piwap::ImageToTreatManager*>("Piwap::ImageToTreatManager*");

ImageToTreatManager::ImageToTreatManager(QObject *parent) : QAbstractListModel(parent)
{

}

int ImageToTreatManager::rowCount(const QModelIndex &parent) const {
	Q_UNUSED(parent)

	return _imagesUrls.size();
}

QVariant ImageToTreatManager::data(const QModelIndex &index, int role) const {

	switch (role) {
	case Qt::DisplayRole:
		return _imagesUrls.at(index.row());
	default:
		break;
	}

	return QVariant();

}

void ImageToTreatManager::removeImage (int row){

	if (row >= 0 && row < _imagesUrls.size()) {
		beginRemoveRows(QModelIndex(), row, row);
		_imagesUrls.removeAt(row);
		endRemoveRows();
	}

}
void ImageToTreatManager::insertImage (QString url) {

	beginInsertRows(QModelIndex(), _imagesUrls.size(), _imagesUrls.size());
	_imagesUrls.push_back(url);
	endInsertRows();

}

} // namespace Piwap
