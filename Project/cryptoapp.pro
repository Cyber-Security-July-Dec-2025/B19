# CryptoApp.pro
# This file configures the Qt project, linking the necessary modules and the Crypto++ library.

QT += core gui widgets

CONFIG += c++17

TARGET = CryptoApp
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    cryptomanager.cpp

HEADERS += \
    mainwindow.h \
    cryptomanager.h

FORMS += \
    mainwindow.ui

# --- Crypto++ Library Configuration ---
# Tells the linker where to find the Crypto++ library (libcryptopp.a).
# '$$PWD/' refers to the current project directory.
LIBS += -L$$PWD/ -lcryptopp

# Tells the compiler where to find the Crypto++ header files.
# Assumes headers are in a 'cryptopp_headers' subfolder.
INCLUDEPATH += $$PWD/cryptopp_headers

# Link necessary Windows libraries for Crypto++
win32 {
    LIBS += -lws2_32 -lbcrypt
}
