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

template <typename ScalarT>
ImageArray<ScalarT> convertGrayImg2RGB(ImageArray<ScalarT> const& gray) {

    if (gray.empty()) {
        return ImageArray<ScalarT>();
    }

    typename ImageArray<ScalarT>::ShapeBlock originalShape = gray.shape();
    typename ImageArray<ScalarT>::ShapeBlock shape = originalShape;
    shape[2] = 3;

    ImageArray<ScalarT> ret(shape);

    for (int i = 0; i < shape[0]; i++) {
        for (int j = 0; j < shape[1]; j++) {
            for (int c = 0; c < shape[2]; c++) {
                ret.atUnchecked(i,j,c) = gray.valueUnchecked(i,j,0);
            }
        }
    }

    return ret;
}

template <typename ScalarT>
ImageArray<ScalarT> convertGrayAlphaImg2RGBA(ImageArray<ScalarT> const& gray) {

    if (gray.empty()) {
        return ImageArray<ScalarT>();
    }

    typename ImageArray<ScalarT>::ShapeBlock originalShape = gray.shape();
    typename ImageArray<ScalarT>::ShapeBlock shape = originalShape;
    shape[2] = 4;

    ImageArray<ScalarT> ret(shape);

    for (int i = 0; i < shape[0]; i++) {
        for (int j = 0; j < shape[1]; j++) {
            for (int c = 0; c < 3; c++) {
                ret.atUnchecked(i,j,c) = gray.valueUnchecked(i,j,0);
            }
            ret.atUnchecked(i,j,3) = gray.valueUnchecked(i,j,1);
        }
    }

    return ret;
}

/*!
 * \brief convertCMYKImg2RGB simple fallback conversion function, should not be used for serious conversion
 * \param cmyk cmyk image data
 * \return the estimated rgb representation of the image in rgb
 */
template <typename ScalarT>
ImageArray<ScalarT> convertCMYKImg2RGB(ImageArray<ScalarT> const& cmyk) {

    if (cmyk.empty()) {
        return ImageArray<ScalarT>();
    }

    typename ImageArray<ScalarT>::ShapeBlock originalShape = cmyk.shape();
    typename ImageArray<ScalarT>::ShapeBlock shape = originalShape;
    shape[2] = 3;

    constexpr ScalarT whiteLevel = typedWhiteLevel<ScalarT>();

    ImageArray<ScalarT> ret(shape);

    for (int i = 0; i < shape[0]; i++) {
        for (int j = 0; j < shape[1]; j++) {
            float black = float(cmyk.valueUnchecked(i,j,3))/whiteLevel;
            for (int c = 0; c < 3; c++) {
                float color = float(cmyk.valueUnchecked(i,j,c))/whiteLevel;
                ret.atUnchecked(i,j,c) = whiteLevel*(1-color)*(1-black);
            }
        }
    }

    return ret;
}

/*!
 * \brief convertCMYKAImg2RGBA simple fallback conversion function, should not be used for serious conversion
 * \param cmyka cmyka image data
 * \return the estimated rgba representation of the image in rgba
 */
template <typename ScalarT>
ImageArray<ScalarT> convertCMYKAImg2RGBA(ImageArray<ScalarT> const& cmyka) {

    if (cmyka.empty()) {
        return ImageArray<ScalarT>();
    }

    typename ImageArray<ScalarT>::ShapeBlock originalShape = cmyka.shape();
    typename ImageArray<ScalarT>::ShapeBlock shape = originalShape;
    shape[2] = 3;

    constexpr ScalarT whiteLevel = typedWhiteLevel<ScalarT>();

    ImageArray<ScalarT> ret(shape);

    for (int i = 0; i < shape[0]; i++) {
        for (int j = 0; j < shape[1]; j++) {
            float black = float(cmyka.valueUnchecked(i,j,3))/whiteLevel;
            for (int c = 0; c < 3; c++) {
                float color = float(cmyka.valueUnchecked(i,j,c))/whiteLevel;
                ret.atUnchecked(i,j,c) = whiteLevel*(1-color)*(1-black);
            }
            ret.atUnchecked(i,j,3) = cmyka.valueUnchecked(i,j,4);
        }
    }

    return ret;
}

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

    inline static bool imageDataIsEmpty(Image::ImageData const& imgData) {
        auto shape = imageDataShape(imgData);
        if (shape.size() <= 0) {
            return true;
        }
        for (int s : shape) {
            if (s == 0) {
                return true;
            }
        }
        return false;
    }

    template<typename T_out, typename T_in>
    static inline T_out cast_imgdata_scalar_to_type(T_in const& inData) {
        return float(inData)/typedWhiteLevel<T_in>() * typedWhiteLevel<T_out>();
    }

	template<typename T>
	static inline ImageArray<T> cast_imgdata_to_type(ImageData const& imgData) {
		return std::visit([] (auto const& inData) -> ImageArray<T> {

			using ScalarT = typename std::decay_t<decltype(inData)>::ScalarT;

			ImageArray<T> ret(inData.shape());
			for (int i = 0; i < ret.shape()[0]; i++) {
				for (int j = 0; j < ret.shape()[1]; j++) {
					for (int c = 0; c < ret.shape()[2]; c++) {
                        ret.atUnchecked(i,j,c) = cast_imgdata_scalar_to_type<T, ScalarT>(inData.valueUnchecked(i,j,c));
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
