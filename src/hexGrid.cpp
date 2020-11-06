#include "hexGrid.h"

//Constructors

hexGrid::hexGrid(unsigned int s, unsigned int textureUnitWidth, double heightUnit, sf::Texture * tileset):
    textures(3*(s*(s+1))+1),
    heights(3*(s*(s+1))+1),
    flatnesses(3*(s*(s+1))+1),
    edgeOffsets(3*(s*(s+1))+1),
    tileset(tileset),
    textureUnitWidth(textureUnitWidth),
    heightUnit(heightUnit),
    s(s)
{
    init();
}

hexGrid::hexGrid(unsigned int s, unsigned int textureUnitWidth, double heightUnit, sf::Texture * tileset, std::vector<unsigned char> & textures, std::vector<char> & heights, std::vector<unsigned char> & flatnesses, std::vector<unsigned char> & edgeOffsets):
    textures(textures),
    heights(heights),
    flatnesses(flatnesses),
    edgeOffsets(edgeOffsets),
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
    //Ensure that `flatnesses` has the correct number of elements
    if (flatnesses.size() != 3*(s*(s+1)) + 1) {
        initialiseGridFlatnessMapException exc;
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
            double topHeights [9];
            double bottomHeights [9];
            getTileHeights(a,b,pointIndex,topHeights,bottomHeights);
            rows[aPrime+bPrime].setTexture((rows[aPrime+bPrime].noHexes + 2*aPrime - 2*bPrime)/2,xOffset,yOffset,textureUnitWidth);
            rows[aPrime+bPrime+1].setTexture((rows[aPrime+bPrime+1].noHexes + 2*aPrime - 2*bPrime)/2,xOffset,yOffset,textureUnitWidth);
            rows[aPrime+bPrime].setHeights((rows[aPrime+bPrime].noHexes + 2*aPrime - 2*bPrime)/2,topHeights);
            rows[aPrime+bPrime+1].setHeights((rows[aPrime+bPrime+1].noHexes + 2*aPrime - 2*bPrime)/2,bottomHeights);
        }
    }
}

void hexGrid::updateTileHeight(unsigned int a, unsigned int b) {
    unsigned long pointIndex;
    unsigned int aPrime, bPrime;
    getOtherCoordinates(a,b,pointIndex,aPrime,bPrime);
    //Set height on the render row
    double topHeights [9];
    double bottomHeights [9];
    getTileHeights(a,b,pointIndex,topHeights,bottomHeights);
    rows[aPrime+bPrime].setHeights((rows[aPrime+bPrime].noHexes + 2*aPrime - 2*bPrime)/2,topHeights);
    rows[aPrime+bPrime+1].setHeights((rows[aPrime+bPrime+1].noHexes + 2*aPrime - 2*bPrime)/2,bottomHeights);
    //Cycle through surrounding hexagons
    unsigned int rowWidth = b < s ? s + 1 + b : 3*s + 1 - b;
    bool isValid [6] = {
        a + s > b,
        a > 0,
        b > 0,
        b + s > a,
        a < 2*s,
        b < 2*s
    };
    for (unsigned char i=0; i<6; i++) {
        if (isValid[i] && isValid[(i+1)%6]) {
            unsigned int newA = a;
            unsigned int newB = b;
            switch(i) {
                case 0:
                    newA -= 1;
                    break;
                case 1:
                    newA -= 1;
                    newB -= 1;
                    break;
                case 2:
                    newB -= 1;
                    break;
                case 3:
                    newA += 1;
                    break;
                case 4:
                    newA += 1;
                    newB += 1;
                    break;
                case 5:
                    newB += 1;
                    break;
            }
            unsigned long newPointIndex;
            unsigned int newAPrime, newBPrime;
            getOtherCoordinates(newA,newB,newPointIndex,newAPrime,newBPrime);
            //Set height on the render row
            double newTopHeights [9];
            double newBottomHeights [9];
            getTileHeights(newA,newB,newPointIndex,newTopHeights,newBottomHeights);
            rows[newAPrime+newBPrime].setHeights((rows[newAPrime+newBPrime].noHexes + 2*newAPrime - 2*newBPrime)/2,newTopHeights);
            rows[newAPrime+newBPrime+1].setHeights((rows[newAPrime+newBPrime+1].noHexes + 2*newAPrime - 2*newBPrime)/2,newBottomHeights);
        }
    }
}

