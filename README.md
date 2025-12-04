## Submission for Operating System WiSe25/26

by Feliks & Bennet

## Task 03

For task 3 we first of all started with the solution code from task 2
as our task 2 didn't work. We added some of our code, which is mainly
print and cosmetic stuff. 

What we did:

* Added system timer initialization in `system/timer.c` for periodic 
  interrupts (configurable at `start.c`)
* Implemented `dbgu_enable_interrupt()` in `driver/dbgu.c` for 
  interrupt-driven character reception with a buffer 
* Created AIC driver in `driver/aic.c` to configure and route interrupts
* Added `_cpsr_interrupt_enable()` in `system/asm_timer.S` to unmask IRQs
* Implemented `irq_trampoline` in `system/exceptions_asm.S` following 
  AIC protocol (read IVR, call handler, write EOICR)
* Extended `dbgu_init()` to enable receiver/transmitter 
* Created test application in `system/start.c` that waits for keypresses 
  and outputs characters, while timer interrupts print "!" every 1s

## Compiling

The envirnonment assumes that we're working on the andorra server, and
accessing programs for compiling as well as qemu is done through the
/home/mi/linnert/arm directory. Meaning it won't run locally. The compile
scripts as of 04.11.2025 sets library path, tries to compile C code, and
execute the kernel with qemu and afterwards unsetting the library path as
setting the library path breaks other programs such as vim(<3)

by default we run our program with ./compile (while in the lateral_OS)
directory. It may by accident that we submit a version that assumes gdb
in which case either gdb has to be run or the make line in the file
compile has to be changed to the commented out one.


## Scripts

There are now multiple scripts to avoid typing the full commands.
Mainly the compile script is used and the rest is hidden in the 
scripts directory

./compile -> set lib, make, run qemu, unset lib

./gdb -> set lib, run gdb, unset lib

./qemu -> set lib, run qemu, unset lib

./libset and ./unlib

## Theory

For the theory part we used to do latex but we decided it's easier just
to do Markdown instead. For every exercise we add a file with the
solution into the 'theorie' directory.
