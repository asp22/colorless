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

## .colorless

The configuration is split into two parts. 
1. A initialise list of color pairs. These are used for parentheses, square and curly bracket coloring. If a list isn't provided, The colors of the second part of the configuration are used
2. A list defining regular expressions and the corresponding color.

### Example .colorless
```sh
RED_DEF
YEL_DEF
BLU_DEF
GRE_DEF
MAG_DEF
CYA_DEF
WHI_RED
BLA_YEL 

WHI_RED F ERROR
BLA_YEL F WARNING
CYA_DEF S [0-9]{2}:[0-9]{2}:[0-9]{2}.[0-9]{6}
DEF_MAG S [0-9]+\.[0-9]+\.[0-9]+\.[0-9]+
GRE_DEF S true
RED_DEF S false
WHI_RED S disconnected
WHI_GRE S connected
```

colorless supports 8 colors, red(RED), yellow(YEL), blue(blu), green(GRE), magenta(MAG), cyan(CYA), white(WHI), black(BLA) and the default terminal color (DEF). Color definitions take the form XXX_YYY, where XXX is the foreground color and YYY is the background color.

The second part of the configure takes the following form
``` sh
<Color> <S or F> <Regular Expression>
```
'Color' and 'Regular Expression' should be self explanitor, which just leaves S and F needing an explanation.

``` sh
F - F is for Full.
    If a match for the regular expression is found, color the whole line
S - S is for Sub.
    If a match for the regular expression is found, color the matching string only
```
# Quick Start
After checking out the code
```sh
cd src
./configure --prefix=$HOME/my/bin
make
make install
```
After it has built, you can test it by
```sh
# assuming you where in the src directory
cd ..
cd examples
$HOME/my/bin/less example.txt
```
if everything worked out you should see an output that matches the example 'After'.


# Limitations
- The code has been designed to work on *linux*; it may work on mac, but not in windows - sorry
- you'll need a compiler that has c++ 14 support (The main colorizing logic has been written in C++)
- Currently, the binary produced is named ```less```. Take care if you are running ```make install``` with sudo rights. I'll change the name in the future
- My knowledge of ```make``` and ```Makefile```s is poor so I may have done things that don't work for you. Let me know and I'll try to fix it up.

# Problems ?
If you encounter any issues please report it using the issues tab