void hexGrid::getOtherCoordinates(unsigned int a, unsigned int b, unsigned long & pointIndex, unsigned int & aPrime, unsigned int & bPrime) {
    pointIndex = b > s
        ? a + b*(6*s + 1 - b)/2 - s*s
        : a + b*(2*s + 1 + b)/2;
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

void hexGrid::getTileHeights(unsigned int a, unsigned int b, unsigned long pointIndex, double * topHeights, double * bottomHeights) {
    const char height = heights[pointIndex];
    const unsigned int flatness = static_cast<unsigned int>(flatnesses[pointIndex]) + 1;
    const char edgeOffset = edgeOffsets[pointIndex] < height ? height - edgeOffsets[pointIndex] : 0;
    topHeights[0] = bottomHeights[0] = height*heightUnit;
    unsigned int flatnessCumulative [6] = {flatness,flatness,flatness,flatness,flatness,flatness};
    long flatnessHeightCumulative [6] = {flatness*static_cast<long>(height),flatness*static_cast<long>(height),flatness*static_cast<long>(height),flatness*static_cast<long>(height),flatness*static_cast<long>(height),flatness*static_cast<long>(height)};
    //Cycle through surrounding hexagons
    unsigned int rowWidth = b < s ? s + 1 + b : 3*s + 1 - b;
    unsigned long neighbourIndices [6] = {
        pointIndex - 1,
        pointIndex - rowWidth + (b <= s) - 1,
        pointIndex - rowWidth + (b <= s),
        pointIndex + 1,
        pointIndex + rowWidth - (b >= s) + 1,
        pointIndex + rowWidth - (b >= s)
    };
    bool isValid [6] = {
        a + s > b,
        a > 0,
        b > 0,
        b + s > a,
        a < 2*s,
        b < 2*s
    };
    for (unsigned char i=0; i<6; i++) {
        if (isValid[i] && isValid[(i+1)%6]) {
            unsigned int flatnessAt = static_cast<unsigned int>(flatnesses[neighbourIndices[i]]) + 1;
            long flatnessHeightAt = flatnessAt * static_cast<long>(heights[neighbourIndices[i]]);
            flatnessCumulative[i] += flatnessAt;
            flatnessCumulative[(i+1)%6] += flatnessAt;
            flatnessHeightCumulative[i] += flatnessHeightAt;
            flatnessHeightCumulative[(i+1)%6] += flatnessHeightAt;
        }
    }
    double vertexHeights [6] = {0,0,0,0,0,0};
    for (unsigned char i=0; i<6; i++) {
        vertexHeights[i] = static_cast<double>(flatnessHeightCumulative[i])/static_cast<double>(flatnessCumulative[i]);
    }
    //Left vertex
    topHeights[1] = bottomHeights[1] = height - edgeOffset > vertexHeights[(6-gridRotation)%6]
        ? (height - edgeOffset)*heightUnit
        : vertexHeights[(6-gridRotation)%6]*heightUnit;
    topHeights[2] = bottomHeights[2] = vertexHeights[(6-gridRotation)%6]*heightUnit;
    //Up-left vertex
    topHeights[3] = height - edgeOffset > vertexHeights[((6-gridRotation)%6+1)%6]
        ? (height - edgeOffset)*heightUnit
        : vertexHeights[((6-gridRotation)%6+1)%6]*heightUnit;
    topHeights[4] = vertexHeights[((6-gridRotation)%6+1)%6]*heightUnit;
    //Up-right vertex
    topHeights[5] = height - edgeOffset > vertexHeights[((6-gridRotation)%6+2)%6]
        ? (height - edgeOffset)*heightUnit
        : vertexHeights[((6-gridRotation)%6+2)%6]*heightUnit;
    topHeights[6] = vertexHeights[((6-gridRotation)%6+2)%6]*heightUnit;
    //Right vertex
    topHeights[7] = bottomHeights[7] = height - edgeOffset > vertexHeights[((6-gridRotation)%6+3)%6]
        ? (height - edgeOffset)*heightUnit
        : vertexHeights[((6-gridRotation)%6+3)%6]*heightUnit;
    topHeights[8] = bottomHeights[8] = vertexHeights[((6-gridRotation)%6+3)%6]*heightUnit;
    //Down-left vertex
    bottomHeights[3] = height - edgeOffset > vertexHeights[((6-gridRotation)%6+5)%6]
        ? (height - edgeOffset)*heightUnit
        : vertexHeights[((6-gridRotation)%6+5)%6]*heightUnit;
    bottomHeights[4] = vertexHeights[((6-gridRotation)%6+5)%6]*heightUnit;
    //Down-right vertex
    bottomHeights[5] = height - edgeOffset > vertexHeights[((6-gridRotation)%6+4)%6]
        ? (height - edgeOffset)*heightUnit
        : vertexHeights[((6-gridRotation)%6+4)%6]*heightUnit;
    bottomHeights[6] = vertexHeights[((6-gridRotation)%6+4)%6]*heightUnit;
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
    updateTileHeight(a,b);
}

void hexGrid::setTileFlatness(unsigned int a, unsigned int b, unsigned char flatness) {
    //Set flatness in the flatnesses vector
    unsigned long pointIndex;
    unsigned int aPrime, bPrime;
    getOtherCoordinates(a,b,pointIndex,aPrime,bPrime);
    flatnesses[pointIndex] = flatness;
    updateTileHeight(a,b);
}

void hexGrid::setTileInfo(unsigned int a, unsigned int b, unsigned char texChar, char height, unsigned char flatness, unsigned char edgeOffset) {
    //Set info in vectors
    unsigned int texModuland = tileset->getSize().x / textureUnitWidth;
    unsigned long pointIndex;
    unsigned int aPrime, bPrime;
    getOtherCoordinates(a,b,pointIndex,aPrime,bPrime);
    textures[pointIndex] = texChar;
    heights[pointIndex] = height;
    flatnesses[pointIndex] = flatness;
    edgeOffsets[pointIndex] = edgeOffset;
    //Set height and texture on the render row
    unsigned int xOffset = (texChar%texModuland);
    unsigned int yOffset = (texChar/texModuland);
    rows[aPrime+bPrime].setTexture((rows[aPrime+bPrime].noHexes + 2*aPrime - 2*bPrime)/2,xOffset,yOffset,textureUnitWidth);
    rows[aPrime+bPrime+1].setTexture((rows[aPrime+bPrime+1].noHexes + 2*aPrime - 2*bPrime)/2,xOffset,yOffset,textureUnitWidth);
    updateTileHeight(a,b);
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

unsigned char hexGrid::getTileFlatness(unsigned int a, unsigned int b) {
    unsigned long pointIndex;
    unsigned int aPrime, bPrime;
    getOtherCoordinates(a,b,pointIndex,aPrime,bPrime);
    return flatnesses[pointIndex];
}

unsigned char hexGrid::getTileOffset(unsigned int a, unsigned int b) {
    unsigned long pointIndex;
    unsigned int aPrime, bPrime;
    getOtherCoordinates(a,b,pointIndex,aPrime,bPrime);
    return edgeOffsets[pointIndex];
}

void hexGrid::getTileInfo(unsigned int a, unsigned int b, unsigned char & texChar, char & height, unsigned char & flatness, unsigned char & edgeOffset) {
    unsigned long pointIndex;
    unsigned int aPrime, bPrime;
    getOtherCoordinates(a,b,pointIndex,aPrime,bPrime);
    texChar = textures[pointIndex];
    height = heights[pointIndex];
    flatness = flatnesses[pointIndex];
    edgeOffset = edgeOffsets[pointIndex];
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

void hexGrid::setGridFlatnesses(std::vector<unsigned char> & newFlatnesses) {
    if (newFlatnesses.size() != 3*(s*(s+1)) + 1) {
        setGridFlatnessMapException exc;
        throw exc;
    }
    flatnesses = newFlatnesses;
    updateTiles();
}

void hexGrid::setGridOffsets(std::vector<unsigned char> & newEdgeOffsets) {
    if (newEdgeOffsets.size() != 3*(s*(s+1)) + 1) {
        setGridOffsetMapException exc;
        throw exc;
    }
    edgeOffsets = newEdgeOffsets;
    updateTiles();
}

void hexGrid::setGridInfo(unsigned char rotation, std::vector<unsigned char> & newTextures, std::vector<char> & newHeights, std::vector<unsigned char> & newFlatnesses, std::vector<unsigned char> & newEdgeOffsets) {
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
    if (newFlatnesses.size() != 3*(s*(s+1)) + 1) {
        setGridFlatnessMapException exc;
        throw exc;
    }
    if (newEdgeOffsets.size() != 3*(s*(s+1)) + 1) {
        setGridOffsetMapException exc;
        throw exc;
    }
    gridRotation = rotation;
    textures = newTextures;
    heights = newHeights;
    flatnesses = newFlatnesses;
    edgeOffsets = newEdgeOffsets;
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

void hexGrid::getGridFlatnesses(std::vector<unsigned char> & newFlatnesses) {
    newFlatnesses = flatnesses;
}
