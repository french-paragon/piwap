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

#ifndef WATERMARK_H
#define WATERMARK_H

#include "./abstractcompositeoperation.h"

namespace Piwap {
namespace Operations {

class Watermark : public AbstractCompositeOperation
{
    Q_OBJECT
public:

    static constexpr char watermarkOpTypeId[] = "piwapbase/watermark";

    Q_PROPERTY(QString watermarkImageName READ watermarkImageName WRITE setWatermarkImageName NOTIFY watermarkImageNameChanged FINAL)
    Q_PROPERTY(QString anchorMode READ anchorMode WRITE setAnchorMode NOTIFY anchorModeChanged)
    Q_PROPERTY(QStringList possibleAnchorModes READ possibleAnchorModes STORED false CONSTANT)
    Q_PROPERTY(bool doResize READ doResize WRITE setDoResize NOTIFY doResizeChanged)
    Q_PROPERTY(float rate READ rate WRITE setRate NOTIFY rateChanged)

    explicit Watermark(QObject* parent = nullptr);

    virtual int doOperation(Image* image, ImageInfos * infos) const override;

    virtual QString typeId() const override;

    virtual QString getOpName() const override;
    virtual QString getIconUrl() const override;

    virtual QString getPropertiesEditorUrl() const override;

    QString anchorMode() const;
    void setAnchorMode(QString const& mode);
    QStringList possibleAnchorModes() const;

    bool doResize() const;
    void setDoResize(bool doResize);

    float rate() const;
    void setRate(float rate);

    QString watermarkImageName() const;
    void setWatermarkImageName(const QString &newWatermarkImageName);

Q_SIGNALS:

    void anchorModeChanged();
    void doResizeChanged();
    void rateChanged();
    void watermarkImageNameChanged();

protected:

    void loadWatermark() const;

    mutable Image::ImageData _cached_watermark;
    mutable ColorModel _cached_watermark_color_model;
    QString _watermarkImageName;
    Crop::AnchorMode _anchor_mode;
    bool _resize;
    float _rate;

private:
};

class WatermarkOpFactory : public AbstractOperationFactory {
    Q_OBJECT
public :
    explicit WatermarkOpFactory(QObject * parent = nullptr);

    virtual AbstractImageOperation* factorizeOperation(QObject * operationParent = nullptr) const;

    virtual QString getToolTip() const;
};

} // namespace Operations
} // namespace Piwap

#endif // WATERMARK_H
