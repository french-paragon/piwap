/*Piwap, or Picture Warping App, is a GUI to batch process images.

Copyright (C) 2019  Paragon<french.paragon@gmail.com>

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
