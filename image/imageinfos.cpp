#include "imageinfos.h"

#include <QFileInfo>
#include <QDir>

namespace Piwap {

ImageInfos::ImageInfos(QString originalFileUrl, QObject *parent) :
	QObject(parent),
	_infos(originalFileUrl)
{

}

QString ImageInfos::originalFileName() const
{
	return _infos.baseName();
}

QString ImageInfos::originalFolderUrl() const
{
	return _infos.dir().path();
}

QString ImageInfos::originalImageType() const
{
	return _infos.completeSuffix();
}

} // namespace Piwap
