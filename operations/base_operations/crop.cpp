#include "crop.h"

#include <QMetaEnum>

#include <QPoint>
#include <QSize>

#include "image/imageinfos.h"
#include "image/color.h"

namespace Piwap {
namespace Operations {

const QString Crop::cropOpTypeId = "piwapbase/crop";

template<typename ArrayT>
std::optional<Image::ImageData> cropImpl(ArrayT const& inData,
										 QSize const& cropSize,
										 QPoint const& cropStart,
										 QColor const& fillColor) {

	using ScalarT = typename ArrayT::ScalarT;

	int nChannels = inData.shape()[2];

	int initialH = inData.shape()[0];
	int initialW = inData.shape()[1];

	ImageArray<ScalarT> cropped(cropSize.height(), cropSize.width(), nChannels);

	int di = std::max(0, cropSize.height()-initialH)/2;
	int dj = std::max(0, cropSize.width()-initialW)/2;

	float r = fillColor.redF();
	float g = fillColor.greenF();
	float b = fillColor.blueF();

	std::vector<float> color;

	if (nChannels == 3) {

		color = {r,g,b};

	} else if (nChannels == 4) {
		float a = fillColor.alphaF();

		color = {r,g,b,a};
	} else {
		float gray = (r + g + b)/3;
		color = {gray};
	}

	std::vector<ScalarT> convertedFillColor(nChannels);

	for (int c = 0; c < nChannels; c++) {
		convertedFillColor[c] = convertTypedWhiteLevel<ScalarT>(color[std::max<size_t>(c, color.size()-1)]);
	}

	for (int i = 0; i < cropped.shape()[0]; i++) {
		for (int j = 0; j < cropped.shape()[1]; j++) {
			for (int c = 0; c < cropped.shape()[2]; c++) {

				ScalarT val = convertedFillColor[c];

				if (i >= di and i - di < initialH) {
					if (j >= dj and j - dj < initialW) {
						val = inData.valueUnchecked(i-di,j-dj,c);
					}
				}

				cropped.atUnchecked(i,j,c) = val;

			}
		}
	}

	return cropped;
}

std::optional<Image::ImageData> Crop::crop(Image::ImageData const& inData,
										   QSize const& cropSize,
										   QPoint const& cropStart,
										   QColor const& fillColor) {


	return std::visit([& cropSize, & cropStart, & fillColor] (auto const& inData) {
		return cropImpl(inData, cropSize, cropStart, fillColor);}, inData);

}

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

int Crop::doOperation(Image* image, ImageInfos * infos) const {

	int w = _width;
	int h = _height;

	if (w <= 0) {
		w = static_cast<int>(image->width());
	}

	if (h <= 0) {
		h = static_cast<int>(image->height());
	}

	int startXPos = _dx;
	int startYPos = _dy;

	if (_anchor & DirRight) {
		startXPos += static_cast<int>(image->width()) - _width;
	} else if (!(_anchor & DirLeft)) {
		startXPos += (static_cast<int>(image->width()) - _width)/2;
	}

	if (_anchor & DirBottom) {
		startYPos += static_cast<int>(image->height()) - _height;
	} else if (!(_anchor & DirTop)) {
		startYPos += (static_cast<int>(image->height()) - _height)/2;
	}

	int endXPos = startXPos + w;
	int endYPos = startYPos + h;

	if (endXPos < 0 or endYPos < 0 or startXPos >= static_cast<int>(image->width()) or startYPos >= static_cast<int>(image->width())) {
		setError(infos->originalFileName(), tr("Cropping area outside of image boundary, aborting operation !"));
		return 1;
	}

	int cropStartXPos = std::max(startXPos, 0);
	int cropStartYPos = std::max(startYPos, 0);

	int cropEndXPos = std::min(static_cast<int>(image->width()), endXPos);
	int cropEndYPos = std::min(static_cast<int>(image->height()), endYPos);

	int cropWidth = cropEndXPos - cropStartXPos;
	int cropHeight = cropEndYPos - cropStartYPos;

	if (!_allowGrowing) {
		cropWidth = std::min(cropWidth, w);
		cropHeight = std::min(cropHeight, h);
	}

	std::optional<Image::ImageData> croppedOpt = Crop::crop(image->imageData(), QSize(cropWidth, cropHeight), QPoint(cropStartXPos, cropStartYPos), _bg);

	if (!croppedOpt.has_value()) {
		setError(infos->originalFileName(), tr("Invalid crop operation"));
		return 1;
	}

	image->imageData() = std::move(croppedOpt.value());

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
