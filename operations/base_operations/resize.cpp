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

#include "resize.h"

#include <QVariant>
#include <QMetaEnum>
#include <QDebug>

#include "image/imageinfos.h"

namespace Piwap {
namespace Operations {

const QString Resize::resizeOpTypeId = "piwapbase/resize";


template<typename ArrayT, typename Interpolator>
static ArrayT resizeImpl(ArrayT const& img, QSize const& newSize) {

	ArrayT ret(newSize.height(), newSize.width(), img.shape()[2]);

	float scaleHeight = double(img.shape()[0]) / double(newSize.height());
	float scaleWidth = double(img.shape()[1]) / double(newSize.width());

	std::array<float,2> filterRadiuses = {scaleHeight, scaleWidth};

	for (int i = 0; i < newSize.height(); i++) {
		for (int j = 0; j < newSize.width(); j++) {
			for (int c = 0; c < img.shape()[2]; c++) {

				ret.atUnchecked(i,j,c) = Interpolator::interpolate({scaleHeight*i, scaleWidth*j, float(c)}, img, filterRadiuses);

			}
		}
	}

	return ret;

}


std::optional<Image::ImageData> Resize::resize(Image::ImageData const& inData, QSize const& newSize, InterpolationMode interpolationMode) {

	switch(interpolationMode) {
	case Nearest: {
		using IKer = InterpolatorKernel<Nearest>;
		return std::visit([&newSize] (auto const& data) -> Image::ImageData {
			return resizeImpl<std::decay_t<decltype(data)>,IKer>(data, newSize); },
		inData);
		}
	case Linear: {
		using IKer = InterpolatorKernel<Linear>;
		return std::visit([&newSize] (auto const& data) -> Image::ImageData {
			return resizeImpl<std::decay_t<decltype(data)>,IKer>(data, newSize); },
		inData);
		}
	case Area: {
		using IKer = InterpolatorKernel<Area>;
		return std::visit([&newSize] (auto const& data) -> Image::ImageData {
			return resizeImpl<std::decay_t<decltype(data)>,IKer>(data, newSize); },
		inData);
		}
	case Cubic: {
		using IKer = InterpolatorKernel<Cubic>;
		return std::visit([&newSize] (auto const& data) -> Image::ImageData {
			return resizeImpl<std::decay_t<decltype(data)>,IKer>(data, newSize); },
		inData);
		}
	case Lanczos: {
		using IKer = InterpolatorKernel<Lanczos>;
		return std::visit([&newSize] (auto const& data) -> Image::ImageData {
			return resizeImpl<std::decay_t<decltype(data)>,IKer>(data, newSize); },
		inData);
		}
	default:
		break;
	}

	return std::nullopt;
}

Resize::Resize(QObject *parent) :
	AbstractInterpolatingOperation(parent),
	_pix_x(800),
	_pix_y(800),
	_fit_mode(Bigger)
{
	connect(this, &Resize::pix_x_changed, this, &AbstractImageOperation::hasBeenChanged);
	connect(this, &Resize::pix_y_changed, this, &AbstractImageOperation::hasBeenChanged);
	connect(this, &Resize::fitModeChanged, this, &AbstractImageOperation::hasBeenChanged);
}

int Resize::doOperation(Image* image, ImageInfos *infos) const {

	Q_UNUSED(infos);

	if (_pix_x <= 0 && _pix_y <= 0) {
		return 0;
	}

	int newWidth;
	int newHeight;

	float aspectRatio = static_cast<float>(image->width()) / static_cast<float>(image->height());

	if (_pix_x > 0 && _pix_y <= 0) {

		newWidth = _pix_x;
		newHeight = static_cast<int>(roundf(_pix_x/aspectRatio));

	} else if (_pix_y > 0 && _pix_x <= 0) {

		newWidth = static_cast<int>(roundf(_pix_y*aspectRatio));
		newHeight = _pix_y;

	} else {

		switch (_fit_mode) {
		case Deform :
			newWidth = _pix_x;
			newHeight = _pix_y;
			break;
		case Bigger :
		case Smaller:
		{
			float targetAspectRatio = static_cast<float>(_pix_x) / static_cast<float>(_pix_y);

			if (aspectRatio > targetAspectRatio) {

				if (_fit_mode == Bigger) {
					newWidth = static_cast<int>(roundf(_pix_y*aspectRatio));
					newHeight = _pix_y;
				} else { //if (_fit_mode == Smaller)
					newWidth = _pix_x;
					newHeight = static_cast<int>(roundf(_pix_x/aspectRatio));
				}

			} else if (aspectRatio < targetAspectRatio) {

				if (_fit_mode == Bigger) {
					newWidth = _pix_x;
					newHeight = static_cast<int>(roundf(_pix_x/aspectRatio));
				} else { //if (_fit_mode == Smaller)
					newWidth = static_cast<int>(roundf(_pix_y*aspectRatio));
					newHeight = _pix_y;
				}

			} else {
				newWidth = _pix_x;
				newHeight = _pix_y;
			}

		}

			break;

		case Average :
		{
			//(a s.t. (im.cols*a - _pix_x)^2 + (im.rows*a - _pix_y)^2 -> min)
			// derivative: 2*im.cols*(im.cols*a - _pix_x) + 2*im.rows(im.rows*a - _pix_y) = 0
			// (im.cols^2 + im.rows^2)*a = im.cols*_pix_x + im.rows*_pix_y
			// a = (im.cols*_pix_x + im.rows*_pix_y)/(im.cols^2 + im.rows^2)
			float a = static_cast<float>(image->width()*static_cast<size_t>(_pix_x) + image->height()*static_cast<size_t>(_pix_y)) /
					static_cast<float>(image->width()*image->width() + image->height()*image->height());
			newWidth = static_cast<int>(roundf(image->width()*a));
			newHeight = static_cast<int>(roundf(image->height()*a));
		}
			break;

		}

	}

	QSize newSize(static_cast<size_t>(newWidth), static_cast<size_t>(newHeight));

	auto resizedOpt = resize(image->imageData(), newSize, _interpolation_mode);

	if (!resizedOpt.has_value()) {
		setError(infos->originalFileName(), "Invalid resize operation");
		return 1;
	}

	image->imageData() = std::move(resizedOpt.value());

	return 0;

}

QString Resize::typeId() const {
	return resizeOpTypeId;
}

QString Resize::getOpName() const {
	return tr("Resize image");
}

QString Resize::getIconUrl() const {
	return "qrc:/icons/scale_image.svg";
}

QString Resize::getPropertiesEditorUrl() const {
	return "qrc:/qmlcomponents/editors/ResizeOpEditor.qml";
}

int Resize::pix_x() const
{
	return _pix_x;
}

void Resize::setPix_x(int pix_x)
{
	if (pix_x != _pix_x) {
		_pix_x = pix_x;
		Q_EMIT pix_x_changed(_pix_x);
		qDebug() << "Changed pix x value: " <<_pix_x;
	}
}

int Resize::pix_y() const
{
	return _pix_y;
}

void Resize::setPix_y(int pix_y)
{
	if (pix_y != _pix_y) {
		_pix_y = pix_y;
		Q_EMIT pix_y_changed(_pix_y);
		qDebug() << "Changed pix y value: " <<_pix_y;
	}
}

QString Resize::fitMode() const {

	return QVariant::fromValue(_fit_mode).toString();

}

void Resize::setFitMode(QString mode) {

	QMetaEnum m_e = QMetaEnum::fromType<Resize::FitMode>();

	int index = m_e.keyToValue(mode.toStdString().c_str());

	if (index == -1) {
		return;
	}

	Resize::FitMode f = static_cast<Resize::FitMode>(index);

	if (f != _fit_mode) {
		_fit_mode = f;
		Q_EMIT fitModeChanged(fitMode());
		qDebug() << "Changed fit mode: " << fitMode();
	}

}

QStringList Resize::fitModeStrings() const {

	QMetaEnum m_e = QMetaEnum::fromType<Resize::FitMode>();

	int n_keys = m_e.keyCount();
	QStringList ret;
	ret.reserve(n_keys);

	for (int i = 0; i < n_keys; i++) {
		ret << QString(m_e.key(i));
	}

	return ret;

}


ResizeOpFactory::ResizeOpFactory(QObject * parent) : AbstractOperationFactory (parent)
{

}

AbstractImageOperation* ResizeOpFactory::factorizeOperation(QObject * operationParent) const {
	return new Resize(operationParent);
}
QString ResizeOpFactory::getToolTip() const {
	return tr("Resize the image to a fixed size in pixels with some aspect ratio constrains");
}

} // namespace Operations
} // namespace Piwap
