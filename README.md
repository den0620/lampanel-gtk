# lampanel-gtk
E97 Emulator rebuilt from scratch in C + GTK 4 (gtksourceview 5), inspired by [K. Polyakov's "Лампанель"](https://kpolyakov.spb.ru/prog/lamp.htm).

![Current Lampanel view](https://github.com/den0620/lampanel-gtk/blob/main/lampanel-gtk.png)

# Current status

- [x] App is compiling
- [x] App is opening
- [x] "Run" button (does nothing now)
- [x] Code input
- [x] Code Highlighting
- [ ] Code Compilation
- [ ] Code being ran
- [x] By-registor output
- [x] Lamps field
- [ ] Memory view
- [ ] Debugging info??

# Time bounds (WHEN DONE????)
~~Never? Idk I realised I dont like GTK~~
Ok, GTK is actully not that bad I can say after I saw C++

# Building & Running
Maybe install.sh script will be some time soon
```
#####################################################################################################################
# There is no cmake, program should be compiled with gcc and `gtksourceview5` (aur) should be installed             #
# => gcc $(pkg-config --cflags gtksourceview-5) -o lampanel-gtk lampanel-gtk.c $(pkg-config --libs gtksourceview-5) #
# (I use gcc so have no idea how to compile it in clang or etc)                                                     #
#                                                                                                                   #
# Tested with EndeavourOS + KDE (Wayland) + zen kernel                                                              #
#####################################################################################################################
```

