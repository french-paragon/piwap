#ifndef PIWAP_ABSTRACTIMAGEOPERATION_H
#define PIWAP_ABSTRACTIMAGEOPERATION_H

#include <QObject>
#include <opencv2/opencv.hpp>

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

	/*!
	 * \brief doOperation perform the image operation on a cv::Mat
	 * \param image The image the operation is applied onto.
	 * \return 0 in case of sucess, an int > 0 in case of failure.
	 */
	virtual int doOperation(cv::Mat & image, ImageInfos * infos) const = 0;

	virtual QString typeId() const = 0;

	virtual QString getIconUrl() const;
	virtual QString getOpName() const = 0;

	Q_INVOKABLE virtual QString getPropertiesEditorUrl() const = 0;

	virtual QJsonObject asJsonObject() const;
	virtual void setPropertiesFromJsonObject(QJsonObject const& obj);


Q_SIGNALS:

public Q_SLOTS:

protected:

	static const int registrationCodes;
};

} // namespace Piwap

#endif // PIWAP_ABSTRACTIMAGEOPERATION_H
