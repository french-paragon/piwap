#include "abstractimageoperation.h"

#include <QtQml>

namespace Piwap {

const int AbstractImageOperation::registrationCodes = qRegisterMetaType<Piwap::AbstractImageOperation*>("Piwap::AbstractImageOperation*");

AbstractImageOperation::AbstractImageOperation(QObject *parent) : QObject(parent)
{

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


} // namespace Piwap
