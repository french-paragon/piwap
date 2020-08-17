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
