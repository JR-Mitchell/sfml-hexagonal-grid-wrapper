#include "main.h"
#include "fstream"
#include <random>
#include <algorithm>

sf::Color shader(unsigned int a, unsigned int b, unsigned char texChar, char height, sf::Vector3f normal) {
    unsigned char baseBrightness = 105;
    double heightBrightness = 1;
    sf::Vector3f sunDirection (0.1,0.1,0.98994949366);
    double normalMagnitude = sqrt(normal.x*normal.x + normal.y*normal.y + normal.z*normal.z);
    double dotProduct = (sunDirection.x*normal.x + sunDirection.y*normal.y + sunDirection.z*normal.z)/normalMagnitude;
    double totalBrightness = heightBrightness*height + baseBrightness + (255-baseBrightness)*dotProduct;
    if (totalBrightness < 0) {
        totalBrightness = 0;
    } else if (totalBrightness > 255) {
        totalBrightness = 255;
    }
    return sf::Color(totalBrightness,totalBrightness,totalBrightness,255);
}

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
    //Load the sprite from locally saved file
    sf::Texture spriteTex;
    if (!spriteTex.loadFromFile("sprite.png")) {
        loadResourceException exc;
        throw exc;
    }
    //Set up character sprite
    gridSprite characterSprite(-127,0.1,0.1,spriteTex,sf::IntRect(0,0,64,64));
    characterSprite.setOrigin(32,64);
    characterSprite.setScale(1.f/64.f,1.f/64.f);
    //Set up map
    //Create a gridWrapper
    gridWrapper wrapper(30,320,0.5,&tileset,shader);
    {
        std::vector<unsigned char> textures(10000);
        std::vector<char> height(10000);
        std::vector<unsigned char> flatness(10000);
        std::vector<unsigned char> offset(10000);
        static std::uniform_int_distribution<unsigned char> texDist(0,3);
        static std::uniform_int_distribution<unsigned char> flatDist(0,200);
        static std::uniform_int_distribution<char> heightDist(-6,6);
        static std::default_random_engine generator;

        std::generate(textures.begin(),textures.end(),[](){return texDist(generator);});
        std::generate(height.begin(),height.end(),[](){return heightDist(generator);});
        std::generate(flatness.begin(),flatness.end(),[](){return flatDist(generator);});
        std::generate(offset.begin(),offset.end(),[](){return flatDist(generator);});
        wrapper.setData(100,textures,height,flatness,offset);
    }

    wrapper.setOrigin(5,5);
    std::cout << "Origin set" << std::endl;

    wrapper.scale(10,10);
    
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
        }
        window.clear(sf::Color::Black);
        window.draw(wrapper);
        window.display();
    }
    //Window closes
    std::cout << "Goodbye world!" << std::endl;
    return 0;
}
