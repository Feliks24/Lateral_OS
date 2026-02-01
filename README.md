## Submission for Operating System WiSe25/26

by Feliks & Bennet

## Task 05

We started with the example solution since ours didn't fully work last time.

* in lateral_OS/lib/sys_call.c there are basically the library functions for
  system calls that gets used for handling threads including print,getc, kill
current thread, make thread, wait
* the order of these calls is from 1 to 5, 1 being print and 5 wait
* the library function basically mainly call swi #x with x being a number
  between 1 and 5
* for input and output we use the r0 register, where only getc needs an output
* for the testing user thread the thread gets a character and checks if the
  letter is capilized or not and based on that does a loop with printf and
busy_wait or instead uses the library functions that are system calls
* swi handling happens in lateral_OS/system/exception.c
* take the number from swi by getting the swi command and extract the number
* based on the number do different operations
* print and getc are calls to dbgu
* 4 calls start_new_thread with test_print_thread as input
* 5 calls busy_wait; it's not a good solution but we didn't know better
* 3 and default is end_current_thread function
* we wanted to do a userprogram that starts then reads chars and starts threads
  but it didn't work so we commented it out


## Compiling

The envirnonment assumes that we're working on the andorra server, and
accessing programs for compiling as well as qemu is done through the
/home/mi/linnert/arm directory. Meaning it won't run locally. The compile
scripts as of 04.11.2025 sets library path, tries to compile C code, and
execute the kernel with qemu and afterwards unsetting the library path as
setting the library path breaks other programs such as vim(<3)

of course lateral_OS/compile needs to be made compilable with chmod +x compile


