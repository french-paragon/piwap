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

	virtual int doOperation(Image* image, ImageInfos * infos) const;

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
