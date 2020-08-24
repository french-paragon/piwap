/*Piwap, or Picture Warping App, is a GUI to batch process images.

Copyright (C) 2020  Paragon<french.paragon@gmail.com>

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

#include "operationerror.h"

#include "application/application.h"
#include "operations/operationlistmanager.h"
#include "operations/abstractimageoperation.h"

namespace Piwap {

OperationErrorInfos::OperationErrorInfos() :
	_isValid(false)
{

}

OperationErrorInfos::OperationErrorInfos(QString title, QString infos, const AbstractImageOperation *op) :
	_image(title),
	_infos(infos),
	_operation(op),
	_isValid(true)
{

}

OperationErrorInfos::OperationErrorInfos(OperationErrorInfos const& other) :
	_image(other.image()),
	_infos(other.infos()),
	_operation(other._operation),
	_isValid(other._isValid)
{

}

OperationErrorInfos::~OperationErrorInfos() {

}

QString OperationErrorInfos::infos() const
{
	return _infos;
}

QString OperationErrorInfos::image() const
{
	return _image;
}
QString OperationErrorInfos::getOperationInfos() const {

	if (_operation == nullptr) {
		return QObject::tr("no operation");
	}

	Application* app = Piwap::Application::piwapApp();
	OperationListManager* olm = app->operations();

	int id = olm->operations().indexOf(const_cast<Piwap::AbstractImageOperation*>(_operation));

	if (id < 0) {
		return _operation->getOpName();
	} else {
		return QString("%1. ").arg(id) + _operation->getOpName();
	}

}
bool OperationErrorInfos::hasOperation() const {
	return _operation != nullptr;
}
void OperationErrorInfos::clearOperation() {
	_operation = nullptr;
}

bool OperationErrorInfos::isValid() const
{
	return _isValid;
}


const int OperationErrorsList::registrationCode = qRegisterMetaType<Piwap::OperationErrorsList*>("Piwap::OperationErrorsList*");

OperationErrorsList::OperationErrorsList(QObject* parent) :
	QAbstractListModel (parent)
{

}

int OperationErrorsList::rowCount(QModelIndex const& parent) const {
	if (parent != QModelIndex()) {
		return 0;
	}
	return _errors.size();
}
QVariant OperationErrorsList::data(const QModelIndex &index, int role) const {

	int r = index.row();

	switch (role) {
		case Qt::DisplayRole:
		case InfosRole :
			return _errors[r].infos();
		case ImageRole :
			return _errors[r].image();
		case OpInfosRole:
			return  _errors[r].getOperationInfos();
		case HasOpRole:
			return  _errors[r].hasOperation();
		default:
			break;
	}

	return QVariant();

}
QHash<int, QByteArray> OperationErrorsList::roleNames() const {
	QHash<int, QByteArray> roles = QAbstractListModel::roleNames();
	roles.insert(ImageRole, "image");
	roles.insert(InfosRole, "infos");
	roles.insert(OpInfosRole, "operationInfos");
	roles.insert(HasOpRole, "hasOperation");
	return roles;
}

void OperationErrorsList::addError(OperationErrorInfos const& errInfos) {
	beginInsertRows(QModelIndex(), _errors.length(), _errors.length());
	_errors.push_back(errInfos);
	endInsertRows();
}

void OperationErrorsList::removeErrorAt(int row) {
	beginRemoveRows(QModelIndex(), row, row);
	_errors.removeAt(row);
	endRemoveRows();
}

void OperationErrorsList::clearErrors() {
	beginResetModel();
	_errors.clear();
	endResetModel();
}

} // namespace Piwap
