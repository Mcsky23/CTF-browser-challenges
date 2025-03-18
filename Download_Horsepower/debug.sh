#!/bin/bash

# search for d8 process and attach gdb
pid=$(pgrep -f d8)
if [ -z "$pid" ]; then
  echo "d8 process not found"
  exit 1
fi

echo "Attaching to d8 process $pid"
gdb -p $pid