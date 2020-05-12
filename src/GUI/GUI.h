

#pragma once

#include <map>
#include "GameObject.h"
#include "glm/glm.hpp"
#include "SFML/Graphics.hpp"
#include "WindowManager.h"
#include <memory>
#include <iostream>
#include <list>
namespace MBGL{
    namespace GUI {

        struct Outline
        {
            Outline(){};
            Outline(float i_x,float i_y,float i_width,float i_height):x(i_x),y(i_y),width(i_width),height(i_height){};
            Outline(glm::vec4 input):x(input.x),y(input.y),width(input.z),height(input.w){};
            float x=0.f;
            float y=0.f;
            float width=0.f;
            float height=0.f;
        };

        class RenderingUnit
        {
        public:
            RenderingUnit()=default;
            void render_test(std::string mes){
                std::cout << "Rendering Test " << mes << std::endl;
            }
        };

        struct Widget
        {
            virtual void render(RenderingUnit* r_unit)=0;
            RenderingUnit* r_unit;
            Outline m_outline;
        };

        struct Hook
        {
            void hook(Widget* to_hook){
                    std::cout << "HOOK " << std::endl;
                    children.push_back(to_hook);
                };
            void unhook(Widget* to_unhook)
            {
                std::cout << "UNHOOK " << std::endl;
                children.remove(to_unhook);
            }
            std::list<Widget*> children;
        };

        class GUI : public GameObject,public Hook //GUI will be the base class.
        {
        public:
            GUI(WindowManager& mgr){};
            ~GUI(){};
            void init();

            void update();

            void render(){
                std::cout << "GUI MAIN RENDER " << std::endl;
                for(auto& ch:children)
                    ch->render(&r_unit);
            };
        private:
            RenderingUnit r_unit;
        };

        class Unit : public Widget,public Hook
        {
        public:
            Unit(GUI* parent, Outline outline){
                m_outline = outline;
                parent->hook(this);};
            Unit(Unit* parent,Outline outline){
                parent->hook(this);
                m_outline=outline;
                m_parent = parent;
            };
            void render(RenderingUnit* r_unit){
                r_unit->render_test("Unit");
                for(auto& ch:children)
                    ch->render(r_unit);
            };
            ~Unit()
            {
                if(m_parent!= nullptr)
                    m_parent->unhook(this);
            };
        private:
            Unit* m_parent = nullptr;
        };

        class Button : public Widget
        {
        public:
            Button(Unit* parent, Outline outline)
            {
                m_outline=outline;
                parent->hook(this);
                m_parent=parent;
            };
            void render(RenderingUnit *r_unit){
                r_unit->render_test("Button");
            };
            ~Button()
            {
                if(m_parent!= nullptr)
                    m_parent->unhook(this);
            };

        private:
            Unit* m_parent = nullptr;
        };

    }
}



