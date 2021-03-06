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

#include "operationfactorymanager.h"

#include "abstractoperationfactory.h"

#include <QIcon>

namespace Piwap {

int OperationFactoryManager::registrationCodes = qRegisterMetaType<Piwap::OperationFactoryManager*>("Piwap::OperationFactoryManager*");

OperationFactoryManager::OperationFactoryManager(QObject *parent) : QAbstractListModel(parent)
{

}

int OperationFactoryManager::rowCount(const QModelIndex &parent) const {

	Q_UNUSED(parent);
	return _factories.size();

}

QVariant OperationFactoryManager::data(const QModelIndex &index, int role) const {

	switch (role) {
	case Qt::DisplayRole :
		return _factories.at(index.row())->getOpName();
	case Qt::ToolTipRole :
		return _factories.at(index.row())->getToolTip();
	case IconPathRole :
		return _factories.at(index.row())->getIconUrl();
	case Qt::DecorationRole :
		return QIcon(_factories.at(index.row())->getIconUrl());
	case TypeIdRole:
		return _factories.at(index.row())->getOpTypeId();
	default:
		return QVariant();
	}

}

QHash<int, QByteArray> OperationFactoryManager::roleNames() const {

	QHash<int, QByteArray> ret = QAbstractListModel::roleNames();

	ret.insert(TypeIdRole, "typeId");
	ret.insert(IconPathRole, "iconPath");

	return ret;

}

bool OperationFactoryManager::insertFactory(AbstractOperationFactory* f) {

	if (hasFactoryInstalled(f->getOpTypeId())) {
		return false;
	}

	beginInsertRows(QModelIndex(), _factories.size(), _factories.size());
	_factories.push_back(f);
	endInsertRows();

	return true;

}

bool OperationFactoryManager::removeFactory(QString typeId) {

	int index_to_remove = factoryRow(typeId);

	if (index_to_remove == -1) {
		return false;
	}

	beginRemoveRows(QModelIndex(), index_to_remove, index_to_remove);
	_factories.removeAt(index_to_remove);
	endRemoveRows();

	return true;

}

bool OperationFactoryManager::hasFactoryInstalled(QString typeId) const {

	return factoryRow(typeId) >= 0;

}
int OperationFactoryManager::factoryRow(QString typeId) const {

	for (int i = 0; i < _factories.size(); i++) {
		if (_factories.at(i)->getOpTypeId() == typeId) {
			return i;
		}
	}

	return -1;

}

AbstractOperationFactory* OperationFactoryManager::factoryByType(QString typeId) const {

	int row = factoryRow(typeId);

	if (row < 0) {
		return nullptr;
	}

	return _factories.at(row);

}

QString OperationFactoryManager::typeIdByRow(int row) const{
	return _factories.at(row)->getOpTypeId();
}

} // namespace Piwap
