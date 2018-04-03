# GenieInterface and webserver
The GenieInterface is used to display ..... data on the 4D LCD display.

LCD 4D systems control

 TODO LIST 


[] start up script

[] snelheid verhogen

[] evert ontslaan PRIO

[] tekst overzichtelijker maken

[] webgui 

[] fifo verbeteren

--- 
## Installation

### Requirements
Make sure the library
> wiringPi is installed
To install the genieInterface and webserver. Go to the directory with install.sh.
run:
> ./install.sh

### Disable and edit files
To enable serial communication remove in /boot/cmd.... TODO: complete this.

Make sure serial console is disabled. Also known as 'Agetty'.

To check if the ttyAMA0 is in use, type the following command.
> lsof | grep AMA

This command should be enough. If nothing is shown, the serial port is not being used by a process.

## Genie Shell
There is a small script function added for testing individual functions. The shell is commented in the genieInterface.c file. Uncomment the shell and use the command:
> make

To start the shell enter:
> ./genieInterface -s

The shell is limited. You can add extra functionality in shell.c.

## Debugging genieInterface.c
To debug genieInterface enter the following command in the directory whit the Makefile
>make dg

For final release use:
> make release

### Novatel commands
Here are some commands that can be used for debugging. These commands will have to be sent to the Novatel Chip.
> log version ontime 1

> log gpgga ontime 1

> log ipconfig ontime 1

> log inspvaa ontime 1

> log gphdt onchanged


## Raspberry Pi compact GPIO options
The pinout for the SGR7:



   | GPIO | FUNC1         | FUNC2     |
   | :--- | :---:         | ---:      |
   |    1 | OEM7          | Pos_valid |
   |    2 | OEM7          | Error     |
   |    3 | OEM7          | Me-Ready  |
   |    4 | OEM7          | N_Reset   |
   |    5 | OEM7          | RS232/422 |
   |    6 | OEM7          | SPI/RS    |
   |   12 | display       |           |
   |   13 | display       |           |
   |   14 | display       |           |
   |   15 | display       |           |
   |   19 | switch        | SPIQ      |
   |   20 | switch        | SDA       |
   |   21 | switch        | SCL       |
   |   24 | switch        | SP1_1     |
   |   26 | INT_Powerdown |           |
   |   27 | KILL          |           |
   |   40 | switch        | SCONF1_1  |
   |   41 | switch        | SCONF1_1  |
   |   42 | switch        | PSO_1     |
   |   43 | switch        | SPIS_N    |
   |      |               |           |


## Starting the webserver

To start the webserver, use the command: 
> sudo python manage.py runserver 0.0.0.0:80 

in the shell.

if the port is in use:
> sudo fuser -k 80/tcp

## Webserver files

* api.py: manages the Json requests and parses the serial data
* genieInterface.py parses serial data to genieInterface.c

## Workflow
Todo
