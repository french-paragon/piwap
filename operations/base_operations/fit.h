/*Piwap, or Picture Warping App, is a GUI to batch process images.

Copyright (C) 2019  Paragon<french.paragon@gmail.com>

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
	Q_PROPERTY(QString expandMode READ expandMode WRITE setExpandMode NOTIFY expandModeChanged)

	Q_PROPERTY(QColor bg READ bg WRITE setBg NOTIFY bg_changed)

	Q_PROPERTY(QStringList possibleExpandModes READ expandModeStrings STORED false CONSTANT)

	enum ExpandMode {
		None = 0,
		X = 1,
		Y = 2
	};

	Q_ENUM(ExpandMode)

	explicit Fit(QObject *parent = nullptr);

	virtual int doOperation(Magick::Image & image, ImageInfos * infos) const;

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

	QString expandMode() const;
	void setExpandMode(QString mode);

	QStringList expandModeStrings() const;

Q_SIGNALS:

	void pix_x_changed(int pix_x);
	void pix_y_changed(int pix_y);

	void bg_changed(QColor bg);

	void expandModeChanged(QString newExpandMode);

public Q_SLOTS:

protected:

	int _pix_x;
	int _pix_y;

	QColor _bg;

	ExpandMode _expand_mode;

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
