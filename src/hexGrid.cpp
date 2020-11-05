#include "hexGrid.h"

//Constructors

hexGrid::hexGrid(unsigned int s, unsigned int textureUnitWidth, double heightUnit, sf::Texture * tileset):
    textures(3*(s*(s+1))+1),
    heights(3*(s*(s+1))+1),
    sharpnesses(3*(s*(s+1))+1),
    tileset(tileset),
    textureUnitWidth(textureUnitWidth),
    heightUnit(heightUnit),
    s(s)
{
    init();
}

hexGrid::hexGrid(unsigned int s, unsigned int textureUnitWidth, double heightUnit, sf::Texture * tileset, std::vector<unsigned char> & textures, std::vector<char> & heights, std::vector<unsigned char> & sharpnesses):
    textures(textures),
    heights(heights),
    sharpnesses(sharpnesses),
    tileset(tileset),
    textureUnitWidth(textureUnitWidth),
    heightUnit(heightUnit),
    s(s)
{
    //Ensure that `textures` has the correct number of elements
    if (textures.size() != 3*(s*(s+1)) + 1) {
        initialiseGridTextureMapException exc;
        throw exc;
    }
    //Ensure that `heights` has the correct number of elements
    if (heights.size() != 3*(s*(s+1)) + 1) {
        initialiseGridHeightMapException exc;
        throw exc;
    }
    //Ensure that `sharpnesses` has the correct number of elements
    if (sharpnesses.size() != 3*(s*(s+1)) + 1) {
        initialiseGridSharpnessMapException exc;
        throw exc;
    }
    init();
}

//Private methods

void hexGrid::draw(sf::RenderTarget & target, sf::RenderStates states) const {
    states.transform *= getTransform();
    states.texture = tileset;
    for (auto i: rows) {
        target.draw(i, states);
    }
}

void hexGrid::init() {
    //Initialise rows
    for (unsigned int i = 0; i < s; i++) {
        rows.emplace_back(2*i + 1, textureUnitWidth, true);
        rows.back().setPosition(s-i,0.5*i);
    }
    for (unsigned int i = 0; i < s+1; i++) {
        rows.emplace_back(2*s + 1, textureUnitWidth, true);
        rows.back().setPosition(0,0.5*s + i);
        rows.emplace_back(2*s + 1, textureUnitWidth, false);
        rows.back().setPosition(0,0.5*s + i + 0.5);
    }
    for (unsigned int i = 0; i < s; i++) {
        rows.emplace_back(2*(s-i) - 1, textureUnitWidth, false);
        rows.back().setPosition(i+1,1+1.5*s+0.5*i);
    }
    updateTiles();
}

void hexGrid::updateTiles() {
    //Map textures to rows
    unsigned int texModuland = tileset->getSize().x / textureUnitWidth;
    for (unsigned int b = 0; b <= 2*s; b++) {
        const unsigned int bottom = b < s ? 0 : b - s;
        const unsigned int top = b < s ? b + s : 2*s;
        for (unsigned int a = bottom; a <= top; a++) {
            unsigned long pointIndex;
            unsigned int aPrime, bPrime;
            getOtherCoordinates(a,b,pointIndex,aPrime,bPrime);
            unsigned int xOffset = (textures[pointIndex]%texModuland);
            unsigned int yOffset = (textures[pointIndex]/texModuland);
            //Work out position in render rows
            rows[aPrime+bPrime].setTexture((rows[aPrime+bPrime].noHexes + 2*aPrime - 2*bPrime)/2,xOffset,yOffset,textureUnitWidth);
            rows[aPrime+bPrime+1].setTexture((rows[aPrime+bPrime+1].noHexes + 2*aPrime - 2*bPrime)/2,xOffset,yOffset,textureUnitWidth);
            rows[aPrime+bPrime].setHeights((rows[aPrime+bPrime].noHexes + 2*aPrime - 2*bPrime)/2,heights[pointIndex]*heightUnit,heights[pointIndex]*heightUnit,heights[pointIndex]*heightUnit,heights[pointIndex]*heightUnit,heights[pointIndex]*heightUnit);
            rows[aPrime+bPrime+1].setHeights((rows[aPrime+bPrime+1].noHexes + 2*aPrime - 2*bPrime)/2,heights[pointIndex]*heightUnit,heights[pointIndex]*heightUnit,heights[pointIndex]*heightUnit,heights[pointIndex]*heightUnit,heights[pointIndex]*heightUnit);
        }
    }
}

