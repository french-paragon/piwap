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

#include "abstractcompositeoperation.h"

#include <QMetaEnum>

namespace Piwap {
namespace Operations {

template<typename T_up, typename T_down>
T_down blendScalar(T_up p_up, T_down p_down, float rate, AbstractCompositeOperation::BlendingMode bMode) {

    float up = Image::cast_imgdata_scalar_to_type<float>(p_up);
    float down = Image::cast_imgdata_scalar_to_type<float>(p_down);

    float comp;

    switch (bMode) {
    case AbstractCompositeOperation::Normal:
        comp = up;
        break;
    case AbstractCompositeOperation::Under:
        comp = down;
        break;
    case AbstractCompositeOperation::Multiply:
        comp = up*down;
        break;
    case AbstractCompositeOperation::Screen:
        comp = 1 - (1-up)*(1-down);
        break;
    case AbstractCompositeOperation::Overlay:
        comp = (down < 0.5) ? 2*up*down : 1 - 2*(1-up)*(1-down);
        break;
    case AbstractCompositeOperation::HardLight:
        comp = (up < 0.5) ? 2*up*down : 1 - 2*(1-up)*(1-down);
        break;
    case AbstractCompositeOperation::Addition:
        comp = up + down;
        break;
    case AbstractCompositeOperation::Subtract:
        comp = down - up;
        break;
    case AbstractCompositeOperation::Difference:
        comp = std::abs(down - up);
        break;
    case AbstractCompositeOperation::Min:
        comp = std::min(up, down);
        break;
    case AbstractCompositeOperation::Max:
        comp = std::max(up, down);
        break;
    }

    comp = std::min<float>(1, std::max<float>(comp, 0));

    comp = rate*comp + (1-rate)*down;

    return Image::cast_imgdata_scalar_to_type<T_down>(comp);

}

std::optional<Image::ImageData> AbstractCompositeOperation::blend(Image::ImageData const& layerAbove,
                                                                  ColorModel aboveColorModel,
                                                                  Image::ImageData const& layerBelow,
                                                                  ColorModel belowColorModel,
                                                                  BlendingMode blendingMode,
                                                                  Crop::AnchorMode anchorMode,
                                                                  bool resize,
                                                                  AbstractInterpolatingOperation::InterpolationMode interpolationMode,
                                                                  float rate) {

    std::array<int,3> belowShape = Image::imageDataShape(layerBelow);
    std::array<int,3> aboveShape = Image::imageDataShape(layerAbove);

    if (resize) {

        if (belowShape == aboveShape) {
            return blend(layerAbove, aboveColorModel, layerBelow, belowColorModel, blendingMode, anchorMode, false, interpolationMode, rate);
        }

        QSize belowSize(belowShape[1], belowShape[0]);
        std::optional<Image::ImageData> resizedOpt = Resize::resize(layerAbove, belowSize, interpolationMode);

        if (!resizedOpt.has_value()) {
            return std::nullopt;
        }
        return blend(resizedOpt.value(), aboveColorModel, layerBelow, belowColorModel, blendingMode, anchorMode, false, interpolationMode, rate);
    }

    std::array<int,3> upIdxDelta {0,0,0};
    std::array<int,3> cropDelta {1,1,0};

    if (anchorMode & Crop::AnchorDir::DirBottom) {
        cropDelta[0] = 2;
    } else if (anchorMode & Crop::AnchorDir::DirTop) {
        cropDelta[0] = 0;
    }

    if (anchorMode & Crop::AnchorDir::DirRight) {
        cropDelta[0] = 2;
    } else if (anchorMode & Crop::AnchorDir::DirLeft) {
        cropDelta[0] = 0;
    }

    for (int i = 0; i < 2; i++) {
        int delta = aboveShape[i] - belowShape[i];
        delta /= 2;
        delta *= cropDelta[i];
        upIdxDelta[i] = -delta;
    }

    return std::visit([&layerAbove, &upIdxDelta, aboveColorModel, belowColorModel, rate, blendingMode] (auto const& below) -> Image::ImageData {

        using BelowScalarT = typename std::remove_reference<decltype(below)>::type::ScalarT;

        return std::visit([&below, &upIdxDelta, aboveColorModel, belowColorModel, rate, blendingMode] (auto const& above) -> Image::ImageData {

            using AboveScalarT = typename std::remove_reference<decltype(below)>::type::ScalarT;

            ImageArray<BelowScalarT> composed(below.shape());

            std::array<int,3> shape = composed.shape();
            std::array<int,3> abv_shape = above.shape();

            int alpha_idx = alphaChannelIndex(belowColorModel);
            int alpha_above_idx = alphaChannelIndex(aboveColorModel);
            int nCRaw = nChannels(belowColorModel);
            int nC = nCRaw;

            int nCRaw_above = nChannels(aboveColorModel);
            int nC_above = nCRaw_above;

            if (alpha_idx >= 0) {
                nC -= 1;
            }

            if (alpha_above_idx >= 0) {
                nC_above -= 1;
            }

            std::vector<int> channels;
            channels.reserve(nC);
            std::vector<int> channels_above_raw;
            channels_above_raw.reserve(nC_above);

            for (int c = 0; c < nCRaw; c++) {
                if (nC < nCRaw and c == alpha_idx) {
                    continue;
                }
                channels.push_back(c);
            }

            for (int c = 0; c < nCRaw_above; c++) {
                if (nC_above < nCRaw_above and c == alpha_above_idx) {
                    continue;
                }
                channels_above_raw.push_back(c);
            }

            std::vector<int> channels_above(channels.size());

            for (size_t c = 0; c < channels.size(); c++) {
                channels_above[c] = channels_above_raw[std::min(c,channels_above_raw.size()-1)];
            }

            float alpha = 1;
            float alpha_above = 1;

            for (int i = 0; i < shape[0]; i++) {
                int i_abv = i + upIdxDelta[0];
                for (int j = 0; j < shape[1]; j++) {
                    int j_abv = j + upIdxDelta[0];

                    if (i_abv < 0 or i_abv >= abv_shape[0] or j_abv < 0 or j_abv >= abv_shape[1]) {
                        for (int c = 0; c < shape[2]; c++) {
                            composed.atUnchecked(i,j,c) = below.valueUnchecked(i,j,c);
                        }
                        continue;
                    }

                    if (alpha_idx >= 0) {
                        alpha = below.valueUnchecked(i,j,alpha_idx);
                    }

                    if (alpha_idx >= 0) {
                        alpha_above = above.valueUnchecked(i_abv,j_abv,alpha_idx);
                    }

                    float convertedAlpha = std::min<float>(alpha+alpha_above,1);

                    if (alpha_idx >= 0) {
                        composed.atUnchecked(i,j,alpha_idx) = Image::cast_imgdata_scalar_to_type<BelowScalarT>(convertedAlpha);
                    }

                    float local_rate = alpha_above*rate;

                    for (int k = 0; k < channels.size(); k++) {
                        int c = channels[k];
                        int c_abv = channels_above[k];

                        BelowScalarT s_below = below.valueUnchecked(i,j,c);
                        AboveScalarT s_above = above.valueUnchecked(i_abv,j_abv,alpha_idx);

                        composed.atUnchecked(i,j,c) = blendScalar(s_above, s_below, local_rate, blendingMode);
                    }
                }
            }

            return composed;

        }, layerAbove);
    }, layerBelow);

}

AbstractCompositeOperation::AbstractCompositeOperation(QObject *parent) :
    AbstractImageOperation(parent),
    _blendingMode(Normal)
{

}

QString AbstractCompositeOperation::blendingMode() const {
    return QVariant::fromValue(_blendingMode).toString();
}
void AbstractCompositeOperation::setBlendingMode(QString const& mode) {
    QMetaEnum m_e = QMetaEnum::fromType<AbstractCompositeOperation::BlendingMode>();

    int index = m_e.keyToValue(mode.toStdString().c_str());

    if (index == -1) {
        return;
    }

    AbstractCompositeOperation::BlendingMode f = static_cast<AbstractCompositeOperation::BlendingMode>(index);

    if (f != _blendingMode) {
        _blendingMode = f;
        Q_EMIT blendingModeChanged();
    }
}
QStringList AbstractCompositeOperation::blendingModeStrings() const {
    QMetaEnum m_e = QMetaEnum::fromType<AbstractCompositeOperation::BlendingMode>();

    int n_keys = m_e.keyCount();
    QStringList ret;
    ret.reserve(n_keys);

    for (int i = 0; i < n_keys; i++) {
        ret << QString(m_e.key(i));
    }

    return ret;
}

} // namespace Operations
} // namespace Piwap
