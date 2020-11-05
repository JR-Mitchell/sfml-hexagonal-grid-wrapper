#include "renderRow.h"

renderRow::renderRow(unsigned int noHexes, unsigned int textureWidth, bool topFirst): triangles(sf::Triangles, 9*noHexes), topFirst(topFirst), noHexes(noHexes){
    const double sqrtTerm = sqrt(1.0/3.0);
    bool isTop = topFirst;
    for (unsigned int i=0; i<noHexes; i++) {
        const double bottom = static_cast<double>(isTop);
        const double top = static_cast<double>(!isTop);
        //Centre of the hex
        triangles[9*i].position = triangles[3*(3*i + 1)].position = triangles[3*(3*i + 2)].position = sf::Vector2f(i,sqrtTerm*bottom);
        triangles[9*i].texCoords = triangles[3*(3*i + 1)].texCoords = triangles[3*(3*i + 2)].texCoords = sf::Vector2f(0.5*textureWidth,0.5*textureWidth);
        //Vertices of the hex
        triangles[9*i + 1].position = sf::Vector2f(i-2.0/3.0,sqrtTerm*bottom);
        triangles[9*i + 2].position = triangles[3*(3*i + 1) + 1].position = sf::Vector2f(i-1.0/3.0,sqrtTerm*top);
        triangles[3*(3*i + 1) + 2].position = triangles[3*(3*i + 2) + 1].position = sf::Vector2f(i+1.0/3.0,sqrtTerm*top);
        triangles[3*(3*i + 2) + 2].position = sf::Vector2f(i+2.0/3.0,sqrtTerm*bottom);
        //Texture coordinates for vertices
        triangles[9*i + 1].texCoords = sf::Vector2f(0,0.5*textureWidth);
        triangles[9*i + 2].texCoords = triangles[3*(3*i + 1) + 1].texCoords = sf::Vector2f(0.25*textureWidth,bottom*textureWidth);
        triangles[3*(3*i + 1) + 2].texCoords = triangles[3*(3*i + 2) + 1].texCoords = sf::Vector2f(0.75*textureWidth,bottom*textureWidth);
        triangles[3*(3*i + 2) + 2].texCoords = sf::Vector2f(1*textureWidth,0.5*textureWidth);
        isTop = !isTop;
    }
}

void renderRow::draw(sf::RenderTarget & target, sf::RenderStates states) const {
    states.transform *= getTransform();
    target.draw(triangles, states);
}

void renderRow::setTexture(unsigned int hexIndex, unsigned int xOffset, unsigned int yOffset, unsigned int textureWidth) {
    const bool isTop = (static_cast<unsigned int>(topFirst) + hexIndex)%2 == 1;
    double bottom = static_cast<double>(isTop);
    double top = static_cast<double>(!isTop);
    //Centre of the hex
    triangles[9*hexIndex].texCoords = triangles[3*(3*hexIndex + 1)].texCoords = triangles[3*(3*hexIndex + 2)].texCoords = sf::Vector2f((0.5+xOffset)*textureWidth,(0.5+yOffset)*textureWidth);
    //Texture coordinates for vertices
    triangles[9*hexIndex + 1].texCoords = sf::Vector2f(xOffset*textureWidth,(0.5+yOffset)*textureWidth);
    triangles[9*hexIndex + 2].texCoords = triangles[3*(3*hexIndex + 1) + 1].texCoords = sf::Vector2f((0.25+xOffset)*textureWidth,(bottom+yOffset)*textureWidth);
    triangles[3*(3*hexIndex + 1) + 2].texCoords = triangles[3*(3*hexIndex + 2) + 1].texCoords = sf::Vector2f((0.75+xOffset)*textureWidth,(bottom+yOffset)*textureWidth);
    triangles[3*(3*hexIndex + 2) + 2].texCoords = sf::Vector2f((1 + xOffset)*textureWidth,(0.5+yOffset)*textureWidth);
}

void renderRow::setHeights(unsigned int hexIndex, double centerHeight, double farLeftHeight, double leftHeight, double rightHeight, double farRightHeight) {
    const double sqrtTerm = sqrt(1.0/3.0);
    const bool isTop = (static_cast<unsigned int>(topFirst) + hexIndex)%2 == 1;
    const double bottom = static_cast<double>(isTop);
    const double top = static_cast<double>(!isTop);
    //Centre of the hex
    triangles[9*hexIndex].position = triangles[3*(3*hexIndex + 1)].position = triangles[3*(3*hexIndex + 2)].position = sf::Vector2f(hexIndex,sqrtTerm*bottom - centerHeight);
    //Vertices of the hex
    triangles[9*hexIndex + 1].position = sf::Vector2f(hexIndex-2.0/3.0,sqrtTerm*bottom - farLeftHeight);
    triangles[9*hexIndex + 2].position = triangles[3*(3*hexIndex + 1) + 1].position = sf::Vector2f(hexIndex-1.0/3.0,sqrtTerm*top - leftHeight);
    triangles[3*(3*hexIndex + 1) + 2].position = triangles[3*(3*hexIndex + 2) + 1].position = sf::Vector2f(hexIndex+1.0/3.0,sqrtTerm*top - rightHeight);
    triangles[3*(3*hexIndex + 2) + 2].position = sf::Vector2f(hexIndex+2.0/3.0,sqrtTerm*bottom - farRightHeight);
}