void hexGrid::getOtherCoordinates(unsigned int a, unsigned int b, unsigned long & pointIndex, unsigned int & aPrime, unsigned int & bPrime) {
    pointIndex = b > s
        ? a + b*(6*s + 3 - b)/2 - s*(s+2)
        : a + b*(2*s + 3 + b)/2;
    switch(gridRotation) {
        case 1:
            aPrime = s + a - b;
            bPrime = a;
            break;
        case 2:
            aPrime = 2*s - b;
            bPrime = s + a - b;
            break;
        case 3:
            aPrime = 2*s - a;
            bPrime = 2*s - b;
            break;
        case 4:
            aPrime = s - a + b;
            bPrime = 2*s - a;
            break;
        case 5:
            aPrime = b;
            bPrime = s + b - a;
            break;
        default:
            aPrime = a;
            bPrime = b;
            break;
    }
}

//Public methods

//Setters for individual hexagons

void hexGrid::setTileTexture(unsigned int a, unsigned int b, unsigned char texChar) {
    //Set texChar in the textures vector
    unsigned int texModuland = tileset->getSize().x / textureUnitWidth;
    unsigned long pointIndex;
    unsigned int aPrime, bPrime;
    getOtherCoordinates(a,b,pointIndex,aPrime,bPrime);
    textures[pointIndex] = texChar;
    //Set texture on the render row
    unsigned int xOffset = (texChar%texModuland);
    unsigned int yOffset = (texChar/texModuland);
    rows[aPrime+bPrime].setTexture((rows[aPrime+bPrime].noHexes + 2*aPrime - 2*bPrime)/2,xOffset,yOffset,textureUnitWidth);
    rows[aPrime+bPrime+1].setTexture((rows[aPrime+bPrime+1].noHexes + 2*aPrime - 2*bPrime)/2,xOffset,yOffset,textureUnitWidth);
}

void hexGrid::setTileHeight(unsigned int a, unsigned int b, char height) {
    //Set height in the heights vector
    unsigned long pointIndex;
    unsigned int aPrime, bPrime;
    getOtherCoordinates(a,b,pointIndex,aPrime,bPrime);
    heights[pointIndex] = height;
    //Set height on the render row
    rows[aPrime+bPrime].setHeights((rows[aPrime+bPrime].noHexes + 2*aPrime - 2*bPrime)/2,height*heightUnit,height*heightUnit,height*heightUnit,height*heightUnit,height*heightUnit);
    rows[aPrime+bPrime+1].setHeights((rows[aPrime+bPrime+1].noHexes + 2*aPrime - 2*bPrime)/2,height*heightUnit,height*heightUnit,height*heightUnit,height*heightUnit,height*heightUnit);
}

void hexGrid::setTileSharpness(unsigned int a, unsigned int b, unsigned char sharpness) {
    //Set sharpness in the sharpnesses vector
    unsigned long pointIndex;
    unsigned int aPrime, bPrime;
    getOtherCoordinates(a,b,pointIndex,aPrime,bPrime);
    sharpnesses[pointIndex] = sharpness;
    //Set height on the render row
    char height = heights[pointIndex];
    rows[aPrime+bPrime].setHeights((rows[aPrime+bPrime].noHexes + 2*aPrime - 2*bPrime)/2,height*heightUnit,height*heightUnit,height*heightUnit,height*heightUnit,height*heightUnit);
    rows[aPrime+bPrime+1].setHeights((rows[aPrime+bPrime+1].noHexes + 2*aPrime - 2*bPrime)/2,height*heightUnit,height*heightUnit,height*heightUnit,height*heightUnit,height*heightUnit);
}

