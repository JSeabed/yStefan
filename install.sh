#!bin/sh

GENIE_START = "genieInterface.sh"

if [ $EUID != 0 ]; then
        sudo "$0" "$@"
        exit $?
fi

#copy startup script to init.d and update rc
sudo cp $GENIE_START /etc/init.d/
update-rc.d $GENIE_START defaults

#TODO install genie lib

make release

echo "Done."
