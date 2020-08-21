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

#include "save.h"

#include "image/imageinfos.h"

#include <QDir>

#include <Magick++.h>

namespace Piwap {
namespace Operations {

const QString Save::saveOpTypeId = "piwapbase/save";

Save::Save(QObject *parent) :
	AbstractImageOperation(parent),
	_folderUrl("<folder>"),
	_fileName("<filename>"),
	_fileType("<filetype>"),
	_compressionParameter(90)
{

	connect(this, &Save::folderUrlChanged, this, &AbstractImageOperation::hasBeenChanged);
	connect(this, &Save::fileNameChanged, this, &AbstractImageOperation::hasBeenChanged);
	connect(this, &Save::fileTypeChanged, this, &AbstractImageOperation::hasBeenChanged);
	connect(this, &Save::compressionParameterChanged, this, &AbstractImageOperation::hasBeenChanged);

}

int Save::doOperation(Magick::Image &image, ImageInfos * infos) const {

	QString outPutFolder = _folderUrl;

	if (_folderUrl.startsWith("<folder>", Qt::CaseInsensitive)) {

		outPutFolder = QDir::fromNativeSeparators(infos->originalFolderUrl());

		if (!outPutFolder.endsWith('/')) {
			outPutFolder += '/';
		}

		QString trail = QDir::fromNativeSeparators(_folderUrl.mid(8));

		if (trail.startsWith('/')) {
			trail = trail.mid(1);
		}

		outPutFolder += trail;

		if (!outPutFolder.endsWith('/')) {
			outPutFolder += '/';
		}

	}

	QDir out(outPutFolder);

	if (!out.exists()) {
		out.mkpath(".");
	}

	QString fileName = _fileName;

	if (fileName.indexOf("<filename>", 0, Qt::CaseInsensitive) >= 0) {
		QStringList parts = fileName.split("<filename>", QString::KeepEmptyParts, Qt::CaseInsensitive);
		QString constructed = "";

		for (int i = 0; i < parts.size(); i++) {
			constructed += parts.at(i);

			if (i < parts.size() - 1) {
				constructed += infos->originalFileName();
			}
		}

		fileName = constructed;
	}

	QString type = _fileType;

	if (type.compare("<filetype>", Qt::CaseInsensitive) == 0) {
		type = infos->originalImageType();
	}

	QString fPath = outPutFolder + fileName + "." + type;

	size_t compressionParameter = static_cast<size_t>(std::max(0, _compressionParameter));

	if (compressionParameter > 100) {
		compressionParameter = 100;
	}

	image.quality(compressionParameter);

	try {

		image.write(fPath.toStdString());

	} catch (Magick::Exception & error) {
		Q_UNUSED(error);
		setError(infos->originalFilePath(), tr("Unable to save to %1").arg(fPath));
		return 1;
	}

	return 0;

}

QString Save::typeId() const {
	return saveOpTypeId;
}

QString Save::getOpName() const {
	return tr("Save");
}

QString Save::getIconUrl() const {
	return "qrc:/icons/save.svg";
}

QString Save::getPropertiesEditorUrl() const {
	return "qrc:/qmlcomponents/editors/SaveOpEditor.qml";
}

QString Save::getFolderUrl() const
{
	return _folderUrl;
}

void Save::setFolderUrl(const QString &folderUrl)
{
	if (folderUrl != _folderUrl) {
		_folderUrl = folderUrl;
		Q_EMIT folderUrlChanged(folderUrl);
	}
}

QString Save::getFileName() const
{
	return _fileName;
}

void Save::setFileName(const QString &fileName)
{
	if (fileName != _fileName) {
		_fileName = fileName;
		Q_EMIT fileNameChanged(fileName);
	}
}

QString Save::getFileType() const
{
	return _fileType;
}

void Save::setFileType(const QString &fileType)
{
	if (fileType != _fileType) {
		_fileType = fileType;
		Q_EMIT fileTypeChanged(fileType);
	}
}

int Save::getCompressionParameter() const
{
	return _compressionParameter;
}

void Save::setCompressionParameter(int compressionParameter)
{
	if (_compressionParameter != compressionParameter) {
		_compressionParameter = compressionParameter;
		Q_EMIT compressionParameterChanged(compressionParameter);
	}
}

SaveOpFactory::SaveOpFactory(QObject * parent) : AbstractOperationFactory (parent)
{

}

AbstractImageOperation* SaveOpFactory::factorizeOperation(QObject * operationParent) const {
	return new Save(operationParent);
}

QString SaveOpFactory::getToolTip() const {
	return tr("Operation to save images to any format but with minimal options");
}

} // namespace Operations
} // namespace Piwap
