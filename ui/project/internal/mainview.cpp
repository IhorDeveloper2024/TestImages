#include "mainview.h"
#include <QQmlApplicationEngine>
#include <QApplication>
#include "datacontroller.h"
#include "imagepic.h"

namespace {
static const QUrl path{"qrc:/Main.qml"};

QString getImportPath() {
    return QDir::currentPath() + "/qml_plugins/imports/";
}

class QMLApplicationEngine : public QQmlApplicationEngine {
public:
    QMLApplicationEngine(MainView *parent = nullptr) : QQmlApplicationEngine(parent) {
        auto provider = new ImagePic();
        addImageProvider("ImagePic", provider);
        rootContext()->setContextProperty("ImagePic", provider);
        addImportPath(getImportPath());
        connect(this, &QQmlApplicationEngine::objectCreationFailed,
                parent, &MainView::screenCreationFailed);
        load(path);
    }
};

void registerTypes() {
    qmlRegisterSingletonInstance<DataController>(
        "DataController", 1, 0, "DataController", DataController::Instance());
}
}

void MainView::start(QApplication &app, const QString&path) {
    connect(this, &MainView::screenCreationFailed,
                     &app, []() { QCoreApplication::exit(-1); });
    registerTypes();
    DataController::Instance()->setDefaultImageFolderPath(path);
    new QMLApplicationEngine(this);
}
