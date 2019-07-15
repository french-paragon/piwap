#ifndef OPERATIONFACTORYMANAGER_H
#define OPERATIONFACTORYMANAGER_H

#include <QAbstractListModel>

namespace Piwap {

class AbstractOperationFactory;

class OperationFactoryManager : public QAbstractListModel
{
	Q_OBJECT
public:

	enum SpecialRoles {
		TypeIdRole = Qt::UserRole,
		IconPathRole = Qt::UserRole + 1
	};

	explicit OperationFactoryManager(QObject *parent = nullptr);

	virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
	virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

	virtual QHash<int, QByteArray> roleNames() const;

	bool insertFactory(AbstractOperationFactory* f);
	bool removeFactory(QString typeId);
	bool hasFactoryInstalled(QString typeId) const;
	int factoryRow(QString typeId) const;
	AbstractOperationFactory* factoryByType(QString typeId) const;

	Q_INVOKABLE QString typeIdByRow(int row) const;

Q_SIGNALS:

public Q_SLOTS:

private:

	static int registrationCodes;

	QList<AbstractOperationFactory*> _factories;
};

} // namespace Piwap

#endif // OPERATIONFACTORYMANAGER_H
