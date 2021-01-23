#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "protocol_error.hpp"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

	QGuiApplication app(argc, argv);

	qmlRegisterUncreatableMetaObject(bioconverter::staticMetaObject, // static meta object
									 "bioconverterenums",				// import statement
									 1, 0,						    // major and minor version of the import
									 "SlaveError",                // name in QML
									 "Error: only enums");          // error in case someone tries to create a MyNamespace object


	QQmlApplicationEngine engine;
	const QUrl url(QStringLiteral("qrc:/main.qml"));
	QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
					 &app, [url](QObject *obj, const QUrl &objUrl) {
		if (!obj && url == objUrl)
			QCoreApplication::exit(-1);
	}, Qt::QueuedConnection);
	engine.load(url);

	return app.exec();
}
