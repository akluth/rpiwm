# rpiwm - a minimalistic window manager (not only) for the Raspberry Pi

## Why?

Because X is actually not that bad and I wanted to write something for my Pi.

## Build

Just make sure you've got all X includes on your system. After that, just hit

    make

or even compile the files by hand.


## Run

Add it to your `.xinitrc`:

    exec /path/to/rpiwm

## Functions

...not much. Currently you can do the following:

    `Alt+Tab` - circulate through windows
    `Alt+Mouse1` - move windows
    `Alt+F12` - exit rpiwm

In the future the ability to tile the windows just as in awesome will be available.


## Customization

Just have a look at `config.h` and configure the things you want.
