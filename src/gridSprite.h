//This file declares and defines the properties of the gridSprite class
#ifndef GRIDSPRITE_H
#define GRIDSPRITE_H

#include <SFML/Graphics.hpp>

struct gridSpriteData {
    //The minimum height to render the sprite at - for, e.g, flying sprites
    char minHeight;
    //The a coordinate of the sprite
    double a;
    //The b coordinate of the sprite
    double b;
    //The name key of the sprite
    unsigned char textureSheetKey;
    //The int rect of the sprite
    sf::IntRect * rectangle;

    gridSpriteData(char minHeight, double a, double b, unsigned char textureSheetKey, sf::IntRect * rectangle)
        : minHeight(minHeight), a(a), b(b), textureSheetKey(textureSheetKey), rectangle(rectangle) {}
};

/**
 * gridSprite class provides sf::Sprite with the information necessary to
 * draw onto the grid
 */
class gridSprite: public sf::Sprite {
private:
    //The minimum height to render the sprite at - for, e.g, flying sprites
    char minHeight;
    //The a coordinate of the sprite
    double a;
    //The b coordinate of the sprite
    double b;
public:

    //Constructors

    /**
     * Default constructor method for gridSprite
     *
     * @param minHeight: the minimum height to render the sprite at
     * @param a: the a coordinate of the sprite
     * @param b: the b coordinate of the sprite
     */
    gridSprite(char minHeight,double a,double b);

    /**
     * Constructor method for gridSprite with a source texture
     *
     * @param minHeight: the minimum height to render the sprite at
     * @param a: the a coordinate of the sprite
     * @param b: the b coordinate of the sprite
     * @param texture: the source texture for the sprite
     */
    gridSprite(char minHeight,double a,double b, const sf::Texture &texture);

    /**
     * Constructor method for gridSprite from a sub-rectangle of a source texture
     *
     * @param minHeight: the minimum height to render the sprite at
     * @param a: the a coordinate of the sprite
     * @param b: the b coordinate of the sprite
     * @param texture: the source texture for the sprite
     * @param rectangle: sub-rectangle of the texture to assign to the sprite
     */
    gridSprite(char minHeight,double a,double b, const sf::Texture &texture, const sf::IntRect &rectangle);

    //Misc methods
    /**
     * Adds the sprite to a hexGrid
     *
     * @param gridPtr: pointer to the grid to add the sprite to
     */
    void addToGrid(class hexGrid * gridPtr);

    //Getter methods

    /**
     * Gets the minHeight of the sprite
     *
     * @returns: the minHeight of the sprite
     */
    char getMinHeight();

    /**
     * Gets the a,b coordinates of the sprite
     *
     * @returns: an sfml 2d vector with x=a, y=b
     */
    sf::Vector2<double> getABCoords();

    //Setter methods

    /**
     * Sets the minHeight of the sprite
     *
     * @param minHeight: the value to set minHeight to
     * @param gridPtr: pointer to the grid that the sprite is on
     */
    void setMinHeight(char minHeight, class hexGrid * gridPtr);

    /**
     * Sets the a,b coordinates of the sprite
     *
     * @param a: the value to set a to
     * @param b: the value to set a to
     * @param gridPtr: pointer to the grid that the sprite is on
     */
    void setAB(double a, double b, class hexGrid * gridPtr);
};

#endif // !GRIDSPRITE_H
