#!/bin/bash

#This script is used to initialise the gpio-pins of the Pi. These pins are used for the display. 
# After the initialisation. The script will read the GPIO pin used for shutdown.

$display_pin1 = 12
$display_pin2 = 13
#TODO
$shutdown_pin = 1691
$battery_pin = 1691

#Set gpio as output
gpio mode $display_pin1 out
gpio mode $display_pin2 out

#Set gpio output high
gpio write $display_pin1 1
gpio write $display_pin2 1


#TODO: Change pin value
while [ 1 ]
      do
	    shutdown_state = $(gpio read $shutdown_pin)
	    battery_state = $(gpio read $battery_pin)
	    if [ $state -eq 1 ] || [ $state2 -eq 1 ] ; then
		    #TODO, change display
		    killall python
		    shutdown now
	    fi
	done
