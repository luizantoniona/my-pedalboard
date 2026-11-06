#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>

#include <App/AppContext.h>
#include <App/RegisterAppTypes.h>

int main( int argc, char* argv[] ) {
    QCoreApplication::setOrganizationName( QStringLiteral( "MyPedalboard" ) );
    QCoreApplication::setApplicationName( QStringLiteral( "MyPedalboard" ) );

    QQuickStyle::setStyle( QStringLiteral( "Basic" ) );

    QGuiApplication app( argc, argv );

    AppContext context;
    context.initialize();

    RegisterAppTypes::registerTypes( context );

    QQmlApplicationEngine engine;

    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreationFailed, &app,
        []() { QCoreApplication::exit( -1 ); }, Qt::QueuedConnection );

    engine.loadFromModule( "MyPedalboardAppComponents", "Main" );

    return app.exec();
}
