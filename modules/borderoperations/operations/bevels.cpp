#include "bevels.h"

#include <list>
#include <QFile>

#include "operations/helper_functions/alphachannelsutils.h"

namespace Piwap {
namespace BorderOperations {

const QString Bevels::bevelOpTypeId = "piwapborders/bevel";

Bevels::Bevels(QObject *parent) :
	AbstractImageOperation (parent),
	_radius(50)
{
	connect(this, &Bevels::radiusChanged, this, &AbstractImageOperation::hasBeenChanged);
}

int Bevels::doOperation(Image *image, ImageInfos * infos) const {


	Q_UNUSED(infos);

	ColorModel inModel = image->colorModel();
	ColorModel outModel = addColorModelAlphaChannel(inModel);

	int verticalRadius = _radius;
	int horizontalRadius = _radius;

	image->imageData() = std::visit([inModel, verticalRadius, horizontalRadius] (auto const& inData) -> Image::ImageData {
		using ArrayT = std::decay_t<decltype(inData)>;
		using ScalarT = typename ArrayT::ScalarT;
		ImageArray<ScalarT> ret = addAlphaChannel(inData, inModel);
		int alphaChannel = ret.shape()[2]-1;

		for (int i = 0; i < std::min(verticalRadius, ret.shape()[0]); i++) {

			for (int j = 0; j < std::min(horizontalRadius,ret.shape()[1]); j++) {

				float alphaMask = 0;
				float count = 0;

				for (int di = -1; di <= 1; di++) {
					for (int dj = -1; dj <= 1; dj++) {
						float radiusSqr = 0;
						radiusSqr += float((i-verticalRadius)*(i-verticalRadius))/(verticalRadius*verticalRadius);
						radiusSqr += float((j-horizontalRadius)*(j-horizontalRadius))/(horizontalRadius*horizontalRadius);

						alphaMask += (radiusSqr < 1) ? 1 : 0;
						count += 1;
					}
				}

				alphaMask /= count;

				if (alphaMask >= 0.9999) {
					alphaMask = 1;
				}

				ret.at(i, j, alphaChannel) = alphaMask*float(typedWhiteLevel<ScalarT>());
				ret.at(ret.shape()[0] - i - 1, j, alphaChannel) = alphaMask*float(typedWhiteLevel<ScalarT>());
				ret.at(i, ret.shape()[1] - j - 1, alphaChannel) = alphaMask*float(typedWhiteLevel<ScalarT>());
				ret.at(ret.shape()[0] - i - 1, ret.shape()[1] - j - 1, alphaChannel) = alphaMask*float(typedWhiteLevel<ScalarT>());

			}

		}

		return ret;

	}, image->imageData());

	image->setColorModel(outModel);

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
