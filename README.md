# TodoFinder
CLI utility to scan source files for todo comments. Like the emacs [fixmee](https://github.com/rolandwalker/fixmee) plugin severity is determined by the number of 'O' or 'E' charaters that trail the todo/fixme statement

Currently only supports Linux, but could easily be ported to Windows by reimplementing parseFile to not use mmap()

Designed to map the call to the program to a vim keybinding

Accepted arguments

|Short|        Long          |                         Description                     |
|:---:|:--------------------:|:-------------------------------------------------------:|
|-h   |--help                |Display this message                                     |
|-bd  |--base-dir            |Set the base directory to search                         |
|-ed  |--exclude-dir         |Add a directory name to skip searching                   |
|-ae  |--add-extension       |Add a file extension to the whitelist                    |
|-ee  |--exclude-extension   |Add a file extension to the blacklist                    |
|-obf |--orderby-file        |Order output by file grouping                            |
|-obs |--orderby-severity    |Order output by severity                                 |
|-etc |--enable-terminalcolor|Enable colored output to the terminal  (CURRENTLY UNUSED)|
|-nfn |--no-filename         |Disable the filename printing                            |
|-ofn |--order-filenameby    |Order the filenames by: [alphabetical, discovery, length]|

Example of usage that I have mapped in vim
ftodo -ed=external -ae=.h -ae=.hpp -ae=.cpp -ae=.c -ofn=length
