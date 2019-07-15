#ifndef RESIZE_H
#define RESIZE_H

#include "operations/abstractimageoperation.h"
#include "operations/abstractoperationfactory.h"

namespace Piwap {
namespace Operations {

class Resize : public AbstractImageOperation
{
	Q_OBJECT
public:

	static const QString resizeOpTypeId;

	Q_PROPERTY(int pix_x READ pix_x WRITE setPix_x NOTIFY pix_x_changed)
	Q_PROPERTY(int pix_y READ pix_y WRITE setPix_y NOTIFY pix_y_changed)
	Q_PROPERTY(QString fitMode READ fitMode WRITE setFitMode NOTIFY fitModeChanged)
	Q_PROPERTY(QString interpolationMode READ interpolationMode WRITE setInterpolationMode NOTIFY interpolationModeChanged)

	Q_PROPERTY(QStringList possibleFitModes READ fitModeStrings STORED false CONSTANT)
	Q_PROPERTY(QStringList possibleInterpolationModes READ interpModeStrings STORED false CONSTANT)

	explicit Resize(QObject *parent = nullptr);

	enum FitMode {
		Smaller = 0, //both dimensions are computed so that one is equal to set parameter an the other is smaller, aspect ratio is maintained
		Bigger = 1, //both dimensions are computed so that one is equal to the set parameter and the other is bigger, aspect ratio is maintained
		Average = 2, //both dimensions are computed so that one is smaller and the other bigger than the set parameter. The values are computed such that the image is in average the closest size possible to the desired size while preserving aspect ratio.
		Deform = 3, //The image is deformed such that both dimensions are equal to the set parameter
	};

	Q_ENUM(FitMode)

	enum InterpolationMode {
		Nearest = cv::INTER_NEAREST,
		Linear = cv::INTER_LINEAR,
		Area = cv::INTER_AREA,
		Cubic = cv::INTER_CUBIC,
		Lanczos = cv::INTER_LANCZOS4
	};

	Q_ENUM(InterpolationMode)

	virtual int doOperation(cv::Mat & image, ImageInfos * infos) const;

	virtual QString typeId() const;

	virtual QString getOpName() const;
	virtual QString getIconUrl() const;

	virtual QString getPropertiesEditorUrl() const;


	int pix_x() const;
	void setPix_x(int pix_x);

	int pix_y() const;
	void setPix_y(int pix_y);

	QString fitMode() const;
	void setFitMode(QString mode);

	QString interpolationMode() const;
	void setInterpolationMode(QString mode);

	QStringList fitModeStrings() const;
	QStringList interpModeStrings() const;

Q_SIGNALS:

	void pix_x_changed(int newVal);
	void pix_y_changed(int newVal);
	void fitModeChanged(QString FitMode);
	void interpolationModeChanged(QString Mode);

public Q_SLOTS:

protected:

	int _pix_x;
	int _pix_y;
	FitMode _fit_mode;
	InterpolationMode _interpolation_mode;



};

class ResizeOpFactory : public AbstractOperationFactory {
	Q_OBJECT
public :
	explicit ResizeOpFactory(QObject * parent = nullptr);

	virtual AbstractImageOperation* factorizeOperation(QObject * operationParent = nullptr) const;

	virtual QString getToolTip() const;
};

} // namespace Operations
} // namespace Piwap

#endif // RESIZE_H
