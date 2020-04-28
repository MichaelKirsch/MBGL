#include "library.h"



void hello() {

    sf::Vector2f d;
    d.x = 1.0f;
    std::cout << "Hello, World!" << std::to_string(d.x) << std::endl;
}

sf::RenderWindow openWindow() {
    return sf::RenderWindow(sf::VideoMode(600,600),"HEllo");
}
