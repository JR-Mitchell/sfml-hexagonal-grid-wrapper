#include "hexGrid.h"

hexGrid::hexGrid(unsigned int s, unsigned int textureUnitWidth, sf::Texture * tileset):
    triangles(sf::Triangles, 3*6*(3*(s*(s+1))+1)),
    textures(3*(s*(s+1))+1),
    tileset(tileset),
    textureUnitWidth(textureUnitWidth)
{
    init(s);
}

hexGrid::hexGrid(unsigned int s, unsigned int textureUnitWidth, sf::Texture * tileset, std::vector<unsigned char> textures):
    triangles(sf::Triangles, 3*6*(3*(s*(s+1))+1)),
    textures(textures),
    tileset(tileset),
    textureUnitWidth(textureUnitWidth)
{
    //Ensure that `textures` has the correct number of elements
    if (textures.size() != 3*(s*(s+1)) + 1) {
        initialiseGridTextureMapException exc;
        throw exc;
    }
    init(s);
}

hexGrid::hexGrid(unsigned int s, unsigned int textureUnitWidth, sf::Texture * tileset, std::string texturesFilename):
    triangles(sf::Triangles, 3*6*(3*(s*(s+1))+1)),
    textures(0),
    tileset(tileset),
    textureUnitWidth(textureUnitWidth)
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
    init(s);
}

