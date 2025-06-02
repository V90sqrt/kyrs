#include "mainwindow.h"
#include <QMenuBar>
#include <QMessageBox>
#include <QFileDialog>
#include <QInputDialog>
#include <QSettings>
#include <QHeaderView>
#include <QApplication>
#include <QTranslator>
#include <QDir>
#include <QDebug>
#include <QFile>
#include <QCoreApplication>
#include <string>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    tableView(new QTableView(this)),
    model(new BuyerModel(this)),
    controller(new BuyerController(model, this)),
    selectedColumn(0)
{
    currentLanguage = "ru";
    translator = new QTranslator(this);

    QString translationsPath = QCoreApplication::applicationDirPath() + "/translations/";
    std::string qmFile = translationsPath.toStdString() + currentLanguage + ".qm";

    QFile file(QString::fromStdString(qmFile));
    qDebug() << "Translation file exists:" << file.exists() << qmFile.c_str();

    bool loaded = translator->load(QString::fromStdString(qmFile));
    qDebug() << "Translator loaded from file:" << loaded << qmFile.c_str();

    if (loaded) {
        qApp->installTranslator(translator);
    }

    setupTable();
    setupActions();
    setupMenus();
    setCentralWidget(tableView);
    loadApplicationSettings();
    updateTranslations();
    connect(tableView->horizontalHeader(), &QHeaderView::sectionClicked, this, &MainWindow::sortTableByColumn);
    tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(tableView, &QTableView::customContextMenuRequested, this, &MainWindow::displayContextMenu);
}

MainWindow::~MainWindow() {
    saveApplicationSettings();
}

void MainWindow::setupTable() {
    tableView->setModel(model);
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    tableView->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::SelectedClicked);
    tableView->horizontalHeader()->setStretchLastSection(true);
    tableView->setAlternatingRowColors(true);
}

void MainWindow::setupActions() {
    importAction = new QAction(tr("Import"), this);
    exportAction = new QAction(tr("Export"), this);
    createAction = new QAction(tr("Add Buyer"), this);
    deleteAction = new QAction(tr("Delete"), this);
    columnSearchAction = new QAction(tr("Search in Column"), this);
    globalSearchAction = new QAction(tr("Global Search"), this);
    aboutAction = new QAction(tr("About"), this);
    russianAction = new QAction(tr("Russian"), this);
    englishAction = new QAction(tr("English"), this);
    belarusianAction = new QAction(tr("Belarusian"), this);

    connect(importAction, &QAction::triggered, this, &MainWindow::importData);
    connect(exportAction, &QAction::triggered, this, &MainWindow::exportData);
    connect(createAction, &QAction::triggered, this, &MainWindow::createItem);
    connect(deleteAction, &QAction::triggered, this, &MainWindow::deleteItem);
    connect(columnSearchAction, &QAction::triggered, this, &MainWindow::findInCurrentColumn);
    connect(globalSearchAction, &QAction::triggered, this, &MainWindow::globalSearch);
    connect(aboutAction, &QAction::triggered, this, &MainWindow::showAboutWindow);
    connect(russianAction, &QAction::triggered, this, &MainWindow::changeLanguage);
    connect(englishAction, &QAction::triggered, this, &MainWindow::changeLanguage);
    connect(belarusianAction, &QAction::triggered, this, &MainWindow::changeLanguage);
}

void MainWindow::setupMenus() {
    fileMenu = menuBar()->addMenu(tr("File"));
    fileMenu->addAction(importAction);
    fileMenu->addAction(exportAction);

    editMenu = menuBar()->addMenu(tr("Edit"));
    editMenu->addAction(createAction);
    editMenu->addAction(deleteAction);
    editMenu->addAction(globalSearchAction);

    languageMenu = menuBar()->addMenu(tr("Language"));
    languageMenu->addAction(russianAction);
    languageMenu->addAction(englishAction);
    languageMenu->addAction(belarusianAction);

    helpMenu = menuBar()->addMenu(tr("Help"));
    helpMenu->addAction(aboutAction);
}

void MainWindow::showAboutWindow() {
    AboutDialog dlg(this);
    dlg.exec();
}

void MainWindow::importData() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Text Files (*.txt)"));
    if (!fileName.isEmpty()) {
        if (!BuyerModel::verifyFileFormat(fileName)) {
            QMessageBox::warning(this, tr("Error"), tr("File structure is invalid!"));
            return;
        }
        if (!controller->importData(fileName)) {
            QMessageBox::warning(this, tr("Error"), tr("Failed to import file!"));
        }
    }
}

void MainWindow::exportData() {
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "", tr("Text Files (*.txt)"));
    if (!fileName.isEmpty()) {
        if (!controller->exportData(fileName)) {
            QMessageBox::warning(this, tr("Error"), tr("Failed to export file!"));
        }
    }
}

void MainWindow::createItem() {
    HousingBuyer buyer;
    controller->addBuyer(buyer);
}

void MainWindow::deleteItem() {
    QModelIndexList selection = tableView->selectionModel()->selectedRows();
    if (!selection.isEmpty()) {
        controller->removeBuyer(selection.first().row());
    }
}

