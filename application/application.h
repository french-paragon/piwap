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

	Q_PROPERTY(bool is_saved READ getSaveState NOTIFY SaveStateChanged)
	Q_PROPERTY(QString opened_file READ openedFile NOTIFY openedFileChanged)

	static Application* piwapApp();

	explicit Application(int &argc, char **argv);

	OperationFactoryManager *operationFactoryManager() const;

	OperationListManager *operations() const;

	ImageToTreatManager *images() const;

	void init();

	bool getSaveState() const;
	QString openedFile() const;

Q_SIGNALS:

	void SaveStateChanged(bool saveState);
	void openedFileChanged(QString file);

public Q_SLOTS:

	void addOpToProject(QString opTypeId);
	void removeOpFromProject(int id);

	void treatImages();

	void saveOperations(QString outFile);
	void loadOperations(QString inFile);

protected:

	void markSaved();
	void markUnsaved();

	void setOpenedFile(QString file);

	void loadOperationsFactories();

	OperationFactoryManager* _operationFactoryManager;
	OperationListManager* _operations;

	ImageToTreatManager* _images;

	bool _saveState;
	QString _openedFile;

};

} // namespace Piwap

#endif // APPLICATION_H
