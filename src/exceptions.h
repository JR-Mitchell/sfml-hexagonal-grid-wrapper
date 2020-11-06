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
 * Exception for when a hexGrid is initialised with a `textures` param with the wrong number of elements
 */
class initialiseGridTextureMapException: public std::exception {
    virtual const char* what() const throw() {
        return "initialiseGridException: the texture mapping used to initialise the grid had the incorrect number of elements.";
    }
};

/**
 * Exception for when a hexGrid is initialised with a `heights` param with the wrong number of elements
 */
class initialiseGridHeightMapException: public std::exception {
    virtual const char* what() const throw() {
        return "initialiseGridException: the height mapping used to initialise the grid had the incorrect number of elements.";
    }
};

/**
 * Exception for when a hexGrid is initialised with a `flatnesses` param with the wrong number of elements
 */
class initialiseGridFlatnessMapException: public std::exception {
    virtual const char* what() const throw() {
        return "initialiseGridException: the flatness mapping used to initialise the grid had the incorrect number of elements.";
    }
};

/**
 * Exception for when a hexGrid is initialised with an `edgeOffsets` param with the wrong number of elements
 */
class initialiseGridOffsetMapException: public std::exception {
    virtual const char* what() const throw() {
        return "initialiseGridException: the edge offset mapping used to initialise the grid had the incorrect number of elements.";
    }
};

/**
 * Exception for when a hexGrid is rotated to a value outside 0,1,2,3,4,5
 */
class setGridRotationException: public std::exception {
    virtual const char* what() const throw() {
        return "setGridException: the requested rotation is outside of accepted values (0,1,2,3,4,5).";
    }
};

/**
 * Exception for when a hexGrid has texture map set with a `textures` param with the wrong number of elements
 */
class setGridTextureMapException: public std::exception {
    virtual const char* what() const throw() {
        return "setGridException: the texture mapping applied to the grid had the incorrect number of elements.";
    }
};

/**
 * Exception for when a hexGrid has height map set with a `heights` param with the wrong number of elements
 */
class setGridHeightMapException: public std::exception {
    virtual const char* what() const throw() {
        return "setGridException: the height mapping applied to the grid had the incorrect number of elements.";
    }
};

/**
 * Exception for when a hexGrid has flatness set with a `flatnesses` param with the wrong number of elements
 */
class setGridFlatnessMapException: public std::exception {
    virtual const char* what() const throw() {
        return "setGridException: the flatness mapping applied to the grid had the incorrect number of elements.";
    }
};

/**
 * Exception for when a hexGrid has edge offsets set with an 'edgeOffsets` param with the wrong number of elements
 */
class setGridOffsetMapException: public std::exception {
    virtual const char* what() const throw() {
        return "setGridException: the edge offset mapping applied to the grid had the incorrect number of elements.";
    }
};

#endif // !EXCEPTIONS_H
