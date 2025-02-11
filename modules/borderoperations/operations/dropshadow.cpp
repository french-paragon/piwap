#include "dropshadow.h"

#include "operations/helper_functions/alphachannelsutils.h"

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

int DropShadow::doOperation(Image *image, ImageInfos * infos) const {

	Q_UNUSED(infos)

	ColorModel inModel = image->colorModel();
	ColorModel outModel = addColorModelAlphaChannel(inModel);

	image->imageData() = std::visit([inModel, this] (auto const& inData) -> Image::ImageData {

		using ArrayT = std::decay_t<decltype(inData)>;
		using ScalarT = typename ArrayT::ScalarT;

		ColorModel outModel = addColorModelAlphaChannel(inModel);

		int inAlphaChannel = alphaChannelIndex(inModel);
		int outAlphaChannel = alphaChannelIndex(outModel);
		int nOutChannels = nChannels(outModel);

		int expendRadius = std::ceil(_blur_radius);

		int dSizeX = std::max(0,_dx-expendRadius) + 2*expendRadius;
		int dSizeY = std::max(0,_dy-expendRadius) + 2*expendRadius;

		ImageArray<ScalarT> ret(inData.shape()[0]+dSizeY,inData.shape()[1]+dSizeX,nOutChannels);

		std::vector<ScalarT> bgCol = getChannelValueFromQColor<ScalarT>(outModel,_bg);

		for (int i = 0; i < ret.shape()[0]; i++) {

			int im_i = i - expendRadius + std::min(0,_dy);
			int shade_i = im_i - _dy;

			int shadeDistI = (shade_i < 0) ? -shade_i : std::max(0, shade_i - inData.shape()[0]);
			float shadeAttI = std::max<float>(0,_blur_radius-shadeDistI)/_blur_radius;

			for (int j = 0; j < ret.shape()[1]; j++) {

				int im_j = j - expendRadius + std::min(0,_dx);
				int shade_j = im_j - _dx;

				int shadeDistJ = (shade_j < 0) ? -shade_j : std::max(0, shade_j - inData.shape()[1]);
				float shadeAttJ = std::max<float>(0,_blur_radius-shadeDistI)/_blur_radius;

				float shadeBlurAlphaCoeff = shadeAttI*shadeAttJ;

				for (int c = 0; c < nOutChannels; c++) {
					float fgAlpha = 0;

					if (im_i >= 0 and im_i < inData.shape()[0] and im_j >= 0 and im_j < inData.shape()[1]) {

						fgAlpha = float(typedWhiteLevel<ScalarT>());

						if (inAlphaChannel >= 0 and inAlphaChannel < inData.shape()[2]) {
							fgAlpha = inData.valueUnchecked(im_i,im_j,inAlphaChannel);
						}
					}

					float fgAlphaCoeff = fgAlpha/float(typedWhiteLevel<ScalarT>());

					if (c == outAlphaChannel) {
						float alpha = shadeBlurAlphaCoeff*float(bgCol[c]);

						if (im_i >= 0 and im_i < inData.shape()[0] and im_j >= 0 and im_j < inData.shape()[1]) {
							alpha = alpha + fgAlpha - alpha * fgAlphaCoeff;
						}

						ret.atUnchecked(i,j,c) = alpha;
					} else {

						float val = bgCol[c];
						float alpha = shadeBlurAlphaCoeff*float(bgCol[c]);

						if (im_i >= 0 and im_i < inData.shape()[0] and im_j >= 0 and im_j < inData.shape()[1]) {
							val = inData.valueUnchecked(im_i,im_j,inAlphaChannel) + alpha * val * (1-fgAlphaCoeff);
						}

						ret.atUnchecked(i,j,c) = val;

					}
				}

			}

		}

		return ret;

	}, image->imageData());

	image->setColorModel(outModel);

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
