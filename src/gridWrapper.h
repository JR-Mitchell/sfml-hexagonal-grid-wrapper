//This file declares and defines the properties of the gridWrapper class
#ifndef GRIDWRAPPER_H
#define GRIDWRAPPER_H

#include "hexGrid.h"
#include "RTree.h"

/**
 * The gridWrapper class encapsulates HexGrids to provide a view
 * on an overall map, centred on a specified sprite or point
 */
class gridWrapper: public sf::Drawable, public sf::Transformable {
private:
    //GRID INFORMATION

    //The grid origin
    double originA;
    double originB;

    //The grid to wrap
    hexGrid grid;

    //The side length of the visible grid
    const unsigned int gridSideLength;

    // Overload of sf::Drawable's draw() method
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    // MAP INFORMATION
    //The side length of the overall map
    unsigned int dataSideLength;


    //The texture map
    std::vector<unsigned char> textures;

    //The height map
    std::vector<char> heights;

    //The flatness map
    std::vector<unsigned char> flatnesses;

    //The edgeOffsets map
    std::vector<unsigned char> edgeOffsets;

    //SPRITE INFORMATION
    std::vector<gridSprite> drawableSprites;

    std::vector<gridSpriteData> mapSprites;

    // RTree<gridSpriteData*,double,2> spriteTree;

    // TILESET INFORMATION

    std::vector<sf::Texture *> tilesheets;
    
public:
    //Constructors

    /**
     * Basic constructor method
     * 
     * @param dataSideLength: the side length of the overall map to pull data from
     * @param gridSideLength: the side length of the visible grid
     * 
     */
    gridWrapper(unsigned int gridSideLength, unsigned int textureUnitWidth, double heightUnit, sf::Texture * tileset, std::function<sf::Color(unsigned int a, unsigned int b, unsigned char texChar, char height, sf::Vector3f normal)> colourer);     

    /**
     * 
     */
    void setDataSideLength(unsigned int dataSideLength);

    void setData(unsigned int dataSideLength, std::vector<unsigned char> textures, std::vector<char> heights, std::vector<unsigned char> flatnesses, std::vector<unsigned char> edgeOffsets);

    void setOrigin(unsigned int a, unsigned int b);

    unsigned char addTexture(const sf::Texture * texture);

    void addSprite(char minHeight,double a,double b, unsigned char textureSheetKey, sf::IntRect &rectangle);
};

#endif // !GRIDWRAPPER_H