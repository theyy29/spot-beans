
dbg="-g -DDEBUG=4"
#$dbg=""

echo RUNNING:
echo gcc two.c -o spot-beans $dbg
gcc two.c -o spot-beans $dbg
