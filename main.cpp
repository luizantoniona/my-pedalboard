#include <QGuiApplication>
#include <QLocale>
#include <QQmlApplicationEngine>
#include <QSurfaceFormat>
#include <QTranslator>

#include <Core/RegisterTypes.h>

int main( int argc, char* argv[] ) {

    QGuiApplication app( argc, argv );

    QSurfaceFormat format;
    format.setSamples( 8 );
    QSurfaceFormat::setDefaultFormat( format );

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for ( const QString& locale : uiLanguages ) {
        const QString baseName = "my-pedalboard_" + QLocale( locale ).name();
        if ( translator.load( ":/translations/" + baseName ) ) {
            app.installTranslator( &translator );
            break;
        }
    }

    QQmlApplicationEngine engine;

    RegisterTypes::registerTypes();

    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreationFailed, &app,
        []() { QCoreApplication::exit( -1 ); },

        Qt::QueuedConnection );

    engine.loadFromModule( "main", "Main" );

    return app.exec();
}
