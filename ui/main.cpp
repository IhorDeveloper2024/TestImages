#include <QApplication>
#include "mainview.h"
#include "commandlineparser.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    CommandLineParser parser(app);
    MainView mainView;
    mainView.start(app, parser.path());
    return app.exec();
}
