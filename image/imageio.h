#ifndef IMAGEIO_H
#define IMAGEIO_H

class QObject;

namespace Magick {
	class Image;
}

namespace Piwap {

class ImageInfos;

ImageInfos* openImage(const char* fileName, Magick::Image & imageDest, QObject *infoParent = nullptr);

}// namespace Piwap

#endif // IMAGEIO_H
