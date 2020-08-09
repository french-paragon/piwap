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

	Q_PROPERTY(QString project_file_ext READ projectFileExt CONSTANT)

	Q_PROPERTY(bool is_saved READ getSaveState NOTIFY SaveStateChanged)
	Q_PROPERTY(QString opened_file READ openedFile NOTIFY openedFileChanged)
	Q_PROPERTY(QStringList recent_files READ recentFiles NOTIFY recentFilesChanged)

	static Application* piwapApp();

	explicit Application(int &argc, char **argv);

	OperationFactoryManager *operationFactoryManager() const;

	OperationListManager *operations() const;

	ImageToTreatManager *images() const;

	void init();

	bool getSaveState() const;
	QString openedFile() const;

	QStringList recentFiles() const;

	QString projectFileExt() const;

Q_SIGNALS:

	void SaveStateChanged(bool saveState);
	void openedFileChanged(QString file);
	void recentFilesChanged(QStringList files);

public Q_SLOTS:

	void addOpToProject(QString opTypeId);
	void removeOpFromProject(int id);

	void treatImages();

	void newOperationsList();
	void saveOperations(QString outFile);
	void loadOperations(QString inFile);

protected:

	void markSaved();
	void markUnsaved();

	void setOpenedFile(QString file);
	void addRecentFile(QString file);

	void loadOperationsFactories();

	OperationFactoryManager* _operationFactoryManager;
	OperationListManager* _operations;

	ImageToTreatManager* _images;

	bool _saveState;
	QString _openedFile;

};

} // namespace Piwap

#endif // APPLICATION_H
