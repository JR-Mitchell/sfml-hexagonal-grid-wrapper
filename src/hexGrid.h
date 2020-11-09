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

    // Vector mapping the index of a grid hexagon to the flatness of that hexagon
    std::vector<unsigned char> flatnesses;

    // Vector mapping the index of a grid hexagon to the edge offset of that hexagon
    std::vector<unsigned char> edgeOffsets;

    // Method used for shading / colouring a tile from its characteristics
    std::function<sf::Color(unsigned int a, unsigned int b, unsigned char texChar, char height, sf::Vector3f normal)> colourer;

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
     * Updates the height of a specific tile, making necessary changes to neighbouring tiles as well
     *
     * @param a: the 'a' coordinate of the hexagon; i.e the number of down-and-right moves from the top hexagon
     * @param b: the 'b' coordinate of the hexagon; i.e the number of down-and-left moves from the top hexagon
     */
    void updateTileHeight(unsigned int a, unsigned int b);

    /**
     * Calculates the point index and rotated coordinates for given a and b
     *
     * @param a: the 'a' coordinate of the hexagon; i.e the number of down-and-right moves from the top hexagon
     * @param b: the 'b' coordinate of the hexagon; i.e the number of down-and-left moves from the top hexagon
     * @param pointIndex: reference to a variable to set to the index of the hexagon
     * @param aPrime: reference to a variable to set to the rotated a coordinate
     * @param bPrime: reference to a variable to set to the rotated b coordinate
     */
    void getOtherCoordinates(unsigned int a, unsigned int b, unsigned long & pointIndex, unsigned int & aPrime, unsigned int & bPrime);

    /**
     * Calculates the 9 height values based on neighbouring tile heights for a given tile
     * @param a: the 'a' coordinate of the hexagon; i.e the number of down-and-right moves from the top hexagon
     * @param b: the 'b' coordinate of the hexagon; i.e the number of down-and-left moves from the top hexagon
     * @param pointIndex: the index of the hexagon
     * @param topHeights: size-9 double array to update with the heights of the upper half
     * @param bottomHeights: size-9 double array to update with the heights of the lower half
     */
    void getTileHeights(unsigned int a, unsigned int b, unsigned long pointIndex, double * topHeights, double * bottomHeights);

