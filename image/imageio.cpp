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

#include "imageio.h"
#include "imageinfos.h"

#include "operations/helper_functions/alphachannelsutils.h"

#include<math.h>
using std::isnan;
#include <jpeglib.h>
#include <jpegint.h>

#include <png.h>

#include <tiff.h>

#include <sys/stat.h>
#include <stdio.h>
#include <zip.h>

#define MAGICKCORE_QUANTUM_DEPTH 8
#define MAGICKCORE_HDRI_ENABLE 0

#include <Magick++.h>

namespace Piwap {

ImageWithInfos readPngFile(const char* file, QObject *imgParent, QObject *infoParent, bool fileIsMemoryBuffer = false, int fileBufferLen = 0);

ImageWithInfos openKraOraFile(const char* fileName, QObject *imgParent = nullptr, QObject *infoParent = nullptr) {

	struct zip *f_zip = nullptr;
	int err = 0;

	f_zip = zip_open(fileName, ZIP_RDONLY, &err);

	if(err != ZIP_ER_OK or f_zip == nullptr) {
		return {nullptr, nullptr};
	}

	zip_int64_t visu = zip_name_locate(f_zip,"mergedimage.png",0);
	if (visu==-1) {
		return {nullptr, nullptr};
	}

	struct zip_stat file_stat;
	struct zip_file *file_zip=nullptr;
	zip_stat_index(f_zip, static_cast<zip_uint64_t>(visu), 0, &file_stat);

	if(zip_stat(f_zip, file_stat.name, 0, &file_stat) == -1) {
		return {nullptr, nullptr};
	}

	file_zip=zip_fopen(f_zip, file_stat.name, ZIP_FL_UNCHANGED);

	if(!file_zip) {
		return {nullptr, nullptr};
	}

	char *str = nullptr;
	try {
		str = new char[static_cast<size_t>(file_stat.size+1)];
		memset(str, 0, static_cast<size_t>(file_stat.size+1));
	} catch(std::bad_alloc& ba){
		Q_UNUSED(ba);
		return {nullptr, nullptr};
	}

	if(zip_fread(file_zip, str, static_cast<size_t>(file_stat.size)) != static_cast<zip_int64_t>(file_stat.size)) {
		delete [] str;
		return {nullptr, nullptr};
	}

	constexpr bool fileIsMemoryBuffer = true;

	ImageWithInfos ret = readPngFile(str, imgParent, infoParent, fileIsMemoryBuffer, file_stat.size);
	ret.infos = new ImageInfos(fileName, infoParent);

	delete [] str;

	return ret;
}

ImageWithInfos readPngFile(const char* file, QObject *imgParent, QObject *infoParent, bool fileIsMemoryBuffer, int fileBufferLen) {

	FILE *fp = nullptr;

	if (fileIsMemoryBuffer) {
		if (fileBufferLen > 0) {
			fp = fmemopen(const_cast<char*>(file), static_cast<size_t>(fileBufferLen), "rb");
		}
	} else {
		fp = fopen(file, "rb");
	}

	if (!fp) {
		return {nullptr, nullptr};
	}

	constexpr int nSigBytes = 8;
	std::array<uint8_t,nSigBytes> header;

	if (fread(header.data(), 1, nSigBytes, fp) != nSigBytes) {
		return {nullptr, nullptr};
	}

	bool is_png = (png_sig_cmp(header.data(), 0, nSigBytes) == 0);
	if (!is_png) {
		return {nullptr, nullptr};
	}

	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
												 (png_voidp) nullptr,
												 nullptr, //error func
												 nullptr); //warning func
	if (!png_ptr) {
		   return {nullptr, nullptr};
	}

	png_infop info_ptr = png_create_info_struct(png_ptr);

	if (!info_ptr) {
		png_destroy_read_struct(&png_ptr, NULL, NULL);
		return {nullptr, nullptr};
	}

	if (setjmp(png_jmpbuf(png_ptr))) {

		png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);   \

