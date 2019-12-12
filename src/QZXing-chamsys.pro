# ChamSys build configurations.
# 
# Building QZXing in Qt Creator
# =============================
# 
# Open the project using the QZXing-chamsys.pro file, and copy one of the
# following qmake options to Project -> Build -> Build Steps -> qmake ->
# Additional arguments:
# 
# CONFIG+=chamsys_quickq # For QuickQ Designer/Console
# CONFIG+=chamsys_magicq # MagicQ PC/Console
# CONFIG+=chamsys_remote # For MagicQ or QuickQ remote app
# CONFIG+=chamsys_all    # Build a library that will work with all three
# 
# 
# Building MagicQ/QuickQ/Remote in Qt Creator
# ===========================================
# 
# To build MagicQ/QuickQ/Remote with QR code support, first open and build the
# QZXing project using the instructions above with the same version of Qt you
# using to build the main project.
# 
# Then set LIBS and INCLUDEPATH in the QuickQ/MagicQ/Remote qmake Additional
# arguments, for example:
# 
# LIBS+="-L/home/james/git/qzxing/build-QZXing-Desktop_Qt_5_11_1_GCC_64bit-Debug/" INCLUDEPATH+="/home/james/git/qzxing/src/"


CONFIG += staticlib

chamsys_quickq {
  CONFIG += \
          enable_encoder_qr_code \
          qzxing_qml
}

chamsys_magicq {
  CONFIG += \
          enable_encoder_qr_code
}

chamsys_remote {
  CONFIG += \
          enable_decoder_qr_code \
          qzxing_qml
}

chamsys_all {
  CONFIG += \
          enable_encoder_qr_code \
          enable_decoder_qr_code \
          qzxing_qml
}


VERSION = 2.3

TARGET = QZXing
TEMPLATE = lib

DEFINES -= DISABLE_LIBRARY_FEATURES

include(QZXing-components.pri)
