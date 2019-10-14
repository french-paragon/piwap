#include "application.h"

#include "operations/operationfactorymanager.h"
#include "operations/operationlistmanager.h"

#include "image/imagetotreatmanager.h"
#include "image/imageio.h"
#include "image/imageinfos.h"

#include "operations/base_operations/resize.h"
#include "operations/base_operations/save.h"

#include <QUrl>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonDocument>

#include <QMetaProperty>

namespace Piwap {

const QString Application::PROJECT_FILE_EXT = ".pictwarpingops";

Application* Application::piwapApp() {

	QCoreApplication* app = QCoreApplication::instance();

	return qobject_cast<Piwap::Application*>(app);

}

Application::Application(int &argc, char **argv) : QGuiApplication(argc, argv)
{
	QCoreApplication::setOrganizationName(ORG_NAME);
	QCoreApplication::setOrganizationDomain(ORG_DOMAIN);
	QCoreApplication::setApplicationName(APP_NAME);

	_operationFactoryManager = new OperationFactoryManager(this);
	_operations = new OperationListManager(this);
	_images = new ImageToTreatManager(this);
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

	loadOperationsFactories();

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

	if (_operations->rowCount() == 0) {
		return;
	}

	while (_images->rowCount() > 0) {

		QString imFile = _images->data(_images->index(0), Qt::DisplayRole).toString();

		QUrl url(imFile);

		imFile = url.toLocalFile();

		cv::Mat img;

		ImageInfos* info = openImage(imFile.toStdString().c_str(), img, this);

		if (img.empty() || info == nullptr) {
			continue;
		}

		for (int i = 0; i < _operations->rowCount(); i++) {
			AbstractImageOperation* op = _operations->opAtRow(i);

			if (op->doOperation(img, info)) {
				//manage error here
				break;
			}
		}

		delete info;
		img.deallocate();

		_images->removeImage(0);

	}

}

void Application::saveOperations(QString outFile) {

	if (_operations->rowCount() == 0) {
		return;
	}

	QJsonArray ops;

	for (int i = 0; i < _operations->rowCount(); i++) {

		AbstractImageOperation* op = _operations->opAtRow(i);

		QJsonObject jop;

		const QMetaObject* mobj = op->metaObject();

		for (int i = 0; i < mobj->propertyCount(); i++) {

			if (!mobj->property(i).isStored(op)) {
				continue;
			}

			const char* prop = mobj->property(i).name();

			jop.insert(QString(prop), QJsonValue::fromVariant(op->property(prop)));
		}

		QList<QByteArray> dynamicProperties = op->dynamicPropertyNames();

		for (QByteArray cpropName : dynamicProperties) {

			jop.insert(QString(cpropName), QJsonValue::fromVariant(op->property(cpropName.data())));

		}

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


}

void Application::loadOperationsFactories() {

	_operationFactoryManager->insertFactory(new Operations::ResizeOpFactory(this));
	_operationFactoryManager->insertFactory(new Operations::SaveOpFactory(this));

}

ImageToTreatManager *Application::images() const
{
    return _images;
}

} // namespace Piwap
