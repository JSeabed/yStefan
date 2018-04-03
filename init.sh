#!/bin/bash

#This script is used to initialise the gpio-pins of the Pi. These pins are used for the display. 

$pin1 = 12
$pin2 = 13

#Set gpio as output
gpio mode $pin1 out
gpio mode $pin2 out

#Set gpio output high
gpio write $pin1 1
gpio write $pin2 1
