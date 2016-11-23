XLennart
========

An XBill Modification.

Maintainer Status
-----------------

(08/03/2015)

I am no longer maintaining XLennart. If you would like to continue this 
project, contact me via this email: staff@bloodbathsoftworks.com

Compiling
---------

XLennart is very simple to compile, as all you need to do is:

	./configure

	make

	sudo make install

libXpm is required when using Motif or Athena widgets.

About
-----

Welcome to XLennart...

Ever get the feeling that nothing is going right? You're a sysadmin, and 
someone's trying to destroy your computers. The little people running 
around the screen are trying to infect your computers with SystenD [TM], a 
virus cleverly designed to resemble a popular init system. Your objective
is to click the mouse on them, ending the potential threat. If one of the 
people reaches a computer, it will attempt to replace your operating system 
with the virus it carries. It will then attempt to run off the screen with 
your vital software. The game ends when only 1 (or 0) of your computers are 
being productive. Additionally, some computers are connected with network 
cables. When one computer on a network becomes infected, a spark will be 
sent down the cable, and will infect the computer on the other end when it 
reaches there.

Clicking the button on one of the little people will cause it to cry out in 
pain and melt (id software eat your heart out!), dropping the stolen os if 
it is carrying one.  If a computer is running SystenD or is temporarily 
off, the os can be dragged back to the computer (or another computer 
compatible with that os).  To extinguish a spark drag the bucket of water 
from the upper left corner onto it.

The status bar at the bottom tells the following:
	
- Number of Lenns on/off the screen
- Number of Computers running their init/off/SystenD
- Level
- Score

Known Issues
------------

Though XLennart should work fine on most Linux and BSD distros, there are 
still some problems since xBill is so old. 

If you encounter a problem with Motif, V.R. from darknedgy.net did this
to compile XLennart:

	./configure --disable-motif

	Then I edited the Makefile and added -lXpm to LDFLAGS.

	Then I had to edit UI.c and remove the #ifdef USE_MOTIF from the guis
	[] array, otherwise I got a missing symbol error.

	make 

This might not be an issue anymore with the improvements done to 1.1

Releases
--------

- Source Code (1.1.1)

https://github.com/Xylemon/xlennart/archive/1.1.1/xlennart-1.1.1.tar.gz

- Source Code (1.1):

https://github.com/Xylemon/xlennart/archive/1.1/xlennart-1.1.tar.gz

- Source Code (1.0):

https://github.com/Xylemon/xlennart/archive/1.0/xlennart-1.0.tar.gz

- ALT Linux:

http://sisyphus.ru/en/srpm/Sisyphus/xlennart

- Arch Linux:

https://aur.archlinux.org/packages/xlennart-git/

- Chakra: 

http://chakraos.org/ccr/packages.php?ID=6940

- FreeBSD:

https://bugs.freebsd.org/bugzilla/show_bug.cgi?id=195977

- OpenBSD:

pkg_add xlennart


Options
-------

	-l <n>	Start at level n.

	--gui <gui>
		Use a specific front end.  The possible values are
		gtk, motif, and athena.  Note that some of these
		may not be compiled into the binary.

	-size <size>
		Play on a field of size x size, instead of the normal
		400x400.  <size> must be larger than 400.

	-v	Print version number and exit.

	-h	Print help and exit.

	When using the GTK gui, all standard GTK options are supported.
	When using the Athena or Motif GUI, all standard X Intrinsics
	options are supported.


Screenshots
-----------

1.1 (Outdated):

![alt tag](https://www.bloodbathsoftworks.com/xylemon/images/projects/screenshots/xlennart/xlennartscreen3.png)

1.0 (Outdated):

![alt tag](https://www.bloodbathsoftworks.com/xylemon/images/projects/screenshots/xlennart/xlennartscreen1.png)
![alt tag](https://www.bloodbathsoftworks.com/xylemon/images/projects/screenshots/xlennart/xlennartscreen2.png)

Credits
-------

- Xylemon <staff@bloodbathsoftworks.com>: "Programmer" and artwork.
- Daniel Reurich <daniel@reurich.kiwi.nz>: Graphical character rework.
- Brian Wellington <bwelling@xbill.org>: Programmer for XBill.
- Matias Duarte <matias@hyperimage.com>: Artist for XBill.

See Credits.md for everyone involved.

Disclaimer
----------

XLennart is **NOT** a personal attack against anyone, it's just a harmless 
parody/commentary on a situation in Linux and Unix history. I do not 
desire or condone the act of violence against any of the Red Hat 
developers. Remember folks, this is just a game!

Also note that this game is **NOT** affiliated with Bloodbath Softworks, any
mentions of the copyright going to Bloodbath Softworks in the previous 
releases were simply a mistake. Only relation Bloodbath had to XLennart 
was the hosting of the main page and files. This will be reuploaded to 
another domain someday...