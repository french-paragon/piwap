#include "imageio.h"
#include "imageinfos.h"

#include <opencv2/opencv.hpp>

namespace Piwap {

ImageInfos* openImage(const char *fileName, cv::Mat & imageDest, QObject *infoParent) {

	imageDest = cv::imread(cv::String(fileName), cv::IMREAD_UNCHANGED);

	if (imageDest.empty()) {
		return nullptr;
	}

	ImageInfos* infos = new ImageInfos(QString(fileName), infoParent);

	return infos;

}


}// namespace Piwap
