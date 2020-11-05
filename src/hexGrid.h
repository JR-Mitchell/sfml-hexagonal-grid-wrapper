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

    //Variable members

    //The rotation of the grid in 30 degree intervals - either 0,1,2,3,4, or 5.
    unsigned char gridRotation = 0;

    //The length of one of the sides of the hexagonal grid
    const unsigned int s;

    // Width in pixels of a single tile in the provided tileset
    unsigned int textureUnitWidth;

    //Scale factor for heights
    double heightUnit;

    // Underlying std::vector storing the rows of the gird
    std::vector<renderRow> rows;

    // Tileset to use for textures of the terrain
    sf::Texture * tileset;

    // Vector mapping the index of a grid hexagon to the texture in the tileset that it should be textured with
    std::vector<unsigned char> textures;

    // Vector mapping the index of a grid hexagon to the height of that hexagon
    std::vector<char> heights;

    // Vector mapping the index of a grid hexagon to the sharpness of that hexagon
    std::vector<unsigned char> sharpnesses;

    //Methods

    // Overload of sf::Drawable's draw() method
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    /**
     * Generic method for initialising the underlying sf::VertexArray.
     * Called by all overloads of the constructor
     */
    void init();

    /**
     * Updates the entire map with changes to textures - utilised in initialisation, rotation and loading
     */
    void updateTiles();

    /**
     * Calculates the point index and rotated coordinates for given a and b
     *
     * @param a: the 'a' coordinate of the hexagon; i.e the number of down-and-right moves from the top hexagon
     * @param b: the 'b' coordinate of the hexagon; i.e the number of down-and-left moves from the top hexagon
     * @param height: reference to a variable to set to the index of the hexagon
     * @param aPrime: reference to a variable to set to the rotated a coordinate
     * @param bPrime: reference to a variable to set to the rotated b coordinate
     */
    void getOtherCoordinates(unsigned int a, unsigned int b, unsigned long & pointIndex, unsigned int & aPrime, unsigned int & bPrime);

