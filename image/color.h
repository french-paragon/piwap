#ifndef COLOR_H
#define COLOR_H

#include <cstdint>
#include <type_traits>
#include <limits>

#include <QColor>

namespace Piwap {

template<typename T>
constexpr T typedWhiteLevel() {
	if constexpr (std::is_integral_v<T>) {
		return std::numeric_limits<T>::max();
	} else {
		return 1;
	}
}

template<typename outT, typename inT>
outT convertTypedWhiteLevel(inT inVal) {

	if (inVal == typedWhiteLevel<inT>()) {
		return typedWhiteLevel<outT>();
	}

	double tmp = inVal;
	tmp /= typedWhiteLevel<inT>();
	tmp *= typedWhiteLevel<outT>();

	return static_cast<outT>(tmp);
}

enum ColorModel {
	Gray,
	GrayA,
	RGB,
	RGBA,
	BGR,
	BGRA,
	CMYK,
	CMYKA
};

int constexpr nChannels(ColorModel colorModel) {
	switch (colorModel) {
	case Gray:
		return 1;
	case GrayA:
		return 2;
	case RGB:
		return 3;
	case RGBA:
		return 4;
	case BGR:
		return 3;
	case BGRA:
		return 4;
	case CMYK:
		return 4;
	case CMYKA:
		return 5;
	}

	return -1;
}

int constexpr alphaChannelIndex(ColorModel colorModel) {
	switch (colorModel) {
	case Gray:
		return -1;
	case GrayA:
		return 1;
	case RGB:
		return -1;
	case RGBA:
		return 3;
	case BGR:
		return -1;
	case BGRA:
		return 3;
	case CMYK:
		return -1;
	case CMYKA:
		return 4;
	}

	return -1;
}

inline ColorModel removeColorModelAlphaChannel(ColorModel inModel) {
	switch (inModel) {
	case Gray:
		return Gray;
	case GrayA:
		return Gray;
	case RGB:
		return RGB;
	case RGBA:
		return RGB;
	case BGR:
		return BGR;
	case BGRA:
		return BGR;
	case CMYK:
		return CMYK;
	case CMYKA:
		return CMYK;
	}

	return Gray;
}

inline ColorModel addColorModelAlphaChannel(ColorModel inModel) {
	switch (inModel) {
	case Gray:
		return GrayA;
	case GrayA:
		return GrayA;
	case RGB:
		return RGBA;
	case RGBA:
		return RGBA;
	case BGR:
		return BGRA;
	case BGRA:
		return BGRA;
	case CMYK:
		return CMYKA;
	case CMYKA:
		return CMYKA;
	}

	return Gray;
}

template <typename ScalarT>
inline std::vector<ScalarT> getChannelValueFromQColor(ColorModel colorModel, QColor const& color) {

	if (colorModel == RGB) {

		ScalarT r = typedWhiteLevel<ScalarT>()*color.redF();
		ScalarT g = typedWhiteLevel<ScalarT>()*color.greenF();
		ScalarT b = typedWhiteLevel<ScalarT>()*color.blueF();

		return {r,g,b};

	} else if (colorModel == RGBA) {

		ScalarT r = typedWhiteLevel<ScalarT>()*color.redF();
		ScalarT g = typedWhiteLevel<ScalarT>()*color.greenF();
		ScalarT b = typedWhiteLevel<ScalarT>()*color.blueF();
		ScalarT a = typedWhiteLevel<ScalarT>()*color.alphaF();

		return {r,g,b,a};

	} else if (colorModel == BGR) {

		ScalarT r = typedWhiteLevel<ScalarT>()*color.redF();
		ScalarT g = typedWhiteLevel<ScalarT>()*color.greenF();
		ScalarT b = typedWhiteLevel<ScalarT>()*color.blueF();

		return {b,g,r};

	} else if (colorModel == BGRA) {

		ScalarT r = typedWhiteLevel<ScalarT>()*color.redF();
		ScalarT g = typedWhiteLevel<ScalarT>()*color.greenF();
		ScalarT b = typedWhiteLevel<ScalarT>()*color.blueF();
		ScalarT a = typedWhiteLevel<ScalarT>()*color.alphaF();

		return {b,g,r,a};

	} else if (colorModel == CMYK) {

		ScalarT c = typedWhiteLevel<ScalarT>()*color.cyanF();
		ScalarT m = typedWhiteLevel<ScalarT>()*color.magentaF();
		ScalarT y = typedWhiteLevel<ScalarT>()*color.yellowF();
		ScalarT k = typedWhiteLevel<ScalarT>()*color.blackF();

		return {c,m,y,k};

	} else if (colorModel == CMYKA) {

		ScalarT c = typedWhiteLevel<ScalarT>()*color.cyanF();
		ScalarT m = typedWhiteLevel<ScalarT>()*color.magentaF();
		ScalarT y = typedWhiteLevel<ScalarT>()*color.yellowF();
		ScalarT k = typedWhiteLevel<ScalarT>()*color.blackF();
		ScalarT a = typedWhiteLevel<ScalarT>()*color.alphaF();

		return {c,m,y,k,a};

	} else if (colorModel == GrayA) {

		float r = color.redF();
		float g = color.greenF();
		float b = color.blueF();

		ScalarT gray = typedWhiteLevel<ScalarT>()*(r + g + b)/3;
		ScalarT a = typedWhiteLevel<ScalarT>()*color.alphaF();

		return {gray, a};

	} else {

		float r = color.redF();
		float g = color.greenF();
		float b = color.blueF();

		ScalarT gray = typedWhiteLevel<ScalarT>()*(r + g + b)/3;

		return {gray};
	}

}

} // namespace Piwap

#endif // COLOR_H
