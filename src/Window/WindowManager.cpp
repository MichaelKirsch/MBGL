

#include "WindowManager.h"

void MBGL::WindowManager::init(int width, int height) {
    auto perspectiveProjection = glm::perspective(glm::radians(45.f), (float)800/(float)600, 0.1f, 2000.f);
    sf::ContextSettings settings;
    m_mouse.setPosition({0,0});
    sf::Sound t;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::B))
    {

    }
    settings.depthBits = 24;
    settings.attributeFlags = settings.Core;
    settings.antialiasingLevel = 4;
    settings.minorVersion = 3;
    settings.majorVersion = 4;
    m_window.create(sf::VideoMode(width,height),"OpenGLContext",sf::Style::Default,settings);
    m_window.setActive();
    m_window.requestFocus();
    if(!gladLoadGL())
    {
        throw std::runtime_error("Glad is not loading");
    }
    setClearColor();
    glEnable(GL_DEPTH_TEST);
}

void MBGL::WindowManager::clearWindow() {
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
}

void MBGL::WindowManager::setClearColor(glm::vec4 newcol) {
    m_clearColor = newcol;
    glClearColor(newcol.x,newcol.y,newcol.z,newcol.a);
}

float MBGL::WindowManager::swapBuffer(bool debugFrameTime) {
    m_window.display();
    if(debugFrameTime)
    {
        auto now = std::chrono::steady_clock::now();
        auto dif = now-last;
        last=now;
        return std::chrono::duration_cast<std::chrono::milliseconds>(dif).count();
    }
    return 0.f;
}

void MBGL::WindowManager::refactor() {
    glViewport(0, 0, m_window.getSize().x,m_window.getSize().y);
}

