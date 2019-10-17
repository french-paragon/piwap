#ifndef ABSTRACTOPERATIONFACTORY_H
#define ABSTRACTOPERATIONFACTORY_H

#include <QObject>

namespace Piwap {

class AbstractImageOperation;

class AbstractOperationFactory : public QObject
{
	Q_OBJECT
public:
	explicit AbstractOperationFactory(QObject *parent = nullptr);

	virtual AbstractImageOperation* factorizeOperation(QObject * operationParent = nullptr) const = 0;

	virtual QString getOpTypeId() const;

	virtual QString getIconUrl() const;
	virtual QString getOpName() const;
	virtual QString getToolTip() const;



Q_SIGNALS:

public Q_SLOTS:
};

} // namespace Piwap

#endif // ABSTRACTOPERATIONFACTORY_H
