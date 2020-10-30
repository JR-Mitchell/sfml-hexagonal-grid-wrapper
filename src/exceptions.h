#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>

class loadResourceException: public std::exception {
    virtual const char* what() const throw() {
        return "loadResourceException: the requested resource was not successfully loaded.";
    }
};

class initialiseGridTextureMapException: public std::exception {
    virtual const char* what() const throw() {
        return "initialiseGridException: the texture mapping used to initialise the grid had the incorrect number of elements.";
    }
};

#endif // !EXCEPTIONS_H
