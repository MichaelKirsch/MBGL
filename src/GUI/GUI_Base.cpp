

#include "GUI_Base.h"

void MBGL::GUI_Base::init() {

}

void MBGL::GUI_Base::update() {

}

void MBGL::GUI_Base::render() {

}

MBGL::GUI_Base::~GUI_Base() {

}

MBGL::GUI_Base::GUI_Base(sf::Window &window) {
    base_outline.upperLeft={0.f,0.f};
    base_outline.size = {(float)window.getSize().x,(float)window.getSize().y};
}

