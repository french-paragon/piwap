#ifndef OPERATIONLISTMANAGER_H
#define OPERATIONLISTMANAGER_H

#include <QAbstractListModel>

namespace Piwap {

class AbstractImageOperation;

class OperationListManager : public QAbstractListModel
{
	Q_OBJECT
public:

	enum SpecialRoles {
		IconPathRole = Qt::UserRole
	};

	OperationListManager(QObject* parent = nullptr);

	virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
	virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

	virtual QHash<int, QByteArray> roleNames() const;

	void removeOp (int row);
	void insertOp (AbstractImageOperation* op, int p_row = -1);

	void replaceOps(QList<AbstractImageOperation*> list, bool cleanup = true);

	Q_INVOKABLE Piwap::AbstractImageOperation* opAtRow(int row);

	QList<AbstractImageOperation *> operations() const;

Q_SIGNALS:

	void hasBeenChanged();

public Q_SLOTS:

	void moveAction(int from, int to);

private:

	static const int registrationCode;

	QList<AbstractImageOperation*> _operations;
};

} // namespace Piwap

#endif // OPERATIONLISTMANAGER_H
