DEBUG=-g -DDEBUG=4 -DDBGTHREAD=4

# 'NORMAL' TARGETS
all: nodebase.o queue.o command.o

clean:
	rm *.o
run:
	echo "Nothing to run!"

# SUB-TARGETS
nodebase.o:
	g++ -c nodebase.cpp $(DEBUG) -o nodebase.o
queue.o:
	g++ -c queue.cpp $(DEBUG) -o queue.o

command.o:
	g++ -c command.cpp $(DEBUG) -o command.o
