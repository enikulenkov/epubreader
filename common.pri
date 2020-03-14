DEFINES += QT_NO_CAST_FROM_ASCII QT_NO_CAST_TO_ASCII QT_NO_KEYWORDS QT_NO_URL_CAST_FROM_STRING

QMAKE_CXXFLAGS += -fno-exceptions -g -O0 -std=c++11 -pedantic -Wno-long-long
QMAKE_LFLAGS += -fno-exceptions -g -O0 -Wl,--as-needed -Wl,-O0

unix {
    isEmpty(PREFIX) {
        PREFIX = /usr
    }

    BINDIR = $$PREFIX/bin
    LIBDIR = $$PREFIX/lib
    PKGLIBDIR = $$LIBDIR/epubreader
    TRANSLATIONSDIR = $$PREFIX/share/epubreader/translations

    DBUSDIR = /usr/share/dbus-1/services

    LOCALE_PATH=/usr/share/locale
}
