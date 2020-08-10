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

#include "reload.h"

#include "image/imageio.h"

#include <QAbstractListModel>

namespace Piwap {
namespace Operations {

class reloadCheckPointModel: public QAbstractListModel {

public:
	reloadCheckPointModel(Reload* parent) :
		QAbstractListModel(parent),
		_cache()
	{

	}

	virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
	virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

	QStringList _cache;

};

int reloadCheckPointModel::rowCount(const QModelIndex &parent) const {
	if (parent != QModelIndex()) {
		return 0;
	}

	return _cache.size()+1;
}

QVariant reloadCheckPointModel::data(const QModelIndex &index, int role) const {

	if (role != Qt::DisplayRole) {
		return QVariant();
	}

	if (index.row() > _cache.size()) {
		return QVariant();
	}

	if (index.row() == 0) {
		return tr("No checkpoint");
	}

	return _cache.at(index.row()-1);

}

const QString Reload::reloadOpTypeId = "piwapbase/reload";

Reload::Reload(QObject *parent) : AbstractImageOperation(parent)
{
	_p_checkpoints = new reloadCheckPointModel(this);
}

int Reload::doOperation(Magick::Image & image, ImageInfos * infos) const {

	return reloadImage(image, infos);

}

QString Reload::typeId() const {
	return reloadOpTypeId;
}

QString Reload::getIconUrl() const {
	return "qrc:/icons/reload.svg";
}

QString Reload::getOpName() const {
	return "Reload";
}

QString Reload::getPropertiesEditorUrl() const {
	return "qrc:/qmlcomponents/editors/ReloadOpEditor.qml";
}

QAbstractItemModel* Reload::possibleCheckpoints() const {
	return _p_checkpoints;
}

QString Reload::checkpoint() const {
	return _p_checkpoints->data(_p_checkpoints->index(0,0)).toString();
}

void Reload::setCheckpoint(QString checkpoint) {
	Q_UNUSED(checkpoint);
}


ReloadOpFactory::ReloadOpFactory(QObject * parent): AbstractOperationFactory (parent) {

}

AbstractImageOperation* ReloadOpFactory::factorizeOperation(QObject * operationParent) const {
	return new Reload(operationParent);
}

QString ReloadOpFactory::getToolTip() const {
	return tr("Reload the image or reset a checkpoint if a checkpoint is set.");
}


} // namespace Operations
} // namespace Piwap
