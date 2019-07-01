#include "resize.h"

#include <QVariant>
#include <QMetaEnum>
#include <QDebug>

namespace Piwap {
namespace Operations {

const QString Resize::resizeOpTypeId = "piwapbase/resize";

Resize::Resize(QObject *parent) :
	AbstractImageOperation(parent),
	_pix_x(800),
	_pix_y(800),
	_fit_mode(Bigger),
	_interpolation_mode(Cubic)
{

}

int Resize::doOperation(cv::Mat & image, ImageInfos *infos) const {

	Q_UNUSED(infos);

	if (_pix_x <= 0 && _pix_y <= 0) {
		return 0;
	}

	int newWidth;
	int newHeight;

	float aspectRatio = static_cast<float>(image.cols) / static_cast<float>(image.rows);

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
			float a = static_cast<float>(image.cols*_pix_x + image.rows*_pix_y) / static_cast<float>(image.cols*image.cols + image.rows*image.rows);
			newWidth = static_cast<int>(roundf(image.cols*a));
			newHeight = static_cast<int>(roundf(image.rows*a));
		}
			break;

		}

	}

	cv::Mat newIm(newHeight, newWidth, image.type());

	cv::resize(image, newIm, newIm.size(), 0, 0, _interpolation_mode);

	image = newIm;

	return 0;

}

QString Resize::typeId() const {
	return resizeOpTypeId;
}

QString Resize::getOpName() const {
	return tr("Resize image");
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

QString Resize::interpolationMode() const {
	return QVariant::fromValue(_interpolation_mode).toString();
}

void Resize::setInterpolationMode(QString mode) {

	QMetaEnum m_e = QMetaEnum::fromType<Resize::InterpolationMode>();

	int index = m_e.keyToValue(mode.toStdString().c_str());

	if (index == -1) {
		return;
	}

	Resize::InterpolationMode f = static_cast<Resize::InterpolationMode>(index);

	if (f != _interpolation_mode) {
		_interpolation_mode = f;
		Q_EMIT interpolationModeChanged(interpolationMode());
		qDebug() << "Changed interpolation mode: " << interpolationMode();
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
QStringList Resize::interpModeStrings() const {

	QMetaEnum m_e = QMetaEnum::fromType<Resize::InterpolationMode>();

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

} // namespace Operations
} // namespace Piwap
