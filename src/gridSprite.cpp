#include "gridSprite.h"
#include "hexGrid.h"

//Constructors

gridSprite::gridSprite(char minHeight, double a, double b): minHeight(minHeight), a(a), b(b), sf::Sprite() {
}

gridSprite::gridSprite(char minHeight, double a, double b, const sf::Texture &texture): minHeight(minHeight), a(a), b(b), sf::Sprite(texture) {
}

gridSprite::gridSprite(char minHeight, double a, double b, const sf::Texture &texture, const sf::IntRect &rectangle): minHeight(minHeight), a(a), b(b), sf::Sprite(texture,rectangle) {
}

//Misc methods
void gridSprite::addToGrid(hexGrid * gridPtr) {
    gridPtr->sprites.push_back(this);
    gridPtr->placeSprite(this);
}

//Getters
char gridSprite::getMinHeight() {
    return minHeight;
}

sf::Vector2<double> gridSprite::getABCoords() {
    return sf::Vector2<double>(a,b);
}

//Setters
void gridSprite::setMinHeight(char newMinHeight, hexGrid * gridPtr) {
    gridPtr->removeSprite(this);
    minHeight = newMinHeight;
    gridPtr->placeSprite(this);
}

void gridSprite::setAB(double newA, double newB, hexGrid * gridPtr) {
    gridPtr->removeSprite(this);
    a = newA;
    b = newB;
    gridPtr->placeSprite(this);
}
