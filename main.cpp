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

    const QUrl url( QUrl::fromLocalFile( QCoreApplication::applicationDirPath() + "/qml/main.qml" ) );

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [ url ]( const QObject* obj, const QUrl& objUrl ) {
            if ( !obj && url == objUrl ) {
                QCoreApplication::exit( -1 );
            }
        },
        Qt::QueuedConnection );
    engine.load( url );

    return app.exec();
}
