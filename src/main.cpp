#include "main.h"
#include "fstream"

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
    //Load texture and height map
    std::vector<unsigned char> texMap;
    std::vector<char> heightMap;
    std::vector<unsigned char> edgeMap;
    std::ifstream infile("earth.dat");
    if (infile.is_open()) {
        infile.seekg(0, std::ios_base::end);
        auto fileSize = infile.tellg();
        texMap.resize(fileSize);
        heightMap.resize(fileSize);
        edgeMap.resize(fileSize);
        infile.seekg(0, std::ios_base::beg);
        infile.read((char *)(&heightMap[0]), fileSize);
        infile.seekg(0, std::ios_base::beg);
        infile.read((char *)(&texMap[0]), fileSize);
    } else {
        throw 1;
    }
    for (unsigned long i=0; i < texMap.size(); i++) {
        edgeMap[i] = texMap[i] > 1 ? texMap[i] - 1 : 0;
    }
    std::vector<unsigned char> flatnessMap(texMap.size(),1);
    //Create a hexGrid of side length 20, textured with tileset.png with the mapping defined in "earth.dat"
    unsigned int sideLen = 20;
    hexGrid draw(sideLen,320,0.5,&tileset,texMap,heightMap,flatnessMap,edgeMap,shader);
    //Shift the grid's scale and position on the screen to be fully visible
    double scale = 700.d / (2*sideLen+1);
    draw.setPosition(152-scale,0.5*scale);
    draw.setScale(scale,scale);
    unsigned char rot = 0;
    //Keep the window open until closed
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::E) {
                    rot = (rot + 1) % 6;
                    draw.setGridRotation(rot);
                } else if (event.key.code == sf::Keyboard::Q) {
                    rot = (rot + 5) % 6;
                    draw.setGridRotation(rot);
                } else if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                }
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
