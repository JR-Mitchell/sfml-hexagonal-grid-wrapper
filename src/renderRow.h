//This file declares and defines the properties of the renderRow class
#ifndef RENDERROW_H
#define RENDERROW_H

#include <SFML/Graphics.hpp>
#include <math.h>
#include <vector>
#include <functional>

/**
 * renderRow class encapsulates sf::VertexArray (and later, sprites) to
 * ensure that rendering takes place from the top of the 2d plane downwards,
 * ensuring objects further back in the orthographic view are not rendered
 * on top of objects closer in the orthographic view
 */
class renderRow: public sf::Drawable, public sf::Transformable {
private:
    // whether the first triangle is part of the top of a hexagon
    const bool topFirst;
    // Underlying sf::VertexArray storing the 3 equilateral triangles for each hexagon in the grid
    sf::VertexArray triangles;
    // Underlying sf::VertexArray storing the 3 extrusion edges for each hexagon in the grid
    sf::VertexArray edges;
    // Overload of sf::Drawable's draw() method
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
public:
    const unsigned int noHexes;
    /**
     * Constructor method for renderRow
     *
     * @param noHexes: the number of hexagons for this row.
     * @param textureWidth: width of a single tile in the tileset
     * @param topFirst: whether the first triangle is part of the top of a hexagon
     */
    renderRow(unsigned int noHexes, unsigned int textureWidth, bool topFirst);
    /**
     * Sets the texture for a particular hexagon
     *
     * @param hexIndex: the index of the hexagon to set texture for
     * @param xOffset: the x offset of the starting pixel
     * @param yOffset: the y offset of the starting pixel
     * @param textureWidth: width of a single tile in the tileset
     */
    void setTexture(unsigned int hexIndex, unsigned int xOffset, unsigned int yOffset, unsigned int textureWidth);
    /**
     * Sets the heights for a particular hexagon
     *
     * @param hexIndex: the index of the hexagon to set texture for
     * @param heights: array of 9 heights: of the centre (0), leftmost hex vertex (1), leftmost edge bottom (2),
     *      slightly left hex vertex (3), slightly left edge bottom (4), slightly right hex vertex (5), slightly
     *      right edge bottom (6), rightmost hex vertex (7), rightmost edge bottom (8)
     * @param colourer: method that takes the vector normal to a surface and returns the colour to shade that surface
     */
    void setHeights(unsigned int hexIndex, double * heights, std::function<sf::Color(sf::Vector3f)> colourer);
};

#endif // !RENDERROW_H