void hexGrid::setTileInfo(unsigned int a, unsigned int b, unsigned char texChar, char height, unsigned char sharpness) {
    //Set info in vectors
    unsigned int texModuland = tileset->getSize().x / textureUnitWidth;
    unsigned long pointIndex;
    unsigned int aPrime, bPrime;
    getOtherCoordinates(a,b,pointIndex,aPrime,bPrime);
    textures[pointIndex] = texChar;
    heights[pointIndex] = height;
    sharpnesses[pointIndex] = sharpness;
    //Set height and texture on the render row
    unsigned int xOffset = (texChar%texModuland);
    unsigned int yOffset = (texChar/texModuland);
    rows[aPrime+bPrime].setTexture((rows[aPrime+bPrime].noHexes + 2*aPrime - 2*bPrime)/2,xOffset,yOffset,textureUnitWidth);
    rows[aPrime+bPrime+1].setTexture((rows[aPrime+bPrime+1].noHexes + 2*aPrime - 2*bPrime)/2,xOffset,yOffset,textureUnitWidth);
    rows[aPrime+bPrime].setHeights((rows[aPrime+bPrime].noHexes + 2*aPrime - 2*bPrime)/2,height*heightUnit,height*heightUnit,height*heightUnit,height*heightUnit,height*heightUnit);
    rows[aPrime+bPrime+1].setHeights((rows[aPrime+bPrime+1].noHexes + 2*aPrime - 2*bPrime)/2,height*heightUnit,height*heightUnit,height*heightUnit,height*heightUnit,height*heightUnit);
}

//Getters for individual hexagons

unsigned char hexGrid::getTileTexture(unsigned int a, unsigned int b) {
    unsigned long pointIndex;
    unsigned int aPrime, bPrime;
    getOtherCoordinates(a,b,pointIndex,aPrime,bPrime);
    return textures[pointIndex];
}

char hexGrid::getTileHeight(unsigned int a, unsigned int b) {
    unsigned long pointIndex;
    unsigned int aPrime, bPrime;
    getOtherCoordinates(a,b,pointIndex,aPrime,bPrime);
    return heights[pointIndex];
}

unsigned char hexGrid::getTileSharpness(unsigned int a, unsigned int b) {
    unsigned long pointIndex;
    unsigned int aPrime, bPrime;
    getOtherCoordinates(a,b,pointIndex,aPrime,bPrime);
    return sharpnesses[pointIndex];
}

void hexGrid::getTileInfo(unsigned int a, unsigned int b, unsigned char & texChar, char & height, unsigned char & sharpness) {
    unsigned long pointIndex;
    unsigned int aPrime, bPrime;
    getOtherCoordinates(a,b,pointIndex,aPrime,bPrime);
    texChar = textures[pointIndex];
    height = heights[pointIndex];
    sharpness = sharpnesses[pointIndex];
}


//Setters for the entire grid

void hexGrid::setGridRotation(unsigned char rotation) {
    if (rotation > 5) {
        setGridRotationException exc;
        throw exc;
    }
    gridRotation = rotation;
    updateTiles();
}

void hexGrid::setGridTextures(std::vector<unsigned char> & newTextures) {
    if (newTextures.size() != 3*(s*(s+1)) + 1) {
        setGridTextureMapException exc;
        throw exc;
    }
    textures = newTextures;
    updateTiles();
}

void hexGrid::setGridHeights(std::vector<char> & newHeights) {
    if (newHeights.size() != 3*(s*(s+1)) + 1) {
        setGridHeightMapException exc;
        throw exc;
    }
    heights = newHeights;
    updateTiles();
}

void hexGrid::setGridSharpnesses(std::vector<unsigned char> & newSharpnesses) {
    if (newSharpnesses.size() != 3*(s*(s+1)) + 1) {
        setGridSharpnessMapException exc;
        throw exc;
    }
    sharpnesses = newSharpnesses;
    updateTiles();
}

void hexGrid::setGridInfo(unsigned char rotation, std::vector<unsigned char> & newTextures, std::vector<char> & newHeights, std::vector<unsigned char> & newSharpnesses) {
    if (rotation > 5) {
        setGridRotationException exc;
        throw exc;
    }
    if (newTextures.size() != 3*(s*(s+1)) + 1) {
        setGridTextureMapException exc;
        throw exc;
    }
    if (newHeights.size() != 3*(s*(s+1)) + 1) {
        setGridHeightMapException exc;
        throw exc;
    }
    if (newSharpnesses.size() != 3*(s*(s+1)) + 1) {
        setGridSharpnessMapException exc;
        throw exc;
    }
    gridRotation = rotation;
    textures = newTextures;
    heights = newHeights;
    sharpnesses = newSharpnesses;
    updateTiles();
}

//Getters for the entire grid

unsigned char hexGrid::getGridRotation() {
    return gridRotation;
}

void hexGrid::getGridTextures(std::vector<unsigned char> & newTextures) {
    newTextures = textures;
}

void hexGrid::getGridHeights(std::vector<char> & newHeights) {
    newHeights = heights;
}

void hexGrid::getGridSharpnesses(std::vector<unsigned char> & newSharpnesses) {
    newSharpnesses = sharpnesses;
}
