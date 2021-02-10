# ChamSys build configurations.
#
# To include in a Qt project, add the following to the .pro file:
#
#   CONFIG += chamsys_magicq
#   include(3rdparty/qzxing/src/QZXing-chamsys.pri)

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

include(QZXing-components.pri)
