//This file declares and defines the properties of the hexGrid class
#ifndef HEXGRID_H
#define HEXGRID_H

#include "renderRow.h"
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
    // Underlying std::vector storing the rows of the gird
    std::vector<renderRow> rows;
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
     * @param a: the 'a' coordinate of the hexagon to change; i.e the number of down-and-right moves from the top hexagon
     * @param a: the 'b' coordinate of the hexagon to change; i.e the number of down-and-left moves from the top hexagon
     */
    void setTexture(unsigned int a, unsigned int b, unsigned char texChar);
    /**
     * Loads the textures map from a local data file
     *
     * @param texturesFilename: the name of the file to load from
     */
    void loadTexturesFromFile(std::string texturesFilename);
    /**
     * Saves the textures map from a local data file
     *
     * @param texturesFilename: the name of the file to save to
     */
    void saveTexturesToFile(std::string texturesFilename);
};

#endif // !HEXGRID_H
