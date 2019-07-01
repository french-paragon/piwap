#ifndef IMAGETOTREATMANAGER_H
#define IMAGETOTREATMANAGER_H

#include <QAbstractListModel>

namespace Piwap {

class ImageToTreatManager : public QAbstractListModel
{
	Q_OBJECT
public:
	explicit ImageToTreatManager(QObject *parent = nullptr);

	virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
	virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

Q_SIGNALS:

public Q_SLOTS:

	void removeImage (int row);
	void insertImage (QString url);

protected:

	QList<QString> _imagesUrls;

private:

	static const int registrationCodes;
};

} // namespace Piwap

#endif // IMAGETOTREATMANAGER_H
