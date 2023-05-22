# Programming-1sem-labs
Here are the laboratory work of the course "programming" of the first semester of study.

## Meta information editor
The editor is a console program that accepts arguments such as the file name through the 'filepath' parameter, as well as one of the selected commands:
1) 'show' - displays all metadata as a table.
2) 'set=prop_name --value=prop_value' - sets the value of a specific metadata field with the name 'prop_name' to the value 'prop_value'.
3) 'get=prop_name' - outputs metadata with the name 'prop_name'.

## Archiver
The archiver is a console application that accepts the following parameters as arguments:
1) --file FILE - The name of the file archive that the archiver will work with.
2) --create - Command for creating a file archive.
3) --extract - Command for extracting files from the file archive.
4) --list - Command for providing a list of files stored in the archive.
5) FILE1 FILE2 ... FILEN - Free arguments for passing a list of files to be packed.

## Game of life
It is Implementation of Conway's "Game of Life"
>The Game of Life, also known simply as Life, is a cellular automaton devised by the British mathematician John Horton Conway in 1970.It is a zero-player game, meaning that its evolution is determined by its initial state, requiring no further input. One interacts with the Game of Life by creating an initial configuration and observing how it evolves.
> -- <cite>[Wikipedia](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life)</cite>
> 
It is a console application that takes the following parameters as arguments:
1) --input input_file.bmp
Where input_file.bmp is a monochrome picture in bmp format,
storing the initial situation (first generation) of the game
2) --output dir_name
The name of the directory for storing generations of the game in the form
monochrome pictures
3) --max_iter N
The maximum number of generations that can emulate
program. Optional parameter, default is infinity
4) --dump_freq N
The frequency with which the program should save generations of the form
Pictures. Optional parameter, default is 1
