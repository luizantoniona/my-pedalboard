#include <QGuiApplication>
#include <QLocale>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QSurfaceFormat>

#include <Control/RegisterControlTypes.h>
#include <Engine/RegisterEngineTypes.h>

int main( int argc, char* argv[] ) {

    QQuickStyle::setStyle( "Basic" );

    QGuiApplication app( argc, argv );

    QSurfaceFormat format;
    format.setSamples( 8 );
    QSurfaceFormat::setDefaultFormat( format );

    QQmlApplicationEngine engine;

    RegisterControlTypes::registerTypes();
    RegisterEngineTypes::registerTypes();

    QObject::connect( &engine, &QQmlApplicationEngine::objectCreationFailed, &app, []() { QCoreApplication::exit( -1 ); }, Qt::QueuedConnection );

    engine.loadFromModule( "main", "Main" );

    return app.exec();
}
