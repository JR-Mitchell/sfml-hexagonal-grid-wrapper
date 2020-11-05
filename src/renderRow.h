//This file declares and defines the properties of the renderRow class
#ifndef RENDERROW_H
#define RENDERROW_H

#include <SFML/Graphics.hpp>
#include <math.h>
#include <vector>

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
     * @param centreHeight: the height of the hex center
     * @param farLeftHeight: the height of the leftmost hex vertex
     * @param leftHeight: the height of the slightly left hex vertex
     * @param rightHeight: the height of the slightly right hex vertex
     * @param farRightHeight: the height of the rightmost hex vertex
     */
    void setHeights(unsigned int hexIndex, double centreHeight, double farLeftHeight, double leftHeight, double rightHeight, double farRightHeight);
};

#endif // !RENDERROW_H
