#ifndef HOMEPAGECONTROL_H
#define HOMEPAGECONTROL_H

#include <QObject>

class HomePageControl : public QObject {
    Q_OBJECT

public:
    HomePageControl( QObject* parent = nullptr );
};

#endif // HOMEPAGECONTROL_H
