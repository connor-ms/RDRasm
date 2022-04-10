QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    lib/Qt-AES/qaesencryption.cpp \
    src/main.cpp \
    src/rage/iopcode.cpp \
    src/rage/opcodefactory.cpp \
    src/rage/opcodes/include.cpp \
    src/rage/opcodes/methods.cpp \
    src/rage/opcodes/misc.cpp \
    src/rage/opcodes/string.cpp \
    src/rage/script.cpp \
    src/util/lzx.c \
    src/util/util.cpp \
    src/util/xcompress.cpp \
    src/widgets/disassembler.cpp \
    src/widgets/launchscreen.cpp \
    src/widgets/opcodetable.cpp

HEADERS += \
    lib/Qt-AES/qaesencryption.h \
    src/rage/iopcode.h \
    src/rage/opcodefactory.h \
    src/rage/opcodes/float.h \
    src/rage/opcodes/integer.h \
    src/rage/opcodes/methods.h \
    src/rage/opcodes/misc.h \
    src/rage/opcodes/stack.h \
    src/rage/opcodes/string.h \
    src/rage/opcodes/vector.h \
    src/rage/script.h \
    src/util/lzx.h \
    src/util/util.h \
    src/util/xcompress.h \
    src/util/zconf.h \
    src/util/zlib.h \
    src/widgets/disassembler.h \
    src/widgets/launchscreen.h \
    src/widgets/opcodetable.h

FORMS += \
    src/widgets/disassembler.ui \
    src/widgets/launchscreen.ui

include(lib/QHexView/QHexView.pri)

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res/res.qrc

RC_ICONS = res/icon.ico

unix|win32: LIBS += -L$$PWD/lib/ -lzlib

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.
