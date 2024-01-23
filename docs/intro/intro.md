# Intro

## Getting Started

First of all you need to download the zip from Github or clone the Github repo
using your `git clone` command.

```code
git clone https://github.com/vygr/ChrysaLisp.git
```

This will create a ChrysaLisp directory with all you need to try the demos and
develop for ChrysaLisp.

### Mac

Install Homebrew ! It's the easy way.

https://brew.sh/

Install the SDL framework from Homebrew with:

```code
brew install sdl2
```

### Linux, PI64

You should be able to use your package manager to install via `apt-get` or
similar with:

```code
sudo apt-get install libsdl2-dev
```

### Windows 64bit

Unzip the snapshot.zip file with your favorite Zip application. With 7Zip just
right click on the snapshot.zip file and choose 'Extract Here'.

Download and install the SDL frameworks from:
https://www.libsdl.org/download-2.0.php

Get the development versions for both frameworks and unzip them somewhere. Copy
the .dll files from the lib folders into your ChrysaLisp folder.

SDL2.dll
zlib1.dll

## Building the platform bootstrap

### Mac, Linux, PI64

Go into the ChrysaLisp directory and type `make install`. This will unpack the
boot image files, create the ChrysaLisp directory structures and use your C
compiler to compile and link the platform bootstrap executable.

There are a few other options available `clean` `boot` that you might need as
you get further involved, details are in the README.md file.

### Windows 64bit

Currently the main.exe bootstrap comes prebuilt. You can by all means create a
Visual Studio project and compile the main.c bootstrap yourself. Enjoy.

## Running

### Mac, Linux, PI64

After a successful `make` of the platform bootstrap you can now run the TUI
(Text User Interface) or the GUI (Graphical User Interface) and try the demos.

For a TUI or GUI type:

```code
./run_tui.sh  # see available cmds in ./cmd/
./run.sh
```

If you wish to try a more complex network demo, there are ring, mesh, cube,
tree and star network launch files provided. Documentation for these is
provided in the README.md file.

### Windows 64bit

For command line (batch) usage:

```code
.\run_tui.bat
.\run.bat
```

To use PowerShell:
First configure your PowerShell installation to allow scripts to run

Open PowerShell as administrator (only needs to be done once)

```code
Set-ExecutionPolicy -ExecutionPolicy Bypass
```

Run PowerShell

```code
.\run_tui.ps1
.\run.ps1
```

## Stopping

In the TUI situation you can always get back to your platform terminal prompt
by `CNTRL-C` in the terminal window.

If you wish to shutdown all the background processes that are created to
simulate the network of virtual CPU's, use:

### Mac, Linux, PI64

```code
./stop.sh
```

### Windows 64bit

```code
.\stop.bat
```

## Building ChrysaLisp

From the TUI or from the GUI Terminal app, the `make` command allows you to
compile the system, create a new boot image, cross compile for the various
supported platforms, create their boot images and scan the source files to
build the reference documentation.

```code
make
```

Compile any files required based on the age of edited source files and produced
binaries.

```code
make all
```

Compile all source files, regardless of ages of files.

```code
make docs
```

Scan source files and create the reference documents.

```code
make platforms
```

Compile and build all platforms not just the current platform.

```code
make it
```

All of the above.

You can use combinations if you like. Such as:

```code
make all platforms boot
```

This will compile all files for all platforms and create the boot_image files
for all platforms.

## Running the Lisp

At a ChrysaLisp terminal prompt type:

```code
lisp
```

You will see the sign on message for the Lisp interpreter and then you can use
the REPL directly to experiment.

## How to bridge ChrysaLisp Lisp subnets over ChrysaLib hubs !

First go and install the ChrysaLib project and build it according to the
README.md !

Remote machine, say my Raspberry Pi4 at 192.168.1.94, over in the living room:

```code
../../C++/ChrysaLib/hub_node -shm&
./run.sh or ./run_tui.sh
link CLB-L1
```

Local machine, say my MacBook:

```code
../../C++/ChrysaLib/hub_node -shm 192.168.1.94&
./run.sh or ./run_tui.sh
link CLB-L1
```

The `link` command is typed in at your ChrysaLisp TUI or Terminal. Notice that
we run the `hub_node`'s in the background.
