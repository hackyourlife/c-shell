C-Shell
=======

Did you ever have "a lot" of space, like e.g. an entire 4k page, and you had to
write some complex shellcode, but you couldn't be bothered to do it in
assembly?

This project provides a minimal project template to develop shellcode in C, for
the AMD64 target. Edit the file `src/main.c` and add your custom logic. A
minimal libc is available with basic string and I/O functions.


Requirements
------------

- gcc
- make


How to build
------------

Just run `make`. It's that simple.


How to run
----------

You can test your shellcode by running `build/shellcode.elf`. You can also use
the minimal loader and run `build/loader.elf build/shellcode.bin` which will
run the shellcode just like how you will run it in the final target
application.


Where is my shellcode?
----------------------

After compiling your shellcode with `make`, you can get the raw binary data
from `build/shellcode.bin`


I have some weird requirements
------------------------------

If you need to perform some perparation before you can run C code, like when
`rsp` points to nowhere, you can edit `src/_start.s` to set up the environment
appropriately.
