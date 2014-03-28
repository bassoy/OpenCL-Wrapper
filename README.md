OpenCL Utility Toolkit
==================

The OpenCL Utility Toolkit contains C++ Wrapper classes for a more convient utilization of the OpenCL API. It allows to efficiently instantiate platforms, device, contexts, queues and other OpenCL objects taking the pain to work with C-functions. Moreover, as C++ is used to provide a convenient interface, we make heavily use of the RAII programming idiom. Thus, deallocation and freeing memory can be easily managed by introducing scopes for the OpenCL objects. The Tutorial folder provides basic examples how multiple objects are instantiated and worked with. The toolkit is tested under Ubuntu 12.04 64-bit, Suse 12.3 with the GCC (>= 4.6.3) and CLANG. Several CPU and GPUs have been programmed utilizing the Intel, AMD/ATI and Nvidia OpenCL drivers featuring the OpenCL standard 1.1 and 1.2.




Folders
--------------

* Code
	* Contains the OpenCL Utiltiy Toolkit.
* Tutorial
	* Contains examples.

Files
--------------
* library.mk
	* Is used to change the OpenCL include and library path directory. You need to modify the library paths. It is assumed that Nvidia uses the OpenCL standard 1.1. All others use the OpenCL standard 1.2.
* README.md
	* This file.
