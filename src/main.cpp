#include "main.h"
#include "fstream"

int main() {
    std::cout << "Hello world!" << std::endl;
    sf::RenderWindow window(sf::VideoMode(1024,768), "HexGrid Demo Window");
    sf::Texture tilemap;
    if (!tilemap.loadFromFile("tileset.png")) {
        loadResourceException exc;
        throw exc;
    }
    unsigned int sideLen = 20;
    hexGrid draw(sideLen,320,&tilemap,"earth.dat");
    double scale = 900.d / (2*sideLen+1);
    draw.setPosition(-5*scale,0.5*scale);
    draw.setScale(scale,scale);
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        window.clear(sf::Color::Black);
        window.draw(draw);
        window.display();
    }
    std::cout << "Goodbye world!" << std::endl;
    return 0;
}
