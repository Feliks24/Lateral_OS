Compiling

The envirnonment assumes that we're working on the andorra server, and accessing programs for compiling as well as qemu is done through the /home/mi/linnert/arm directory. Meaning it won't run locally. The compile scripts as of 04.11.2025 sets library path, tries to compile C code, and execute the kernel with qemu and afterwards unsetting the library path as setting the library path breaks other programs such as vim
