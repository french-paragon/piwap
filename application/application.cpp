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

#include "application.h"
#include "operationplugin.h"

#include "operations/operationfactorymanager.h"
#include "operations/operationlistmanager.h"
#include "operations/operationerror.h"

#include "image/imagetotreatmanager.h"
#include "image/imageio.h"
#include "image/imageinfos.h"

#include "operations/base_operations/resize.h"
#include "operations/base_operations/save.h"
#include "operations/base_operations/fit.h"
#include "operations/base_operations/crop.h"
#include "operations/base_operations/reload.h"
#include "operations/base_operations/checkpoint.h"
#include "operations/base_operations/backgroundcolor.h"

#include <QUrl>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonDocument>

#include <QMetaProperty>

#include <QSettings>

#include <QDir>
#include <QFileInfo>
#include <QPluginLoader>

namespace Piwap {

const QString Application::PROJECT_FILE_EXT = ".pictwarpingops";

Application* Application::piwapApp() {

	QCoreApplication* app = QCoreApplication::instance();

	return qobject_cast<Piwap::Application*>(app);

}

Application::Application(int &argc, char **argv) : QGuiApplication(argc, argv),
	_saveState(true),
	_openedFile("")
{
	QCoreApplication::setOrganizationName(ORG_NAME);
	QCoreApplication::setOrganizationDomain(ORG_DOMAIN);
	QCoreApplication::setApplicationName(APP_NAME);

	_operationFactoryManager = new OperationFactoryManager(this);
	_operations = new OperationListManager(this);
	_images = new ImageToTreatManager(this);
	_errors = new OperationErrorsList(this);

	connect(_operations, &OperationListManager::hasBeenChanged, this, &Application::markUnsaved);
}

OperationFactoryManager *Application::operationFactoryManager() const
{
	return _operationFactoryManager;
}

OperationListManager *Application::operations() const
{
	return _operations;
}

void Application::init() {

	searchPlugins();
	loadOperationsFactories();

}

bool Application::getSaveState() const {
	return _saveState;
}
QString Application::openedFile() const {
	return _openedFile;
}


QStringList Application::recentFiles() const {

	QSettings s;
	return s.value("recent_files", QVariant::fromValue(QStringList())).toStringList();

}

QString Application::projectFileExt() const {
	return PROJECT_FILE_EXT;
}

void Application::addOpToProject(QString opTypeId) {

	AbstractOperationFactory* f = _operationFactoryManager->factoryByType(opTypeId);

	if (f == nullptr) {
		return;
	}

	AbstractImageOperation* o = f->factorizeOperation(_operations);

	_operations->insertOp(o);

}

void Application::removeOpFromProject(int id) {

	_operations->removeOp(id);

}

void Application::treatImages() {

	_errors->clearErrors();

	if (_operations->rowCount() == 0) {
		_errors->addError(OperationErrorInfos("", tr("No operations in queue")));
		return;
	}

	if (!_operations->hasSaveOp()) {
		_errors->addError(OperationErrorInfos("", tr("No save operation in queue")));
		return;
	}

	while (_images->rowCount() > 0) {

		QString imFile = _images->data(_images->index(0), Qt::DisplayRole).toString();
		_images->removeImage(0);

		QUrl url(imFile);

		imFile = imFile.startsWith("file:") ? url.toLocalFile() : imFile;

		ImageWithInfos imgWithInfos = openImage(imFile.toStdString().c_str(), this, this);

		if (imgWithInfos.image == nullptr) {
			_errors->addError(OperationErrorInfos(imFile.toStdString().c_str(), tr("Error reading image")));
			if (imgWithInfos.infos != nullptr) {
				delete imgWithInfos.infos;
			}
			continue;
		}

		if (imgWithInfos.infos == nullptr) {
			_errors->addError(OperationErrorInfos(imFile.toStdString().c_str(), tr("Error reading image metadata")));
			if (imgWithInfos.image != nullptr) {
				delete imgWithInfos.image;
			}
			continue;
		}

		for (int i = 0; i < _operations->rowCount(); i++) {
			AbstractImageOperation* op = _operations->opAtRow(i);

			if (op->doOperation(imgWithInfos.image, imgWithInfos.infos)) {
				if (op->getError().isValid()) {
					_errors->addError(op->getError());
				} else {
					_errors->addError(OperationErrorInfos(imFile, tr("Unknown error"), op));
				}
				break;
			}
		}

		delete imgWithInfos.image;
		delete imgWithInfos.infos;

	}

}

void Application::newOperationsList() {

	_operations->clear();
	setOpenedFile("");
	markSaved();
}

void Application::saveOperations(QString outFile) {

	if (_operations->rowCount() == 0) {
		return;
	}

	QJsonArray ops;

	for (int i = 0; i < _operations->rowCount(); i++) {

		AbstractImageOperation* op = _operations->opAtRow(i);

		QJsonObject jop = op->asJsonObject();

		ops.push_back(jop);
	}

	QJsonDocument doc;

	doc.setArray(ops);
	QByteArray datas = doc.toJson();

	QString fileName = (outFile.startsWith("file:")) ? QUrl(outFile).toLocalFile() : outFile;

	if (!fileName.endsWith(PROJECT_FILE_EXT)) {
		fileName += PROJECT_FILE_EXT;
	}

	QFile out(fileName);

	if(!out.open(QIODevice::WriteOnly)){
		return;
	}

	qint64 w_stat = out.write(datas);
	out.close();

	if(w_stat < 0){
		return;
	}

	setOpenedFile(fileName);
	markSaved();

}

void Application::loadOperations(QString inFile) {

	QString fileName = (inFile.startsWith("file:")) ? QUrl(inFile).toLocalFile() : inFile;
	QFile opsFile(fileName);

	opsFile.open(QIODevice::ReadOnly);
	QByteArray data = opsFile.readAll();
	opsFile.close();

	QJsonParseError errors;
	QJsonDocument doc = QJsonDocument::fromJson(data, &errors);

	if(errors.error != QJsonParseError::NoError){
		return;
	}

	if (!doc.isArray()) {
		return;
	}

	QJsonArray arr = doc.array();

	QList<AbstractImageOperation*> next;
	next.reserve(arr.size());

	for (QJsonValue val : arr) {

		if (!val.isObject()) {
			goto clean_and_return;
		}

		QJsonObject obj = val.toObject();

		if (!obj.contains("typeId")) {
			goto clean_and_return;
		}

		QJsonValue typeId = obj.value("typeId");

		if (!typeId.isString()) {
			goto clean_and_return;
		}

		QString tId = typeId.toString();

		AbstractOperationFactory* f = _operationFactoryManager->factoryByType(tId);

		if (f == nullptr) {
			goto clean_and_return;
		}

		AbstractImageOperation* o = f->factorizeOperation(_operations);
		o->setPropertiesFromJsonObject(obj);

		next.push_back(o);

	}

	_operations->replaceOps(next);
	markSaved();
	setOpenedFile(fileName);

	return;

	clean_and_return :

	for(AbstractImageOperation* op : next) {
		op->deleteLater();
	}

	return;

}
void Application::markSaved() {

	if (_saveState == false) {
		_saveState = true;
		Q_EMIT SaveStateChanged(_saveState);
	}

}
void Application::markUnsaved(){

	if (_saveState == true) {
		_saveState = false;
		Q_EMIT SaveStateChanged(_saveState);
	}

}

void Application::setOpenedFile(QString file) {
	_openedFile = file;
	Q_EMIT openedFileChanged(file);

	if (!file.isEmpty()) {
		addRecentFile(file);
	}
}

void Application::addRecentFile(QString file) {
	QStringList oldRecentFiles = recentFiles();

	QFileInfo newF(file);

	QStringList newRecentFiles;

	QSettings s;
	int max_recent_files = s.value("max_recent_files", 6).toInt();

	newF.makeAbsolute(); //ensure the path is absolute.
	oldRecentFiles.removeAll(newF.filePath());
	newRecentFiles << newF.filePath();

	for(int i = 0; i < std::min(oldRecentFiles.size(), max_recent_files); i++) {
		newRecentFiles << oldRecentFiles[i];
	}

	s.setValue("recent_files", QVariant::fromValue(newRecentFiles));

	Q_EMIT recentFilesChanged(newRecentFiles);
}

void Application::searchPlugins() {

	QDir pluginsDir = QDir(QCoreApplication::applicationDirPath());

	if (pluginsDir.cd("modules")) { //built but not installed

		for(QFileInfo info : pluginsDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot)) {
			QDir pluginDir(info.absoluteFilePath());
			for(QFileInfo inf : pluginDir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot)) {
				if (inf.suffix() == "so") {
					loadOperationPlugin(inf.absoluteFilePath());
				}
			}
		}
	} else { //installed
		pluginsDir.cdUp();
		pluginsDir.cd("lib");
		pluginsDir.cd("piwap");
		pluginsDir.cd("modules");

		for(QFileInfo info : pluginsDir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot)) {
			if (info.suffix() == "so") {
				loadOperationPlugin(info.absoluteFilePath());
			}
		}
	}

}

