##XLennart

XLennart 1.0 - An XBill Modification

##Compiling

XLennart is very simple to compile, as all you need to do is:

./configure

make

sudo make install

libXpm is required when using Motif or Athena widgets.

##About

Welcome to XLennart...

Ever get the feeling that nothing is going right?  You're a sysadmin,
and someone's trying to destroy your computers.  The little people
running around the screen are trying to infect your computers with
SystenD [TM], a virus cleverly  designed to resemble a popular
init system.  Your objective is to click the mouse on them, ending
the potential threat.  If one of the people reaches a computer, it will
attempt to replace your operating system with the virus it carries. It
will then attempt to run off the screen with your vital software.  The
game ends when only 1 (or 0) of your computers are being productive.
Additionally, some computers are connected with network cables.  When
one computer on a network becomes infected, a spark will be sent down
the cable, and will infect the computer on the other end when it reaches
there.

Clicking the button on one of the little people will cause it to cry out
in pain and melt (id software eat your heart out!), dropping the stolen
os if it is carrying one.  If a computer is running SystenD or is
temporarily off, the os can be dragged back to the computer (or
another computer compatible with that os).  To extinguish a spark drag
the bucket of water from the upper left corner onto it.

	The status bar at the bottom tells the following:
		Number of Lenns on/off the screen
		Number of Computers running their init/off/SystenD
		Level
		Score

## Known Issues

Though XLennart should work fine on most Linux and BSD distros, there are 
still some problems since xBill is so old. 

If you encounter a problem with Motif, V.R. from darknedgy.net did this to 
compile XLennart:

./configure --disable-motif

Then I edited the Makefile and added -lXpm to LDFLAGS.

Then I had to edit UI.c and remove the #ifdef USE_MOTIF from the guis[] array, 
otherwise I got a missing symbol error.

make 

## OpenBSD Port

Right now there's an unofficial OpenBSD port by Dmitrij D. Czarkoff <czarkoff@gmail.com>

http://article.gmane.org/gmane.os.openbsd.ports/70338/match=xlennart (Outdated)
http://article.gmane.org/gmane.os.openbsd.ports/70343/match=xlennart
http://freshbsd.org/search?project=bitrig-ports&q=xlennart

I'm going to look into making this official and adapt any bug fixes from this port.
Thanks for the work guys!

##Options

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


##Screenshots

![alt tag](http://www.bloodbathsoftworks.com/xylemon/images/screenshots/xlennart/xlennartscreen1.png)
![alt tag](http://www.bloodbathsoftworks.com/xylemon/images/screenshots/xlennart/xlennartscreen2.png)

##Authors

Main Programmer:

	Brian Wellington <bwelling@xbill.org> for XBill 2.1

	Gethyn ThomasQuail <xylem2020@gmail.com> for XLennart

Programming & graphics:

	Matias Duarte <matias@hyperimage.com> for XBill 2.0

	Gethyn ThomasQuail <xylem2020@gmail.com> for XLennart
