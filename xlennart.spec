Summary: Stop Lennart from loading his init system into all the computers.
Name: xlennart
Version: 1.1.1
Release: 3
Copyright: GPL
Group: Amusements/Games
Source: https://github.com/Xylemon/%{name}/archive/%{version}/%{name}-%{version}.tar.gz
BuildRoot: %{_tmppath}/%{name}-root

%description
The XLennart game tests your reflexes as you seek out and destroy all
forms of Lennart, establish a new init system throughout the
universe, and boldly go where no geek has gone before. XLennart may
become an increasingly attractive option as the Unix Age progresses,
and it is NOT very popular at Red Hat.

%prep
%setup -q

%build
./configure --prefix=/usr --localstatedir=/var/lib/games --disable-motif
make

%install
rm -rf $RPM_BUILD_ROOT

make DESTDIR=$RPM_BUILD_ROOT install

( cd $RPM_BUILD_ROOT
  mkdir -p ./usr/share/applications/
  cat > ./usr/share/applications/xlennart.desktop <<EOF
[Desktop Entry]
Version=1.1.1
Type=Application
Name=XLennart
Comment=Save the Unix world.
Exec=xlennart
Icon=xlennart
Terminal=false
Categories=Game;ActionGame;
EOF
)

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root)
/usr/bin/xlennart
%config(noreplace) /var/lib/games/xlennart/scores
/usr/share/games/xlennart
%config /usr/share/applications/xlennart.desktop

%changelog
* Sun Nov 2 2014 Gethyn ThomasQuail <xylem2020@gmail.com>
- First release!
* Tue Dec 9 2014 Gethyn ThomasQuail <xylem2020@gmail.com>
- Clean up in makefile and configure scripts (Michael Shigorin and Dmitrij D. Czarkoff)
- Now uses GTK2 instead of GTK1 (Michael Shigorin)
- Improved Distro graphics and added a new one for a special occasion
- Some improvements to other images like the about page
- Removed unsed and outdated images
- Fixed some small copyright text
* Sun Mar 8 2015 Gethyn ThomasQuail <xylem2020@gmail.com>
- New Lennart sprites (Daniel Reurich)
- New Debian/Devuan packaging (Daniel Reurich)
- Some more makefile clean up (Vitalie Ciubotaru)
- Looking for a new maintainer, please contact me at <xylem2020@gmail.com>
