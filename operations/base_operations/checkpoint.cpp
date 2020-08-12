#include "checkpoint.h"
#include "image/imageinfos.h"

#include <magick/image.h>

namespace Piwap {
namespace Operations {

const QString Checkpoint::checkpointOpTypeId = "piwapbase/checkpoint";

Checkpoint::Checkpoint(QObject *parent) : AbstractImageOperation(parent),
	_has_store(false)
{
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
