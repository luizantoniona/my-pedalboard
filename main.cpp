#include <QGuiApplication>
#include <QLocale>
#include <QQmlApplicationEngine>
#include <QSurfaceFormat>
#include <QTranslator>

int main( int argc, char* argv[] ) {

    QGuiApplication app( argc, argv );

    QSurfaceFormat format;
    format.setSamples( 8 );
    QSurfaceFormat::setDefaultFormat( format );

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for ( const QString& locale : uiLanguages ) {
        const QString baseName = "my-pedalboard_" + QLocale( locale ).name();

        qInfo() << "Installing translation for:" << baseName;
        if ( translator.load( ":/translations/" + baseName ) ) {
            app.installTranslator( &translator );
            break;
        }
    }

    QQmlApplicationEngine engine;
    engine.addImportPath( QCoreApplication::applicationDirPath() + "/qml" );
    engine.loadFromModule( "Main", "Main" );

    if ( engine.rootObjects().isEmpty() ) {
        return -1;
    }

    return app.exec();
}