		fclose(fp);
		return {nullptr, nullptr};
	  }

	png_init_io(png_ptr, fp);
	png_set_sig_bytes(png_ptr, nSigBytes);

	png_read_info(png_ptr, info_ptr);

	int width = png_get_image_width(png_ptr, info_ptr);
	int height = png_get_image_height(png_ptr, info_ptr);
	int bit_depth = png_get_bit_depth(png_ptr, info_ptr);
	int color_type = png_get_color_type(png_ptr, info_ptr);
	int interlace_type = png_get_interlace_type(png_ptr, info_ptr);
	int channels = png_get_channels(png_ptr, info_ptr);
	int rowbytes = png_get_rowbytes(png_ptr, info_ptr);

	ColorModel colorInfos = ColorModel::RGB;

	if (color_type == PNG_COLOR_TYPE_RGBA) {
		colorInfos = ColorModel::RGBA;
	}

	if (color_type == PNG_COLOR_TYPE_GRAY) {
		colorInfos = ColorModel::Gray;
	}

	if (color_type == PNG_COLOR_TYPE_GRAY_ALPHA) {
		colorInfos = ColorModel::GrayA;
	}

	ImageWithInfos ret;

	ret.image = new Image(imgParent);
	ret.image->setColorModel(colorInfos);

	std::array<int,3> shape = {height, width, channels};
	std::vector<png_byte*> rowsBuffer(height);
	int rowLen;

	switch (bit_depth) {
	//1, 2, 4 bit depth unsupported at the moment
	case 8: {
		ret.image->imageData() = ImageArray<uint8_t>(shape);
		rowLen = sizeof(uint8_t)*shape[1]*shape[2];
	}
		break;
	case 16: {
		ret.image->imageData() = ImageArray<uint16_t>(shape);
		rowLen = sizeof(uint16_t)*shape[1]*shape[2];
	}
		break;
	}

	std::byte* dataBuffer = ret.image->dataBuffer();

	for (int i = 0; i < height; i++) {
		rowsBuffer[i] = reinterpret_cast<png_byte*>(dataBuffer+i*rowLen);
	}

	// read image data
	png_read_image(png_ptr, rowsBuffer.data());

	// clear png resource
	png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);

	// close file
	fclose(fp);
	fp = nullptr;

	if (!fileIsMemoryBuffer) {
		ret.infos = new ImageInfos(file, infoParent);
	} else {
		ret.infos = nullptr;
	}

	return ret;

}

bool writePngFile(const char* file, Image* image, ImageInfos* infos) {

	FILE *fp = fopen(file, "wb");
	if (fp == nullptr) {
		return false;
	}

	png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (png_ptr == nullptr) {
		fclose(fp);
		return false;
	}

	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == nullptr) {
	  png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
	  fclose(fp);
	  return false;
	}

	if (setjmp(png_jmpbuf(png_ptr))) {
		png_destroy_write_struct(&png_ptr, &info_ptr);
		fclose(fp);
		return false;
	}

	png_init_io(png_ptr, fp);

	int bit_depth = 8;

	if (Image::imageDataHoldsType<uint16_t>(image->imageData()) or
			Image::imageDataHoldsType<uint32_t>(image->imageData()) or
			Image::imageDataHoldsType<float>(image->imageData())) {
		bit_depth = 16;
	}

	int colorMode = PNG_COLOR_TYPE_RGB_ALPHA;

	if (image->colorModel() == ColorModel::Gray) {
		colorMode = PNG_COLOR_TYPE_GRAY;
	}

	if (image->colorModel() == ColorModel::GrayA) {
		colorMode = PNG_COLOR_TYPE_GRAY_ALPHA;
	}

	if (image->colorModel() == ColorModel::RGB) {
		colorMode = PNG_COLOR_TYPE_RGB;
	}

	Image::ImageData convertedData;

	if (bit_depth == 8) {
		convertedData = Image::cast_imgdata_to_type<uint8_t>(image->imageData());
	} else {
		convertedData = Image::cast_imgdata_to_type<uint16_t>(image->imageData());
	}

	int height = image->height();
	int width = image->width();
	int channels = image->channels();

	int rowLen = (bit_depth/8)*width*channels;

	std::byte* buffer_ptr = Image::dataBuffer(convertedData);

	std::vector<png_byte*> rowsBuffer(height);

	for (int i = 0; i < height; i++) {
		rowsBuffer[i] = reinterpret_cast<png_byte*>(buffer_ptr+i*rowLen);
	}

	png_set_IHDR(png_ptr,
		  info_ptr,
		  width,
		  height,
		  bit_depth,
		  colorMode,
		  PNG_INTERLACE_NONE,
		  PNG_COMPRESSION_TYPE_DEFAULT,
		  PNG_FILTER_TYPE_DEFAULT);

	png_write_info(png_ptr, info_ptr);

	png_write_image(png_ptr, rowsBuffer.data());

	png_write_end(png_ptr, nullptr);

	png_destroy_write_struct(&png_ptr, &info_ptr);

	fclose(fp);

	return true;
}

