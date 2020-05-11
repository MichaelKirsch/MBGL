

#include "GUI_Manager.h"

void MBGL::GUI::GUI_Manager::init() {

}

void MBGL::GUI::GUI_Manager::update() {

}

void MBGL::GUI::GUI_Manager::render() {

}

MBGL::GUI::GUI_Manager::~GUI_Manager() {

}

MBGL::GUI::GUI_Manager::GUI_Manager(MBGL::WindowManager& mgr) {
    auto& window = mgr.getWindow();
    base_outline.upperLeft={0.f,0.f};
    base_outline.size = {(float)window.getSize().x,(float)window.getSize().y};
}

