#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <QtQuick/QQuickPaintedItem>

class Components : public QQuickPaintedItem {
    Q_OBJECT
    QML_ELEMENT
    Q_DISABLE_COPY( Components )

public:
    explicit Components( QQuickItem* parent = nullptr );
    void paint( QPainter* painter ) override;
    ~Components() override;
};

#endif // COMPONENTS_H
