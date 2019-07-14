#include "save.h"

#include "image/imageinfos.h"

#include <QDir>

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

}

int Save::doOperation(cv::Mat & image, ImageInfos * infos) const {

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

	std::vector<int> qualityParam;

	if (type.compare("jpg", Qt::CaseInsensitive) == 0 || type.compare("jpeg", Qt::CaseInsensitive) == 0) {
		if (_compressionParameter >= 0 && _compressionParameter <= 100) {
			qualityParam.push_back(CV_IMWRITE_JPEG_QUALITY);
			qualityParam.push_back(_compressionParameter);
		}
	}

	if (type.compare("png", Qt::CaseInsensitive) == 0) {
		if (_compressionParameter >= 0 && _compressionParameter <= 100) {
			qualityParam.push_back(CV_IMWRITE_PNG_COMPRESSION);
			qualityParam.push_back((_compressionParameter == 100) ? _compressionParameter/10 : 9);
		}
	}

	if (cv::imwrite(fPath.toStdString(), image, qualityParam)) {
		return 0;
	}

	return 1;

}

QString Save::typeId() const {
	return saveOpTypeId;
}

QString Save::getOpName() const {
	return tr("Save");
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
