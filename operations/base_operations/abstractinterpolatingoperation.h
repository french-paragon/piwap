#ifndef ABSTRACTINTERPOLATINGOPERATION_H
#define ABSTRACTINTERPOLATINGOPERATION_H

#include "operations/abstractimageoperation.h"

namespace Piwap {
namespace Operations {

class AbstractInterpolatingOperation : public AbstractImageOperation
{
	Q_OBJECT
public:
	Q_PROPERTY(QString interpolationMode READ interpolationMode WRITE setInterpolationMode NOTIFY interpolationModeChanged)
	Q_PROPERTY(QStringList possibleInterpolationModes READ interpModeStrings STORED false CONSTANT)

	enum InterpolationMode {
		Nearest = cv::INTER_NEAREST,
		Linear = cv::INTER_LINEAR,
		Area = cv::INTER_AREA,
		Cubic = cv::INTER_CUBIC,
		Lanczos = cv::INTER_LANCZOS4
	};

	Q_ENUM(InterpolationMode)

	AbstractInterpolatingOperation(QObject* parent = nullptr);

	QString interpolationMode() const;
	void setInterpolationMode(QString mode);

	QStringList interpModeStrings() const;

Q_SIGNALS:

	void interpolationModeChanged(QString Mode);

protected:

	InterpolationMode _interpolation_mode;
};

} // namespace Operations
} // namespace Piwap

#endif // ABSTRACTINTERPOLATINGOPERATION_H