void hexGrid::init(unsigned int s) {
    unsigned int texModuland = tileset->getSize().x / textureUnitWidth;
    //Back from b=S-1 a=2S
    for (unsigned int bp = s; bp > 0; bp--) {
        for (unsigned int ap = 2*s + 1; ap > 0; ap--) {
            unsigned int a = ap - 1;
            unsigned int b = bp - 1;
            //Check that it's a valid point
            if (a+b >= s) {
                unsigned long pointIndex = (b*(b + 2*s + 3) + 2*a - 2*s)/2;
                unsigned int xOffset = (textures[pointIndex]%texModuland);
                unsigned int yOffset = (textures[pointIndex]/texModuland);
                //Centre of the hex
                for (unsigned char tri = 0; tri < 6; tri++) {
                    //Position
                    triangles[3*(6*pointIndex + tri)].position = sf::Vector2f(a+0.5*b,0.5*sqrt(3)*b);
                    //Texture coordinates
                    triangles[3*(6*pointIndex + tri)].texCoords = sf::Vector2f((xOffset+0.5)*textureUnitWidth,(yOffset+0.5)*textureUnitWidth);
                }
                //Vertices of the hex
                triangles[3*6*pointIndex + 1].position = triangles[3*(6*pointIndex+5) + 2].position = sf::Vector2f(a+0.5*(static_cast<double>(b)-1),0.5*sqrt(3)*b - 0.5/sqrt(3));
                triangles[3*(6*pointIndex+1) + 1].position = triangles[3*6*pointIndex + 2].position = sf::Vector2f(a+0.5*b,0.5*sqrt(3)*b - 1.f/sqrt(3));
                triangles[3*(6*pointIndex+2) + 1].position = triangles[3*(6*pointIndex+1) + 2].position = sf::Vector2f(a+0.5*(b+1),0.5*sqrt(3)*b - 0.5/sqrt(3));
                triangles[3*(6*pointIndex+3) + 1].position = triangles[3*(6*pointIndex+2) + 2].position = sf::Vector2f(a+0.5*(b+1),0.5*sqrt(3)*b + 0.5/sqrt(3));
                triangles[3*(6*pointIndex+4) + 1].position = triangles[3*(6*pointIndex+3) + 2].position = sf::Vector2f(a+0.5*b,0.5*sqrt(3)*b + 1.f/sqrt(3));
                triangles[3*(6*pointIndex+5) + 1].position = triangles[3*(6*pointIndex+4) + 2].position = sf::Vector2f(a+0.5*(static_cast<double>(b)-1),0.5*sqrt(3)*b + 0.5/sqrt(3));
                //Texture coordinates
                triangles[3*6*pointIndex + 1].texCoords = triangles[3*(6*pointIndex+5) + 2].texCoords = sf::Vector2f((xOffset + 0.5 - sqrt(3)/4)*textureUnitWidth, (yOffset + 0.75)*textureUnitWidth);
                triangles[3*(6*pointIndex+1) + 1].texCoords = triangles[3*6*pointIndex + 2].texCoords = sf::Vector2f((xOffset + 0.5)*textureUnitWidth, (yOffset+1)*textureUnitWidth);
                triangles[3*(6*pointIndex+2) + 1].texCoords = triangles[3*(6*pointIndex+1) + 2].texCoords = sf::Vector2f((xOffset + 0.5 + sqrt(3)/4)*textureUnitWidth, (yOffset + 0.75)*textureUnitWidth);
                triangles[3*(6*pointIndex+3) + 1].texCoords = triangles[3*(6*pointIndex+2) + 2].texCoords = sf::Vector2f((xOffset + 0.5 + sqrt(3)/4)*textureUnitWidth, (yOffset + 0.25)*textureUnitWidth);
                triangles[3*(6*pointIndex+4) + 1].texCoords = triangles[3*(6*pointIndex+3) + 2].texCoords = sf::Vector2f((xOffset + 0.5)*textureUnitWidth, yOffset*textureUnitWidth);
                triangles[3*(6*pointIndex+5) + 1].texCoords = triangles[3*(6*pointIndex+4) + 2].texCoords = sf::Vector2f((xOffset + 0.5 - sqrt(3)/4)*textureUnitWidth, (yOffset + 0.25)*textureUnitWidth);
            }
        }
    }
    //Forward from b=S a=0
    for (unsigned int b = s; b <= 2*s; b++) {
        for (unsigned int a = 0; a <= 2*s; a++) {
            //Check that it's a valid point
            if (a+b <= 3*s) {
                unsigned long pointIndex = (2*a + 2*s*(3*b - s - 1) + 3*b - b*b)/2;
                unsigned int xOffset = (textures[pointIndex]%texModuland);
                unsigned int yOffset = (textures[pointIndex]/texModuland);
                //Centre of the hex
                for (unsigned char tri = 0; tri < 6; tri++) {
                    //Position
                    triangles[3*(6*pointIndex + tri)].position = sf::Vector2f(a+0.5*b,0.5*sqrt(3)*b);
                    //Texture coordinates
                    triangles[3*(6*pointIndex + tri)].texCoords = sf::Vector2f((xOffset+0.5)*textureUnitWidth,(yOffset+0.5)*textureUnitWidth);
                }
                //Vertices of the hex
                triangles[3*6*pointIndex + 1].position = triangles[3*(6*pointIndex+5) + 2].position = sf::Vector2f(a+0.5*(static_cast<double>(b)-1),0.5*sqrt(3)*b - 0.5/sqrt(3));
                triangles[3*(6*pointIndex+1) + 1].position = triangles[3*6*pointIndex + 2].position = sf::Vector2f(a+0.5*b,0.5*sqrt(3)*b - 1.f/sqrt(3));
                triangles[3*(6*pointIndex+2) + 1].position = triangles[3*(6*pointIndex+1) + 2].position = sf::Vector2f(a+0.5*(b+1),0.5*sqrt(3)*b - 0.5/sqrt(3));
                triangles[3*(6*pointIndex+3) + 1].position = triangles[3*(6*pointIndex+2) + 2].position = sf::Vector2f(a+0.5*(b+1),0.5*sqrt(3)*b + 0.5/sqrt(3));
                triangles[3*(6*pointIndex+4) + 1].position = triangles[3*(6*pointIndex+3) + 2].position = sf::Vector2f(a+0.5*b,0.5*sqrt(3)*b + 1.f/sqrt(3));
                triangles[3*(6*pointIndex+5) + 1].position = triangles[3*(6*pointIndex+4) + 2].position = sf::Vector2f(a+0.5*(static_cast<double>(b)-1),0.5*sqrt(3)*b + 0.5/sqrt(3));
                //Texture coordinates
                triangles[3*6*pointIndex + 1].texCoords = triangles[3*(6*pointIndex+5) + 2].texCoords = sf::Vector2f((xOffset + 0.5 - sqrt(3)/4)*textureUnitWidth, (yOffset + 0.75)*textureUnitWidth);
                triangles[3*(6*pointIndex+1) + 1].texCoords = triangles[3*6*pointIndex + 2].texCoords = sf::Vector2f((xOffset + 0.5)*textureUnitWidth, (yOffset+1)*textureUnitWidth);
                triangles[3*(6*pointIndex+2) + 1].texCoords = triangles[3*(6*pointIndex+1) + 2].texCoords = sf::Vector2f((xOffset + 0.5 + sqrt(3)/4)*textureUnitWidth, (yOffset + 0.75)*textureUnitWidth);
                triangles[3*(6*pointIndex+3) + 1].texCoords = triangles[3*(6*pointIndex+2) + 2].texCoords = sf::Vector2f((xOffset + 0.5 + sqrt(3)/4)*textureUnitWidth, (yOffset + 0.25)*textureUnitWidth);
                triangles[3*(6*pointIndex+4) + 1].texCoords = triangles[3*(6*pointIndex+3) + 2].texCoords = sf::Vector2f((xOffset + 0.5)*textureUnitWidth, yOffset*textureUnitWidth);
                triangles[3*(6*pointIndex+5) + 1].texCoords = triangles[3*(6*pointIndex+4) + 2].texCoords = sf::Vector2f((xOffset + 0.5 - sqrt(3)/4)*textureUnitWidth, (yOffset + 0.25)*textureUnitWidth);
            }
        }
    }
}

