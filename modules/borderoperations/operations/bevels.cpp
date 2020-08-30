#include "bevels.h"

#include <list>
#include <QFile>

#include <magick/quantum.h>

namespace Piwap {
namespace BorderOperations {

const QString Bevels::bevelOpTypeId = "piwapborders/bevel";

Bevels::Bevels(QObject *parent) :
	AbstractImageOperation (parent),
	_radius(50)
{

}

int Bevels::doOperation(Magick::Image & image, ImageInfos * infos) const {


	Q_UNUSED(infos);

	image.matte(true);//set alpha if not present

	Magick::Color white(std::numeric_limits<MagickCore::Quantum>::max(),
					  std::numeric_limits<MagickCore::Quantum>::max(),
					  std::numeric_limits<MagickCore::Quantum>::max());
	Magick::Color black(0,0,0);

	Magick::Image bevelMask(Magick::Geometry(radius(), radius()), black);
	bevelMask.type(Magick::GrayscaleType);
	bevelMask.strokeWidth(0);
	bevelMask.fillColor(white);
	bevelMask.draw(Magick::DrawableCircle(0,0,radius(),0));

	Magick::Image alpha = image;
	alpha.channel(Magick::AlphaChannel);
	alpha.modifyImage();
	alpha.negate();

	ssize_t im_w = static_cast<ssize_t>(alpha.size().width());
	ssize_t im_h = static_cast<ssize_t>(alpha.size().height());

	alpha.composite(bevelMask, im_w-radius(), im_h-radius(), Magick::MultiplyCompositeOp);
	bevelMask.rotate(90);
	alpha.composite(bevelMask, 0, im_h-radius(), Magick::MultiplyCompositeOp);
	bevelMask.rotate(90);
	alpha.composite(bevelMask, 0, 0, Magick::MultiplyCompositeOp);
	bevelMask.rotate(90);
	alpha.composite(bevelMask, im_w-radius(), 0, Magick::MultiplyCompositeOp);

	image.composite(alpha, 0, 0, Magick::CopyOpacityCompositeOp);

	return 0;

}

quint32 Bevels::radius() const
{
	return _radius;
}

void Bevels::setRadius(const quint32 &radius)
{
	if (radius != _radius) {
		_radius = radius;
		Q_EMIT radiusChanged(_radius);
	}
}

QString Bevels::typeId() const {
	return bevelOpTypeId;
}

QString Bevels::getIconUrl() const {
	return "qrc:/borderoperations/icons/bevel.svg";
}

QString Bevels::getOpName() const {
	return tr("Bevel angles");
}

QString Bevels::getPropertiesEditorUrl() const {
	return "qrc:/borderoperations/qml/editors/BevelOpEditor.qml";
}


BevelOpFactory::BevelOpFactory(QObject * parent) : AbstractOperationFactory (parent) {

}

AbstractImageOperation* BevelOpFactory::factorizeOperation(QObject * operationParent) const {
	return new Bevels(operationParent);
}

QString BevelOpFactory::getToolTip() const {
	return tr("Bevel image angles");
}

} // namespace BorderOperations
} // namespace Piwap
