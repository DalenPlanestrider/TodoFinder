# TodoFinder
CLI utility to scan source files for todo comments

Currently only supports Linux, but could easily be ported to Windows by reimplementing parseFile to not use mmap()

Designed to map the call to the program to a vim keybinding

Accepted arguments

-h      --help                    Display this message
-bd     --base-dir                Set the base directory to search
-ed     --exclude-dir             Add a directory name to skip searching
-ae     --add-extension           Add a file extension to the whitelist
-ee     --exclude-extension       Add a file extension to the blacklist
-obf    --orderby-file            Order output by file grouping
-obs    --orderby-severity        Order output by severity
-etc    --enable-terminalcolor    Enable colored output to the terminal  (CURRENTLY UNUSED)
-nfn    --no-filename             Disable the filename printing
-ofn    --order-filenameby        Order the filenames by: [alphabetical, discovery, length]
