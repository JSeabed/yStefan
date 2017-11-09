#Seabed webserver

##important files
Web -> services. Most important:
1. SerialInterface.py
2. messages.py

Static -> js:
1. controller.js


###Controller.js
Main functions:
1. seabed.settings_controller = function(loader_id)



mail send from Karl:

""""""""""""""""""""""""""""""""""""""""
Hello, 

Find notes below:

The Seabed solution has 3 folders, "seabed", "static" and "web"

The front-end javascript that manages the pages is found in 
static/js/controller.js

In that file, each page on the UI and each functionality has its controller function accordingly. which function is called is dictated in the "router.js" file which manages calling the proper functions for the pages accordingly.

Each input filed in the settings page has some "data-" attributes in the HTML. in these attributes, the appropriate commands that are to be executed to make that change of value are stored there.

The HTML templates that are rendered for the front-end can be found under 
web/templates/

each page declares its "page name" in javascript in order to allow the router to launch the proper controller function.

The server-side controller can be found in the
web/views/api.py

this controller manages all ajax-sent commands against the serial port. 

Services:
The services that the solution uses are the following
- SerialInterface (this class is a layer on top of the python serial module and is meant to facilitate the sending and reading of commands and responses.

- asciiParser (this class checks the responses then parses them into an arrray of values to be analysed according to its command)

- Messages (this class handles the parsing of the array of values into meaningful models to be sent to the front end--the output of said model heavily relies on the command that responded-- which is why each command has its specific transformer class)

Regards, 

""""
