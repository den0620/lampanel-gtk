#!/bin/bash

[[ ! -d "build" ]] && mkdir build
cd build

gcc $(pkg-config --cflags gtksourceview-5) -o lampanel-gtk ../src/lampanel-gtk.c $(pkg-config --libs gtksourceview-5)

echo "+------+----------------------------------------+"
echo "| Done | Executable now should be in ./lampanel |"
echo "+------+----------------------------------------+"

