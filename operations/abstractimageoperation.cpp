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

#include "abstractimageoperation.h"
#include "operationerror.h"

#include <QtQml>

namespace Piwap {

const int AbstractImageOperation::registrationCodes = qRegisterMetaType<Piwap::AbstractImageOperation*>("Piwap::AbstractImageOperation*");

AbstractImageOperation::AbstractImageOperation(QObject *parent) :
	QObject(parent),
	_error(nullptr)
{

}

AbstractImageOperation::~AbstractImageOperation() {
	clearError();
}

QString AbstractImageOperation::getIconUrl() const {
	return "qrc:/icons/operation.svg";
}


QJsonObject AbstractImageOperation::asJsonObject() const {

	QJsonObject jop;

	const QMetaObject* mobj = metaObject();

	for (int i = 0; i < mobj->propertyCount(); i++) {

		if (!mobj->property(i).isStored(this)) {
			continue;
		}

		const char* prop = mobj->property(i).name();

		jop.insert(QString(prop), QJsonValue::fromVariant(property(prop)));
	}

	QList<QByteArray> dynamicProperties = dynamicPropertyNames();

	for (QByteArray cpropName : dynamicProperties) {

		jop.insert(QString(cpropName), QJsonValue::fromVariant(property(cpropName.data())));

	}

	return jop;

}

void AbstractImageOperation::setPropertiesFromJsonObject(QJsonObject const& obj) {

	for (QString prop : obj.keys()) {

		if (prop == "typeId") {
			continue;
		}

		QVariant v = obj.value(prop).toVariant();
		setProperty(prop.toLocal8Bit().data(), v);
	}

}

bool AbstractImageOperation::event(QEvent *e) {

	if (e->type() == QEvent::DynamicPropertyChange) {
		Q_EMIT hasBeenChanged();
		return true;
	}

	return QObject::event(e);

}

const OperationErrorInfos AbstractImageOperation::invalidError;

const OperationErrorInfos &AbstractImageOperation::getError() const {

	if (_error == nullptr) {
		return invalidError;
	}

	return *_error;
}
bool AbstractImageOperation::hasError() const {
	return
			_error != nullptr;
}
void AbstractImageOperation::clearError() {

	if (_error != nullptr) {
		delete _error;
	}
	_error = nullptr;
}

void AbstractImageOperation::setError(QString imFile, QString infos) const {

	clearError();
	_error = new OperationErrorInfos(imFile, infos, this);
}

void AbstractImageOperation::clearError() const {

	if (_error != nullptr) {
		delete _error;
	}
	_error = nullptr;
}

} // namespace Piwap
