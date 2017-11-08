# GenieInterface
LCD 4D systems control




##Shell
There is a small script function added for testing individual functions. The shell is commented in the genieInterface.c file. Uncomment the shell and use the command:
> make

To start the shell enter:
> ./genieInterface -s

The shell is limited. You can add extra functionality in shell.c.

##Debugging
To debug genieInterface enter the following command in the directory whit the Makefile
>make dg

## Raspberry Pi compact GPIO options



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


## Todo
- [x] demo
- [ ] todo


