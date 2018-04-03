#!/bin/bash

#This script is used to shutdown the SGR7 when the powerbutton is pushed.
#

#TODO: Change pin value
$pin = 1691
state = $(gpio read $pin)

if [ $state -eq 1 ]
    then
        killall python
        shutdown now
fi
