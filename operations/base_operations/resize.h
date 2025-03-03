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

#ifndef RESIZE_H
#define RESIZE_H

#include "operations/abstractimageoperation.h"
#include "operations/abstractoperationfactory.h"
#include "operations/base_operations/abstractinterpolatingoperation.h"

#include <optional>

namespace Piwap {
namespace Operations {

class Resize : public AbstractInterpolatingOperation
{
	Q_OBJECT
public:

	static std::optional<Image::ImageData> resize(Image::ImageData const& inData, QSize const& newSize, InterpolationMode interpolationMode);

	static const QString resizeOpTypeId;

	Q_PROPERTY(int pix_x READ pix_x WRITE setPix_x NOTIFY pix_x_changed)
	Q_PROPERTY(int pix_y READ pix_y WRITE setPix_y NOTIFY pix_y_changed)
	Q_PROPERTY(QString fitMode READ fitMode WRITE setFitMode NOTIFY fitModeChanged)

	Q_PROPERTY(QStringList possibleFitModes READ fitModeStrings STORED false CONSTANT)

	explicit Resize(QObject *parent = nullptr);

	enum FitMode {
		Smaller = 0, //both dimensions are computed so that one is equal to set parameter an the other is smaller, aspect ratio is maintained
		Bigger = 1, //both dimensions are computed so that one is equal to the set parameter and the other is bigger, aspect ratio is maintained
		Average = 2, //both dimensions are computed so that one is smaller and the other bigger than the set parameter. The values are computed such that the image is in average the closest size possible to the desired size while preserving aspect ratio.
		Deform = 3, //The image is deformed such that both dimensions are equal to the set parameter
	};

	Q_ENUM(FitMode)

	virtual int doOperation(Image *image, ImageInfos * infos) const;

	virtual QString typeId() const;

	virtual QString getOpName() const;
	virtual QString getIconUrl() const;

	virtual QString getPropertiesEditorUrl() const;


	int pix_x() const;
	void setPix_x(int pix_x);

	int pix_y() const;
	void setPix_y(int pix_y);

	QString fitMode() const;
	void setFitMode(QString mode);

	QStringList fitModeStrings() const;

Q_SIGNALS:

	void pix_x_changed(int newVal);
	void pix_y_changed(int newVal);
	void fitModeChanged(QString FitMode);

public Q_SLOTS:

protected:

	int _pix_x;
	int _pix_y;
	FitMode _fit_mode;

};

class ResizeOpFactory : public AbstractOperationFactory {
	Q_OBJECT
public :
	explicit ResizeOpFactory(QObject * parent = nullptr);

	virtual AbstractImageOperation* factorizeOperation(QObject * operationParent = nullptr) const;

	virtual QString getToolTip() const;
};

} // namespace Operations
} // namespace Piwap

#endif // RESIZE_H
