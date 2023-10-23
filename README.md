# lampanel-gtk
E97 Emulator rebuilt from scratch in C + GTK 4, inspired by [K. Polyakov's "Лампанель"](https://kpolyakov.spb.ru/prog/lamp.htm).

# Current status

- [x] App is compiling
- [x] App is opening
- [x] "Run" button
- [x] Code input
- [ ] Code being ran
- [x] By-registor output
- [x] Lamps field
- [ ] Debugging info??

# Time bounds (WHEN DONE????)
Some time in future

# Building & Running
```
#####################################################################################################################
# There is no cmake, program should be compiled with gcc and `gtksourceview5` (aur) should be installed             #
# => gcc $(pkg-config --cflags gtksourceview-5) -o lampanel-gtk lampanel-gtk.c $(pkg-config --libs gtksourceview-5) #
# (I use gcc so have no idea how to compile it in clang or etc)                                                     #
#                                                                                                                   #
# Tested with EndeavourOS + KDE (Wayland) + zen kernel                                                              #
#####################################################################################################################
```

