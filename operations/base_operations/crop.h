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

#ifndef CROP_H
#define CROP_H

#include <QColor>

#include "operations/abstractimageoperation.h"
#include "operations/abstractoperationfactory.h"

#include <optional>

namespace Piwap {
namespace Operations {

class Crop : public AbstractImageOperation
{
	Q_OBJECT
public:

	static std::optional<Image::ImageData> crop(Image::ImageData const& inData, QSize const& cropSize, QPoint const& cropStart, QColor const& fillColor);

	static const QString cropOpTypeId;

	Q_PROPERTY(int width READ width WRITE setWidth NOTIFY widthChanged)
	Q_PROPERTY(int height READ height WRITE setHeight NOTIFY heightChanged)
	Q_PROPERTY(int dx READ dx WRITE setDx NOTIFY dxChanged)
	Q_PROPERTY(int dy READ dy WRITE setDy NOTIFY dyChanged)
	Q_PROPERTY(bool allowGrowing READ allowGrowing WRITE setAllowGrowing NOTIFY allowGrowingChanged)
	Q_PROPERTY(QString anchor READ anchor WRITE setAnchor NOTIFY anchorChanged)

	Q_PROPERTY(QColor bg READ bg WRITE setBg NOTIFY bgChanged)

	Q_PROPERTY(QStringList possibleAnchors READ anchorModeStrings STORED false CONSTANT)

	enum AnchorDir {
		NoDir = 0,
		DirTop = 0x01,
		DirBottom = 0x02,
		DirLeft = 0x04,
		DirRight = 0x08
	};

	enum AnchorMode {
		TopLeft = DirTop|DirLeft,
		Top = DirTop,
		TopRight = DirTop|DirRight,
		Left = DirLeft,
		Center = NoDir,
		Right = DirRight,
		BottomLeft = DirBottom|DirLeft,
		Bottom = DirBottom,
		BottomRight = DirBottom|DirRight
	};

	Q_ENUM(AnchorMode)

	explicit Crop(QObject *parent = nullptr);

	virtual int doOperation(Image *image, ImageInfos * infos) const override;

	virtual QString typeId() const override;

	virtual QString getOpName() const override;
	virtual QString getIconUrl() const override;

	virtual QString getPropertiesEditorUrl() const override;

	int width() const;
	void setWidth(int width);

	int height() const;
	void setHeight(int height);

	int dx() const;
	void setDx(int dx);

	int dy() const;
	void setDy(int dy);

	QString anchor() const;
	void setAnchor(QString const& anchor);

	bool allowGrowing() const;
	void setAllowGrowing(bool allowGrowing);

	QColor bg() const;
	void setBg(const QColor &bg);

	QStringList anchorModeStrings() const;

Q_SIGNALS:

	void widthChanged(int width);
	void heightChanged(int height);
	void dxChanged(int dx);
	void dyChanged(int dy);
	void anchorChanged(QString anchor);
	void allowGrowingChanged(bool allowGrowing);
	void bgChanged(QColor bg);

protected:

	int _width;
	int _height;

	int _dx;
	int _dy;

	AnchorMode _anchor;

	bool _allowGrowing;

	QColor _bg;
};

class CropOpFactory : public AbstractOperationFactory {
	Q_OBJECT
public :
	explicit CropOpFactory(QObject * parent = nullptr);

	virtual AbstractImageOperation* factorizeOperation(QObject * operationParent = nullptr) const;

	virtual QString getToolTip() const;
};

} // namespace Operations
} // namespace Piwap

#endif // CROP_H
