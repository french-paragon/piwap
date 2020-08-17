#ifndef BACKGROUNDCOLOR_H
#define BACKGROUNDCOLOR_H

#include <QColor>

#include "operations/abstractimageoperation.h"
#include "operations/abstractoperationfactory.h"

namespace Piwap {
namespace Operations {

class BackgroundColor : public AbstractImageOperation
{
	Q_OBJECT
public:

	static const QString backgroundOpTypeId;

	Q_PROPERTY(QColor bg READ bg WRITE setBg NOTIFY bg_changed)

	explicit BackgroundColor(QObject *parent = nullptr);

	virtual int doOperation(Magick::Image & image, ImageInfos * infos) const;

	virtual QString typeId() const;

	virtual QString getOpName() const;
	virtual QString getIconUrl() const;

	virtual QString getPropertiesEditorUrl() const;

	QColor bg() const;
	void setBg(const QColor &bg);

Q_SIGNALS:

	void bg_changed(QColor bg);

public Q_SLOTS:

protected:

	QColor _bg;
};

class BackgroundOpFactory : public AbstractOperationFactory {
	Q_OBJECT
public :
	explicit BackgroundOpFactory(QObject * parent = nullptr);

	virtual AbstractImageOperation* factorizeOperation(QObject * operationParent = nullptr) const;

	virtual QString getToolTip() const;
};

} // namespace Operations
} // namespace Piwap

#endif // BACKGROUNDCOLOR_H
