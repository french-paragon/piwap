#ifndef IMAGEINFOS_H
#define IMAGEINFOS_H

#include <QObject>
#include <QFileInfo>

namespace Piwap {

class ImageInfos : public QObject
{
	Q_OBJECT
public:
	explicit ImageInfos(QString originalFileUrl, QObject *parent = nullptr);

	QString originalFileName() const;

	QString originalFolderUrl() const;

	QString originalImageType() const;

Q_SIGNALS:

public Q_SLOTS:

protected:

	QFileInfo _infos;


};

} // namespace Piwap

#endif // IMAGEINFOS_H
