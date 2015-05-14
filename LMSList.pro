TEMPLATE = lib
CONFIG += qt plugin

TARGET = LMSListModel

QT += qml quick widgets sql


TARGET = $$qtLibraryTarget($$TARGET)
uri = com.pelagicore.LMSListModel

SOURCES += \
    lmslistmodel.cpp \
    database.cpp \
    lmslistmodelplugin.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
#include(deployment.pri)

HEADERS += \
    lmslistmodel.h \
    database.h \
    lmslistmodelplugin.h

OTHER_FILES += \
    main.qml \
    qmldir

!equals(_PRO_FILE_PWD_, $$OUT_PWD) {
copy_qmldir.target = $$OUT_PWD/qmldir
copy_qmldir.depends = $$_PRO_FILE_PWD_/qmldir
copy_qmldir.commands = $(COPY_FILE) \"$$replace(copy_qmldir.depends, /, $$QMAKE_DIR_SEP)\" \"$$replace(copy_qmldir.target, /, $$QMAKE_DIR_SEP)\"
QMAKE_EXTRA_TARGETS += copy_qmldir
PRE_TARGETDEPS += $$copy_qmldir.target
}

qmldir.files = qmldir
unix {
installPath = $$[QT_INSTALL_QML]/$$replace(uri, \\., /)
    qmldir.path = $$installPath
    target.path = $$installPath
    INSTALLS += target qmldir
}
