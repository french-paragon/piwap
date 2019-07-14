#include "abstractoperationfactory.h"

#include "abstractimageoperation.h"

namespace Piwap {

AbstractOperationFactory::AbstractOperationFactory(QObject *parent) : QObject(parent)
{

}

QString AbstractOperationFactory::getOpTypeId() const {

	AbstractImageOperation* op = factorizeOperation();

	QString TypeId = op->typeId();

	delete op;

	return TypeId;

}

QString AbstractOperationFactory::getIconUrl() const {

	AbstractImageOperation* op = factorizeOperation();

	QString IconUrl = op->getIconUrl();

	delete op;

	return IconUrl;

}

QString AbstractOperationFactory::getOpName() const {

	AbstractImageOperation* op = factorizeOperation();

	QString OpName = op->getOpName();

	delete op;

	return OpName;

}

QString AbstractOperationFactory::getToolTip() const {

	return "";

}

} // namespace Piwap
