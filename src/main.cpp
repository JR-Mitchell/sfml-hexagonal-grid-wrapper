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
    //Set up grass sprites
    std::vector<gridSprite> grassSprites;
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
    std::vector<unsigned char> flatnessMap(texMap.size(),1);
    for (unsigned long i=0; i < texMap.size(); i++) {
        flatnessMap[i] = texMap[i] == 0 ? 255 : 0;
        edgeMap[i] = texMap[i] > 1 ? 1 : 0;
    }
    //Create a hexGrid of side length 20, textured with tileset.png with the mapping defined in "earth.dat"
    unsigned int sideLen = 20;
    hexGrid draw(sideLen,320,0.5,&tileset,texMap,heightMap,flatnessMap,edgeMap,shader);
    //Shift the grid's scale and position on the screen to be fully visible
    double scale = 700.d / (2*sideLen+1);
    draw.setPosition(152-scale,0.5*scale);
    draw.setScale(scale,scale);
    //Add grass sprites to grassy areas
    grassSprites.emplace_back(-127,6.01,0.01,spriteTex,sf::IntRect(64,64,64,64));
    grassSprites.emplace_back(-127,2.34,8.55,spriteTex,sf::IntRect(64,64,64,64));
    grassSprites.emplace_back(-127,19.81,15.3,spriteTex,sf::IntRect(64,64,64,64));
    for (auto iter = grassSprites.begin(), end = grassSprites.end(); iter != end; iter++) {
        iter->setOrigin(32,64);
        iter->setScale(1.f/64.f,1.f/64.f);
        iter->addToGrid(&draw);
    }
    //Add the character sprite
    characterSprite.addToGrid(&draw);
    unsigned char rot = 0;
    //Keep the window open until closed
    double aUpDirections [] = {-1,-1,0,1,1,0};
    double aLeftDirections [] = {-1,1,2,1,-1,-2};
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
                } else if (event.key.code == sf::Keyboard::W) {
                    characterSprite.setTextureRect(sf::IntRect(0,64,64,64));
                    auto currPos = characterSprite.getABCoords();
                    characterSprite.setAB(currPos.x + 0.1*aUpDirections[rot], currPos.y + 0.1*aUpDirections[(rot+1)%6],&draw);
                } else if (event.key.code == sf::Keyboard::A) {
                    characterSprite.setTextureRect(sf::IntRect(64,0,64,64));
                    auto currPos = characterSprite.getABCoords();
                    characterSprite.setAB(currPos.x + 0.1*aLeftDirections[rot], currPos.y + 0.1*aLeftDirections[(rot+1)%6],&draw);
                } else if (event.key.code == sf::Keyboard::S) {
                    characterSprite.setTextureRect(sf::IntRect(0,0,64,64));
                    auto currPos = characterSprite.getABCoords();
                    characterSprite.setAB(currPos.x + 0.1*aUpDirections[(rot+3)%6], currPos.y + 0.1*aUpDirections[(rot+4)%6],&draw);
                } else if (event.key.code == sf::Keyboard::D) {
                    characterSprite.setTextureRect(sf::IntRect(128,0,-64,64));
                    auto currPos = characterSprite.getABCoords();
                    characterSprite.setAB(currPos.x + 0.1*aLeftDirections[(rot+3)%6], currPos.y + 0.1*aLeftDirections[(rot+4)%6],&draw);
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
