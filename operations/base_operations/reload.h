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

#ifndef RELOAD_H
#define RELOAD_H

#include "operations/abstractimageoperation.h"
#include "operations/abstractoperationfactory.h"

class QAbstractItemModel;

namespace Piwap {
namespace Operations {

class Reload : public AbstractImageOperation
{
	Q_OBJECT
public:

	static const QString reloadOpTypeId;

	Q_PROPERTY(QAbstractItemModel* possibleCheckpoints READ possibleCheckpoints CONSTANT)
	Q_PROPERTY(QString checkpoint READ checkpoint WRITE setCheckpoint NOTIFY checkpointChanged)

	explicit Reload(QObject *parent = nullptr);

	virtual int doOperation(Magick::Image & image, ImageInfos * infos) const;

	virtual QString typeId() const;

	virtual QString getIconUrl() const;
	virtual QString getOpName() const;

	Q_INVOKABLE virtual QString getPropertiesEditorUrl() const;

	QAbstractItemModel* possibleCheckpoints() const;

	QString checkpoint() const;

Q_SIGNALS:

	void checkpointChanged(QString checkpoint);

public Q_SLOTS:

	void setCheckpoint(QString checkpoint);

private:

	QAbstractItemModel* _p_checkpoints;

};

class ReloadOpFactory : public AbstractOperationFactory {
	Q_OBJECT
public :
	explicit ReloadOpFactory(QObject * parent = nullptr);

	virtual AbstractImageOperation* factorizeOperation(QObject * operationParent = nullptr) const;

	virtual QString getToolTip() const;
};

} // namespace Operations
} // namespace Piwap

#endif // RELOAD_H
