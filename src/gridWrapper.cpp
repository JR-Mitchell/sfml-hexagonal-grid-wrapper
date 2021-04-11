#include "gridWrapper.h"

void gridWrapper::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();
    target.draw(grid,states);
}

gridWrapper::gridWrapper(unsigned int gridSideLength, unsigned int textureUnitWidth, double heightUnit, sf::Texture * tileset, std::function<sf::Color(unsigned int a, unsigned int b, unsigned char texChar, char height, sf::Vector3f normal)> colourer):
gridSideLength(gridSideLength), grid(gridSideLength,textureUnitWidth,heightUnit,tileset,colourer) {
}

void gridWrapper::setDataSideLength(unsigned int newDataSideLength) {
    dataSideLength = newDataSideLength;
    const unsigned long square = newDataSideLength*newDataSideLength;
    textures.resize(square);
    heights.resize(square);
    flatnesses.resize(square);
    edgeOffsets.resize(square);
}

void gridWrapper::setData(unsigned int newDataSideLength, std::vector<unsigned char> newTextures, std::vector<char> newHeights, std::vector<unsigned char> newFlatnesses, std::vector<unsigned char> newEdgeOffsets) {
    dataSideLength = newDataSideLength;
    const unsigned long square = newDataSideLength*newDataSideLength;
    //Check that sizes match
    if (newTextures.size() != square) {
        initialiseGridTextureMapException exc;
        throw exc;
    }
    //Ensure that `heights` has the correct number of elements
    if (newHeights.size() != square) {
        initialiseGridHeightMapException exc;
        throw exc;
    }
    //Ensure that `flatnesses` has the correct number of elements
    if (newFlatnesses.size() != square) {
        initialiseGridFlatnessMapException exc;
        throw exc;
    }
    //Ensure that `edgeOffsets` has the correct number of elements
    if (newEdgeOffsets.size() != square) {
        initialiseGridOffsetMapException exc;
        throw exc;
    }
    //Assign
    textures = newTextures;
    heights = newHeights;
    flatnesses = newFlatnesses;
    edgeOffsets = newEdgeOffsets;
}

void gridWrapper::setOrigin(unsigned int a, unsigned int b) {
    originA = a;
    originB = b;
    unsigned char gridRotation = grid.getGridRotation();
    std::vector<unsigned char> gridTextures;
    std::vector<char> gridHeights;
    std::vector<unsigned char> gridFlatnesses;
    std::vector<unsigned char> gridOffsets;
    unsigned long gridSize = 3*(gridSideLength*(gridSideLength+1) + 1);
    gridTextures.reserve(gridSize);
    gridHeights.reserve(gridSize);
    gridFlatnesses.reserve(gridSize);
    gridOffsets.reserve(gridSize);
    unsigned long position = 0;
    for (unsigned int bOffset = 0; bOffset <= 2*gridSideLength; bOffset++) {
        unsigned long startIndex = a + dataSideLength*(b+bOffset) + (bOffset > gridSideLength)*(bOffset-gridSideLength);
        unsigned long endIndex = a + dataSideLength*(b+bOffset) + (bOffset < gridSideLength ? gridSideLength+bOffset : 2*gridSideLength);
        unsigned long difference = endIndex - startIndex;
        gridTextures.insert(gridTextures.begin()+position,textures.begin()+startIndex,textures.begin()+endIndex+1);
        gridHeights.insert(gridHeights.begin()+position,heights.begin()+startIndex,heights.begin()+endIndex+1);
        gridFlatnesses.insert(gridFlatnesses.begin()+position,flatnesses.begin()+startIndex,flatnesses.begin()+endIndex+1);
        gridOffsets.insert(gridOffsets.begin()+position,edgeOffsets.begin()+startIndex,edgeOffsets.begin()+endIndex+1);
        position += difference;
    }
    grid.setGridInfo(gridRotation,gridTextures,gridHeights,gridFlatnesses,gridOffsets);
}

void gridWrapper::addSprite(char minHeight,double a,double b, unsigned char textureSheetKey, sf::IntRect &rectangle) {
    mapSprites.emplace_back(minHeight,a,b,textureSheetKey,&rectangle);
    double ab[2] = {a,b};
    // spriteTree.Insert(ab,ab,&mapSprites.back());
}