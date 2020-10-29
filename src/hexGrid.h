#ifndef HEXGRID_H
#define HEXGRID_H

#include <SFML/Graphics.hpp>
#include <math.h>

class hexGrid: public sf::Drawable, public sf::Transformable {
private:
    sf::VertexArray triangles;
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
public:
    hexGrid(unsigned int sidelength);
};

#endif // !HEXGRID_H
