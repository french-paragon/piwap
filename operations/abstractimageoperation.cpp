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

} // namespace Piwap
