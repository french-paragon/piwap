#include "imageio.h"
#include "imageinfos.h"

#include <Magick++/Image.h>

namespace Piwap {

ImageInfos* openImage(const char *fileName, Magick::Image & imageDest, QObject *infoParent) {

	imageDest.read(std::string(fileName));

	if (!imageDest.isValid()) {
		return nullptr;
	}

	ImageInfos* infos = new ImageInfos(QString(fileName), infoParent);

	return infos;

}


}// namespace Piwap
