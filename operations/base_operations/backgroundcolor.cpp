/*Piwap, or Picture Warping App, is a GUI to batch process images.

Copyright (C) 2020  Paragon<french.paragon@gmail.com>

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

#include "backgroundcolor.h"

namespace Piwap {
namespace Operations {


const QString BackgroundColor::backgroundOpTypeId = "piwapbase/backgroundcolor";

BackgroundColor::BackgroundColor(QObject *parent) :
	AbstractImageOperation(parent),
	_bg(255, 255, 255)
{

}

int BackgroundColor::doOperation(Magick::Image & image, ImageInfos * infos) const {

	Q_UNUSED(infos);

	int r = _bg.red();
	int g = _bg.green();
	int b = _bg.blue();

	if (MAGICKCORE_QUANTUM_DEPTH == 16) {
		r <<= 8;
		g <<= 8;
		b <<= 8;
	}

	Magick::Color bg(static_cast<Magick::Quantum>(r), static_cast<Magick::Quantum>(g), static_cast<Magick::Quantum>(b));

	image.backgroundColor(bg);

	return  0;
}

QString BackgroundColor::typeId() const {
	return  backgroundOpTypeId;
}

QString BackgroundColor::getOpName() const {
	return tr("Background color");
}
QString BackgroundColor::getIconUrl() const {
	return "qrc:/icons/color_bg.svg";
}

QString BackgroundColor::getPropertiesEditorUrl() const {
	return "qrc:/qmlcomponents/editors/BackgroundOpEditor.qml";
}

QColor BackgroundColor::bg() const {
	return _bg;
}
void BackgroundColor::setBg(const QColor &bg) {
	if (bg != _bg) {
		_bg = bg;
		Q_EMIT bg_changed(_bg);
	}
}


BackgroundOpFactory::BackgroundOpFactory(QObject * parent) : AbstractOperationFactory (parent) {

}

AbstractImageOperation* BackgroundOpFactory::factorizeOperation(QObject * operationParent) const {
	return new BackgroundColor(operationParent);
}

QString BackgroundOpFactory::getToolTip() const {
	return tr("Set image background color.");
}

} // namespace Operations
} // namespace Piwap
