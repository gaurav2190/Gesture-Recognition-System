Gesture-Recognition-System
==========================

Gesture Recognition System is used to recognize the gestures to execute specific function on computer system. It uses OpenCV and cvBlob libraries to access functions for the same. It requires Ubuntu 12.04 Linux operating system. 

Description
-----------

It is a program which helps in detecting the colored blobs to process the functions.


Requirements
------------

-Ubuntu 12.04 linux operating system
-OpenCV-2.4.0 library
-cvblob library
-gcc compiler

OpenCV-2.4.0 libraries above can be installed using instruction given on site- www.opencv.willowgarage.com .

For cvblob library:
-Download the cvblob library from the link :https://code.google.com/p/cvblob/ to your home directory.
-After that extract it in the same directory.
-Then enter into the extracted directory.
-Enter "make" command in the terminal and press enter.
-copy the output file "libblob.a" to the /usr/local/lib/ .
-register the new library by typing ldconfig and press enter.

Compilation instructions
------------------------

-The package contains a Makefile which is used to compile the programs.
-Open the terminal.
-Go to the directory and then type "make" and press enter as given below
$make

-The command above compiles the code and helps in generating the executable file named as "test".

-After compilation type "./test" and press enter
$./test

-this will execute the file.

-To quit the program type "q" or "Q" in the terminal. 


Author
-------

Gaurav Khatri
