#!/bin/bash

# some older test, doesn't work and complains and I get this message on command line: "QApplication::qAppName: Please instantiate the QApplication object first"
# I also can't enter text after command executes
#echo "Hello World!"
#exec konsole --noclose -e cat ~/.aliases

g++ TestFrameWork/Client.c -o TestFrameWork/Client
for i in {1..20}
do
# opens terminal but then I can't control terminal afterwards
NEW_UUID=$(cat /dev/urandom | tr -dc 'a-zA-Z0-9' | fold -w 5 | head -n 1)
xterm -hold -e ./TestFrameWork/Client $NEW_UUID &
done