void hexGrid::draw(sf::RenderTarget & target, sf::RenderStates states) const {
    states.transform *= getTransform();
    states.texture = tileset;
    target.draw(triangles, states);
}

void hexGrid::setTexture(unsigned long pointIndex, unsigned char texChar) {
    textures[pointIndex] = texChar;
    //Calculate position on tileset
    unsigned int texModuland = tileset->getSize().x / textureUnitWidth;
    unsigned int xOffset = (texChar%texModuland);
    unsigned int yOffset = (texChar/texModuland);
    //Centre of the hex
    for (unsigned char tri = 0; tri < 6; tri++) {
        triangles[3*(6*pointIndex + tri)].texCoords = sf::Vector2f((xOffset+0.5)*textureUnitWidth,(yOffset+0.5)*textureUnitWidth);
    }
    //Vertices of the hex
    triangles[3*6*pointIndex + 1].texCoords = triangles[3*(6*pointIndex+5) + 2].texCoords = sf::Vector2f((xOffset + 0.5 - sqrt(3)/4)*textureUnitWidth, (yOffset + 0.75)*textureUnitWidth);
    triangles[3*(6*pointIndex+1) + 1].texCoords = triangles[3*6*pointIndex + 2].texCoords = sf::Vector2f((xOffset + 0.5)*textureUnitWidth, (yOffset+1)*textureUnitWidth);
    triangles[3*(6*pointIndex+2) + 1].texCoords = triangles[3*(6*pointIndex+1) + 2].texCoords = sf::Vector2f((xOffset + 0.5 + sqrt(3)/4)*textureUnitWidth, (yOffset + 0.75)*textureUnitWidth);
    triangles[3*(6*pointIndex+3) + 1].texCoords = triangles[3*(6*pointIndex+2) + 2].texCoords = sf::Vector2f((xOffset + 0.5 + sqrt(3)/4)*textureUnitWidth, (yOffset + 0.25)*textureUnitWidth);
    triangles[3*(6*pointIndex+4) + 1].texCoords = triangles[3*(6*pointIndex+3) + 2].texCoords = sf::Vector2f((xOffset + 0.5)*textureUnitWidth, yOffset*textureUnitWidth);
    triangles[3*(6*pointIndex+5) + 1].texCoords = triangles[3*(6*pointIndex+4) + 2].texCoords = sf::Vector2f((xOffset + 0.5 - sqrt(3)/4)*textureUnitWidth, (yOffset + 0.25)*textureUnitWidth);
}
