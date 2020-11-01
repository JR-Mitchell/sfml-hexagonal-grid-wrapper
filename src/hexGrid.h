//This file declares and defines the properties of the hexGrid class
#ifndef HEXGRID_H
#define HEXGRID_H

#include <SFML/Graphics.hpp>
#include <math.h>
#include <vector>
#include <fstream>
#include <string>
#include "exceptions.h"

/**
 * hexGrid class provides the base functionality for rendering a hexagonal grid structure
 */
class hexGrid: public sf::Drawable, public sf::Transformable {
private:
    /**
     * Generic method for initialising the underlying sf::VertexArray.
     * Called by all overloads of the constructor
     *
     * @param sidelength: the length of one of the sides of the hexagonal grid
     */
    void init(unsigned int sidelength);
    // Width of a single tile in the provided tileset, in pixels
    unsigned int textureUnitWidth;
    // Underlying sf::VertexArray storing the 6 equilateral triangles for each hexagon in the grid
    sf::VertexArray triangles;
    // Tileset to apply textures to the terrain with
    sf::Texture * tileset;
    // Vector mapping the index of a grid hexagon to the texture in the tileset that it should be textured with
    std::vector<unsigned char> textures;
    // Overload of sf::Drawable's draw() method
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
public:
    // Constructor method with no provided texture mapping. Sets each hexagon to texture 0.
    hexGrid(unsigned int sidelength, unsigned int textureUnitWidth, sf::Texture * tileset);
    // Constructor method with a texture mapping provided by a data file
    hexGrid(unsigned int sidelength, unsigned int textureUnitWidth, sf::Texture * tileset, std::string texturesFilename);
    // Constructor method with a texture mapping passed directly in as an std::vector<unsigned char>
    hexGrid(unsigned int sidelength, unsigned int textureUnitWidth, sf::Texture * tileset, std::vector<unsigned char> textures);
    /**
     * Sets the texture in the tileset of a particular hexagon
     *
     * @param pointIndex: the index of the hexagon whose texture is set
     * @param texChar: the index in the tileset of the texture to set for this hexagon
     */
    void setTexture(unsigned long pointIndex, unsigned char texChar);
};

#endif // !HEXGRID_H
