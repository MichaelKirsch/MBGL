#pragma once
#include <iostream>
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"

void hello();

sf::RenderWindow openWindow();

sf::Mouse getMouse(){return sf::Mouse();};

sf::Music music(){return sf::Music();};