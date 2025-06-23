# Big File Reverser

## Key Idea
The key idea is to swap the memory in place first, then fix the encoding later.
This saves a lot of disk I/O when swapping memory in place.

## Design
I designed `FileReverser` with dependencies `IDataProvider` and `IEncodingFixer`.

### IEncodingFixer
`IEncodingFixer` is responsible for fixing the encoding.
Currently, only `Big5EncodingFixer` is supported.

### IDataProvider
`IDataProvider` is responsible for reading and writing data (mapping and unmapping data).
There are two implementations: stream and mmap.

## Analysis

### Stream
For stream, the I/O involves a deliberate cycle: the operating system reads data from disk into kernel space, 
then copies it into a user-provided buffer via system calls. This incurs more overhead but gives you precise control 
over buffer size, read position, and memory usage.

### Mmap
For mmap, it leverages the virtual memory system to “project” the file into your address space. 
There’s no immediate data copy — when you access a memory region, the kernel loads the corresponding page lazily from disk. 
This means fewer syscalls and often much better throughput, especially for large files or in-place processing. 
But since loading is demand-driven, scattered access patterns can cause cache misses and I/O thrashing.

## Test Results
### Test file: 57MB, `CHUNK_SIZE` set to 8MB  
- For stream, it takes around 450ms.  
- For mmap, it takes around 350ms.
### Test file: 25MB, `CHUNK_SIZE` set to 8MB  
- For stream, it takes around 200ms.  
- For mmap, it takes around 133ms. 

## TODO
1. Make it multi-threaded
2. Make it recoverable

## Usage

```sh
cmake .
make BigFileReverseTest
./BigFileReverseTest
```
