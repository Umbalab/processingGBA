# Processing.gba
A C language graphics library inspired by processing to create Game Boy Advance ROMs.

The main idea here was to develop a library with graphics functions for GBA Mode 5. To achieve this, I adapted the writing style of some of the Processing functions and some of their examples.

# How to make things work properly

Here we will basically need five (5) things:

1. DevKitPro;

2. A text editor that helps highlight lines of code (usually called an Integrated Development Environment - IDE);

3. An emulator that supports the Game Boy Advance;

4. An image converter (jpeg, png, and others) to C; and

5. The processingGBA library.

## 1. Install DevKitPro

The first thing to do, whether to make the library work normally or to create any GBA ROM, is to install DevKitPro:
https://devkitpro.org/wiki/Getting_Started

For Windows users, download and install the .exe from this repository:

https://github.com/devkitPro/installer/releases

For Linux and MacOS users, follow the instructions here:

https://github.com/devkitPro/pacman/releases/tag/v6.0.2

## 2. Install some IDE
Editing code to work on a computer can be done using even a simple notepad program. The editing program depends largely on the programmer's preference. There are some relatively simple ones that are very good, such as Notepad++, Sublime, and many others. After testing multiple IDEs over the years, for beginners, I recommend starting with Visual Studio Code. While I'm not a proponent of proprietary software, I say this because Visual Studio can be installed on Windows, macOS, and Linux. It's easy to install, provides tools that can assist in the development of programs beyond the scope of this library, and is used by many companies.

Visual Studio

https://visualstudio.microsoft.com/pt-br/

## 3. Get a GBA Emulator
Again, choosing a Game Boy Advance emulator largely depends on the user's preferences and familiarity. I've seen a lot of praise for emulators like mGBA and many others. Personally, I've always used Visual Boy Advance.

To use Visual Boy Advance, just download the program and extract it somewhere on your computer. Then configure the controls, set it to a larger viewing scale, and you're done. It's a good idea to open any .gba file and set Visual Boy as the default software for opening this type of file.

Visual Boy Advance download site:

https://visualboyadvance.org/

## 4. Image converter
There are multiple image converters we could use to add sprites. I use the GBA GFX Converter, which is available at GBADEV.org.

Link to download the GBA GFX Converter:

https://gbadev.org/tools.php?showinfo=147

The 3DSage tutorial explains how to use the program easily:

https://youtu.be/6ecgELrwAnQ?si=F1-216dZbZ7yBPbU

## 5. The library
Finally, to install the library, simply download the master from this directory (https://github.com/Umbalab/processingGBA/) unzip it and place the ProcessingGBA folder in the C: directory of your computer.

To start any of the projects using the library:
1. Copy the template folder, or copy one of the examples found in the "Examples" folder and paste it into another directory on your computer.

2. Open the MSys2 terminal by going to start>devkitPro>MSys2.

3. In the terminal that opens, type the command "CD," which stands for "change directory," press space, drag the root folder we copied in the first step, or type the directory path, and press Enter.

4. Edit the "scene" file inside the "source" folder of the copied directory.

5. In the MSys2 terminal, type the command "make" and press Enter.

6. Done, now we have a .gba file that was generated inside the directory we are creating :)

# Examples


# Due credits and acknowledgements

I thank the programmers who made their code available for consultation on personal and organizational websites and GitHub. I also thank the members of the Creative Programming group on Telegram for inspiring me to continue coding. I also thank Bianca Caroline Orsso for providing me with a welcoming environment for software development. I dedicate this library to our daughter Alice, who will be born in August 2025.

At no time did I receive any funding for this project. I developed it in a few late nights between October and November 2024 and have now resumed work in July 2025 to make it public before it disappears from my files. I haven't documented many things, and I don't really remember how certain things work, so unfortunately, I don't plan on giving the project much attention in the future. I hope community members will continue and improve certain things I couldn't achieve in time for release—like adding additional features or improving the software's performance.

To be clear, I used the following as a basis for this library:

The files provided by 3DSage:

https://github.com/3DSage/GBA_Mode_5_Starter

For the tutorial developed by 3DSage, which is very explanatory:

https://www.youtube.com/watch?v=6ecgELrwAnQ&t=323s

The Mode 5 example file from gbadev.org:

https://www.gbadev.org/demos.php?showinfo=323

Tonc (coranac) explanations:

https://www.coranac.com/tonc/text/bitmaps.htm

Patater explanations:

https://www.patater.com/gbaguy/gba/ch5.htm

Explanations and tools contained in gbadev:

https://gbadev.org/index.php

It wasn't effective by far, but some times, AI chatbots were assisted with chatgpt3 (https://chatgpt.com/) and Meta AI (https://www.meta.ai/). At great cost, these chatbots helped me fix certain bugs in the code, as well as help me find alternatives for some of the functions, so they should be mentioned here.

All project typography was taken from the Butano project:

https://github.com/GValiente/butano

The majority of the functions developed are based on processing.org functions:

https://processing.org/reference/

Likewise, the examples were adapted from the example library that comes with the Processing IDE download:

https://processing.org/download



Hugs from Curitiba (PR), Brazil,

Mateus F. Lima Pelanda (umbalab)

July 21, 2025
