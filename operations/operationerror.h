#ifndef OPERATIONERROR_H
#define OPERATIONERROR_H

#include <QString>
#include <QAbstractListModel>

namespace Piwap {

class AbstractImageOperation;

class OperationErrorInfos
{
public:

	OperationErrorInfos();
	OperationErrorInfos(OperationErrorInfos const& other);
	OperationErrorInfos(QString image, QString infos, AbstractImageOperation const* op = nullptr);
	~OperationErrorInfos();

	QString image() const;
	QString infos() const;
	QString getOperationInfos() const;
	bool hasOperation() const;

	bool isValid() const;

private:
	void clearOperation();

	QString _image;
	QString _infos;
	const AbstractImageOperation* _operation;
	bool _isValid;
};

class OperationErrorsList : public QAbstractListModel
{
	Q_OBJECT
public:
	enum ErrorRoles {
			ImageRole = Qt::UserRole + 1,
		    InfosRole = Qt::UserRole + 2,
			OpInfosRole = Qt::UserRole + 3,
			HasOpRole = Qt::UserRole + 4
	    };

	explicit OperationErrorsList(QObject* parent = nullptr);

	virtual int rowCount(QModelIndex const& parent = QModelIndex()) const;
	virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

	void addError(OperationErrorInfos const& errInfos);
	Q_INVOKABLE void removeErrorAt(int row);
	Q_INVOKABLE void clearErrors();

protected:

	static const int registrationCode;
	QHash<int, QByteArray> roleNames() const;

	QList<OperationErrorInfos> _errors;
};

} // namespace Piwap

#endif // OPERATIONERROR_H