public:
    //Constructors

    /**
     * Constructor method for an empty grid
     *
     * @param sidelength: the length of one of the sides of the grid
     * @param textureUnitWidth: width in pixels of a single tile in the provided tileset
     * @param heightUnit: the scale factor for heights
     * @param tileset: tileset to use for textures of the terrain
     * @param colourer: method used to colour/shade tiles depending on their height, position, texture and surface vector normals
     */
    hexGrid(unsigned int sidelength, unsigned int textureUnitWidth, double heightUnit, sf::Texture * tileset, std::function<sf::Color(unsigned int a, unsigned int b, unsigned char texChar, char height, sf::Vector3f normal)> colourer);

    /**
     * Constructor method for a grid with values copied from provided std::vectors
     *
     * @param sidelength: the length of one of the sides of the grid
     * @param textureUnitWidth: width in pixels of a single tile in the provided tileset
     * @param heightUnit: the scale factor for heights
     * @param tileset: tileset to use for textures of the terrain
     * @param textures: vector mapping the index of a grid hexagon to the texture in the tileset that it should be textured with
     * @param heights: vector mapping the index of a grid hexagon to the height of that hexagon
     * @param flatnesses: vector mapping the index of a grid hexagon to the flatness of that hexagon
     * @param edgeOffsets: vector mapping the index of a grid hexagon to the edge offset of that hexagon
     * @param colourer: method used to colour/shade tiles depending on their height, position, texture and surface vector normals
     */
    hexGrid(unsigned int sidelength, unsigned int textureUnitWidth, double heightUnit, sf::Texture * tileset, std::vector<unsigned char> & textures, std::vector<char> & heights, std::vector<unsigned char> & flatnesses, std::vector<unsigned char> & edgeOffsets, std::function<sf::Color(unsigned int a, unsigned int b, unsigned char texChar, char height, sf::Vector3f normal)> colourer);

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
     * Sets the flatness of a particular hexagon
     *
     * @param a: the 'a' coordinate of the hexagon to change; i.e the number of down-and-right moves from the top hexagon
     * @param b: the 'b' coordinate of the hexagon to change; i.e the number of down-and-left moves from the top hexagon
     * @param flatness: the flatness to set to
     */
    void setTileFlatness(unsigned int a, unsigned int b, unsigned char flatness);

    /**
     * Sets the edge offset of a particular hexagon
     *
     * @param a: the 'a' coordinate of the hexagon to change; i.e the number of down-and-right moves from the top hexagon
     * @param b: the 'b' coordinate of the hexagon to change; i.e the number of down-and-left moves from the top hexagon
     * @param edgeOffset: the edge offset to set to
     */
    void setTileOffset(unsigned int a, unsigned int b, unsigned char edgeOffset);

    /**
     * Sets the texture, height and flatness of a particular hexagon
     *
     * @param a: the 'a' coordinate of the hexagon to change; i.e the number of down-and-right moves from the top hexagon
     * @param b: the 'b' coordinate of the hexagon to change; i.e the number of down-and-left moves from the top hexagon
     * @param texChar: the index of the texture in the tileset
     * @param height: the height to set to
     * @param flatness: the flatness to set to
     * @param edgeOffset: the edge offset to set to
     */
    void setTileInfo(unsigned int a, unsigned int b, unsigned char texChar, char height, unsigned char flatness, unsigned char edgeOffset);

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
     * Gets the flatness of a particular hexagon
     *
     * @param a: the 'a' coordinate of the hexagon to get; i.e the number of down-and-right moves from the top hexagon
     * @param b: the 'b' coordinate of the hexagon to get; i.e the number of down-and-left moves from the top hexagon
     * @returns: the flatness of this tile
     */
    unsigned char getTileFlatness(unsigned int a, unsigned int b);

    /**
     * Gets the edge offset of a particular hexagon
     *
     * @param a: the 'a' coordinate of the hexagon to get; i.e the number of down-and-right moves from the top hexagon
     * @param b: the 'b' coordinate of the hexagon to get; i.e the number of down-and-left moves from the top hexagon
     * @returns: the edge offset of this tile
     */
    unsigned char getTileOffset(unsigned int a, unsigned int b);

    /**
     * Gets the info for a particular hexagon by modifying the variables passed by reference
     *
     * @param a: the 'a' coordinate of the hexagon to get; i.e the number of down-and-right moves from the top hexagon
     * @param b: the 'b' coordinate of the hexagon to get; i.e the number of down-and-left moves from the top hexagon
     * @param texChar: reference to a variable to set to the index of the texture in the tileset of the tile
     * @param height: reference to a variable to set to the height of the tile
     * @param flatness: reference to a variable to set to the flatness of the tile
     * @param edgeOffset: reference to a variable to set to the edge offset of the tile
     */
    void getTileInfo(unsigned int a, unsigned int b, unsigned char & texChar, char & height, unsigned char & flatness, unsigned char & edgeOffset);

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
     * Sets the flatness map for the entire grid
     *
     * @param flatnesses: vector mapping the index of a grid hexagon to the flatness of that hexagon
     */
    void setGridFlatnesses(std::vector<unsigned char> & flatnesses);

    /**
     * Sets the edge offset map for the entire grid
     *
     * @param edgeOffsets: vector mapping the index of a grid hexagon to the edge offset of that hexagon
     */
    void setGridOffsets(std::vector<unsigned char> & edgeOffsets);

    /**
     * Sets the rotation, texture, height and flatness maps for the entire grid
     *
     * @param gridRotation: the value to set gridRotation to
     * @param textures: vector mapping the index of a grid hexagon to the texture in the tileset that it should be textured with
     * @param heights: vector mapping the index of a grid hexagon to the height of that hexagon
     * @param flatnesses: vector mapping the index of a grid hexagon to the flatness of that hexagon
     * @param edgeOffsets: vector mapping the index of a grid hexagon to the edge offset of that hexagon
     */
    void setGridInfo(unsigned char gridRotation, std::vector<unsigned char> & textures, std::vector<char> & heights, std::vector<unsigned char> & flatnesses, std::vector<unsigned char> & edgeOffsets);

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
     * Gets the flatness map for the entire grid by copying into a vector passed by reference
     *
     * @param flatnesses: reference to a vector to copy the flatness map to
     */
    void getGridFlatnesses(std::vector<unsigned char> & flatnesses);

};

#endif // !HEXGRID_H
