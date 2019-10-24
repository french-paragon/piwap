#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtSvg>

#include <QDebug>

#include "application/application.h"

int main(int argc, char *argv[])
{
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	Piwap::Application app(argc, argv);
	app.setWindowIcon(QIcon(":/icons/logo.svg"));

	app.init();

	QQmlApplicationEngine engine;
	engine.rootContext()->setContextProperty("piwapp", QVariant::fromValue(&app));
	engine.load(QUrl(QLatin1String("qrc:/main.qml")));

	return app.exec();
}
