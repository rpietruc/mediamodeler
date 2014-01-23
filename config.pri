# Install paths

QMM_INSTALL_PREFIX = $$[QT_INSTALL_PREFIX]

unix {
    QMM_INSTALL_PREFIX = /usr/local
}

win32 {
    QMM_INSTALL_PREFIX = C:/QMediaModeler
}

QMM_INSTALL_BIN     = $${QMM_INSTALL_PREFIX}/bin
QMM_INSTALL_LIBS    = $${QMM_INSTALL_PREFIX}/lib
QMM_INSTALL_HEADERS = $${QMM_INSTALL_PREFIX}/include/qmediamodeler
QMM_INSTALL_PLUGINS = $${QMM_INSTALL_PREFIX}/share/qmediamodeler/plugins
QMM_INSTALL_INI     = $${QMM_INSTALL_PREFIX}/share/qmediamodeler/ini
