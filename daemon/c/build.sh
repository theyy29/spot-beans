
make







#echo --------------------------------------------------
#
#dbg="-g -DDEBUG=4 -DDBGTHREAD=4"
##dbg=""
#
#echo SKIPPING:
#echo --------------------------------------------------
#echo gcc socketio.c -o socketio $dbg
##gcc socketio.c -o socketio $dbg
#echo gcc threading.c command.o -o threading $dbg $links
##gcc threading.c command.o -o threading $dbg $links
#echo --------------------------------------------------
#
#links="-lpthread libspotify.so"
#
#echo BUILDING:
#echo --------------------------------------------------
#echo gcc -c spi.c -o spi.o $dbg
#gcc -c spi.c -o spi.o $dbg
#echo --------------------------------------------------
#echo gcc -c command.c -o command.o $dbg
#gcc -c command.c -o command.o $dbg
#echo --------------------------------------------------
#echo gcc -c commands.c -o commands.o $dbg
#gcc -c commands.c -o commands.o $dbg
#echo --------------------------------------------------
#echo gcc commands.o command.o spi.o tns.c -o tns $dbg $links
#gcc commands.o command.o spi.o tns.c -o tns $dbg $links
#echo --------------------------------------------------
