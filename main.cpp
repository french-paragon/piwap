/*Piwap, or Picture Warping App, is a GUI to batch process images.

Copyright (C) 2019  Paragon<french.paragon@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

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

	QObject::connect(&engine, &QQmlApplicationEngine::quit, &app, &QCoreApplication::quit);

	return app.exec();
}
