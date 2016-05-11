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

GPP = \
    g++ -std=c++11

WritelogInetServer: $(OBJS)
	$(GPP) -O2 -o WritelogInetServer -static $(OBJS)

envServer.o: envServer.cpp ServerState.h
	$(GPP) -O2 -DWITH_NOSERVEREQUEST -c envServer.cpp

stdsoap2.o: stdsoap2.cpp ServerState.h
	$(GPP) -O2 -DWITH_NONAMESPACES -c stdsoap2.cpp

C1C.o: C1C.cpp ServerState.h
	$(GPP) -O2 -c C1C.cpp

C1impl.o: C1impl.cpp ServerState.h
	$(GPP) -O2 -c C1impl.cpp

C1Server.o: C1Server.cpp ServerState.h
	$(GPP) -O2 -c C1Server.cpp

C2C.o: C2C.cpp ServerState.h
	$(GPP) -O2 -c C2C.cpp

C2impl.o: C2impl.cpp ServerState.h
	$(GPP) -O2 -c C2impl.cpp

C2Server.o: C2Server.cpp ServerState.h
	$(GPP) -O2 -c C2Server.cpp

envC.o: envC.cpp ServerState.h
	$(GPP) -O2 -c envC.cpp

WritelogInetServer.o: WritelogInetServer.cpp ServerState.h
	$(GPP) -O2 -c WritelogInetServer.cpp

