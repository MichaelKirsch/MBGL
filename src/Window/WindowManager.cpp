

#include "WindowManager.h"

void MBGL::WindowManager::init() {
    m_window.create(sf::VideoMode(700,700),"OpenGLContext",sf::Style::Default);
    m_window.setActive();
    if(!gladLoadGL())
    {
        throw std::runtime_error("Glad is not loading");
    }
    glViewport(0, 0, m_window.getSize().x,m_window.getSize().y);
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

