echo --------------------------------------------------

dbg="-g -DDEBUG=4"
#$dbg=""

echo SKIPPING:
echo --------------------------------------------------
echo gcc socketio.c -o socketio $dbg
gcc socketio.c -o socketio $dbg

links="-lpthread"

echo BUILDING:
echo --------------------------------------------------
echo gcc -c command.c -o command.o
gcc -c command.c -o command.o
echo --------------------------------------------------
echo gcc threading.c command.o -o threading $dbg $links
gcc threading.c command.o -o threading $dbg $links