bool Application::loadOperationPlugin(QString const& path) {

	QPluginLoader* plugin = new QPluginLoader(path, this);
	plugin->load();

	if (plugin->instance() == nullptr) {
		delete plugin;
		return false;
	}

	OperationPlugin* opPlugin = qobject_cast<Piwap::OperationPlugin*>(plugin->instance());

	if (opPlugin != nullptr) {
		_loadedOpPlugins.push_back(plugin);
	}

	return true;

}

void Application::loadOperationsFactories() {

	//Load operations from plugins.
	for(QPluginLoader* plugin : _loadedOpPlugins) {
		OperationPlugin* opPlugin = qobject_cast<OperationPlugin*>(plugin->instance());
		opPlugin->loadPluginOperations(_operationFactoryManager);
	}

	//Load base operations.
	_operationFactoryManager->insertFactory(new Operations::ResizeOpFactory(this));
	_operationFactoryManager->insertFactory(new Operations::FitOpFactory(this));
	_operationFactoryManager->insertFactory(new Operations::CropOpFactory(this));
	_operationFactoryManager->insertFactory(new Operations::BackgroundOpFactory(this));
	_operationFactoryManager->insertFactory(new Operations::CheckpointOpFactory(this));
	_operationFactoryManager->insertFactory(new Operations::ReloadOpFactory(this));
	_operationFactoryManager->insertFactory(new Operations::SaveOpFactory(this));

}

OperationErrorsList *Application::errors() const
{
    return _errors;
}

ImageToTreatManager *Application::images() const
{
    return _images;
}

} // namespace Piwap
