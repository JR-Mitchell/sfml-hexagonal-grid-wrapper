#include "renderRow.h"

renderRow::renderRow(unsigned int noHexes, unsigned int textureWidth, bool topFirst): triangles(sf::Triangles, 9*noHexes), edges(sf::Quads, 12*noHexes), topFirst(topFirst), noHexes(noHexes){
    const double sqrtTerm = sqrt(1.0/3.0);
    sf::Color grey(0,0,0,100);
    bool isTop = topFirst;
    for (unsigned int i=0; i<noHexes; i++) {
        const double bottom = static_cast<double>(isTop);
        const double top = static_cast<double>(!isTop);
        //Centre of the hex
        triangles[9*i].position = triangles[3*(3*i + 1)].position = triangles[3*(3*i + 2)].position = sf::Vector2f(i,sqrtTerm*bottom);
        triangles[9*i].texCoords = triangles[3*(3*i + 1)].texCoords = triangles[3*(3*i + 2)].texCoords = sf::Vector2f(0.5*textureWidth,0.5*textureWidth);
        //Vertices of the hex
        triangles[9*i + 1].position = edges[12*i].position = edges[12*i + 1].position = sf::Vector2f(i-2.0/3.0,sqrtTerm*bottom);
        triangles[9*i + 2].position = triangles[3*(3*i + 1) + 1].position = edges[12*i + 2].position = edges[12*i + 3].position = edges[4*(3*i + 1)].position = edges[4*(3*i + 1) + 1].position = sf::Vector2f(i-1.0/3.0,sqrtTerm*top);
        triangles[3*(3*i + 1) + 2].position = triangles[3*(3*i + 2) + 1].position = edges[4*(3*i + 1) + 2].position = edges[4*(3*i + 1) + 3].position = edges[4*(3*i + 2)].position = edges[4*(3*i + 2) + 1].position = sf::Vector2f(i+1.0/3.0,sqrtTerm*top);
        triangles[3*(3*i + 2) + 2].position = edges[4*(3*i + 2) + 2].position = edges[4*(3*i + 2) + 3].position = sf::Vector2f(i+2.0/3.0,sqrtTerm*bottom);
        //Texture coordinates for vertices and edges
        triangles[9*i + 1].texCoords = edges[12*i + 1].texCoords = sf::Vector2f(0.25*textureWidth,0.5*textureWidth);
        triangles[9*i + 2].texCoords = triangles[3*(3*i + 1) + 1].texCoords = edges[12*i + 2].texCoords = edges[4*(3*i + 1) + 1].texCoords = sf::Vector2f(0.375*textureWidth,0.5*(0.5+top)*textureWidth);
        triangles[3*(3*i + 1) + 2].texCoords = triangles[3*(3*i + 2) + 1].texCoords = edges[4*(3*i + 1) + 2].texCoords = edges[4*(3*i + 2) + 1].texCoords = sf::Vector2f(0.625*textureWidth,0.5*(0.5 + top)*textureWidth);
        triangles[3*(3*i + 2) + 2].texCoords = edges[4*(3*i + 2) + 2].texCoords = sf::Vector2f(0.75*textureWidth,0.5*textureWidth);
        edges[12*i].texCoords = sf::Vector2f(0,0.5*textureWidth);
        edges[12*i + 3].texCoords = edges[4*(3*i + 1)].texCoords = sf::Vector2f(0,top*textureWidth);
        edges[4*(3*i + 1) + 3].texCoords = edges[4*(3*i + 2)].texCoords = sf::Vector2f(1,top*textureWidth);
        edges[4*(3*i + 2) + 3].texCoords = sf::Vector2f(1,0.5*textureWidth);
        isTop = !isTop;
        for (unsigned int j=0; j<12; j++) {
//            edges[12*i + j].color = grey;
        }
    }
}

void renderRow::draw(sf::RenderTarget & target, sf::RenderStates states) const {
    states.transform *= getTransform();
    target.draw(edges, states);
    target.draw(triangles, states);
}

