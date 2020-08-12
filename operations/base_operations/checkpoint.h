#ifndef CHECKPOINT_H
#define CHECKPOINT_H

#include "operations/abstractimageoperation.h"
#include "operations/abstractoperationfactory.h"

#include <QObject>

namespace Piwap {
namespace Operations {

class Checkpoint : public AbstractImageOperation
{
	Q_OBJECT
public:

	static const QString checkpointOpTypeId;

	Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)

	explicit Checkpoint(QObject *parent = nullptr);

	virtual int doOperation(Magick::Image & image, ImageInfos * infos) const;
	int restoreCheckpoint(Magick::Image & image, ImageInfos * infos) const;

	virtual QString typeId() const;

	virtual QString getIconUrl() const;
	virtual QString getOpName() const;

	Q_INVOKABLE virtual QString getPropertiesEditorUrl() const;

	QString name() const;

Q_SIGNALS:

	void nameChanged(QString name);

public Q_SLOTS:

	void setName(QString name);

private:

	mutable Magick::Image _store;
	mutable ImageInfos * _storeInfos;
	mutable bool _has_store;

	QString _checkpoint_name;
};

class CheckpointOpFactory : public AbstractOperationFactory {
	Q_OBJECT
public :
	explicit CheckpointOpFactory(QObject * parent = nullptr);

	virtual AbstractImageOperation* factorizeOperation(QObject * operationParent = nullptr) const;

	virtual QString getToolTip() const;
};

} // namespace Operations
} // namespace Piwap

#endif // CHECKPOINT_H
