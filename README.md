# About

Header only Netbpm using STL, C++11.

NetBPM is a simple image format which supports both human readable ASCII and binary formats.
For more information see https://en.wikipedia.org/wiki/Netpbm_format

## Formats supported

# Usage

    #include "netbpm.hpp"

 and you're off!

 netbpm is simply a class. To use it

## Read.

To read from a file (either binary or ascii)...

    netbpm data;
    std::ifstream file("image.ppm");
    file >> data;

Alternatively you can construct a netbpm from a string.

    netbpm data(ppmSyntaxAsString);

Or construct from raw byte data.

## Write to file.

To write to a file...

    netbpm data;
    std::ofstream file("image.ppm");
    file << data;       // for binary
or
    file << data.ascii();       // for ascii

## Read from ascii string.




## Construction from raw

# Other functions...

## Pixel access

## Rotations and flips

## Entension






# Tests


# Roadmap

Support for PAM.
Iterators for pixel operations.