QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/main.cpp \
    src/rage/compiler.cpp \
    src/rage/iopcode.cpp \
    src/rage/opcodefactory.cpp \
    src/rage/opcodes/enter.cpp \
    src/rage/opcodes/helper.cpp \
    src/rage/opcodes/include.cpp \
    src/rage/opcodes/methods.cpp \
    src/rage/opcodes/misc.cpp \
    src/rage/opcodes/string.cpp \
    src/rage/script.cpp \
    src/util/crypto/aes256.cpp \
    src/util/crypto/lzx.c \
    src/util/util.cpp \
    src/util/crypto/xcompress.cpp \
    src/widgets/disassembler.cpp \
    src/widgets/editdialog.cpp \
    src/widgets/launchscreen.cpp \
    src/widgets/opcodetable.cpp

HEADERS += \
    src/rage/compiler.h \
    src/rage/iopcode.h \
    src/rage/opcodefactory.h \
    src/rage/opcodes/enter.h \
    src/rage/opcodes/float.h \
    src/rage/opcodes/helper.h \
    src/rage/opcodes/integer.h \
    src/rage/opcodes/methods.h \
    src/rage/opcodes/misc.h \
    src/rage/opcodes/stack.h \
    src/rage/opcodes/string.h \
    src/rage/opcodes/vector.h \
    src/rage/script.h \
    src/util/crypto/aes256.h \
    src/util/crypto/lzx.h \
    src/util/util.h \
    src/util/crypto/xcompress.h \
    src/util/crypto/zconf.h \
    src/util/crypto/zlib.h \
    src/widgets/disassembler.h \
    src/widgets/editdialog.h \
    src/widgets/launchscreen.h \
    src/widgets/opcodetable.h

FORMS += \
    src/widgets/disassembler.ui \
    src/widgets/editdialog.ui \
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
