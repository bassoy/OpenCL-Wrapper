OpenCL C++-Wrapper Tutorial
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
