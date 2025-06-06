#ifndef NAVIGATIONBARCONTROL_H
#define NAVIGATIONBARCONTROL_H

#include <QObject>

class NavigationBarControl : public QObject {
    Q_OBJECT

public:
    NavigationBarControl( QObject* parent = nullptr );
};

#endif // NAVIGATIONBARCONTROL_H
