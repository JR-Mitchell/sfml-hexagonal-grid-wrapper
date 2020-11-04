#include "hexGrid.h"

hexGrid::hexGrid(unsigned int s, unsigned int textureUnitWidth, sf::Texture * tileset):
    textures(3*(s*(s+1))+1),
    tileset(tileset),
    textureUnitWidth(textureUnitWidth),
    s(s)
{
    init();
}

hexGrid::hexGrid(unsigned int s, unsigned int textureUnitWidth, sf::Texture * tileset, std::vector<unsigned char> textures):
    textures(textures),
    tileset(tileset),
    textureUnitWidth(textureUnitWidth),
    s(s)
{
    //Ensure that `textures` has the correct number of elements
    if (textures.size() != 3*(s*(s+1)) + 1) {
        initialiseGridTextureMapException exc;
        throw exc;
    }
    init();
}

hexGrid::hexGrid(unsigned int s, unsigned int textureUnitWidth, sf::Texture * tileset, std::string texturesFilename):
    textures(0),
    tileset(tileset),
    textureUnitWidth(textureUnitWidth),
    s(s)
{
    std::vector<unsigned char> fileData;
    std::ifstream infile(texturesFilename);
    //Load data from specified file
    if (infile.is_open()) {
        infile.seekg(0, std::ios_base::end);
        auto fileSize = infile.tellg();
        fileData.resize(fileSize);
        infile.seekg(0, std::ios_base::beg);
        infile.read((char*)(&fileData[0]), fileSize);
    } else {
        loadResourceException exc;
        throw exc;
    }
    //Ensure that loaded data has the correct number of elements
    if (fileData.size() != 3*(s*(s+1)) + 1) {
        initialiseGridTextureMapException exc;
        throw exc;
    }
    textures = fileData;
    init();
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
            unsigned long pointIndex = b > s
                ? a + b*(6*s + 3 - b)/2 - s*(s+2)
                : a + b*(2*s + 3 + b)/2;
            unsigned int xOffset = (textures[pointIndex]%texModuland);
            unsigned int yOffset = (textures[pointIndex]/texModuland);
            //Work out position in render rows
            unsigned int aPrime = a;
            unsigned int bPrime = b;
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
                    break;
            }
            rows[aPrime+bPrime].setTexture((rows[aPrime+bPrime].noHexes + 2*aPrime - 2*bPrime)/2,xOffset,yOffset,textureUnitWidth);
            rows[aPrime+bPrime+1].setTexture((rows[aPrime+bPrime+1].noHexes + 2*aPrime - 2*bPrime)/2,xOffset,yOffset,textureUnitWidth);
        }
    }
}

void hexGrid::draw(sf::RenderTarget & target, sf::RenderStates states) const {
    states.transform *= getTransform();
    states.texture = tileset;
    for (auto i: rows) {
        target.draw(i, states);
    }
}

void hexGrid::setTexture(unsigned int a, unsigned int b, unsigned char texChar) {
    //Set texChar in the textures vector
    unsigned int texModuland = tileset->getSize().x / textureUnitWidth;
    unsigned long pointIndex = b > s
        ? a + b*(6*s + 3 - b)/2 - s*(s+2)
        : a + b*(2*s + 3 + b)/2;
    textures[pointIndex] = texChar;
    //Set texture on the render row
    unsigned int xOffset = (texChar%texModuland);
    unsigned int yOffset = (texChar/texModuland);
            unsigned int aPrime = a;
            unsigned int bPrime = b;
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
                    break;
            }
    rows[aPrime+bPrime].setTexture((rows[aPrime+bPrime].noHexes + 2*aPrime - 2*bPrime)/2,xOffset,yOffset,textureUnitWidth);
    rows[aPrime+bPrime+1].setTexture((rows[aPrime+bPrime+1].noHexes + 2*aPrime - 2*bPrime)/2,xOffset,yOffset,textureUnitWidth);
}

void hexGrid::loadTexturesFromFile(std::string texturesFilename) {
    std::vector<unsigned char> fileData;
    std::ifstream infile(texturesFilename);
    //Load data from specified file
    if (infile.is_open()) {
        infile.seekg(0, std::ios_base::end);
        auto fileSize = infile.tellg();
        fileData.resize(fileSize);
        infile.seekg(0, std::ios_base::beg);
        infile.read((char*)(&fileData[0]), fileSize);
    } else {
        loadResourceException exc;
        throw exc;
    }
    //Ensure that loaded data has the correct number of elements
    if (fileData.size() != textures.size()) {
        initialiseGridTextureMapException exc;
        throw exc;
    }
    textures = fileData;
    updateTiles();
}

void hexGrid::saveTexturesToFile(std::string texturesFilename) {
    std::ofstream outfile(texturesFilename);
    //Load data from specified file
    if (outfile.is_open()) {
        for (auto &i: textures) {
            outfile << i;
        }
    } else {
        loadResourceException exc;
        throw exc;
    }
}

void hexGrid::setGridRotation(unsigned char rotation) {
    if (rotation > 5) {
        //TODO: proper exception
        throw 1;
    }
    gridRotation = rotation;
    updateTiles();
}
