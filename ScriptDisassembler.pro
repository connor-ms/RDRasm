QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    lib/Qt-AES/qaesencryption.cpp \
    src/main.cpp \
    src/rage/opcode.cpp \
    src/rage/opcodebase.cpp \
    src/rage/script.cpp \
    src/util/lzx.c \
    src/util/util.cpp \
    src/widgets/disassembler.cpp \
    src/widgets/launchscreen.cpp

HEADERS += \
    lib/Qt-AES/qaesencryption.h \
    src/rage/opcode.h \
    src/rage/opcodebase.h \
    src/rage/script.h \
    src/util/lzx.h \
    src/util/util.h \
    src/widgets/disassembler.h \
    src/widgets/launchscreen.h

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
