/*Piwap, or Picture Warping App, is a GUI to batch process images.

Copyright (C) 2024  Paragon<french.paragon@gmail.com>

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

#ifndef PIWAP_IMAGE_H
#define PIWAP_IMAGE_H

#include <QObject>
#include <QSize>

#include <variant>

#include <MultidimArrays/MultidimArrays.h>

#include "./color.h"

namespace Piwap {

template<typename T>
class ImageArray : public Multidim::Array<T,3> {
public:
	/*!
	 * \brief ImageArray default constructor
	 */
	ImageArray() : Multidim::Array<T,3>() {

	}
	/*!
	 * \brief ImageArray construct an image array with a proper shape and stides expected from an image.
	 * \param height
	 * \param width
	 * \param channels
	 */
	ImageArray(int height, int width, int channels) :
		Multidim::Array<T,3>(std::array<int,3>{height, width, channels}, std::array<int,3>{width*channels, channels, 1}) {

	}
	ImageArray(std::array<int, 3> const& shape) :
		ImageArray(shape[0], shape[1], shape[2]) {

	}

	ImageArray(ImageArray<T> const& other) : Multidim::Array<T,3>(other) {

	}

	ImageArray(ImageArray<T> && other) : Multidim::Array<T,3>(other) {

	}

	ImageArray& operator=(ImageArray<T> const& other) {
		Multidim::Array<T,3>::operator=(other);
		return *this;
	}

	ImageArray& operator=(ImageArray<T> && other) {
		Multidim::Array<T,3>::operator=(other);
		return *this;
	}
};

class Image : public QObject
{
public:

	using PixelData = std::variant<uint8_t, uint16_t, uint32_t,float>;

	using ImageData = std::variant<
	ImageArray<uint8_t>,
	ImageArray<uint16_t>,
	ImageArray<uint32_t>,
	ImageArray<float>>;

	template<typename T>
	static inline constexpr bool imageDataHoldsType(ImageData const& data) {
		return std::holds_alternative<ImageArray<T>>(data);
	}

	inline static std::array<int,3> imageDataShape(Image::ImageData const& imgData) {
		return std::visit([] (auto const& inData) -> std::array<int,3> {return inData.shape();}, imgData);
	}
	template<typename T>
	static inline ImageArray<T> cast_imgdata_to_type(ImageData const& imgData) {
		return std::visit([] (auto const& inData) -> ImageArray<T> {

			using ScalarT = typename std::decay_t<decltype(inData)>::ScalarT;

			ImageArray<T> ret(inData.shape());
			for (int i = 0; i < ret.shape()[0]; i++) {
				for (int j = 0; j < ret.shape()[1]; j++) {
					for (int c = 0; c < ret.shape()[2]; c++) {
						ret.atUnchecked(i,j,c) = float(inData.valueUnchecked(i,j,c))/typedWhiteLevel<ScalarT>() * typedWhiteLevel<T>();
					}
				}
			}
			return ret;
		}, imgData);
	}

	static inline std::byte* dataBuffer(ImageData & imgData) {
		return std::visit([] (auto & data) -> std::byte* { return reinterpret_cast<std::byte*>(&data.at(0,0,0)); }, imgData);
	}

	Image(QObject* parent = nullptr);

	inline ImageData& imageData() {
		return _data;
	}
	inline ImageData const& imageData() const {
		return _data;
	}

	inline int height() const {
		return std::visit([this] (auto const& imgData) { return imgData.shape()[0]; }, _data);
	}

	inline int width() const {
		return std::visit([this] (auto const& imgData) { return imgData.shape()[1]; }, _data);
	}

	inline int channels() const {
		return std::visit([this] (auto const& imgData) { return imgData.shape()[2]; }, _data);
	}

	inline QSize shape() const {
		return std::visit([this] (auto const& imgData) { return QSize(imgData.shape()[1], imgData.shape()[0]); }, _data);
	}

	inline std::byte* dataBuffer() {
		return dataBuffer(_data);
	}

	inline int alphaChannel() const {
		return alphaChannelIndex(_colorModel);
	}

	inline ColorModel colorModel() const {
		return _colorModel;
	}

	inline void setColorModel(ColorModel colorModel) {
		_colorModel = colorModel;
	}

protected:

	ColorModel _colorModel;
	ImageData _data;
};

} // namespace Piwap

#endif // PIWAP_IMAGE_H
