DEBUG=-g -DDEBUG=4 -DDBGTHREAD=4

# 'NORMAL' TARGETS
all: thread.o

clean:
	rm *.o
run:
	echo "Nothing to run!"

# SUB-TARGETS
thread.o:
	g++ -std=c++11 -c thread.cpp $(DEBUG) -o thread.o
