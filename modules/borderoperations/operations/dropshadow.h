#ifndef PIWAP_BORDEROPERATIONS_DROPSHADOW_H
#define PIWAP_BORDEROPERATIONS_DROPSHADOW_H

#include <QColor>

#include "operations/abstractimageoperation.h"
#include "operations/abstractoperationfactory.h"

namespace Piwap {
namespace BorderOperations {

class DropShadow : public AbstractImageOperation
{
    Q_OBJECT
public:
    static const QString dropShadowOpTypeId;

    Q_PROPERTY(qint32 dx READ dx WRITE setDx NOTIFY dxChanged)
    Q_PROPERTY(qint32 dy READ dy WRITE setDy NOTIFY dyChanged)

    Q_PROPERTY(qreal blurRadius READ blurRadius WRITE setBlurRadius NOTIFY blurRadiusChanged)

    Q_PROPERTY(QColor bg READ bg WRITE setBg NOTIFY bgChanged)

    DropShadow(QObject *parent = nullptr);

    virtual int doOperation(Magick::Image & image, ImageInfos * infos) const;

    virtual QString typeId() const;

    virtual QString getIconUrl() const;
    virtual QString getOpName() const;

    virtual QString getPropertiesEditorUrl() const;

    qint32 dx() const;
    void setDx(const qint32 &dx);

    qint32 dy() const;
    void setDy(const qint32 &dy);

    qreal blurRadius() const;
    void setBlurRadius(qreal blur_radius);

    QColor bg() const;
    void setBg(const QColor &bg);

Q_SIGNALS:

    void dxChanged(qint32 dx);
    void dyChanged(qint32 dy);

    void blurRadiusChanged(qreal blur_radius);

    void bgChanged(QColor bg);

protected:

    qint32 _dx;
    qint32 _dy;
    qreal _blur_radius;

    QColor _bg;


};

class DropShadowOpFactory : public AbstractOperationFactory {
        Q_OBJECT
public :
        explicit DropShadowOpFactory(QObject * parent = nullptr);

        virtual AbstractImageOperation* factorizeOperation(QObject * operationParent = nullptr) const;

        virtual QString getToolTip() const;
};

} // namespace BorderOperations
} // namespace Piwap

#endif // PIWAP_BORDEROPERATIONS_DROPSHADOW_H
