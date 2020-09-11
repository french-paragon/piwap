#include "dropshadow.h"

namespace Piwap {
namespace BorderOperations {

const QString DropShadow::dropShadowOpTypeId = "piwapborders/dropshadow";

DropShadow::DropShadow(QObject *parent) :
    AbstractImageOperation(parent),
    _dx(5),
    _dy(5),
    _blur_radius(5),
    _bg(0,0,0)
{
	connect(this, &DropShadow::dxChanged, this, &AbstractImageOperation::hasBeenChanged);
	connect(this, &DropShadow::dyChanged, this, &AbstractImageOperation::hasBeenChanged);
	connect(this, &DropShadow::blurRadiusChanged, this, &AbstractImageOperation::hasBeenChanged);
	connect(this, &DropShadow::bgChanged, this, &AbstractImageOperation::hasBeenChanged);
}

int DropShadow::doOperation(Magick::Image & image, ImageInfos * infos) const {

    Q_UNUSED(infos)

    Magick::Color white(std::numeric_limits<MagickCore::Quantum>::max(),
                      std::numeric_limits<MagickCore::Quantum>::max(),
                      std::numeric_limits<MagickCore::Quantum>::max());

    Magick::Image alpha = image;
    alpha.channel(Magick::AlphaChannel);
    alpha.modifyImage();

    size_t f_radius = 2*((_blur_radius > 0.0) ? static_cast<size_t>(std::ceil(_blur_radius)) : 0);

	ssize_t sdx = (_dx > static_cast<int>(f_radius)) ? _dx : static_cast<int>(f_radius);
	ssize_t sdy = (_dy > static_cast<int>(f_radius)) ? _dy : static_cast<int>(f_radius);

	ssize_t idx = (_dx < static_cast<int>(f_radius)) ? static_cast<int>(f_radius)-_dx : 0;
	ssize_t idy = (_dy < static_cast<int>(f_radius)) ? static_cast<int>(f_radius)-_dy : 0;

	size_t nW = image.columns() + f_radius + static_cast<size_t>(std::max(static_cast<int>(f_radius),std::abs(_dx)));
	size_t nH = image.rows() + f_radius + static_cast<size_t>(std::max(static_cast<int>(f_radius),std::abs(_dy)));

    alpha.extent(Magick::Geometry(nW, nH, -sdx, -sdy), white);
    if (f_radius > 0) {
        alpha.blur(f_radius, _blur_radius);
    }

    alpha.negate();

    int r = _bg.red();
    int g = _bg.green();
    int b = _bg.blue();

    if (MAGICKCORE_QUANTUM_DEPTH == 16) {
        r <<= 8;
        g <<= 8;
        b <<= 8;
    }

    Magick::Color bg(static_cast<Magick::Quantum>(r), static_cast<Magick::Quantum>(g), static_cast<Magick::Quantum>(b));

    Magick::Image comp(Magick::Geometry(nW,nH), bg);
    comp.depth(image.depth());
    comp.composite(alpha, 0, 0, Magick::CopyOpacityCompositeOp);
    comp.composite(image, idx, idy, Magick::OverCompositeOp);

    image = comp;

    return 0;
}

QString DropShadow::typeId() const {
    return dropShadowOpTypeId;
}

QString DropShadow::getIconUrl() const {
    return "qrc:/borderoperations/icons/drop_shadow.svg";
}
QString DropShadow::getOpName() const {
    return tr("Drop shadow");
}

QString DropShadow::getPropertiesEditorUrl() const {
    return "qrc:/borderoperations/qml/editors/DropShadowOpEditor.qml";
}

qint32 DropShadow::dx() const
{
    return _dx;
}

void DropShadow::setDx(const qint32 &dx)
{
    _dx = dx;
}

qint32 DropShadow::dy() const
{
    return _dy;
}

void DropShadow::setDy(const qint32 &dy)
{
    _dy = dy;
}

qreal DropShadow::blurRadius() const
{
    return _blur_radius;
}

void DropShadow::setBlurRadius(qreal blur_radius)
{
    _blur_radius = blur_radius;
}

QColor DropShadow::bg() const
{
    return _bg;
}

void DropShadow::setBg(const QColor &bg)
{
    _bg = bg;
}


DropShadowOpFactory::DropShadowOpFactory(QObject * parent) : AbstractOperationFactory(parent) {

}

AbstractImageOperation* DropShadowOpFactory::factorizeOperation(QObject * operationParent) const {
    return new DropShadow(operationParent);
}

QString DropShadowOpFactory::getToolTip() const {
    return tr("Add a drop shadow");
}

} // namespace BorderOperations
} // namespace Piwap
