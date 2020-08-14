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
