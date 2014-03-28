OpenCL Tutorial
==================

The tutorials shall provide examples and illustrate how convenient heterogeneous programming can be. In order to be able to use the tutorials, 'make' the 'libOclWrapper.a'. If you want to create projects, you need to 

* add your new project with
	* a new makefile
	* [a project file '*.pri']
	* all other necessary source and header files
* modify the outer makefile by adding the makefile within your new project folder
* [add your project file '*.pri' to the outer project file '*.pro']


Files
--------------
* Makefile
	* Includes other makefiles which enables to call make for all projects
* tutorial.pro
	* contains all projects, which can be opened with QT-Creator
* Readme.md
	* This file.


Projects
-------------
1. Platform: queries platform and device
2. Context:  creates two contexts on a given platform
3. Queue:    creates two queues on different contexts
4. Program:  creates a program with a kernel
5. Buffer:   creates several buffers on different contexts
6. Kernel:   creates kernels from string and file. Shows also how to work with OpenCL vectors.
7. Events:   creates several queues and performs read/write operations asynchronously. Synchronizes with queues.
8. Matrix:   shows how to work with host matrices and how to perform comparison.
9. Minimum:  performs a minimum operations on vectors and shows how to work with local memory.
