## Dependencies ##

This implementation requires GTest to be installed on your device and a C++ compiler that is capable of compiling C++23.

## Build ##

```
$ mkdir build
$ cd build
$ cmake ..
$ make
```

## Performance ##

May be measured by running `encrypt` binary from the `build` directory.
On a machine with an AMD Ryzen 7 3800X processor and 32GBb of RAM, the bandwidth is approximately 120 Mbps for encoding and 105 Mbps for decoding.
