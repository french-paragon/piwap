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

	virtual int doOperation(cv::Mat & image, ImageInfos * infos) const;

	virtual QString typeId() const;

	virtual QString getOpName() const;

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
};

} // namespace Operations
} // namespace Piwap

#endif // SAVE_H
