#ifndef BEVELS_H
#define BEVELS_H

#include "operations/abstractimageoperation.h"
#include "operations/abstractoperationfactory.h"

namespace Piwap {
namespace BorderOperations {

class Bevels: public AbstractImageOperation
{
	Q_OBJECT
public:
	static const QString bevelOpTypeId;

	Q_PROPERTY(quint32 radius READ radius WRITE setRadius NOTIFY radiusChanged)

	explicit Bevels(QObject *parent = nullptr);

	virtual int doOperation(Image* image, ImageInfos * infos) const;

	virtual QString typeId() const;

	virtual QString getIconUrl() const;
	virtual QString getOpName() const;

	virtual QString getPropertiesEditorUrl() const;

	quint32 radius() const;
	void setRadius(const quint32 &radius);

Q_SIGNALS:

	void radiusChanged(quint32 r);

protected:

	quint32 _radius;
};



class BevelOpFactory : public AbstractOperationFactory {
	Q_OBJECT
public :
	explicit BevelOpFactory(QObject * parent = nullptr);

	virtual AbstractImageOperation* factorizeOperation(QObject * operationParent = nullptr) const;

	virtual QString getToolTip() const;
};

} // namespace BorderOperations
} // namespace Piwap

#endif // BEVELS_H
