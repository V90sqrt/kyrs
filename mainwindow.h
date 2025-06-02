#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableView>
#include <QMenu>
#include <QAction>
#include <QTranslator>
#include <string>
#include "buyermodel.h"
#include "buyercontroller.h"
#include "aboutdialog.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void showAboutWindow();
    void importData();
    void exportData();
    void createItem();
    void deleteItem();
    void findInCurrentColumn();
    void globalSearch();
    void sortTableByColumn(int column);
    void displayContextMenu(const QPoint &pos);
    void changeLanguage();

private:
    void setupActions();
    void setupMenus();
    void setupTable();
    void loadApplicationSettings();
    void saveApplicationSettings();
    void updateTranslations();

    QTableView *tableView;
    BuyerModel *model;
    BuyerController *controller;

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *languageMenu;
    QMenu *helpMenu;

    QAction *importAction;
    QAction *exportAction;
    QAction *createAction;
    QAction *deleteAction;
    QAction *columnSearchAction;
    QAction *globalSearchAction;
    QAction *aboutAction;
    QAction *russianAction;
    QAction *englishAction;
    QAction *belarusianAction;

    QTranslator *translator;
    std::string currentLanguage;
    int selectedColumn;
};

#endif // MAINWINDOW_H
