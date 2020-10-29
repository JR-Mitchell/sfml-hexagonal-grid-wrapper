#include "main.h"

int main() {
    std::cout << "Hello world!" << std::endl;
    sf::RenderWindow window(sf::VideoMode(1024,768), "HexGrid Demo Window");
    unsigned int sideLen = 20;
    hexGrid draw(sideLen);
    double scale = 700.d / (2*sideLen+1);
    draw.setPosition(1.5*scale,1.5*scale);
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
