# uwidget

**uwidget** is a C++23 header only library designed to test custom containers.

It provides the `Widget` class along with several Policy structs to inject different behaviours, such as `NoMove` and `ThrowAtNthOperation<CopyAssignment>`.

# Getting Started
To use **uwidget**, either:
- Copy the contents `include/uwidget` directory into your project (to, for example `third_party/uwidget`)
- Install the headers with `sudo make install` (or set `UW_INSTALL_PREFIX` to install to a custom location) and use `find_package(uwidget)`

# Upcoming Features
**Features**
- [ ] Dedicated integration with testing frameworks like Google Test and Catch2
- [ ] Memory corruption testing with some sort of sentinel byte guards

**Misc**
- [ ] A proper README
- [ ] Single-header amalgamation
- [ ] Included examples using `uwidget` to test behaviours of STL containers

