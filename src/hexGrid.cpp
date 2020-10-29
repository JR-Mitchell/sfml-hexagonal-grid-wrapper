#include "hexGrid.h"

hexGrid::hexGrid(unsigned int s): triangles(sf::Triangles, 3*6*(3*(s*(s+1))+1)) {
    //Back from b=S-1 a=2S
    for (unsigned int bp = s; bp > 0; bp--) {
        for (unsigned int ap = 2*s + 1; ap > 0; ap--) {
            unsigned int a = ap - 1;
            unsigned int b = bp - 1;
            //Check that it's a valid point
            if (a+b >= s) {
                unsigned long pointIndex = (b*(b + 2*s + 3) + 2*a - 2*s)/2;
                for (unsigned char tri = 0; tri < 6; tri++) {
                    triangles[3*(6*pointIndex + tri)].position = sf::Vector2f(a+0.5*b,0.5*sqrt(3)*b);
                    triangles[3*(6*pointIndex + tri)].color = sf::Color::Red;
                    triangles[3*(6*pointIndex + tri) + 1].color = sf::Color::Green;
                    triangles[3*(6*pointIndex + tri) + 2].color = sf::Color::Blue;
                }
                triangles[3*6*pointIndex + 1].position = triangles[3*(6*pointIndex+5) + 2].position = sf::Vector2f(a+0.5*(static_cast<double>(b)-1),0.5*sqrt(3)*b - 0.5/sqrt(3));
                triangles[3*(6*pointIndex+1) + 1].position = triangles[3*6*pointIndex + 2].position = sf::Vector2f(a+0.5*b,0.5*sqrt(3)*b - 1.f/sqrt(3));
                triangles[3*(6*pointIndex+2) + 1].position = triangles[3*(6*pointIndex+1) + 2].position = sf::Vector2f(a+0.5*(b+1),0.5*sqrt(3)*b - 0.5/sqrt(3));
                triangles[3*(6*pointIndex+3) + 1].position = triangles[3*(6*pointIndex+2) + 2].position = sf::Vector2f(a+0.5*(b+1),0.5*sqrt(3)*b + 0.5/sqrt(3));
                triangles[3*(6*pointIndex+4) + 1].position = triangles[3*(6*pointIndex+3) + 2].position = sf::Vector2f(a+0.5*b,0.5*sqrt(3)*b + 1.f/sqrt(3));
                triangles[3*(6*pointIndex+5) + 1].position = triangles[3*(6*pointIndex+4) + 2].position = sf::Vector2f(a+0.5*(static_cast<double>(b)-1),0.5*sqrt(3)*b + 0.5/sqrt(3));
            }
        }
    }
    //Forward from b=S a=0
    for (unsigned int b = s; b <= 2*s; b++) {
        for (unsigned int a = 0; a <= 2*s; a++) {
            //Check that it's a valid point
            if (a+b <= 3*s) {
                unsigned long pointIndex = (2*a + 2*s*(3*b - s - 1) + 3*b - b*b)/2;
                for (unsigned char tri = 0; tri < 6; tri++) {
                    triangles[3*(6*pointIndex + tri)].position = sf::Vector2f(a+0.5*b,0.5*sqrt(3)*b);
                    triangles[3*(6*pointIndex + tri)].color = sf::Color::Red;
                    triangles[3*(6*pointIndex + tri) + 1].color = sf::Color::Green;
                    triangles[3*(6*pointIndex + tri) + 2].color = sf::Color::Blue;
                }
                triangles[3*6*pointIndex + 1].position = triangles[3*(6*pointIndex+5) + 2].position = sf::Vector2f(a+0.5*(static_cast<double>(b)-1),0.5*sqrt(3)*b - 0.5/sqrt(3));
                triangles[3*(6*pointIndex+1) + 1].position = triangles[3*6*pointIndex + 2].position = sf::Vector2f(a+0.5*b,0.5*sqrt(3)*b - 1.f/sqrt(3));
                triangles[3*(6*pointIndex+2) + 1].position = triangles[3*(6*pointIndex+1) + 2].position = sf::Vector2f(a+0.5*(b+1),0.5*sqrt(3)*b - 0.5/sqrt(3));
                triangles[3*(6*pointIndex+3) + 1].position = triangles[3*(6*pointIndex+2) + 2].position = sf::Vector2f(a+0.5*(b+1),0.5*sqrt(3)*b + 0.5/sqrt(3));
                triangles[3*(6*pointIndex+4) + 1].position = triangles[3*(6*pointIndex+3) + 2].position = sf::Vector2f(a+0.5*b,0.5*sqrt(3)*b + 1.f/sqrt(3));
                triangles[3*(6*pointIndex+5) + 1].position = triangles[3*(6*pointIndex+4) + 2].position = sf::Vector2f(a+0.5*(static_cast<double>(b)-1),0.5*sqrt(3)*b + 0.5/sqrt(3));
            }
        }
    }
}

void hexGrid::draw(sf::RenderTarget & target, sf::RenderStates states) const {
    states.transform *= getTransform();
    target.draw(triangles, states);
}
