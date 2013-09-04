LDFLAGS=-ldl


all: boblight-cycle

boblight-cycle.o: boblight-cycle.cpp hsvcolor.h

hsvcolor.o: hsvcolor.cpp hsvcolor.h

boblight-cycle: boblight-cycle.o hsvcolor.o
	$(CXX) $(LDFLAGS) $^ -o $@