public:
    //Constructors

    /**
     * Constructor method for an empty grid
     *
     * @param sidelength: the length of one of the sides of the grid
     * @param textureUnitWidth: width in pixels of a single tile in the provided tileset
     * @param heightUnit: the scale factor for heights
     * @param tileset: tileset to use for textures of the terrain
     */
    hexGrid(unsigned int sidelength, unsigned int textureUnitWidth, double heightUnit, sf::Texture * tileset);

    /**
     * Constructor method for a grid with values copied from provided std::vectors
     *
     * @param sidelength: the length of one of the sides of the grid
     * @param textureUnitWidth: width in pixels of a single tile in the provided tileset
     * @param heightUnit: the scale factor for heights
     * @param tileset: tileset to use for textures of the terrain
     * @param textures: vector mapping the index of a grid hexagon to the texture in the tileset that it should be textured with
     * @param heights: vector mapping the index of a grid hexagon to the height of that hexagon
     * @param sharpnesses: vector mapping the index of a grid hexagon to the sharpness of that hexagon
     */
    hexGrid(unsigned int sidelength, unsigned int textureUnitWidth, double heightUnit, sf::Texture * tileset, std::vector<unsigned char> & textures, std::vector<char> & heights, std::vector<unsigned char> & sharpnesses);

    //Setters for individual hexagons

    /**
     * Sets the texture in the tileset of a particular hexagon
     *
     * @param a: the 'a' coordinate of the hexagon to change; i.e the number of down-and-right moves from the top hexagon
     * @param b: the 'b' coordinate of the hexagon to change; i.e the number of down-and-left moves from the top hexagon
     * @param texChar: the index of the texture in the tileset
     */
    void setTileTexture(unsigned int a, unsigned int b, unsigned char texChar);

    /**
     * Sets the height of a particular hexagon
     *
     * @param a: the 'a' coordinate of the hexagon to change; i.e the number of down-and-right moves from the top hexagon
     * @param b: the 'b' coordinate of the hexagon to change; i.e the number of down-and-left moves from the top hexagon
     * @param height: the height to set to
     */
    void setTileHeight(unsigned int a, unsigned int b, char height);

    /**
     * Sets the sharpness of a particular hexagon
     *
     * @param a: the 'a' coordinate of the hexagon to change; i.e the number of down-and-right moves from the top hexagon
     * @param b: the 'b' coordinate of the hexagon to change; i.e the number of down-and-left moves from the top hexagon
     * @param sharpness: the sharpness to set to
     */
    void setTileSharpness(unsigned int a, unsigned int b, unsigned char sharpness);

    /**
     * Sets the texture, height and sharpness of a particular hexagon
     *
     * @param a: the 'a' coordinate of the hexagon to change; i.e the number of down-and-right moves from the top hexagon
     * @param b: the 'b' coordinate of the hexagon to change; i.e the number of down-and-left moves from the top hexagon
     * @param texChar: the index of the texture in the tileset
     * @param height: the height to set to
     * @param sharpness: the sharpness to set to
     */
    void setTileInfo(unsigned int a, unsigned int b, unsigned char texChar, char height, unsigned char sharpness);

    //Getters for individual hexagons

    /**
     * Gets the texture in the tileset of a particular hexagon
     *
     * @param a: the 'a' coordinate of the hexagon to get; i.e the number of down-and-right moves from the top hexagon
     * @param b: the 'b' coordinate of the hexagon to get; i.e the number of down-and-left moves from the top hexagon
     * @returns: the index of this tile's texture in the tileset
     */
    unsigned char getTileTexture(unsigned int a, unsigned int b);

    /**
     * Gets the height of a particular hexagon
     *
     * @param a: the 'a' coordinate of the hexagon to get; i.e the number of down-and-right moves from the top hexagon
     * @param b: the 'b' coordinate of the hexagon to get; i.e the number of down-and-left moves from the top hexagon
     * @returns: the height of this tile
     */
    char getTileHeight(unsigned int a, unsigned int b);

    /**
     * Gets the sharpness of a particular hexagon
     *
     * @param a: the 'a' coordinate of the hexagon to get; i.e the number of down-and-right moves from the top hexagon
     * @param b: the 'b' coordinate of the hexagon to get; i.e the number of down-and-left moves from the top hexagon
     * @returns: the sharpness of this tile
     */
    unsigned char getTileSharpness(unsigned int a, unsigned int b);

    /**
     * Gets the info for a particular hexagon by modifying the variables passed by reference
     *
     * @param a: the 'a' coordinate of the hexagon to get; i.e the number of down-and-right moves from the top hexagon
     * @param b: the 'b' coordinate of the hexagon to get; i.e the number of down-and-left moves from the top hexagon
     * @param texChar: reference to a variable to set to the index of the texture in the tileset of the tile
     * @param height: reference to a variable to set to the height of the tile
     * @param sharpness: reference to a variable to set to the sharpness of the tile
     */
    void getTileInfo(unsigned int a, unsigned int b, unsigned char & texChar, char & height, unsigned char & sharpness);

    //Setters for the entire grid

    /**
     * Sets the rotation of the grid to a particular value
     *
     * @param gridRotation: the value to set gridRotation to
     */
    void setGridRotation(unsigned char gridRotation);

    /**
     * Sets the texture map for the entire grid
     *
     * @param textures: vector mapping the index of a grid hexagon to the texture in the tileset that it should be textured with
     */
    void setGridTextures(std::vector<unsigned char> & textures);

    /**
     * Sets the height map for the entire grid
     *
     * @param heights: vector mapping the index of a grid hexagon to the height of that hexagon
     */
    void setGridHeights(std::vector<char> & heights);

    /**
     * Sets the sharpness map for the entire grid
     *
     * @param sharpnesses: vector mapping the index of a grid hexagon to the sharpness of that hexagon
     */
    void setGridSharpnesses(std::vector<unsigned char> & sharpnesses);

    /**
     * Sets the rotation, texture, height and sharpness maps for the entire grid
     *
     * @param gridRotation: the value to set gridRotation to
     * @param textures: vector mapping the index of a grid hexagon to the texture in the tileset that it should be textured with
     * @param heights: vector mapping the index of a grid hexagon to the height of that hexagon
     * @param sharpnesses: vector mapping the index of a grid hexagon to the sharpness of that hexagon
     */
    void setGridInfo(unsigned char gridRotation, std::vector<unsigned char> & textures, std::vector<char> & heights, std::vector<unsigned char> & sharpnesses);

    //Getters for the entire grid

    /**
     * Gets the rotation of the grid
     *
     * @returns: the rotation of the grid
     */
    unsigned char getGridRotation();

    /**
     * Gets the texture map for the entire grid by copying into a vector passed by reference
     *
     * @param textures: reference to a vector to copy the texture map to
     */
    void getGridTextures(std::vector<unsigned char> & textures);

    /**
     * Gets the height map for the entire grid by copying into a vector passed by reference
     *
     * @param heights: reference to a vector to copy the height map to
     */
    void getGridHeights(std::vector<char> & heights);

    /**
     * Gets the sharpness map for the entire grid by copying into a vector passed by reference
     *
     * @param sharpnesses: reference to a vector to copy the sharpness map to
     */
    void getGridSharpnesses(std::vector<unsigned char> & sharpnesses);

};

#endif // !HEXGRID_H
