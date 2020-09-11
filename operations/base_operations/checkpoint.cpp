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

#include "checkpoint.h"
#include "image/imageinfos.h"

#include <Magick++/Image.h>

namespace Piwap {
namespace Operations {

const QString Checkpoint::checkpointOpTypeId = "piwapbase/checkpoint";

Checkpoint::Checkpoint(QObject *parent) : AbstractImageOperation(parent),
	_has_store(false)
{
	connect(this, &Checkpoint::nameChanged, this, &AbstractImageOperation::hasBeenChanged);
	_storeInfos = new ImageInfos("", this);
}

int Checkpoint::doOperation(Magick::Image & image, ImageInfos * infos) const {
	_store = image;
	_storeInfos->copyInfosFromOther(*infos);
	_has_store = true;

	return 0;
}
int Checkpoint::restoreCheckpoint(Magick::Image & image, ImageInfos * infos) const {

	if (!_has_store) {
		return 1;
	}

	image = _store;
	infos->copyInfosFromOther(*_storeInfos);

	return 0;
}

QString Checkpoint::typeId() const {
	return checkpointOpTypeId;
}

QString Checkpoint::getIconUrl() const {
	return "qrc:/icons/checkpoint.svg";
}
QString Checkpoint::getOpName() const {
	return  tr("Checkpoint");
}

QString Checkpoint::getPropertiesEditorUrl() const {
	return "qrc:/qmlcomponents/editors/CheckpointOpEditor.qml";
}

QString Checkpoint::name() const {
	return _checkpoint_name;
}

void Checkpoint::setName(QString name) {
	if (_checkpoint_name != name) {
		_checkpoint_name = name;
		Q_EMIT nameChanged(_checkpoint_name);
	}
}

CheckpointOpFactory::CheckpointOpFactory(QObject * parent) : AbstractOperationFactory (parent)
{

}

AbstractImageOperation* CheckpointOpFactory::factorizeOperation(QObject * operationParent) const {
	return new Checkpoint(operationParent);
}

QString CheckpointOpFactory::getToolTip() const {
	return tr("Define a checkpoint that can be reloaded later.");
}

} // namespace Operations
} // namespace Piwap
