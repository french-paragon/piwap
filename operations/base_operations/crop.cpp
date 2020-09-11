#include "crop.h"

#include <QMetaEnum>

#include "image/imageinfos.h"

namespace Piwap {
namespace Operations {

const QString Crop::cropOpTypeId = "piwapbase/crop";

Crop::Crop(QObject *parent) :
	AbstractImageOperation (parent),
	_width(600),
	_height(600),
	_dx(0),
	_dy(0),
	_anchor(Center),
	_allowGrowing(false),
	_bg(255, 255, 255)
{
	connect(this, &Crop::widthChanged, this, &AbstractImageOperation::hasBeenChanged);
	connect(this, &Crop::heightChanged, this, &AbstractImageOperation::hasBeenChanged);
	connect(this, &Crop::dxChanged, this, &AbstractImageOperation::hasBeenChanged);
	connect(this, &Crop::dyChanged, this, &AbstractImageOperation::hasBeenChanged);
	connect(this, &Crop::anchorChanged, this, &AbstractImageOperation::hasBeenChanged);
	connect(this, &Crop::allowGrowingChanged, this, &AbstractImageOperation::hasBeenChanged);
	connect(this, &Crop::bgChanged, this, &AbstractImageOperation::hasBeenChanged);

}

int Crop::doOperation(Magick::Image & image, ImageInfos * infos) const {

	int w = _width;
	int h = _height;

	if (w <= 0) {
		w = static_cast<int>(image.columns());
	}

	if (h <= 0) {
		h = static_cast<int>(image.rows());
	}

	int startXPos = _dx;
	int startYPos = _dy;

	if (_anchor & DirRight) {
		startXPos += static_cast<int>(image.columns()) - _width;
	} else if (!(_anchor & DirLeft)) {
		startXPos += (static_cast<int>(image.columns()) - _width)/2;
	}

	if (_anchor & DirBottom) {
		startYPos += static_cast<int>(image.rows()) - _height;
	} else if (!(_anchor & DirTop)) {
		startYPos += (static_cast<int>(image.rows()) - _height)/2;
	}

	int endXPos = startXPos + w;
	int endYPos = startYPos + h;

	if (endXPos < 0 or endYPos < 0 or startXPos >= static_cast<int>(image.columns()) or startYPos >= static_cast<int>(image.columns())) {
		setError(infos->originalFileName(), tr("Cropping area outside of image boundary, aborting operation !"));
		return 1;
	}

	int cropStartXPos = std::max(startXPos, 0);
	int cropStartYPos = std::max(startYPos, 0);

	int cropEndXPos = std::min(static_cast<int>(image.columns()), endXPos);
	int cropEndYPos = std::min(static_cast<int>(image.rows()), endYPos);

	int cropWidth = cropEndXPos - cropStartXPos;
	int cropHeight = cropEndYPos - cropStartYPos;

	image.crop(Magick::Geometry(static_cast<size_t>(cropWidth), static_cast<size_t>(cropHeight), cropStartXPos, cropStartYPos));

	if (_allowGrowing and (cropWidth < w || cropHeight < h)) {

		int r = _bg.red();
		int g = _bg.green();
		int b = _bg.blue();

		if (MAGICKCORE_QUANTUM_DEPTH == 16) {
			r <<= 8;
			g <<= 8;
			b <<= 8;
		}

		Magick::Color bg(static_cast<Magick::Quantum>(r), static_cast<Magick::Quantum>(g), static_cast<Magick::Quantum>(b));

		image.extent(Magick::Geometry(static_cast<size_t>(w), static_cast<size_t>(h), cropStartXPos - startXPos, cropStartYPos - startYPos), bg);

	}

	return 0;
}

QString Crop::typeId() const {
	return cropOpTypeId;
}

QString Crop::getOpName() const {
	return tr("Crop image");
}
QString Crop::getIconUrl() const {
	return "qrc:/icons/crop_image.svg";
}

QString Crop::getPropertiesEditorUrl() const {
	return "qrc:/qmlcomponents/editors/CropOpEditor.qml";
}

int Crop::width() const
{
	return _width;
}

void Crop::setWidth(int width)
{
	if (width != _width) {
		_width = width;
		Q_EMIT widthChanged(width);
	}
}

int Crop::height() const
{
	return _height;
}

void Crop::setHeight(int height)
{
	if (height != _height) {
		_height = height;
		Q_EMIT heightChanged(height);
	}
}

int Crop::dx() const
{
	return _dx;
}

void Crop::setDx(int dx)
{
	if (dx != _dx) {
		_dx = dx;
		Q_EMIT dxChanged(dx);
	}
}

int Crop::dy() const
{
	return _dy;
}

void Crop::setDy(int dy)
{
	if (dy != _dy) {
		_dy = dy;
		Q_EMIT dyChanged(dy);
	}
}

QString Crop::anchor() const {
	QString r = QVariant::fromValue(_anchor).toString();
	return r;
}
void Crop::setAnchor(QString const& anch) {

	QMetaEnum m_e = QMetaEnum::fromType<Crop::AnchorMode>();

	int index = m_e.keyToValue(anch.toStdString().c_str());

	if (index == -1) {
		return;
	}

	Crop::AnchorMode a = static_cast<Crop::AnchorMode>(index);

	if (a != _anchor) {
		_anchor = a;
		Q_EMIT anchorChanged(anchor());
	}
}

bool Crop::allowGrowing() const
{
	return _allowGrowing;
}

void Crop::setAllowGrowing(bool allowGrowing)
{
	if (allowGrowing != _allowGrowing) {
		_allowGrowing = allowGrowing;
		Q_EMIT allowGrowingChanged(allowGrowing);
	}
}

QColor Crop::bg() const
{
	return _bg;
}

void Crop::setBg(const QColor &bg)
{
	if (bg != _bg) {
		_bg = bg;
		Q_EMIT bgChanged(bg);
	}
}

QStringList Crop::anchorModeStrings() const {

	QMetaEnum m_e = QMetaEnum::fromType<Crop::AnchorMode>();

	int n_keys = m_e.keyCount();
	QStringList ret;
	ret.reserve(n_keys);

	for (int i = 0; i < n_keys; i++) {
		ret << QString(m_e.key(i));
	}

	return ret;
}

CropOpFactory::CropOpFactory(QObject * parent) : AbstractOperationFactory (parent)
{

}

AbstractImageOperation* CropOpFactory::factorizeOperation(QObject * operationParent ) const {
	return new Crop(operationParent);
}

QString CropOpFactory::getToolTip() const {
	return tr("Crop image to a given size.");
}

} // namespace Operations
} // namespace Piwap