ImageWithInfos readJpgFile(const char* file, QObject *imgParent, QObject *infoParent) {

	jpeg_decompress_struct dcinfo;
	jpeg_error_mgr jerr;

	dcinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&dcinfo);

	FILE * infile;
	if ((infile = fopen(file, "rb")) == nullptr) {
		return {nullptr, nullptr};
	}

	jpeg_stdio_src(&dcinfo, infile);
	jpeg_read_header(&dcinfo, true);
	jpeg_start_decompress(&dcinfo);

	//shape of the image
	std::array<int,3> shape = {static_cast<int>(dcinfo.output_height),
							   static_cast<int>(dcinfo.output_width),
							   static_cast<int>(dcinfo.output_components)};

	ImageArray<uint8_t> data(shape);

	uint8_t* buffer = &data.at(0,0,0);

	std::vector<uint8_t*> scanlines(shape[0]);

	for (int i = 0; i < shape[0]; i++) {
		scanlines[i] = buffer + i*data.strides()[0];
	}

	int nLinesRead = 0;

	while (dcinfo.output_scanline < shape[0]) {
		int nLines = jpeg_read_scanlines(&dcinfo, scanlines.data() + nLinesRead, 1);
		nLinesRead += nLines;

		if (nLines == 0) {
			break;
		}
	}

	//encure ressources are freed
	jpeg_finish_decompress(&dcinfo);
	jpeg_destroy_decompress(&dcinfo);
	fclose(infile);
	infile = nullptr;

	if (nLinesRead != shape[0]) {
		return {nullptr, nullptr};
	}

	ImageWithInfos ret;

	ret.image = new Image(imgParent);
	ret.image->imageData() = data;

	ColorModel colorInfos = ColorModel::RGB;

	//TODO: check if more color space formats should be supported
	if (dcinfo.jpeg_color_space == JCS_GRAYSCALE) {
		colorInfos = ColorModel::Gray;
	}

	if (dcinfo.jpeg_color_space == JCS_CMYK) {
		colorInfos = ColorModel::CMYK;
	}

	ret.image->setColorModel(colorInfos);
	ret.infos = new ImageInfos(file, infoParent);

	return ret;

}

bool writeJpgFile(const char* file, Image* image, ImageInfos* infos) {

	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;

	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_compress(&cinfo);

	FILE * outfile;

	if ((outfile = fopen(file, "wb")) == nullptr) {
		return false;
	}

	jpeg_stdio_dest(&cinfo, outfile);

	cinfo.input_components = 3;

	cinfo.image_width = image->width();
	cinfo.image_height = image->height();

	cinfo.in_color_space = JCS_RGB;

	if (image->colorModel() == ColorModel::Gray or image->colorModel() == ColorModel::GrayA) {
		cinfo.in_color_space = JCS_GRAYSCALE;
		cinfo.input_components = 1;
	}

	jpeg_set_defaults(&cinfo);

	jpeg_start_compress(&cinfo, true);

	std::vector<uint8_t> bgColor = {255, 255, 255};
	ImageArray<uint8_t> data = removeAlphaChannel(Image::cast_imgdata_to_type<uint8_t>(image->imageData()), image->colorModel(), bgColor);

	uint8_t* buffer = &data.at(0,0,0);

	std::vector<uint8_t*> scanlines(data.shape()[0]);

	for (int i = 0; i < data.shape()[0]; i++) {
		scanlines[i] = buffer + i*data.strides()[0];
	}


	int nLinesWritten = 0;

	while (cinfo.next_scanline < data.shape()[0]) {
		int nLines = jpeg_write_scanlines(&cinfo, scanlines.data() + nLinesWritten, 1);
		nLinesWritten += nLines;

		if (nLines == 0) {
			break;
		}
	}

	jpeg_finish_compress(&cinfo);
	jpeg_destroy_compress(&cinfo);

	fclose(outfile);

	if (nLinesWritten != data.shape()[0]) {
		return false;
	}

	return true;
}

