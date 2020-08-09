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

#include "operationlistmanager.h"

#include "abstractimageoperation.h"

#include <QIcon>

namespace Piwap {

const int OperationListManager::registrationCode = qRegisterMetaType<Piwap::OperationListManager*>("Piwap::OperationListManager*");

OperationListManager::OperationListManager(QObject *parent) : QAbstractListModel (parent)
{

}

int OperationListManager::rowCount(const QModelIndex &parent) const {

	Q_UNUSED(parent);

	return _operations.size();
}
QVariant OperationListManager::data(const QModelIndex &index, int role) const {

	if (index.row() >= _operations.size() || index.row() < 0) {
		return QVariant();
	}

	switch (role) {
	case Qt::DisplayRole :
		return _operations.at(index.row())->getOpName();
	case IconPathRole :
		return _operations.at(index.row())->getIconUrl();
	case Qt::DecorationRole :
		return QIcon(_operations.at(index.row())->getIconUrl());
	default :
		return QVariant();
	}

}
QHash<int, QByteArray> OperationListManager::roleNames() const {

	QHash<int, QByteArray> ret = QAbstractListModel::roleNames();
	ret.insert(IconPathRole, "iconPath");

	return ret;

}

void OperationListManager::clear () {

	beginRemoveRows(QModelIndex(), 0, _operations.size()-1);

	for (AbstractImageOperation* op : _operations) {
		op->deleteLater();
		disconnect(op, &AbstractImageOperation::hasBeenChanged, this, &OperationListManager::hasBeenChanged);
	}
	_operations.clear();

	endRemoveRows();

	Q_EMIT hasBeenChanged();
}

void OperationListManager::removeOp (int p_row) {

	if (abs(p_row) > _operations.size()) {
		return;
	}

	if (p_row == _operations.size()) {
		return;
	}

	int row = p_row;

	if (p_row < 0) {
		row = -p_row;
	}

	beginRemoveRows(QModelIndex(), row, row);
	AbstractImageOperation* op = _operations.at(row);
	op->deleteLater();
	_operations.removeAt(row);
	endRemoveRows();

	disconnect(op, &AbstractImageOperation::hasBeenChanged, this, &OperationListManager::hasBeenChanged);
	Q_EMIT hasBeenChanged();

}

void OperationListManager::insertOp (AbstractImageOperation* op, int p_row) {

	if (p_row > _operations.size() || (-p_row - 1) > _operations.size()) {
		return;
	}

	int row = p_row;

	if (p_row < 0) {
		row = _operations.size() + p_row + 1;
	}

	beginInsertRows(QModelIndex(), row, row);
	op->setParent(this);
	_operations.insert(row, op);
	endInsertRows();

	connect(op, &AbstractImageOperation::hasBeenChanged, this, &OperationListManager::hasBeenChanged);
	Q_EMIT hasBeenChanged();

}

void OperationListManager::replaceOps(QList<AbstractImageOperation*> list, bool cleanup) {

	beginResetModel();

	if (cleanup) {
		for (AbstractImageOperation* op : _operations) {
			op->deleteLater();
		}
	}

	for (AbstractImageOperation* op : _operations) {
		disconnect(op, &AbstractImageOperation::hasBeenChanged, this, &OperationListManager::hasBeenChanged);
	}

	_operations = list;

	for (AbstractImageOperation* op : _operations) {
		connect(op, &AbstractImageOperation::hasBeenChanged, this, &OperationListManager::hasBeenChanged);
	}

	endResetModel();

	Q_EMIT hasBeenChanged();

}

AbstractImageOperation* OperationListManager::opAtRow(int row) {

	if (row < 0 || row >= _operations.size()) {
		return nullptr;
	}

	return _operations.at(row);
}

void OperationListManager::moveAction(int from, int to) {

	int from_row = from;

	if (from < 0) {
		from_row = _operations.size() + from;
	}

	int to_row = to;

	if (to < 0) {
		to_row = _operations.size() + to;
	}

	if (to_row < 0 || from_row < 0 || to_row >= _operations.size() || from_row >= _operations.size()) {
		return;
	}

	if (from_row == to_row) {
		return;
	}

	beginMoveRows(QModelIndex(), from_row, from_row, QModelIndex(), (to_row > from_row) ? to_row + 1 : to_row);
	_operations.move(from_row, to_row);
	endMoveRows();

}

QList<AbstractImageOperation *> OperationListManager::operations() const
{
    return _operations;
}

} // namespace Piwap
