/*Piwap, or Picture Warping App, is a GUI to batch process images.

Copyright (C) 2026  Paragon<french.paragon@gmail.com>

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

#include "watermark.h"

#include <QMetaEnum>

#include "image/image.h"
#include "image/imageinfos.h"
#include "image/imageio.h"

namespace Piwap {
namespace Operations {

Watermark::Watermark(QObject *parent) :
    AbstractCompositeOperation(parent),
    _cached_watermark(ImageArray<uint8_t>())
{
    _rate = 0.1;
}

int Watermark::doOperation(Image* image, ImageInfos * infos) const {

    if (Image::imageDataIsEmpty(_cached_watermark)) {
        loadWatermark();
    }


    if (Image::imageDataIsEmpty(_cached_watermark)) {
        return 0; //no watermark loaded
    }

    Image::ImageData& imgData = image->imageData();

    constexpr AbstractInterpolatingOperation::InterpolationMode interpolationModel = AbstractInterpolatingOperation::Cubic;

    std::optional<Image::ImageData> processed = blend(_cached_watermark, _cached_watermark_color_model,
                                                      imgData, image->colorModel(),
                                                      _blendingMode, _anchor_mode, _resize, interpolationModel, _rate);

    if (!processed.has_value()) {
        setError(infos->originalFileName(), "Impossible to blend watermark on image");
        return 1;
    }

    image->imageData() = std::move(processed.value());

    return 0;
}

QString Watermark::typeId() const {
    return watermarkOpTypeId;
}

QString Watermark::getOpName() const {
    return tr("Watermark");
}
QString Watermark::getIconUrl() const {
    return "qrc:/icons/stamp_image.svg";
}

QString Watermark::getPropertiesEditorUrl() const {
    return "qrc:/qmlcomponents/editors/WatermarkOpEditor.qml";
}

QString Watermark::anchorMode() const {
    return QVariant::fromValue(_anchor_mode).toString();
}
void Watermark::setAnchorMode(QString const& mode) {
    QMetaEnum m_e = QMetaEnum::fromType<Crop::AnchorMode>();

    int index = m_e.keyToValue(mode.toStdString().c_str());

    if (index == -1) {
        return;
    }

    Crop::AnchorMode f = static_cast<Crop::AnchorMode>(index);

    if (f != _anchor_mode) {
        _anchor_mode = f;
        Q_EMIT anchorModeChanged();
    }
}
QStringList Watermark::possibleAnchorModes() const {
    QMetaEnum m_e = QMetaEnum::fromType<Crop::AnchorMode>();

    int n_keys = m_e.keyCount();
    QStringList ret;
    ret.reserve(n_keys);

    for (int i = 0; i < n_keys; i++) {
        ret << QString(m_e.key(i));
    }

    return ret;
}

bool Watermark::doResize() const {
    return _resize;
}
void Watermark::setDoResize(bool doResize) {
    if (doResize == _resize) {
        return;
    }
    _resize = doResize;
    Q_EMIT doResizeChanged();
}

float Watermark::rate() const {
    return _rate;
}
void Watermark::setRate(float rate) {
    if (_rate == rate) {
        return;
    }
    _rate = rate;
    Q_EMIT rateChanged();
}

QString Watermark::watermarkImageName() const
{
    return _watermarkImageName;
}

void Watermark::setWatermarkImageName(const QString &newWatermarkImageName)
{
    if (_watermarkImageName == newWatermarkImageName)
        return;
    _watermarkImageName = newWatermarkImageName;
    Q_EMIT watermarkImageNameChanged();
}

void Watermark::loadWatermark() const {
    ImageWithInfos data = openImage(qPrintable(_watermarkImageName), nullptr, nullptr);
    if (data.image != nullptr) {
        _cached_watermark = std::move(data.image->imageData());
        _cached_watermark_color_model = data.image->colorModel();
        delete data.image;
    }
    if (data.infos != nullptr) {
        delete data.infos;
    }
}

WatermarkOpFactory::WatermarkOpFactory(QObject * parent) : AbstractOperationFactory(parent) {

}

AbstractImageOperation* WatermarkOpFactory::factorizeOperation(QObject * operationParent) const {
    return new Watermark(operationParent);
}

QString WatermarkOpFactory::getToolTip() const {
    return tr("Watermark image");
}


} // namespace Operations
} // namespace Piwap
