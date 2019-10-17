#include "fit.h"

#include <QDebug>

namespace Piwap {
namespace Operations {

const QString Fit::fitOpTypeId = "piwapbase/fit";

Fit::Fit(QObject *parent) :
	AbstractInterpolatingOperation(parent),
	_pix_x(100),
	_pix_y(100),
	_bg(255, 255, 255)
{

}

int Fit::doOperation(cv::Mat & image, ImageInfos * infos) const {

	Q_UNUSED(infos);

	if (_pix_x <= 0 && _pix_y <= 0) {
		return 0;
	}

	int newWidth;
	int newHeight;

	float aspectRatio = static_cast<float>(image.cols) / static_cast<float>(image.rows);

	float targetAspectRatio = static_cast<float>(_pix_x) / static_cast<float>(_pix_y);

	if (aspectRatio > targetAspectRatio) {

		newWidth = _pix_x;
		newHeight = static_cast<int>(roundf(_pix_x/aspectRatio));

	} else if (aspectRatio < targetAspectRatio) {

		newWidth = static_cast<int>(roundf(_pix_y*aspectRatio));
		newHeight = _pix_y;

	} else {

		newWidth = _pix_x;
		newHeight = _pix_y;

	}

	cv::Scalar bg(_bg.blue(), _bg.green(), _bg.red());

	cv::Mat newIm(newHeight, newWidth, image.type());
	cv::resize(image, newIm, newIm.size(), 0, 0, _interpolation_mode);

	if (newWidth != _pix_x || newHeight != _pix_y) {

		int im_depth = image.depth();

		int r = _bg.red();
		int g = _bg.green();
		int b = _bg.blue();

		if (im_depth == CV_16U) {
			r <<= 8;
			g <<= 8;
			b <<= 8;
		}

		double rf = _bg.redF();
		double gf = _bg.greenF();
		double bf = _bg.blueF();

		cv::Scalar bg;

		if (im_depth == CV_8U || im_depth == CV_16U) {
			bg = cv::Scalar(b, g, r);
		} else if (im_depth == CV_32F) {
			bg = cv::Scalar(bf, gf, rf);
		}

		cv::Mat bgImg(_pix_y, _pix_x, image.type());

		int top = (_pix_y-newHeight)/2;
		int bottom = _pix_y-newHeight - top;

		int left = (_pix_x-newWidth)/2;
		int right = _pix_x-newWidth - left;

		cv::copyMakeBorder(newIm, bgImg, top, bottom, left, right, cv::BORDER_CONSTANT, bg);

		newIm = bgImg;

	}

	image = newIm;

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
