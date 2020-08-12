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
#include "application/application.h"
#include "operations/operationlistmanager.h"
#include "operations/base_operations/checkpoint.h"

#include <QAbstractListModel>

namespace Piwap {
namespace Operations {

class reloadCheckPointModel: public QAbstractListModel {

public:
	reloadCheckPointModel(Reload* parent) :
		QAbstractListModel(parent),
		_cache()
	{
		rebuildCache();

		Application* app = Application::piwapApp();
		connect(app->operations(), &OperationListManager::hasBeenChanged, this, &reloadCheckPointModel::rebuildCache);
	}

	virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
	virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

	void rebuildCache() {
		Reload* p = qobject_cast<Reload*>(parent());

		Application* app = Application::piwapApp();

		beginResetModel();

		if (app != nullptr) {
			OperationListManager* opm = app->operations();

			int n = 0;
			QStringList cache_tmp;
			for (int i = 0; i < opm->rowCount(); i++) {

				AbstractImageOperation* op = opm->opAtRow(i);

				Checkpoint* ckp = qobject_cast<Checkpoint*>(op);

				if (ckp != nullptr) {
					cache_tmp << (QVariant::fromValue(++n).toString() + ". " + ((ckp->name().size() > 0) ? ckp->name() : "(no name)"));
				} else {
					Reload* r = qobject_cast<Reload*>(op);

					if (r != nullptr) {
						if (r == p) {
							_cache = cache_tmp;
							endResetModel();
							return;
						}
					}
				}

			}

			_cache = cache_tmp;
			endResetModel();
			return;
		}

		_cache.clear();
		endResetModel();
	}

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

	if (!_checkpoint.isEmpty()) {

		int p_pos = _checkpoint.indexOf('.');

		if (p_pos >= 0) {

			bool ok;
			int checkpointIndex = QVariant(_checkpoint.mid(0, p_pos)).toInt(&ok);

			if (!ok) {
				return 1;
			}

			Application* app = Application::piwapApp();

			int c = 0;

			for(int i = 0; i < app->operations()->rowCount(); i++) {

				Checkpoint* ckp = qobject_cast<Checkpoint*>(app->operations()->opAtRow(i));

				if (ckp != nullptr) {
					c++;
					if (c == checkpointIndex) {
						return ckp->restoreCheckpoint(image, infos);
					}
				}

			}

			return 1;

		} else {
			return 1;
		}

	}

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
