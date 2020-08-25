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

#ifndef PIWAP_ABSTRACTIMAGEOPERATION_H
#define PIWAP_ABSTRACTIMAGEOPERATION_H

#include <QObject>

#include<math.h>
using std::isnan;
#include <Magick++/Image.h>

#include "operations/operationerror.h"

namespace Piwap {

class ImageInfos;

class AbstractImageOperation : public QObject
{
	Q_OBJECT
public:

	Q_PROPERTY(QString typeId READ typeId CONSTANT)

	Q_PROPERTY(QString iconUrl READ getIconUrl STORED false CONSTANT)
	Q_PROPERTY(QString opName READ getOpName STORED false CONSTANT)

	Q_PROPERTY(QString propertiesEditorUrl READ getPropertiesEditorUrl STORED false CONSTANT)

	explicit AbstractImageOperation(QObject *parent = nullptr);
	virtual ~AbstractImageOperation();

	/*!
	 * \brief doOperation perform the image operation on a Magick::Image
	 * \param image The image the operation is applied onto.
	 * \return 0 in case of sucess, an int > 0 in case of failure.
	 */
	virtual int doOperation(Magick::Image & image, ImageInfos * infos) const = 0;

	virtual QString typeId() const = 0;

	virtual QString getIconUrl() const;
	virtual QString getOpName() const = 0;

	virtual bool isSaving() const;

	Q_INVOKABLE virtual QString getPropertiesEditorUrl() const = 0;

	virtual QJsonObject asJsonObject() const;
	virtual void setPropertiesFromJsonObject(QJsonObject const& obj);

	virtual bool event(QEvent *e);

	const OperationErrorInfos& getError() const;
	bool hasError() const;
	void clearError();

Q_SIGNALS:

	void hasBeenChanged();

public Q_SLOTS:

protected:

	static const OperationErrorInfos invalidError;

	void setError(QString imFile, QString infos) const;
	void clearError() const;

	mutable OperationErrorInfos* _error;
	static const int registrationCodes;
};

} // namespace Piwap

#endif // PIWAP_ABSTRACTIMAGEOPERATION_H
