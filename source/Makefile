OBJS = \
    C1C.o \
    C1impl.o \
    C1Server.o \
    C2C.o \
    C2impl.o \
    C2Server.o \
    envC.o \
    envServer.o \
    stdsoap2.o \
    WritelogInetServer.o

WritelogInetServer: $(OBJS)
	g++ -O2 -o WritelogInetServer $(OBJS)

envServer.o: envServer.cpp
	g++ -O2 -DWITH_NOSERVEREQUEST -c envServer.cpp

stdsoap2.o: stdsoap2.cpp
	g++ -O2 -DWITH_NONAMESPACES -c stdsoap2.cpp

C1C.o: C1C.cpp
	g++ -O2 -c C1C.cpp

C1impl.o: C1impl.cpp
	g++ -O2 -c C1impl.cpp

C1Server.o: C1Server.cpp
	g++ -O2 -c C1Server.cpp

C2C.o: C2C.cpp
	g++ -O2 -c C2C.cpp

C2impl.o: C2impl.cpp
	g++ -O2 -c C2impl.cpp

C2Server.o: C2Server.cpp
	g++ -O2 -c C2Server.cpp

envC.o: envC.cpp
	g++ -O2 -c envC.cpp

WritelogInetServer.o: WritelogInetServer.cpp
	g++ -O2 -c WritelogInetServer.cpp

