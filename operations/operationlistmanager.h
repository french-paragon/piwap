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
