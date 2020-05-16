

#include "ImprovedGui.h"

void MBGL::GUI::ColorPalette::loadFromFile(std::string path) {
//load all the colors from a specified file
    std::ifstream file;
    file.open(path);
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {

            colors.emplace_back(getRGBoutOfString(line.substr(0,6)));
        }
        file.close();
    }
}

glm::vec3 MBGL::GUI::ColorPalette::getRGBoutOfString(std::string colorString) {
    float r = (1.f / 255.f) * std::stoi(colorString.substr(0, 2), nullptr, 16);
    float g = (1.f / 255.f) * std::stoi(colorString.substr(2, 2), nullptr, 16);
    float b = (1.f / 255.f) * std::stoi(colorString.substr(4, 2), nullptr, 16);
    return {r, g, b};
}

bool MBGL::GUI::Outline::AABB_point(glm::vec2 point) {
    return (point.x >= x && point.x <= x+width) &&
           (point.y >= y && point.y <= y+height);
}

bool MBGL::GUI::Outline::AABB_outline(MBGL::GUI::Outline &outline) {
    return (x < outline.x + outline.width &&
            x + width > outline.x &&
            y < outline.y + outline.height &&
            y + height > outline.y);
}

float MBGL::GUI::Widget::vec3colortofloat(glm::vec3 &to_convert)
    {
        int x= to_convert.x*255;
        int y =to_convert.y*255;
        int z = to_convert.z*255;
        int w = transparency*255;
        glm::uint32 buffer=x;
        buffer = (buffer<<8)+y;
        buffer = (buffer<<8)+z;
        buffer = (buffer<<8)+w;
        return glm::uintBitsToFloat(buffer);
    }

std::vector<glm::vec3> MBGL::GUI::Widget::generateGPUData() {
    auto col = vec3colortofloat(m_color);
    //we need to generate 2 triangles
    std::vector<glm::vec3> to_return;
    //triangle 1
    to_return.emplace_back(glm::vec3(global_outline.x,global_outline.y,col));
    to_return.emplace_back(glm::vec3(global_outline.x+global_outline.width,global_outline.y,col));
    to_return.emplace_back(glm::vec3(global_outline.x,global_outline.y+global_outline.height,col));
    //triangle 2
    to_return.emplace_back(glm::vec3(global_outline.x+global_outline.width,global_outline.y,col));
    to_return.emplace_back(glm::vec3(global_outline.x+global_outline.width,global_outline.y+global_outline.height,col));
    to_return.emplace_back(glm::vec3(global_outline.x,global_outline.y+global_outline.height,col));
    return to_return;
}

MBGL::GUI::Outline MBGL::GUI::Widget::getOutlineFromOutline(MBGL::GUI::Outline &units_outline, MBGL::GUI::Outline &newOutline) {
    Outline to_return;
    to_return.height = units_outline.height * newOutline.height;
    to_return.width = units_outline.width * newOutline.width;
    to_return.x = units_outline.x + (units_outline.width*newOutline.x);
    to_return.y = units_outline.y + (units_outline.height*newOutline.y);
    return to_return;
}

void MBGL::GUI::Widget::update(glm::vec2 mouse_pos) {
    mouse_is_over = (global_outline.AABB_point(mouse_pos));
    if(mouse_is_over)
        m_color = secondaryColor;
    else
        m_color = primaryColor;
}


void MBGL::GUI::GUI::update(sf::Mouse &mouse) {
    auto pos = mouse.getPosition(m_mgr->getWindow());
    for(auto& e:hooked_children)
    {
        e->update(pixelToPercent(pos));
    }
}

void MBGL::GUI::GUI::generateData(MBGL::GUI::RenderingUnit *r_unit) {
 //empty
}

