

#pragma once

#include <map>
#include "GameObject.h"
#include "glm/glm.hpp"
#include "SFML/Graphics.hpp"
#include "WindowManager.h"
#include <memory>
namespace MBGL{
    namespace GUI {

        struct Outline
        {
            float x=0.f;
            float y=0.f;
            float width=0.f;
            float height=0.f;
        };

        class RenderingUnit
        {
        public:
            RenderingUnit()=default;
        };

        struct Widget
        {
            virtual void render()=0;
            RenderingUnit* r_unit;
        };

        class Hook
        {
            void hook(Widget* to_hook){children.emplace_back(to_hook);};
            std::vector<Widget*> children;
        };

        class GUI : public GameObject,Hook //GUI will be the base class.
        {
        public:
            GUI(WindowManager& mgr);
            ~GUI();
            void init();

            void update();

            void render();
        };

        class Unit : public Widget,Hook
        {
        public:
            Unit(GUI* parent);
            Unit(Unit* parent,Outline outline);
            void render(RenderingUnit* r_unit);
        };

        class Button : public Widget
        {
        public:
            Button(Unit* parent, Outline outline);
        };

        class List : public Unit
        {
        public:

        };


    }
}



