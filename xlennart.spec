Summary: Stop Lennart from loading his init system into all the computers.
Name: xlennart
Version: 1.0
Release: 1
Copyright: GPL
Group: Amusements/Games
Source: http://www.bloobathsoftworks.com/xylemon/uploads/xlennart/%{name}-%{version}.tar.gz
BuildRoot: %{_tmppath}/%{name}-root

%description
The XLennart game tests your reflexes as you seek out and destroy all
forms of Lennart, establish a new init system throughout the
universe, and boldly go where no geek has gone before.  XLennart may
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
  mkdir -p ./etc/X11/applnk/Games
  cat > ./etc/X11/applnk/Games/xlennart.desktop <<EOF
[Desktop Entry]
Version=1.0
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
/usr/share/xlennart
%config /etc/X11/applnk/Games/xlennart.desktop

%changelog
* Sun Oct 29 2014 Gethyn ThomasQuail <xylem2020@gmail.com>
- First release!
