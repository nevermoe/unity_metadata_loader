# unity_metadata_loader

This project will load strings and method/class names in global-metadata.dat to IDA.

## Introduction

As most game hackers may know, if you use IL2CPP to compile an Unity game, all the strings used in your original source code will be stored separately in another file call `global-metadata.dat` so that you are not able to find any strings when you load the game binary to IDA. Moreover, as the game is compiled to native assembly, all the symbols like class names or function names are removed from the binary. This significantly makes the static analysis of these games more difficult and is usually considered to be un-hackable. However, with this IDA plugin, you are able to recover all the class names, method names and string constants and mapping them to IDA. Hacking unity games becomes incredibly easy.


## Installation & Usage

1. Copy Debug/unity_decoder.exe to the same directory as your binary to be analyzed.
2. Copy unity_loader.py to any directory. (I recommned to copy it to the same directory as your binary to be analyzed.)
3. Copy global-metadata.dat to the same directory as your binary to be analyzed. It's located in Data/Managed/Metadata/global-metadata.dat in the ipa file or apk file.
4. Load unity_loader.py using File->Script File.
5. Press Ctrl+Alt+A and all is done.


## Demo

Before recovering the symbols:
![before](https://www.nevermoe.com/wp-content/uploads/2016/09/before.png)

After recovering the symbols:
![after](https://www.nevermoe.com/wp-content/uploads/2016/09/after.png)

## Declaration

You are not able to compile this project because the libil2cpp's source code is removed from this repository intentionally. If you would like to make modification to this project, please search for libil2cpp's source code on your own. However, any pull request is welcome.
