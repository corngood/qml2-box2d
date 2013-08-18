TEMPLATE = lib
TARGET = Box2D
QT += qml quick
CONFIG += qt plugin

TARGET = $$qtLibraryTarget($$TARGET)
uri = Box2D

SOURCES += box2dplugin.cpp \
	box2dworld.cpp \
	box2dbody.cpp \
	box2dfixture.cpp \
	box2ddebugdraw.cpp \
	box2djoint.cpp \
	box2ddistancejoint.cpp \
	box2dprismaticjoint.cpp \
	box2drevolutejoint.cpp \
	box2ddestructionlistener.cpp \
	box2dweldjoint.cpp

HEADERS += \
	box2dplugin.h \
	box2dworld.h \
	box2dbody.h \
	box2dfixture.h \
	box2ddebugdraw.h \
	box2djoint.h \
	box2ddistancejoint.h \
	box2dprismaticjoint.h \
	box2drevolutejoint.h \
	box2ddestructionlistener.h \
	box2dweldjoint.h

include(Box2D/box2d.pri)

OTHER_FILES = qmldir \
    android/version.xml \
    android/res/values-es/strings.xml \
    android/res/values/libs.xml \
    android/res/values/strings.xml \
    android/res/values-it/strings.xml \
    android/res/values-ro/strings.xml \
    android/res/values-fa/strings.xml \
    android/res/values-el/strings.xml \
    android/res/values-id/strings.xml \
    android/res/values-et/strings.xml \
    android/res/values-ja/strings.xml \
    android/res/layout/splash.xml \
    android/res/values-ms/strings.xml \
    android/res/values-ru/strings.xml \
    android/res/values-de/strings.xml \
    android/res/values-zh-rCN/strings.xml \
    android/res/values-rs/strings.xml \
    android/res/values-fr/strings.xml \
    android/res/values-pl/strings.xml \
    android/res/values-zh-rTW/strings.xml \
    android/res/values-pt-rBR/strings.xml \
    android/res/values-nl/strings.xml \
    android/res/values-nb/strings.xml \
    android/src/org/kde/necessitas/ministro/IMinistro.aidl \
    android/src/org/kde/necessitas/ministro/IMinistroCallback.aidl \
    android/src/org/qtproject/qt5/android/bindings/QtApplication.java \
    android/src/org/qtproject/qt5/android/bindings/QtActivity.java \
    android/AndroidManifest.xml \
    android/libjava.so \
    android/Makefile

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

*-g++* {
	QMAKE_CXXFLAGS += -std=c++11
}
