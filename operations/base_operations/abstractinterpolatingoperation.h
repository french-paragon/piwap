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

#ifndef ABSTRACTINTERPOLATINGOPERATION_H
#define ABSTRACTINTERPOLATINGOPERATION_H

#include "operations/abstractimageoperation.h"

namespace Piwap {
namespace Operations {

class AbstractInterpolatingOperation : public AbstractImageOperation
{
	Q_OBJECT
public:
	Q_PROPERTY(QString interpolationMode READ interpolationMode WRITE setInterpolationMode NOTIFY interpolationModeChanged)
	Q_PROPERTY(QStringList possibleInterpolationModes READ interpModeStrings STORED false CONSTANT)

	enum InterpolationMode {//those are openCV compatible
		Nearest = Magick::PointFilter,
		Linear = Magick::TriangleFilter,
		Area = Magick::BoxFilter,
		Cubic = Magick::CatromFilter,
		Lanczos = Magick::LanczosFilter
		//TODO: look if we want to add different filters.
	};

	Q_ENUM(InterpolationMode)

	AbstractInterpolatingOperation(QObject* parent = nullptr);

	QString interpolationMode() const;
	void setInterpolationMode(QString mode);

	QStringList interpModeStrings() const;

Q_SIGNALS:

	void interpolationModeChanged(QString Mode);

protected:

	InterpolationMode _interpolation_mode;
};

} // namespace Operations
} // namespace Piwap

#endif // ABSTRACTINTERPOLATINGOPERATION_H
