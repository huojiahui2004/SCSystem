#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "datacontext.h"
#include "netcontext.h"
#include "systemcontrol.h"
int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    NetContext::getObject()
        ->connectToServer("127.0.0.1",
                          10086);
    SystemControl::getObject();
    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.rootContext()->setContextProperty(
        "dc", DataContext::getObject());
    engine.rootContext()->setContextProperty(
        "nc", NetContext::getObject());
    engine.loadFromModule("SCClient", "Main");

    return QCoreApplication::exec();
}





