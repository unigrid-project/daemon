
Debian
====================
This directory contains files used to package huzud/huzu-qt
for Debian-based Linux systems. If you compile huzud/huzu-qt yourself, there are some useful files here.

## huzu: URI support ##


huzu-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install huzu-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your huzuqt binary to `/usr/bin`
and the `../../share/pixmaps/huzu128.png` to `/usr/share/pixmaps`

huzu-qt.protocol (KDE)

