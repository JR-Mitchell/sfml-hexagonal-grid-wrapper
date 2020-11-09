#include "renderRow.h"

#define PI 3.14159265;

renderRow::renderRow(unsigned int noHexes, unsigned int textureWidth, bool topFirst): triangles(sf::Triangles, 9*noHexes), edges(sf::Quads, 12*noHexes), topFirst(topFirst), noHexes(noHexes){
    const double sqrtTerm = sqrt(1.0/3.0);
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

void renderRow::setHeights(unsigned int hexIndex, double * heights, std::function<sf::Color(sf::Vector3f)> colourer) {
    const double sqrtTerm = sqrt(1.0/3.0);
    const bool isTop = (static_cast<unsigned int>(topFirst) + hexIndex)%2 == 1;
    const double bottom = static_cast<double>(isTop);
    const double top = static_cast<double>(!isTop);
    //Centre of the hex
    triangles[9*hexIndex].position = triangles[3*(3*hexIndex + 1)].position = triangles[3*(3*hexIndex + 2)].position = sf::Vector2f(hexIndex,sqrtTerm*bottom - heights[0]);
    //Leftmost hex vertex
    triangles[9*hexIndex + 1].position = edges[12*hexIndex + 1].position = sf::Vector2f(hexIndex-2.0/3.0,sqrtTerm*bottom - heights[1]);
    //Leftmost edge bottom
    edges[12*hexIndex].position = sf::Vector2f(hexIndex-2.0/3.0,sqrtTerm*bottom - heights[2]);
    //Slightly left hex vertex
    triangles[9*hexIndex + 2].position = triangles[3*(3*hexIndex + 1) + 1].position = edges[12*hexIndex + 2].position = edges[4*(3*hexIndex + 1) + 1].position = sf::Vector2f(hexIndex-1.0/3.0,sqrtTerm*top - heights[3]);
    //Slightly left edge bottom
    edges[12*hexIndex + 3].position = edges[4*(3*hexIndex + 1)].position = sf::Vector2f(hexIndex-1.0/3.0,sqrtTerm*top - heights[4]);
    //Slightly right hex vertex
    triangles[3*(3*hexIndex + 1) + 2].position = triangles[3*(3*hexIndex + 2) + 1].position = edges[4*(3*hexIndex + 1) + 2].position = edges[4*(3*hexIndex + 2) + 1].position = sf::Vector2f(hexIndex+1.0/3.0,sqrtTerm*top - heights[5]);
    //Slightly right edge bottom
    edges[4*(3*hexIndex + 1) + 3].position = edges[4*(3*hexIndex + 2)].position = sf::Vector2f(hexIndex+1.0/3.0,sqrtTerm*top - heights[6]);
    //Rightmost hex vertex
    triangles[3*(3*hexIndex + 2) + 2].position = edges[4*(3*hexIndex + 2) + 2].position = sf::Vector2f(hexIndex+2.0/3.0,sqrtTerm*bottom - heights[7]);
    //Rightmost edge bottom
    edges[4*(3*hexIndex + 2) + 3].position = sf::Vector2f(hexIndex+2.0/3.0,sqrtTerm*bottom - heights[8]);
    //Do colouring
    //Edges
    for (char i=0; i<3; i++) {
        for (char j=0; j<4; j++) {
            edges[4*(3*hexIndex + i) + j].color = colourer(sf::Vector3f(0.5*(i-1),1-2*isTop,0));
        }
    }
    //Faces
    for (char i=0; i<3; i++) {
        sf::Vector3f fTS(static_cast<float>(i+(i==2)-2)/3.f,(i!=0)*sqrtTerm*(top-bottom),heights[1+2*i]);
        sf::Vector3f sTS(static_cast<float>(i+(i!=0)-1)/3.f,(i!=2)*sqrtTerm*(top-bottom),heights[3+2*i]);
        sf::Vector3f normal(fTS.y*sTS.z - fTS.z*sTS.y,fTS.z*sTS.x - fTS.x*sTS.z,fTS.x*sTS.y - fTS.y*sTS.x);
        if (!isTop) {
            normal.x = -normal.x;
            normal.y = -normal.y;
            normal.z = -normal.z;
        }
        sf::Color faceColour = colourer(normal);
        for (char j=0; j<3; j++) {
            triangles[3*(3*hexIndex + i) + j].color = faceColour;
        }
    }
}
