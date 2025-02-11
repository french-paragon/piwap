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

#include "fit.h"

#include <QDebug>
#include <QMetaEnum>
#include <QJsonObject>

#include "resize.h"
#include "image/imageinfos.h"
#include "image/color.h"

namespace Piwap {
namespace Operations {

const QString Fit::fitOpTypeId = "piwapbase/fit";

Fit::Fit(QObject *parent) :
	AbstractInterpolatingOperation(parent),
	_pix_x(800),
	_pix_y(800),
	_bg(255, 255, 255),
	_expand_mode(None)
{
	connect(this, &Fit::pix_x_changed, this, &AbstractImageOperation::hasBeenChanged);
	connect(this, &Fit::pix_y_changed, this, &AbstractImageOperation::hasBeenChanged);
	connect(this, &Fit::bg_changed, this, &AbstractImageOperation::hasBeenChanged);
	connect(this, &Fit::expandModeChanged, this, &AbstractImageOperation::hasBeenChanged);
}

int Fit::doOperation(Image *image, ImageInfos * infos) const {

	Q_UNUSED(infos);

	if (_pix_x <= 0 && _pix_y <= 0) {
		return 0;
	}

	int newWidth;
	int newHeight;

	float aspectRatio = static_cast<float>(image->width()) / static_cast<float>(image->height());

	float targetAspectRatio = static_cast<float>(_pix_x) / static_cast<float>(_pix_y);

	if (aspectRatio > targetAspectRatio) {

		if (_expand_mode == X) {
			newWidth = static_cast<int>(roundf(_pix_y*aspectRatio));
			newHeight = _pix_y;
		} else {
			newWidth = _pix_x;
			newHeight = static_cast<int>(roundf(_pix_x/aspectRatio));
		}

	} else if (aspectRatio < targetAspectRatio) {

		if (_expand_mode == Y) {
			newWidth = _pix_x;
			newHeight = static_cast<int>(roundf(_pix_x/aspectRatio));
		} else {
			newWidth = static_cast<int>(roundf(_pix_y*aspectRatio));
			newHeight = _pix_y;
		}

	} else {

		newWidth = _pix_x;
		newHeight = _pix_y;

	}



	QSize newSize(static_cast<size_t>(newWidth), static_cast<size_t>(newHeight));

	auto resizedOpt = Resize::resize(image->imageData(), newSize, _interpolation_mode);

	if (!resizedOpt.has_value()) {
		setError(infos->originalFileName(), "invalid resize operation");
		return 1;
	}

	int nChannels = image->channels();

	ColorModel imgColorModel = image->colorModel();
	ColorModel outModel = removeColorModelAlphaChannel(imgColorModel);

	int alphaChannel = alphaChannelIndex(imgColorModel);

	if (newWidth < _pix_x || newHeight < _pix_y) {

		std::vector<float> color = getChannelValueFromQColor<float>(outModel, _bg);

		image->imageData() = std::visit([&color, this, newWidth, newHeight, nChannels, alphaChannel] (auto const& inData) -> Image::ImageData {
			using MArrayT = std::decay_t<decltype(inData)>;
			using ScalarT = typename MArrayT::ScalarT;
			ImageArray<ScalarT> ret(_pix_y, _pix_x, nChannels);

			std::vector<ScalarT> convertedColor(nChannels);

			for (int c = 0; c < nChannels; c++) {
				convertedColor[c] = convertTypedWhiteLevel<ScalarT>(color[std::max<size_t>(c, color.size()-1)]);
			}

			int di = (_pix_y - newHeight) / 2;
			int dj = (_pix_x - newWidth) / 2;

			for (int i = 0; i < _pix_y; i++) {
				for (int j = 0; j < _pix_x; j++) {
					for (int c = 0; c < nChannels; c++) {

						if (c == alphaChannel) {
							continue;
						}

						int channel = c;
						if (c > alphaChannel) {
							channel -= 1;
						}

						ScalarT val = convertedColor[channel];

						if (i >= di and i - di < newHeight) {
							if (j >= dj and j - dj < newWidth) {
								ScalarT in = inData.valueUnchecked(i-di,j-dj,c);
								val = in;

								if (alphaChannel >= 0 and alphaChannel < inData.shape()[2]) {
									float alphaCoeff = float(inData.valueUnchecked(i-di,j-dj,alphaChannel))/float(typedWhiteLevel<ScalarT>());
									val = in*alphaCoeff + convertedColor[channel]*(1-alphaCoeff);
								}
							}
						}

						ret.atUnchecked(i,j,channel) = val;
					}
				}
			}

			return ret;
		}, resizedOpt.value());

	}

	return 0;

}

QString Fit::typeId() const {
	return fitOpTypeId;
}

QString Fit::getOpName() const {
	return "Fit on background";
}
QString Fit::getIconUrl() const {
	return "qrc:/icons/scale_image.svg";
}

QString Fit::getPropertiesEditorUrl() const {
	return "qrc:/qmlcomponents/editors/FitOpEditor.qml";
}

int Fit::pix_x() const
{
	return _pix_x;
}

void Fit::setPix_x(int pix_x)
{
	if (pix_x != _pix_x) {
		_pix_x = pix_x;
		Q_EMIT pix_x_changed(_pix_x);
	}
}

int Fit::pix_y() const
{
	return _pix_y;
}

void Fit::setPix_y(int pix_y)
{
	if (pix_y != _pix_y) {
		_pix_y = pix_y;
		Q_EMIT pix_y_changed(_pix_y);
	}
}

QColor Fit::bg() const
{
	return _bg;
}

void Fit::setBg(const QColor &bg)
{
	if (_bg != bg) {
		qDebug() << "BG changed to: " << bg;
		_bg = bg;
		Q_EMIT bg_changed(_bg);
	}
}

QString Fit::expandMode() const {
	return QVariant::fromValue(_expand_mode).toString();
}
void Fit::setExpandMode(QString mode) {

	QMetaEnum m_e = QMetaEnum::fromType<Fit::ExpandMode>();

	int index = m_e.keyToValue(mode.toStdString().c_str());

	if (index == -1) {
		return;
	}

	Fit::ExpandMode e = static_cast<Fit::ExpandMode>(index);

	if (e != _expand_mode) {
		_expand_mode = e;
		Q_EMIT expandModeChanged(expandMode());
	}

}

QStringList Fit::expandModeStrings() const {

	QMetaEnum m_e = QMetaEnum::fromType<Fit::ExpandMode>();

	int n_keys = m_e.keyCount();
	QStringList ret;
	ret.reserve(n_keys);

	for (int i = 0; i < n_keys; i++) {
		ret << QString(m_e.key(i));
	}

	return ret;
}

FitOpFactory::FitOpFactory(QObject * parent) :
	AbstractOperationFactory (parent)
{

}

AbstractImageOperation* FitOpFactory::factorizeOperation(QObject * operationParent) const {
	return new Fit(operationParent);
}

QString FitOpFactory::getToolTip() const {
	return tr("Resize the image to a fixed size in pixels with potentially some margins added");
}

} // namespace Operations
} // namespace Piwap