void renderRow::setTexture(unsigned int hexIndex, unsigned int xOffset, unsigned int yOffset, unsigned int textureWidth) {
    const bool isTop = (static_cast<unsigned int>(topFirst) + hexIndex)%2 == 1;
    double bottom = static_cast<double>(isTop);
    double top = static_cast<double>(!isTop);
    //Centre of the hex
    triangles[9*hexIndex].texCoords = triangles[3*(3*hexIndex + 1)].texCoords = triangles[3*(3*hexIndex + 2)].texCoords = sf::Vector2f((0.5+xOffset)*textureWidth,(0.5+yOffset)*textureWidth);
    //Texture coordinates for vertices and edges
    triangles[9*hexIndex + 1].texCoords = edges[12*hexIndex + 1].texCoords = sf::Vector2f((xOffset+0.25)*textureWidth,(0.5+yOffset)*textureWidth);
    triangles[9*hexIndex + 2].texCoords = triangles[3*(3*hexIndex + 1) + 1].texCoords = edges[12*hexIndex + 2].texCoords = edges[4*(3*hexIndex + 1) + 1].texCoords = sf::Vector2f((0.375+xOffset)*textureWidth,(yOffset+0.5*(0.5 + top))*textureWidth);
    triangles[3*(3*hexIndex + 1) + 2].texCoords = triangles[3*(3*hexIndex + 2) + 1].texCoords = edges[4*(3*hexIndex + 1) + 2].texCoords = edges[4*(3*hexIndex + 2) + 1].texCoords = sf::Vector2f((0.625+xOffset)*textureWidth,(yOffset+0.5*(0.5 + top))*textureWidth);
    triangles[3*(3*hexIndex + 2) + 2].texCoords = edges[4*(3*hexIndex + 2) + 2].texCoords = sf::Vector2f((0.75 + xOffset)*textureWidth,(0.5+yOffset)*textureWidth);
    edges[12*hexIndex].texCoords = sf::Vector2f(xOffset*textureWidth,(yOffset+0.5)*textureWidth);
    edges[12*hexIndex + 3].texCoords = edges[4*(3*hexIndex + 1)].texCoords = sf::Vector2f(xOffset*textureWidth,(yOffset+top)*textureWidth);
    edges[4*(3*hexIndex + 1) + 3].texCoords = edges[4*(3*hexIndex + 2)].texCoords = sf::Vector2f((xOffset+1)*textureWidth,(yOffset+top)*textureWidth);
    edges[4*(3*hexIndex + 2) + 3].texCoords = sf::Vector2f((xOffset+1)*textureWidth,(yOffset+0.5)*textureWidth);
}

void renderRow::setHeights(unsigned int hexIndex, double centerHeight, double farLeftHeight, double leftHeight, double rightHeight, double farRightHeight) {
    const double sqrtTerm = sqrt(1.0/3.0);
    const bool isTop = (static_cast<unsigned int>(topFirst) + hexIndex)%2 == 1;
    const double bottom = static_cast<double>(isTop);
    const double top = static_cast<double>(!isTop);
    //Centre of the hex
    triangles[9*hexIndex].position = triangles[3*(3*hexIndex + 1)].position = triangles[3*(3*hexIndex + 2)].position = sf::Vector2f(hexIndex,sqrtTerm*bottom - centerHeight);
    //Vertices of the hex
    triangles[9*hexIndex + 1].position = edges[12*hexIndex + 1].position = sf::Vector2f(hexIndex-2.0/3.0,sqrtTerm*bottom - farLeftHeight);
    triangles[9*hexIndex + 2].position = triangles[3*(3*hexIndex + 1) + 1].position = edges[12*hexIndex + 2].position = edges[4*(3*hexIndex + 1) + 1].position = sf::Vector2f(hexIndex-1.0/3.0,sqrtTerm*top - leftHeight);
    triangles[3*(3*hexIndex + 1) + 2].position = triangles[3*(3*hexIndex + 2) + 1].position = edges[4*(3*hexIndex + 1) + 2].position = edges[4*(3*hexIndex + 2) + 1].position = sf::Vector2f(hexIndex+1.0/3.0,sqrtTerm*top - rightHeight);
    triangles[3*(3*hexIndex + 2) + 2].position = edges[4*(3*hexIndex + 2) + 2].position = sf::Vector2f(hexIndex+2.0/3.0,sqrtTerm*bottom - farRightHeight);
}
