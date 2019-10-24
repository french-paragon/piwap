#include "abstractinterpolatingoperation.h"

#include <QVariant>
#include <QMetaEnum>

namespace Piwap {
namespace Operations {

AbstractInterpolatingOperation::AbstractInterpolatingOperation(QObject *parent) :
	AbstractImageOperation (parent),
	_interpolation_mode(Cubic)
{
	connect(this, &AbstractInterpolatingOperation::interpolationModeChanged, this, &AbstractImageOperation::hasBeenChanged);
}

QString AbstractInterpolatingOperation::interpolationMode() const {
	return QVariant::fromValue(_interpolation_mode).toString();
}
void AbstractInterpolatingOperation::setInterpolationMode(QString mode) {

	QMetaEnum m_e = QMetaEnum::fromType<AbstractInterpolatingOperation::InterpolationMode>();

	int index = m_e.keyToValue(mode.toStdString().c_str());

	if (index == -1) {
		return;
	}

	AbstractInterpolatingOperation::InterpolationMode f = static_cast<AbstractInterpolatingOperation::InterpolationMode>(index);

	if (f != _interpolation_mode) {
		_interpolation_mode = f;
		Q_EMIT interpolationModeChanged(interpolationMode());
	}

}

QStringList AbstractInterpolatingOperation::interpModeStrings() const {

	QMetaEnum m_e = QMetaEnum::fromType<AbstractInterpolatingOperation::InterpolationMode>();

	int n_keys = m_e.keyCount();
	QStringList ret;
	ret.reserve(n_keys);

	for (int i = 0; i < n_keys; i++) {
		ret << QString(m_e.key(i));
	}

	return ret;
}

} // namespace Operations
} // namespace Piwap
