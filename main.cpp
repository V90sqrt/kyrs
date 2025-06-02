#include "mainwindow.h"
#include <QApplication>
#include <QDir>
#include <QFile>
#include <QLocale>
#include <QTranslator>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QCoreApplication::setOrganizationName("HousingBuyerApp");
    QCoreApplication::setApplicationName("HousingBuyerManagement");

    QDir dir;
    QString translationsPath = QCoreApplication::applicationDirPath() + "/translations";
    if (!dir.exists(translationsPath)) {
        dir.mkpath(translationsPath);
    }

    MainWindow w;
    w.show();

    return app.exec();
}