ImageWithInfos openImage(const char* fileName, QObject *imgParent, QObject *infoParent) {

	QString fName(fileName);

	if (fName.endsWith(".kra", Qt::CaseInsensitive) || fName.endsWith(".ora", Qt::CaseInsensitive)) {
		return openKraOraFile(fileName, imgParent, infoParent);
	} else if (fName.endsWith(".png", Qt::CaseInsensitive)) {
		return readPngFile(fileName, imgParent, infoParent);
	} else if (fName.endsWith(".jpg", Qt::CaseInsensitive) || fName.endsWith(".jpeg", Qt::CaseInsensitive)) {
		return readJpgFile(fileName, imgParent, infoParent);
	} else {
		//By default use imagemagick
		ImageWithInfos ret;
		ret.image = new Image(imgParent);

		try {
			Magick::Image img(fileName);

			int height = img.size().height();
			int width = img.size().width();

			ImageArray<uint8_t> imgArray(height,width,3);

			img.write(0, 0, width, height, "RGBA", Magick::StorageType::CharPixel, &imgArray.at(0,0,0));

			ret.image->imageData() = std::move(imgArray);
			ret.image->setColorModel(ColorModel::RGBA);

		} catch (Magick::Exception &error) {
			ret.image->deleteLater();
			return {nullptr, nullptr};
		}

		ret.infos = new ImageInfos(fileName, infoParent);

		return ret;
	}

	ImageInfos* infos = new ImageInfos(fName, infoParent);

	return {nullptr, nullptr};

}

int reloadImage(Image::ImageData &imageDest, ImageInfos* infos) {

	ImageWithInfos data = openImage(infos->originalFilePath().toStdString().c_str(), nullptr, nullptr);

	if (data.image == nullptr) {
		if (data.infos != nullptr) {
			data.infos->deleteLater();
		}
		return 1;
	}

	imageDest = std::move(data.image->imageData());

	data.image->deleteLater();
	data.infos->deleteLater();

	return 0;

}

bool writeImage(const char* fileName, Image* image, ImageInfos* infos, bool saveExif, bool saveIptc, bool saveXmp) {

	QString fName(fileName);

	bool ok = false;

	if (fName.endsWith(".png", Qt::CaseInsensitive)) {
		ok = writePngFile(fileName, image, infos);
	} else if (fName.endsWith(".jpg", Qt::CaseInsensitive) || fName.endsWith(".jpeg", Qt::CaseInsensitive)) {
		ok = writeJpgFile(fileName, image, infos);
	} else {
		//default case use ImageMagick to write.
		std::string map;

		switch (image->colorModel()) {
		case ColorModel::Gray:
			map = "K";
			break;
		case ColorModel::GrayA:
			map = "KA";
			break;
		case ColorModel::RGB:
			map = "RGB";
			break;
		case ColorModel::RGBA:
			map = "RGBA";
			break;
		case ColorModel::BGR:
			map = "BGR";
			break;
		case ColorModel::BGRA:
			map = "BGRA";
			break;
		case ColorModel::CMYK:
			map = "CMYK";
			break;
		case ColorModel::CMYKA:
			map = "CMYKA";
			break;
		default:
			break;
		}

		Magick::StorageType storage;

		if (!map.empty()) {
			if (Image::imageDataHoldsType<uint8_t>(image->imageData())) {
				storage = Magick::StorageType::CharPixel;
			}
			if (Image::imageDataHoldsType<uint16_t>(image->imageData())) {
				storage = Magick::StorageType::ShortPixel;
			}
			if (Image::imageDataHoldsType<uint32_t>(image->imageData())) {
				storage = Magick::StorageType::IntegerPixel;
			}
			if (Image::imageDataHoldsType<float>(image->imageData())) {
				storage = Magick::StorageType::FloatPixel;
			}

			try {
				Magick::Image img(image->width(), image->height(), map, storage, image->dataBuffer());

				if (img.isValid()) {
					img.write(fileName);
				}
			} catch (Magick::Exception &error) {
				return false;
			}
		}
	}

	if (!ok) {
		return false;
	}

	if (infos->metadataobject() != nullptr) {

		Exiv2::Image::AutoPtr imExiv;

		try {
			imExiv = Exiv2::ImageFactory::open(fileName);
		} catch (Exiv2::Error const&) {
			imExiv = Exiv2::Image::AutoPtr(nullptr);
		}

		if (imExiv.get() != nullptr) {

			Exiv2::ExifData exifD;
			if (saveExif) {
				exifD = infos->metadataobject()->exifData();
			}
			imExiv->setExifData(exifD);

			if (saveIptc) {
				imExiv->setIptcData(infos->metadataobject()->iptcData());
			}

			if (saveXmp) {
				imExiv->setXmpData(infos->metadataobject()->xmpData());
			}

			imExiv->writeMetadata();

		}

	}

	return true;
}


}// namespace Piwap
