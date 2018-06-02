# Colorless

Colorless is a fork of less which colorizes output based on regular expressions defined in a configuration file.

# Why colorless?
At my current work, I make extensive use of less to review large text files and I had always thought that a bit of color would assist in identifying interesting patterns when quickly scanning a file.

# Example

## Before

![N|Solid](https://github.com/asp22/colorless/images/before.png)

## After

![N|Solid](https://github.com/asp22/colorless/images/after.png)


# Limitations
- The code has been designed to work on *linux*; it may work on mac, but not in windows - sorry
- you'll need a compiler that has c++ 14 support (The main colorizing logic has been written in C++)
