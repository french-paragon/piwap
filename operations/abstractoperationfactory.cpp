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
