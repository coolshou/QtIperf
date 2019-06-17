#configure iperf3 first (require openssl)
#build static x64 binary
# ./configure --enable-static --disable-shared LDFLAGS="--static"
# make
# cp src/iperf3 ../bin/linux/x86_64/

#build x86 on x86_64 (require gcc-8-i686-linux-gnu, binutils-i686-linux-gnu)
# make distclean
# ./configure --host=i686-linux-gnu --enable-static --disable-shared LDFLAGS="--static"
# make
# cp src/iperf3 ../bin/linux/x86/

#arm (binutils-arm-linux-gnueabi, gcc-arm-linux-gnueabihf g++-arm-linux-gnueabihf (Acqua or RoadRunner)
#                                 gcc-arm-linux-gnueabi g++-arm-linux-gnueabi (Arietta, Aria or FOX G20 board)
# ABI: Application Binary Interface (ABI) for the ARM Architecture
# EABI: 嵌入式ABI
# armel: softfp
# armhf: hard flot point

#require -lssl/-lcrypto
# ./configure --host=arm-linux-gnueabihf --enable-static --disable-shared LDFLAGS="--static"

#windows (cygwin)
#

#Mac

INCLUDEPATH += $$PWD/iperf/src/
HEADERS += $$INCLUDEPATH/iperf_config.h \
        $$INCLUDEPATH/iperf_time.h \
        $$INCLUDEPATH/iperf_locale.h \
	$$INCLUDEPATH/iperf_api.h 

SOURCES += $$INCLUDEPATH/iperf_time.c \
        $$INCLUDEPATH/iperf_locale.c \
        $$INCLUDEPATH/iperf_api.c

#static link iperf ssl
#LIBS +=  -Wl,-Bstatic -liperf -lssl -lcrypto
LIBS +=   -Wl,-Bstati -liperf -lssl -Wl,-Bdynamic -lcrypto

unix {
LIBS += -lsctp
}
