#ifndef FIT_H
#define FIT_H

#include <QColor>

#include "operations/base_operations/abstractinterpolatingoperation.h"
#include "operations/abstractoperationfactory.h"

namespace Piwap {
namespace Operations {

class Fit : public AbstractInterpolatingOperation
{
	Q_OBJECT
public:

	static const QString fitOpTypeId;

	Q_PROPERTY(int pix_x READ pix_x WRITE setPix_x NOTIFY pix_x_changed)
	Q_PROPERTY(int pix_y READ pix_y WRITE setPix_y NOTIFY pix_y_changed)

	Q_PROPERTY(QColor bg READ bg WRITE setBg NOTIFY bg_changed)

	explicit Fit(QObject *parent = nullptr);

	virtual int doOperation(cv::Mat & image, ImageInfos * infos) const;

	virtual QString typeId() const;

	virtual QString getOpName() const;
	virtual QString getIconUrl() const;

	virtual QString getPropertiesEditorUrl() const;

	int pix_x() const;
	void setPix_x(int pix_x);

	int pix_y() const;
	void setPix_y(int pix_y);

	QColor bg() const;
	void setBg(const QColor &bg);

Q_SIGNALS:

	void pix_x_changed(int pix_x);
	void pix_y_changed(int pix_y);

	void bg_changed(QColor bg);

public Q_SLOTS:

protected:

	int _pix_x;
	int _pix_y;

	QColor _bg;

};

class FitOpFactory : public AbstractOperationFactory {
	Q_OBJECT
public :
	explicit FitOpFactory(QObject * parent = nullptr);

	virtual AbstractImageOperation* factorizeOperation(QObject * operationParent = nullptr) const;

	virtual QString getToolTip() const;
};

} // namespace Operations
} // namespace Piwap

#endif // FIT_H
