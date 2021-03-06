# Use this file to install all compoments of the Seabed webserver and genieInterface. This is used for the SGR7.

#!bin/sh
PATH=/usr/bin:/bin
CUR_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

#gpio init script
INIT_SCRIPT = "init.sh"
SHUTDOWN_SCRIPT = "shutdown.sh"

echo "Installing..."

GENIE_START = "genieInterface.sh"
WEBSERVER_START = "webserver.sh"

if [ $EUID != 0 ]; then
        sudo "$0" "$@"
        exit $?
fi


#TODO install genie lib
# https://github.com/4dsystems/Diablo16-Serial-Linux-Library
echo "installing library"
cd lib/diablo
make
make install
sudo ldconfig -v
cd ../../
echo "done"

echo "install genieInterface"
make install
make release

if [ $? != 0 ]; then
	echo "genieInterface not installed. Futher installation cancled."
        exit $?
fi
echo "done"

echo "installing django and python libs"
pip install Django
pip install pyserial
echo "done"

# webserver TODO

echo "creating startup files"
#copy startup script to init.d and update rc
sudo cp $GENIE_START /etc/init.d/
update-rc.d $GENIE_START defaults

sudo cp $WEBSERVER_START /etc/init.d/
update-rc.d $WEBSERVER_START defaults
echo "done."
echo "installation complete"
