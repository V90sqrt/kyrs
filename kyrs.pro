QT += core gui widgets

CONFIG += c++17

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    aboutdialog.cpp \
    buyermodel.cpp \
    buyercontroller.cpp

HEADERS += \
    mainwindow.h \
    aboutdialog.h \
    buyermodel.h \
    buyercontroller.h

FORMS +=

TRANSLATIONS += \
    be.ts \
    en.ts \
    ru.ts \
    translations/be.ts \
    translations/en.ts \
    translations/ru.ts


RESOURCES += \
    resources.qrc

DESTDIR = bin
OBJECTS_DIR = build/obj
MOC_DIR = build/moc
RCC_DIR = build/rcc
UI_DIR = build/ui
