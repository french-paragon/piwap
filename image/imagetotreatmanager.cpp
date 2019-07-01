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
