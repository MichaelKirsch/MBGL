

#pragma once
#include "SFML/Graphics.hpp"
#include "glad/glad.h"


class WindowManager {
public:
    WindowManager();
    void init();

    ~WindowManager() = default;
private:
    void refactor();
    bool m_Fullscreen;


    sf::Window m_window;
};



