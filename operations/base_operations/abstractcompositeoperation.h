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

#ifndef ABSTRACTCOMPOSITEOPERATION_H
#define ABSTRACTCOMPOSITEOPERATION_H

#include "operations/abstractimageoperation.h"
#include "operations/base_operations/crop.h"
#include "operations/base_operations/resize.h"

namespace Piwap {
namespace Operations {

class AbstractCompositeOperation : public AbstractImageOperation
{
    Q_OBJECT
public:

    Q_PROPERTY(QString blendingMode READ blendingMode WRITE setBlendingMode NOTIFY blendingModeChanged)
    Q_PROPERTY(QStringList possibleBlendingModes READ blendingModeStrings STORED false CONSTANT)

    enum BlendingMode {
        Normal,
        Under,
        Multiply,
        Screen,
        Overlay,
        HardLight,
        Addition,
        Subtract,
        Difference,
        Min,
        Max
        //TODO: look if we want to add more blending modes.
    };

    Q_ENUM(BlendingMode)

    static std::optional<Image::ImageData> blend(Image::ImageData const& layerAbove, ColorModel aboveColorModel,
                                                 Image::ImageData const& layerBelow, ColorModel belowColorModel,
                                                 BlendingMode blendingMode,
                                                 Crop::AnchorMode anchorMode = Crop::AnchorMode::Center,
                                                 bool resize = false,
                                                 AbstractInterpolatingOperation::InterpolationMode interpolationMode =
                                                 AbstractInterpolatingOperation::InterpolationMode::Cubic,
                                                 float rate = 1.0);

    explicit AbstractCompositeOperation(QObject* parent = nullptr);

    QString blendingMode() const;
    void setBlendingMode(QString const& mode);
    QStringList blendingModeStrings() const;



Q_SIGNALS:

    void blendingModeChanged();

protected:

    BlendingMode _blendingMode;
};

} // namespace Operations
} // namespace Piwap

#endif // ABSTRACTCOMPOSITEOPERATION_H
