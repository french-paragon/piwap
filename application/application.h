#ifndef APPLICATION_H
#define APPLICATION_H

#define APP_NAME "Piwap"
#define ORG_NAME "Paragon"
#define ORG_DOMAIN "famillejospin.ch"

#include <QGuiApplication>

namespace Piwap {

	class OperationFactoryManager;
	class OperationListManager;
	class ImageToTreatManager;

class Application : public QGuiApplication
{
	Q_OBJECT
public:

	static const QString PROJECT_FILE_EXT;

	Q_PROPERTY(Piwap::OperationFactoryManager* operationFactoryManager READ operationFactoryManager CONSTANT)
	Q_PROPERTY(Piwap::OperationListManager* operations READ operations CONSTANT)
	Q_PROPERTY(Piwap::ImageToTreatManager* images READ images CONSTANT)

	static Application* piwapApp();

	explicit Application(int &argc, char **argv);

	OperationFactoryManager *operationFactoryManager() const;

	OperationListManager *operations() const;

	ImageToTreatManager *images() const;

	void init();

Q_SIGNALS:

public Q_SLOTS:

	void addOpToProject(QString opTypeId);
	void removeOpFromProject(int id);

	void treatImages();

	void saveOperations(QString outFile);
	void loadOperations(QString inFile);

protected:

	void loadOperationsFactories();

	OperationFactoryManager* _operationFactoryManager;
	OperationListManager* _operations;

	ImageToTreatManager* _images;

};

} // namespace Piwap

#endif // APPLICATION_H
