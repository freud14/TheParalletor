
HEADERS += \
    SocketSelection.h \
    Socket.h \
    sha1Impl.h \
    SHA1.h \
    progressionwindowoperator.h \
    operatorcommunication.h \
    operator.h \
    md5Impl.h \
    MD5.h \
    mainwindowoperator.h \
    invalidsocket.h \
    GeneralProtocol.h \
    Algorithm.h \
    MultiTaches/Thread.h \
    MultiTaches/DequeSafe.hpp \
    MultiTaches/DequeSafe.h \
    BigInteger/NumberlikeArray.hh \
    BigInteger/BigUnsignedInABase.hh \
    BigInteger/BigUnsigned.hh \
    BigInteger/BigIntegerUtils.hh \
    BigInteger/BigIntegerLibrary.hh \
    BigInteger/BigIntegerAlgorithms.hh \
    BigInteger/BigInteger.hh

SOURCES += \
    SocketSelection.cpp \
    Socket.cpp \
    sha1Impl.c \
    SHA1.cpp \
    progressionwindowoperator.cpp \
    operatorcommunication.cpp \
    operator.cpp \
    md5Impl.c \
    MD5.cpp \
    mainwindowoperator.cpp \
    main.cpp \
    invalidsocket.cpp \
    GeneralProtocol.cpp \
    Algorithm.cpp \
    BigInteger/BigUnsignedInABase.cc \
    BigInteger/BigUnsigned.cc \
    BigInteger/BigIntegerUtils.cc \
    BigInteger/BigIntegerAlgorithms.cc \
    BigInteger/BigInteger.cc

#win32:LIBS += $$quote(..\\$${TARGET}\lib\libws2_32 \
#)
win32:LIBS += libws2_32
