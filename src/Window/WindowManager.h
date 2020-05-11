

#pragma once
#include "SFML/Graphics.hpp"
#include "glad.h"
#include "Logger.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <chrono>

namespace MBGL
{
    class WindowManager {
    public:
        void init();
        sf::Window& getWindow(){return m_window;};

        ~WindowManager() = default;
        void clearWindow();
        float swapBuffer(bool debugFrameTime=0);
        void setClearColor(glm::vec4 newcol={0.066, 0.631, 0.831,1.0});

    private:
        void refactor();
        bool m_Fullscreen=0;
        float time_last_frame=0.f;

        std::chrono::_V2::steady_clock::time_point last = std::chrono::steady_clock::now();

        sf::Window m_window;
        glm::vec4 m_clearColor={0.066, 0.631, 0.831,1.0};

    };
}


