# 
a small tool that highlights leaf level GUI-components in an Android application screenshot by parsing and processing metadata

In order to compile it you need Cimg (included in the repo) and ImageMagick and their dependencies
(I have only tested on windows and i didnt do a makefile sorry)

windows: `g++ main.cpp $(pkg-config --cflags --libs Magick++) -lgdi32`

linux/mac: `g++ main.cpp $(pkg-config --cflags --libs Magick++) `

When you run it just type in the name of the <app.package>-<screen#>

for convenience I also put a statically linked pre-compiled exe in the repo (portable.exe). You need 64 bit windows x86 cpu computer to use portable.exe

note that input has to be in the inputs subfolder, and the outputted annotated png will generate in outputs subfolder. Also note that the input needs to be a pair, only of 1 xml file and corresponding 1 png file.
