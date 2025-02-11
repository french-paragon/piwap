/*Piwap, or Picture Warping App, is a GUI to batch process images.

Copyright (C) 2025  Paragon<french.paragon@gmail.com>

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

#ifndef ALPHACHANNELSUTILS_H
#define ALPHACHANNELSUTILS_H

#include "../../image/color.h"
#include "../../image/image.h"

namespace Piwap {

template <typename InArray>
InArray addAlphaChannel(InArray const& inData, ColorModel inColorModel) {

	using ScalarT = typename InArray::ScalarT;

	ColorModel outModel = addColorModelAlphaChannel(inColorModel);

	if (outModel == inColorModel) {
		return inData;
	}

	ImageArray<ScalarT> ret (inData.shape()[0], inData.shape()[1], inData.shape()[2]+1);

	int alphaChannel = inData.shape()[2];

	for (int i = 0; i < inData.shape()[0]; i++) {
		for (int j = 0; j < inData.shape()[1]; j++) {
			for (int c = 0; c < inData.shape()[2]; c++) {
				ret.atUnchecked(i,j,c) = inData.valueUnchecked(i,j,c);
			}

			ret.atUnchecked(i,j,alphaChannel) = typedWhiteLevel<ScalarT>();
		}
	}

	return ret;
}

template <typename InArray>
InArray removeAlphaChannel(InArray const& inData, ColorModel inColorModel, std::vector<typename InArray::ScalarT> const& bgColorValue) {

	using ScalarT = typename InArray::ScalarT;

	ColorModel outModel = removeColorModelAlphaChannel(inColorModel);

	if (outModel == inColorModel) {
		return inData;
	}

	std::array<int,3> shape = {inData.shape()[0], inData.shape()[1], inData.shape()[2]-1};
	ImageArray<ScalarT> ret (shape);

	int alphaChannel = inData.shape()[2]-1;

	for (int i = 0; i < ret.shape()[0]; i++) {
		for (int j = 0; j < ret.shape()[1]; j++) {
			for (int c = 0; c < ret.shape()[2]; c++) {

				int sc = c;
				if (c >= alphaChannel) {
					sc += 1;
				}

				float alphaCoeff = float(inData.value(i,j,alphaChannel))/float(typedWhiteLevel<ScalarT>());

				ret.atUnchecked(i,j,c) = inData.valueUnchecked(i,j,sc)*alphaCoeff + bgColorValue[c]*(1-alphaCoeff);

			}
		}
	}

	return ret;
}

} // namespace Piwap

#endif // ALPHACHANNELSUTILS_H
