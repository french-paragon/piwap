/*Piwap, or Picture Warping App, is a GUI to batch process images.

Copyright (C) 2020  Paragon<french.paragon@gmail.com>

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

#include "backgroundcolor.h"

namespace Piwap {
namespace Operations {


const QString BackgroundColor::backgroundOpTypeId = "piwapbase/backgroundcolor";

BackgroundColor::BackgroundColor(QObject *parent) :
	AbstractImageOperation(parent),
	_bg(255, 255, 255)
{
	connect(this, &BackgroundColor::bg_changed, this, &AbstractImageOperation::hasBeenChanged);
}

template<typename ScalarT>
Image::ImageData setBackgroundColor(ImageArray<ScalarT> const& inData, ColorModel imgColorModel, QColor bgColor) {

	int alphaChannel = alphaChannelIndex(imgColorModel);

	if (alphaChannel < 0 or alphaChannel >= inData.shape()[2]) {
		return inData;
	}

	ColorModel outModel = removeColorModelAlphaChannel(imgColorModel);

	ImageArray<ScalarT> ret(inData.shape()[0],inData.shape()[1], inData.shape()[2]-1);

	int nChannels = ret.shape()[2];

	std::vector<ScalarT> color = getChannelValueFromQColor<ScalarT>(outModel, bgColor);

	for (int i = 0; i < ret.shape()[0]; i++) {
		for (int j = 0; j < ret.shape()[1]; j++) {
			for (int c = 0; c < ret.shape()[2]; c++) {
				int sc = c;
				if (c >= alphaChannel) {
					sc += 1;
				}

				float alphaCoeff = float(inData.value(i,j,alphaChannel))/float(typedWhiteLevel<ScalarT>());

				ret.atUnchecked(i,j,c) = inData.valueUnchecked(i,j,sc)*alphaCoeff + color[c]*(1-alphaCoeff);

			}
		}
	}

	return ret;

}

int BackgroundColor::doOperation(Image *image, ImageInfos * infos) const {

	Q_UNUSED(infos);

	int alphaChannel = image->alphaChannel();

	if (alphaChannel < 0 or alphaChannel >= image->channels()) {
		return 0;
	}

	ColorModel inModel = image->colorModel();
	ColorModel outModel = removeColorModelAlphaChannel(inModel);

	image->imageData() = std::visit([inModel, this] (auto const& inData) {
		return setBackgroundColor(inData, inModel, _bg);
	}, image->imageData());

	image->setColorModel(outModel);

	return  0;
}

QString BackgroundColor::typeId() const {
	return  backgroundOpTypeId;
}

QString BackgroundColor::getOpName() const {
	return tr("Background color");
}
QString BackgroundColor::getIconUrl() const {
	return "qrc:/icons/color_bg.svg";
}

QString BackgroundColor::getPropertiesEditorUrl() const {
	return "qrc:/qmlcomponents/editors/BackgroundOpEditor.qml";
}

QColor BackgroundColor::bg() const {
	return _bg;
}
void BackgroundColor::setBg(const QColor &bg) {
	if (bg != _bg) {
		_bg = bg;
		Q_EMIT bg_changed(_bg);
	}
}


BackgroundOpFactory::BackgroundOpFactory(QObject * parent) : AbstractOperationFactory (parent) {

}

AbstractImageOperation* BackgroundOpFactory::factorizeOperation(QObject * operationParent) const {
	return new BackgroundColor(operationParent);
}

QString BackgroundOpFactory::getToolTip() const {
	return tr("Set image background color.");
}

} // namespace Operations
} // namespace Piwap
