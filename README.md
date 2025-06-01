# HDF5 with Multithreaded Concurrency

An implementation enhancing the native HDF5 library (https://github.com/HDFGroup/hdf5) with multi threading.
A pool of POSIX threads handles chunking, filtering and writing data in parallel.

A new API function is created to handle the new request, namely "H5Dwrite_filter_parallel".

For full description of the base library, see [README](README_HDF5.md) 
