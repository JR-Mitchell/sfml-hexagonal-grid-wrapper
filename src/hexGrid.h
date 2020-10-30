#ifndef HEXGRID_H
#define HEXGRID_H

#include <SFML/Graphics.hpp>
#include <math.h>
#include <vector>
#include <fstream>
#include <string>
#include "exceptions.h"

class hexGrid: public sf::Drawable, public sf::Transformable {
private:
    void init(unsigned int sidelength);
    unsigned int textureUnitWidth;
    sf::VertexArray triangles;
    sf::Texture * tileset;
    std::vector<unsigned char> textures;
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
public:
    hexGrid(unsigned int sidelength, unsigned int textureUnitWidth, sf::Texture * tileset);
    hexGrid(unsigned int sidelength, unsigned int textureUnitWidth, sf::Texture * tileset, std::string texturesFilename);
    hexGrid(unsigned int sidelength, unsigned int textureUnitWidth, sf::Texture * tileset, std::vector<unsigned char> textures);
    void setTexture(unsigned long pointIndex, unsigned char texChar);
};

#endif // !HEXGRID_H
