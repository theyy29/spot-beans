echo --------------------------------------------------

dbg="-g -DDEBUG=4 -DDBGTHREAD=4"
#dbg=""

echo SKIPPING:
echo --------------------------------------------------
echo gcc socketio.c -o socketio $dbg
#gcc socketio.c -o socketio $dbg
echo gcc threading.c command.o -o threading $dbg $links
#gcc threading.c command.o -o threading $dbg $links
echo --------------------------------------------------

links="-lpthread"

echo BUILDING:
echo --------------------------------------------------
echo gcc -c command.c -o command.o $dbg $links
gcc -c command.c -o command.o $dbg $links
echo --------------------------------------------------
echo gcc command.o tns.c -o tns $dbg $links
gcc command.o tns.c -o tns $dbg $links
echo --------------------------------------------------
