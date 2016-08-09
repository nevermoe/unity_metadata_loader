# unity_metadata_loader

This project will load strings in global-metadata.dat to IDA.

## Introduction

As you know, if you use IL2CPP to compile an unity game for iOS, all the strings will be stored in global-metadata.dat so you can't find any strings in IDA which significantly makes the static analysis more difficult. With this project, you are able to load the strings in global-metadata.dat to IDA to make analysis a little bit easier.


## Usage

1. Copy Debug/unity_decoder.exe to the same directory as your binary to be analyzed.
2. Copy unity_loader.py to any directory. (I recommned to copy it to the same directory as your binary to be analyzed.)
3. Copy global-metadata.dat to the same directory as your binary to be analyzed. It's located in Data/Managed/Metadata/global-metadata.dat.
4. Load unity_loader.py by using File->Script File.
5. This is the most difficult step. Open your binary with IDA and find the last position of STRINGLITERALS and put your curse right on it. I will show how to find it later.
6. Press Alt-F8. Everything is done.


## Find last position of STRINGLITERALS

The pointer to STRINGLITERALS are in `__const` section of the binary file. Locate to the `__const` section and drag downwards until you see somethinglike this picture: 

![strlit_end](http://www.nevermoe.com/wp-content/uploads/2016/08/strlit_end.png)

![strlit_end](http://www.hdwallpapers.in/walls/love_heart_candy_pair-wide.jpg)

