#include "fit.h"

#include <QDebug>
#include <QJsonObject>

#include <Magick++/Color.h>

namespace Piwap {
namespace Operations {

const QString Fit::fitOpTypeId = "piwapbase/fit";

Fit::Fit(QObject *parent) :
	AbstractInterpolatingOperation(parent),
	_pix_x(800),
	_pix_y(800),
	_bg(255, 255, 255)
{
	connect(this, &Fit::pix_x_changed, this, &AbstractImageOperation::hasBeenChanged);
	connect(this, &Fit::pix_y_changed, this, &AbstractImageOperation::hasBeenChanged);
	connect(this, &Fit::bg_changed, this, &AbstractImageOperation::hasBeenChanged);
}

int Fit::doOperation(Magick::Image &image, ImageInfos * infos) const {

	Q_UNUSED(infos);

	if (_pix_x <= 0 && _pix_y <= 0) {
		return 0;
	}

	int newWidth;
	int newHeight;

	float aspectRatio = static_cast<float>(image.size().width()) / static_cast<float>(image.size().height());

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

	image.filterType(static_cast<Magick::FilterTypes>(_interpolation_mode));
	image.resize(Magick::Geometry(static_cast<size_t>(newWidth), static_cast<size_t>(newHeight)));

	if (newWidth != _pix_x || newHeight != _pix_y) {

		int r = _bg.red();
		int g = _bg.green();
		int b = _bg.blue();

		if (MAGICKCORE_QUANTUM_DEPTH == 16) {
			r <<= 8;
			g <<= 8;
			b <<= 8;
		}


		Magick::Color bg(static_cast<Magick::Quantum>(r), static_cast<Magick::Quantum>(g), static_cast<Magick::Quantum>(b));

		image.extent(Magick::Geometry(static_cast<size_t>(_pix_x), static_cast<size_t>(_pix_y)), bg, Magick::CenterGravity);

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
