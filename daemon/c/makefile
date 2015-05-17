DEBUG=-g -DDEBUG=4 -DDBGTHREAD=4

# 'NORMAL' TARGETS
all: dir_thread dir_queue main.o net.o nethandle.o
	g++ main.o net.o nethandle.o queue/*.o thread/*.o /home/ethan/Dev/spot-beans/ignore/libspotify/lib/libspotify.so.12 -lpthread -o tns

clean:
	rm *.o
	$(MAKE) clean -C "queue"
	$(MAKE) clean -C "thread"

run:
	./tns

# Subdirectories
.PHONEY: dir_queue dir_thread

dir_queue:
	$(MAKE) all -C "queue"

dir_thread:
	$(MAKE) all -C "thread"


# SUB-TARGETS
main.o:
	g++ -c main.cpp $(DEBUG) -o main.o
net.o:
	g++ -c net.cpp $(DEBUG) -o net.o

nethandle.o:
	g++ -c nethandle.cpp $(DEBUG) -o nethandle.o

