#!/bin/bash

# some older test, doesn't work and complains and I get this message on command line: "QApplication::qAppName: Please instantiate the QApplication object first"
# I also can't enter text after command executes
#echo "Hello World!"
#exec konsole --noclose -e cat ~/.aliases

g++ AppServer.c -o AppServer
for i in {9090..9092}
do
# opens terminal but then I can't control terminal afterwards
xterm -hold -e ./AppServer $i &
done
