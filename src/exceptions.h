//This file declares and defines a few useful exceptions with custom error messages
#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>

/**
 * Exception for when Hexagonal Grid Wraooer cannot find a file in the local directories
 */
class loadResourceException: public std::exception {
    virtual const char* what() const throw() {
        return "loadResourceException: the requested resource was not successfully loaded.";
    }
};

/**
 * Exception for when a hexGrid is initialised with a `texture` param with the wrong number of elements
 */
class initialiseGridTextureMapException: public std::exception {
    virtual const char* what() const throw() {
        return "initialiseGridException: the texture mapping used to initialise the grid had the incorrect number of elements.";
    }
};

#endif // !EXCEPTIONS_H
