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
	case Qt::DecorationRole :
		return QIcon(_operations.at(index.row())->getIconUrl());
	default :
		return QVariant();
	}

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

} // namespace Piwap