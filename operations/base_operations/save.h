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

#ifndef SAVE_H
#define SAVE_H

#include "operations/abstractimageoperation.h"
#include "operations/abstractoperationfactory.h"

namespace Piwap {
namespace Operations {

class Save : public AbstractImageOperation
{
	Q_OBJECT
public:

	static const QString saveOpTypeId;

	Q_PROPERTY(QString folderUrl READ getFolderUrl WRITE setFolderUrl NOTIFY folderUrlChanged)
	Q_PROPERTY(QString fileName READ getFileName WRITE setFileName NOTIFY fileNameChanged)
	Q_PROPERTY(QString fileType READ getFileType WRITE setFileType NOTIFY fileTypeChanged)
	Q_PROPERTY(int compressionParameter READ getCompressionParameter WRITE setCompressionParameter NOTIFY compressionParameterChanged)

	explicit Save(QObject *parent = nullptr);

	virtual int doOperation(Magick::Image & image, ImageInfos * infos) const;

	virtual QString typeId() const;

	virtual QString getOpName() const;
	virtual QString getIconUrl() const;

	virtual bool isSaving() const;

	virtual QString getPropertiesEditorUrl() const;

	QString getFolderUrl() const;
	void setFolderUrl(const QString &folderUrl);

	QString getFileName() const;
	void setFileName(const QString &fileName);

	QString getFileType() const;
	void setFileType(const QString &fileType);

	int getCompressionParameter() const;
	void setCompressionParameter(int compressionParameter);

Q_SIGNALS:

	void folderUrlChanged(QString folder);
	void fileNameChanged(QString filename);
	void fileTypeChanged(QString fileType);
	void compressionParameterChanged(int compression);

public Q_SLOTS:

protected:

	QString _folderUrl;
	QString _fileName;
	QString _fileType;
	int _compressionParameter;
};

class SaveOpFactory : public AbstractOperationFactory {
	Q_OBJECT
public :
	explicit SaveOpFactory(QObject * parent = nullptr);

	virtual AbstractImageOperation* factorizeOperation(QObject * operationParent = nullptr) const;

	virtual QString getToolTip() const;
};

} // namespace Operations
} // namespace Piwap

#endif // SAVE_H
