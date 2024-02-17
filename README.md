# lampanel-gtk
E97 Emulator rebuilt from scratch in C + GTK 4 (gtksourceview 5), inspired by [K. Polyakov's "Лампанель"](https://kpolyakov.spb.ru/prog/lamp.htm).

[![works badge](https://cdn.jsdelivr.net/gh/nikku/works-on-my-machine@v0.2.0/badge.svg)](https://github.com/nikku/works-on-my-machine)
[![GitHub License](https://img.shields.io/github/license/den0620/lampanel-gtk)](https://github.com/den0620/lampanel-gtk/blob/main/LICENSE)

![Current Lampanel view](https://github.com/den0620/lampanel-gtk/blob/main/lampanel-gtk.png)

# Current status

- [x] App compiles
- [x] App opens
- [x] "Run" button (does nothing yet)
- [x] Code input
- [x] Code Highlighting
- [ ] Code Compilation
- [ ] Code runner
- [x] By-registor output
- [x] Lamps field
- [ ] Memory view
- [ ] Debugging info??

# Time bounds (WHEN DONE????)
~~Never? Idk I realised I dont like GTK~~

Ok, I can say GTK is actually not that bad after I saw C++
but still in no hurry

# Building & Running
Firstly you have to install ```gtksourceview5```
- ```apt install libgtksourceview-5-dev```
  on **Debian**-based distros
- ```yay -S gtksourceview5``` (or ```pacman```)
  on **Arch**-based distros

As install.sh script is now available you can just run
```./install.sh```
and binary should be available in ./build

If permission denied do:
```chmod +x ./install.sh```

Sufficient single line: ```./install.sh && ./build/lampanel```

# Contribution

Please help

