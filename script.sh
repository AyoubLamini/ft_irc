#!/bin/bash

echo "exit" | nc 127.0.0.1 4000  &
sleep 6
kill $! 
