# unity_metadata_loader

This project will load strings and method/class names in global-metadata.dat to IDA.

## Introduction

As most game hackers may know, if you use IL2CPP to compile an Unity game, all the strings used in your original source code will be stored separately in another file call `global-metadata.dat` so that you are not able to find any strings when you load the game binary to IDA. Moreover, as the game is compiled to native assembly, all the symbols like class names or function names are removed from the binary. This significantly makes the static analysis of these games more difficult and is usually considered to be un-hackable. However, with this IDA plugin, you are able to recover all the class names, method names and string constants and mapping them to IDA. Hacking unity games becomes incredibly easy.

Please refer to these two blogs for detailed information:

[还原使用IL2CPP编译的unity游戏的symbol（一）](https://www.nevermoe.com/?p=572)

[还原使用IL2CPP编译的unity游戏的symbol（二 ）](https://www.nevermoe.com/?p=597)


## Installation & Usage

**It's difficult to make a stable auto load funcition, so instead, I provide two helper function `LocateMethodPointers()` and `LocateStringLiterals()` to help you locate the address of g_MethodPointers and g_MetadataUsages. But you have to decide by yourself which adresses are the true addresses from the provided candidate**

1. Copy Release/unity_decoder.exe to the same directory as your binary to be analyzed.
2. Copy unity_loader.py to any directory. (I recommned to copy it to the same directory as your binary to be analyzed.)
3. Copy global-metadata.dat to the same directory as your binary to be analyzed. It's located in Data/Managed/Metadata/global-metadata.dat in the ipa file or apk file.
4. Double click `unity_decoder.exe` and you will get two files: `method_name.txt` and `string_literal.txt` 
5. Load unity_loader.py using File->Script File.
6. Enter `LocateMethodPointers()` in IDA console. This function will give you several possible candidate location of method pointers like this:
```
candidate: 0x6f00b0, candidate end: 0x6f1fd4, method numbers: 1993
candidate: 0x70ae4c, candidate end: 0x70b224, method numbers: 246
candidate: 0x70bee8, candidate end: 0x717e60, method numbers: 12254
```
Open the `method_name.txt` file generated at step 4, the first line is the method pointers' numbers. Find the closest number to the suggested candidate. Sometimes the method pointers are seperated to multiple candidates, then you have to add them up.
  
7. Navigate to the candidate address you believe to be the true address and enter `LoadMethods()` in IDA console.

8. To Load string literals, similar to loading method pointers, enter `LocateStringLiterals()` and then `LoadStringLiterals()`.

## Demo

Note: The v24's binary layout is different to the following picture.

Before recovering the symbols:
![before](https://www.nevermoe.com/wp-content/uploads/2016/09/before.png)

After recovering the symbols:
![after](https://www.nevermoe.com/wp-content/uploads/2016/09/after.png)

## Notice

Now you are able to compile this project directly because I added libil2cpp header files.

This branch "v24" now only support 32bit Android build.

For metadata v23 support, see this fork: [https://github.com/kenjiaiko/unity_metadata_loader](https://github.com/kenjiaiko/unity_metadata_loader)
