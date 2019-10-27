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

#include "fit.h"

#include <QDebug>
#include <QMetaEnum>
#include <QJsonObject>

#include <Magick++/Color.h>

namespace Piwap {
namespace Operations {

const QString Fit::fitOpTypeId = "piwapbase/fit";

Fit::Fit(QObject *parent) :
	AbstractInterpolatingOperation(parent),
	_pix_x(800),
	_pix_y(800),
	_bg(255, 255, 255),
	_expand_mode(None)
{
	connect(this, &Fit::pix_x_changed, this, &AbstractImageOperation::hasBeenChanged);
	connect(this, &Fit::pix_y_changed, this, &AbstractImageOperation::hasBeenChanged);
	connect(this, &Fit::bg_changed, this, &AbstractImageOperation::hasBeenChanged);
	connect(this, &Fit::expandModeChanged, this, &AbstractImageOperation::hasBeenChanged);
}

int Fit::doOperation(Magick::Image &image, ImageInfos * infos) const {

	Q_UNUSED(infos);

	if (_pix_x <= 0 && _pix_y <= 0) {
		return 0;
	}

	int newWidth;
	int newHeight;

	float aspectRatio = static_cast<float>(image.size().width()) / static_cast<float>(image.size().height());

	float targetAspectRatio = static_cast<float>(_pix_x) / static_cast<float>(_pix_y);

	if (aspectRatio > targetAspectRatio) {

		if (_expand_mode == X) {
			newWidth = static_cast<int>(roundf(_pix_y*aspectRatio));
			newHeight = _pix_y;
		} else {
			newWidth = _pix_x;
			newHeight = static_cast<int>(roundf(_pix_x/aspectRatio));
		}

	} else if (aspectRatio < targetAspectRatio) {

		if (_expand_mode == Y) {
			newWidth = _pix_x;
			newHeight = static_cast<int>(roundf(_pix_x/aspectRatio));
		} else {
			newWidth = static_cast<int>(roundf(_pix_y*aspectRatio));
			newHeight = _pix_y;
		}

	} else {

		newWidth = _pix_x;
		newHeight = _pix_y;

	}

	image.filterType(static_cast<Magick::FilterTypes>(_interpolation_mode));
	image.resize(Magick::Geometry(static_cast<size_t>(newWidth), static_cast<size_t>(newHeight)));

	if (newWidth < _pix_x || newHeight < _pix_y) {

		int r = _bg.red();
		int g = _bg.green();
		int b = _bg.blue();

		if (MAGICKCORE_QUANTUM_DEPTH == 16) {
			r <<= 8;
			g <<= 8;
			b <<= 8;
		}


		Magick::Color bg(static_cast<Magick::Quantum>(r), static_cast<Magick::Quantum>(g), static_cast<Magick::Quantum>(b));

		image.extent(Magick::Geometry(static_cast<size_t>(_pix_x), static_cast<size_t>(_pix_y)), bg, Magick::CenterGravity);

	}

	return 0;

}

QString Fit::typeId() const {
	return fitOpTypeId;
}

QString Fit::getOpName() const {
	return "Fit on background";
}
QString Fit::getIconUrl() const {
	return "qrc:/icons/scale_image.svg";
}

QString Fit::getPropertiesEditorUrl() const {
	return "qrc:/qmlcomponents/editors/FitOpEditor.qml";
}

int Fit::pix_x() const
{
	return _pix_x;
}

void Fit::setPix_x(int pix_x)
{
	if (pix_x != _pix_x) {
		_pix_x = pix_x;
		Q_EMIT pix_x_changed(_pix_x);
	}
}

int Fit::pix_y() const
{
	return _pix_y;
}

void Fit::setPix_y(int pix_y)
{
	if (pix_y != _pix_y) {
		_pix_y = pix_y;
		Q_EMIT pix_y_changed(_pix_y);
	}
}

QColor Fit::bg() const
{
	return _bg;
}

void Fit::setBg(const QColor &bg)
{
	if (_bg != bg) {
		qDebug() << "BG changed to: " << bg;
		_bg = bg;
		Q_EMIT bg_changed(_bg);
	}
}

QString Fit::expandMode() const {
	return QVariant::fromValue(_expand_mode).toString();
}
void Fit::setExpandMode(QString mode) {

	QMetaEnum m_e = QMetaEnum::fromType<Fit::ExpandMode>();

	int index = m_e.keyToValue(mode.toStdString().c_str());

	if (index == -1) {
		return;
	}

	Fit::ExpandMode e = static_cast<Fit::ExpandMode>(index);

	if (e != _expand_mode) {
		_expand_mode = e;
		Q_EMIT expandModeChanged(expandMode());
	}

}

QStringList Fit::expandModeStrings() const {

	QMetaEnum m_e = QMetaEnum::fromType<Fit::ExpandMode>();

	int n_keys = m_e.keyCount();
	QStringList ret;
	ret.reserve(n_keys);

	for (int i = 0; i < n_keys; i++) {
		ret << QString(m_e.key(i));
	}

	return ret;
}

FitOpFactory::FitOpFactory(QObject * parent) :
	AbstractOperationFactory (parent)
{

}

AbstractImageOperation* FitOpFactory::factorizeOperation(QObject * operationParent) const {
	return new Fit(operationParent);
}

QString FitOpFactory::getToolTip() const {
	return tr("Resize the image to a fixed size in pixels with potentially some margins added");
}

} // namespace Operations
} // namespace Piwap
