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

    // Set application information
    QCoreApplication::setOrganizationName(QStringLiteral("HousingBuyerApp"));
    QCoreApplication::setApplicationName(QStringLiteral("HousingBuyerManagement"));

    // Create translations directory if it doesn't exist
    QDir dir;
    QString translationsPath = QCoreApplication::applicationDirPath() + QStringLiteral("/translations");
    if (!dir.exists(translationsPath)) {
        dir.mkpath(translationsPath);
    }

    // Create main window and show it
    MainWindow w;
    w.show();

    return app.exec();
}
