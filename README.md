# Linux System-Monitor

An interactive Linux system monitor developed as a course project for the [Udacity C++ Nanodegree Program](https://www.udacity.com/course/c-plus-plus-nanodegree--nd213). 

The project is similar to [htop](htop.dev). It provides an overview of the system, as well as the status of the 10 processes with the highest CPU utilization.

This repo includes [starter code provided by Udacity](https://github.com/udacity/CppND-System-Monitor).

My most significant work on the project was the extra credit addition of the Stream class found in [file_manager.cpp](https://github.com/vorian77/Udacity-CppND-System-Monitor/blob/main/src/file_manager.cpp). This feature provides a generic method for extracting (just about) any data from the Linux file system. Conversely, the starter code recommends a process of copying and modifiyin data extraction routines for each of 13 or so data elements. The FileManager provides the same functionality via two functions.

## Table of Contents

* [Dependencies and Pre-requisites](#dependencies-and-pre-requisites)
* [Instructions](#instructions)
* [Known Issues](#known-issues)
* [Future Plans](#future-plans)
* [Support](#support)
* [Contributing](#contributing)
* [Authors and Acknowledgement](#authors-and-acknowledgement)
* [License](#license)

## Dependencies and Pre-requisites

### ncurses
[ncurses](https://www.gnu.org/software/ncurses/) is a library that facilitates text-based graphical output in the terminal. This project relies on ncurses for display output.

Install ncurses within your own Linux environment: `sudo apt install libncurses5-dev libncursesw5-dev`

### Make
This project uses [Make](https://www.gnu.org/software/make/). The Makefile has four targets:
* `build` compiles the source code and generates an executable
* `format` applies [ClangFormat](https://clang.llvm.org/docs/ClangFormat.html) to style the source code
* `debug` compiles the source code and generates an executable, including debugging symbols
* `clean` deletes the `build/` directory, including all of the build artifacts

## Instructions

1. Clone the project repository.

2. Build the project: `make build`

3. Run the resulting executable: `./build/monitor`

![System Monitor](images/monitor_final.png)

## Known Issues

None.

## Future Plans

None.

## Support

Please open an issue to receive help. 


## Contributing

Pull requests are welcome. Please open an issue to discuss any changes you would like to make or see.


## Authors and Acknowledgement

This project contains starter code provided by [Udacity.com](udacity.com), as well as code (cited within the source) that was derived from [StackExchange.com](StackExchange.com) and other sources. 

## License

[MIT](https://choosealicense.com/licenses/mit/)