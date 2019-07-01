#ifndef IMAGEIO_H
#define IMAGEIO_H

class QObject;

namespace cv {
	class Mat;
}

namespace Piwap {

class ImageInfos;

ImageInfos* openImage(const char* fileName, cv::Mat & imageDest, QObject *infoParent = nullptr);

}// namespace Piwap

#endif // IMAGEIO_H
