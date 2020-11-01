#include "main.h"
#include "fstream"

int main() {
    std::cout << "Hello world!" << std::endl;
    //Initialise an SFML render window
    sf::RenderWindow window(sf::VideoMode(1024,768), "HexGrid Demo Window");
    //Load the tileset from locally saved file
    sf::Texture tileset;
    if (!tileset.loadFromFile("tileset.png")) {
        loadResourceException exc;
        throw exc;
    }
    //Create a hexGrid of side length 20, textured with tileset.png with the mapping defined in "earth.dat"
    unsigned int sideLen = 20;
    hexGrid draw(sideLen,320,&tileset,"earth.dat");
    //Shift the grid's scale and position on the screen to be fully visible
    double scale = 900.d / (2*sideLen+1);
    draw.setPosition(-5*scale,0.5*scale);
    draw.setScale(scale,scale);
    //Keep the window open until closed
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
    //Window closes
    std::cout << "Goodbye world!" << std::endl;
    return 0;
}
