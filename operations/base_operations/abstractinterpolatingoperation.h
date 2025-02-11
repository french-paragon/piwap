/*Piwap, or Picture Warping App, is a GUI to batch process images.

Copyright (C) 2019-2024  Paragon<french.paragon@gmail.com>

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
		Nearest = 0,
		Linear = 1,
		Area = 2,
		Cubic = 3,
		Lanczos = 4
		//TODO: look if we want to add different filters.
	};

	/*!
	 * \brief The InterpolatorKernel class represent a generic interpolator based on a given InterpolationMode
	 */
	template<AbstractInterpolatingOperation::InterpolationMode interpolatorMode>
	struct InterpolatorKernel {

		/*!
		 * \brief interpolate interpolate the image data
		 * \param coordinate the coordinate where to interpolate at
		 * \param imgData the image data to use
		 * \param filterRadius radiuses used by certain interpolation kernel (e.g. Area)
		 */
		template <typename T>
		static T interpolate(std::array<float,3> const& coordinate, ImageArray<T> const& imgData,
					  std::array<float,2> const& filterRadius = {0.5,0.5}) {

			if (imgData.empty()) {
				return T();
			}

			if constexpr (interpolatorMode == Nearest) {

				int i = std::clamp<int>(std::round(coordinate[0]),0,imgData.shape()[0]-1);
				int j = std::clamp<int>(std::round(coordinate[1]),0,imgData.shape()[1]-1);
				int c = std::clamp<int>(std::round(coordinate[2]),0,imgData.shape()[2]-1);

				return imgData.valueUnchecked(i,j,c);
			}

			if constexpr (interpolatorMode == Linear) {

				T ret = 0;

				int c = std::clamp<int>(std::round(coordinate[2]),0,imgData.shape()[2]-1);

				for (int di = 0; di < 2; di++) {
					for (int dj = 0; dj < 2; dj++) {

						int i0 = std::floor(coordinate[0] + di);
						int j0 = std::floor(coordinate[1] + dj);

						int i = std::clamp<int>(i0,0,imgData.shape()[0]-1);
						int j = std::clamp<int>(j0,0,imgData.shape()[1]-1);

						float weight_i = 1 - std::abs(i0 - coordinate[0]);
						float weight_j = 1 - std::abs(j0 - coordinate[1]);

						ret += weight_i*weight_j*imgData.valueUnchecked(i,j,c);
					}
				}

				return ret;
			}

			if constexpr (interpolatorMode == Area) {

				T ret = 0;

				int c = std::clamp<int>(std::round(coordinate[2]),0,imgData.shape()[2]-1);

				float radius_i = std::max<float>(1,filterRadius[0]);
				float radius_j = std::max<float>(1,filterRadius[1]);

				double area = 4*radius_i*radius_j;

				int ri = std::max<int>(1, std::ceil(filterRadius[0]));
				int rj = std::max<int>(1, std::ceil(filterRadius[1]));

				for (int di = -ri; di <= ri; di++) {
					for (int dj = -rj; dj <= rj; dj++) {

						int i0 = std::floor(coordinate[0] + di);
						int j0 = std::floor(coordinate[1] + dj);

						int i = std::clamp<int>(i0,0,imgData.shape()[0]-1);
						int j = std::clamp<int>(j0,0,imgData.shape()[1]-1);

						float side_i = std::min(i0+0.5f,coordinate[0] + radius_i) - std::max(i0-0.5f,coordinate[0] - radius_i);
						float side_j = std::min(j0+0.5f,coordinate[1] + radius_j) - std::max(j0-0.5f,coordinate[1] - radius_j);

						float weight = std::max<float>(0,side_i*side_j / area);

						ret += weight*imgData.valueUnchecked(i,j,c);
					}
				}

				return ret;
			}

			if constexpr (interpolatorMode == Cubic) {

				auto bicubicKernel = [] (float const& v) -> float {
					constexpr float a = -0.5;
					float x = std::fabs(v);
					if (x < 1) {
						return (a+2)*x*x*x - (a+3)*x*x + 1;
					} else if (x < 2) {
						return a*x*x*x - 5*a*x*x + 8*a*x - 4*a;
					}
					return 0;
				};

				T ret = 0;

				int c = std::clamp<int>(std::round(coordinate[2]),0,imgData.shape()[2]-1);

				constexpr int ri = 2;
				constexpr int rj = 2;

				for (int di = -ri; di <= ri; di++) {
					for (int dj = -rj; dj <= rj; dj++) {

						int i0 = std::floor(coordinate[0] + di);
						int j0 = std::floor(coordinate[1] + dj);

						int xi = i0 - coordinate[0];
						int xj = j0 - coordinate[1];

						int i = std::clamp<int>(i0,0,imgData.shape()[0]-1);
						int j = std::clamp<int>(j0,0,imgData.shape()[1]-1);

						float weight = bicubicKernel(xi)*bicubicKernel(xj);

						ret += weight*imgData.valueUnchecked(i,j,c);
					}
				}

				return ret;
			}

			if constexpr (interpolatorMode == Lanczos) {
				constexpr int filterOrder = 4;

				auto lanczosKernel = [] (float const& v) -> float {
					float x = std::fabs(v);
					if (x < 1e-5) {
						return 1;
					} else if (x < filterOrder) {
						return filterOrder*std::sin(M_PI*x)*std::sin(M_PI*x/filterOrder)/(M_PIf32*M_PIf32*x*x);
					}
					return 0;
				};

				T ret = 0;

				int c = std::clamp<int>(std::round(coordinate[2]),0,imgData.shape()[2]-1);

				constexpr int ri = filterOrder;
				constexpr int rj = filterOrder;

				for (int di = -ri; di <= ri; di++) {
					for (int dj = -rj; dj <= rj; dj++) {

						int i0 = std::floor(coordinate[0] + di);
						int j0 = std::floor(coordinate[1] + dj);

						int xi = i0 - coordinate[0];
						int xj = j0 - coordinate[1];

						int i = std::clamp<int>(i0,0,imgData.shape()[0]-1);
						int j = std::clamp<int>(j0,0,imgData.shape()[1]-1);

						float weight = lanczosKernel(xi)*lanczosKernel(xj);

						ret += weight*imgData.valueUnchecked(i,j,c);
					}
				}

				return ret;
			}

			return T();
		}
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
