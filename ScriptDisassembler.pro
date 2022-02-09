QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    lib/Qt-AES/qaesencryption.cpp \
    src/main.cpp \
    src/rage/script.cpp \
    src/util/compression.cpp \
    src/util/endianness.cpp \
    src/util/lzx.c \
    src/widgets/disassembler.cpp \
    src/widgets/launchscreen.cpp

HEADERS += \
    lib/Qt-AES/qaesencryption.h \
    src/rage/script.h \
    src/util/compression.h \
    src/util/endianness.h \
    src/util/lzx.h \
    src/widgets/disassembler.h \
    src/widgets/launchscreen.h

FORMS += \
    src/widgets/disassembler.ui \
    src/widgets/launchscreen.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
