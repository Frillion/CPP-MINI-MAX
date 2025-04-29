#!/usr/bin/bash

if [ ! -d "./build_logs" ]; then
    mkdir ./build_logs
fi

python -m build 2> ./build_logs/error_log.txt 1> ./build_logs/build_log.txt
source ./ENV/bin/activate
python -m pip install ./dist/ai-0.5-cp313-cp313-linux_x86_64.whl --force-reinstall &> /dev/null
java -jar ./kiosk.jar &
if [ "$1" == "--debug" ]; then
    gdb python
else
    python ./python_src/gameplayer.py
fi
