# Colorless

Colorless is a fork of less which colorizes output based on regular expressions defined in a configuration file.

# Why colorless?
At my current work, I make extensive use of less to review large text files and I had always thought that a bit of color would assist in identifying interesting patterns when quickly scanning a file.

# Example

## Before

![Before Example](https://github.com/asp22/colorless/blob/master/examples/images/before.png)

## After

![After Example](https://github.com/asp22/colorless/blob/master/examples/images/after.png)

# How it works
in your home folder you will need to create a ```.colorless``` file.  This will be your global configuration file. If you want to have different colorization for different projects you can create a local ```.colorless``` which will override the global settings.

# Limitations
- The code has been designed to work on *linux*; it may work on mac, but not in windows - sorry
- you'll need a compiler that has c++ 14 support (The main colorizing logic has been written in C++)
- Currently, the binary produced is named ```less```. Take care if you are running ```make install``` with sudo rights. I'll change the name in the future
- My knowledge of ```make``` and ```Makefile```s is poor so I may have done things that don't work for you. Let me know and I'll try to fix it up.