void MainWindow::findInCurrentColumn() {
    bool ok;
    int column = selectedColumn;

    QString text = QInputDialog::getText(this, tr("Search in Column"),
                                         tr("Enter search text for column %1:").arg(controller->getHeaderData(column, Qt::Horizontal).toString()),
                                         QLineEdit::Normal, "", &ok);

    if (ok && !text.isEmpty()) {
        QList<int> results = controller->findInColumn(column, text);
        if (!results.isEmpty()) {
            tableView->selectRow(results.first());
            tableView->scrollTo(tableView->model()->index(results.first(), column));
        } else {
            QMessageBox::information(this, tr("Search"), tr("No results found in this column."));
        }
    }
}

void MainWindow::globalSearch() {
    bool ok;
    QString text = QInputDialog::getText(this, tr("Global Search"), tr("Enter search text:"), QLineEdit::Normal, "", &ok);
    if (ok && !text.isEmpty()) {
        QPair<int, int> result = controller->findText(text);
        if (result.first >= 0) {
            tableView->selectRow(result.first);
            tableView->scrollTo(tableView->model()->index(result.first, result.second));
        } else {
            QMessageBox::information(this, tr("Search"), tr("No results found."));
        }
    }
}

void MainWindow::sortTableByColumn(int column) {
    static int currentColumn = -1;
    static Qt::SortOrder currentOrder = Qt::AscendingOrder;

    if (currentColumn == column) {
        currentOrder = (currentOrder == Qt::AscendingOrder) ? Qt::DescendingOrder : Qt::AscendingOrder;
    } else {
        currentColumn = column;
        currentOrder = Qt::AscendingOrder;
    }

    controller->arrangeData(column, currentOrder);
}

void MainWindow::displayContextMenu(const QPoint &pos) {
    QPoint globalPos = tableView->viewport()->mapToGlobal(pos);
    QModelIndex index = tableView->indexAt(pos);
    if (index.isValid()) {
        selectedColumn = index.column();
    }

    QMenu menu(this);
    menu.addAction(createAction);
    menu.addAction(deleteAction);
    menu.addAction(columnSearchAction);
    menu.exec(globalPos);
}

void MainWindow::changeLanguage() {
    QAction *action = qobject_cast<QAction*>(sender());
    if (!action) return;

    std::string lang;
    if (action == russianAction) lang = "ru";
    else if (action == englishAction) lang = "en";
    else if (action == belarusianAction) lang = "be";

    if (lang == currentLanguage) return;

    qApp->removeTranslator(translator);
    currentLanguage = lang;

    delete translator;
    translator = new QTranslator(this);

    QString translationsPath = QCoreApplication::applicationDirPath() + "/translations/";
    std::string qmFile = translationsPath.toStdString() + currentLanguage + ".qm";

    QFile file(QString::fromStdString(qmFile));
    qDebug() << "Translation file exists:" << file.exists() << qmFile.c_str();

    bool loaded = translator->load(QString::fromStdString(qmFile));
    qDebug() << "Translator loaded from file:" << loaded << qmFile.c_str();

    if (loaded) {
        qApp->installTranslator(translator);
    }

    updateTranslations();
    controller->notifyHeaderChanged(Qt::Horizontal, 0, controller->columnCount() - 1);
}

void MainWindow::loadApplicationSettings() {
    QSettings settings("HousingBuyerApp", "MainWindow");
    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("windowState").toByteArray());

    QHeaderView *header = tableView->horizontalHeader();
    for (int i = 0; i < controller->columnCount(); ++i) {
        QString key = QString("column_width_%1").arg(i);
        if (settings.contains(key)) {
            int width = settings.value(key).toInt();
            header->resizeSection(i, width);
        }
    }
}

void MainWindow::saveApplicationSettings() {
    QSettings settings("HousingBuyerApp", "MainWindow");
    settings.setValue("geometry", saveGeometry());
    settings.setValue("windowState", saveState());

    QHeaderView *header = tableView->horizontalHeader();
    for (int i = 0; i < controller->columnCount(); ++i) {
        QString key = QString("column_width_%1").arg(i);
        settings.setValue(key, header->sectionSize(i));
    }
}

void MainWindow::updateTranslations() {
    importAction->setText(tr("Import"));
    exportAction->setText(tr("Export"));
    createAction->setText(tr("Add Buyer"));
    deleteAction->setText(tr("Delete"));
    columnSearchAction->setText(tr("Search in Column"));
    globalSearchAction->setText(tr("Global Search"));
    aboutAction->setText(tr("About"));
    russianAction->setText(tr("Russian"));
    englishAction->setText(tr("English"));
    belarusianAction->setText(tr("Belarusian"));
    fileMenu->setTitle(tr("File"));
    editMenu->setTitle(tr("Edit"));
    languageMenu->setTitle(tr("Language"));
    helpMenu->setTitle(tr("Help"));
    setWindowTitle(tr("Housing Buyer Management"));

    controller->notifyHeaderChanged(Qt::Horizontal, 0, controller->columnCount() - 1);
}
