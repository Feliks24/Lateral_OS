## Submission for Operating System WiSe25/26

by Feliks & Bennet

## Compiling

The envirnonment assumes that we're working on the andorra server, and
accessing programs for compiling as well as qemu is done through the
/home/mi/linnert/arm directory. Meaning it won't run locally. The compile
scripts as of 04.11.2025 sets library path, tries to compile C code, and
execute the kernel with qemu and afterwards unsetting the library path as
setting the library path breaks other programs such as vim

## Scripts

There are now multiple scripts to avoid typing the full commands

./compile -> set lib, make, run qemu, unset lib

./gdb -> set lib, run gdb, unset lib

./qemu -> set lib, run qemu, unset lib

./libset and ./unlib

## Task specific execution

For Task 01 we compile run qemu with gdb and then connect gdb to check outputs

Terminal 1: $./compile

Terminal 2: $./gdb $target remote :"current port"(1111, 1234)

## Latex

The Latex is supposed to be compiled with pdflatex
