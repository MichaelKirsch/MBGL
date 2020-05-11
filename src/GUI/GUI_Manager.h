

#pragma once

#include <map>
#include "GameObject.h"
#include "glm/glm.hpp"
#include "SFML/Graphics.hpp"
#include "WindowManager.h"
#include <memory>
namespace MBGL{
    namespace GUI {
        typedef glm::fvec3 fillColor;

        struct ColorPalette {
            enum Predefined {
                web, dark, light, blueish
            };
            ColorPalette(std::vector<fillColor> custom_colors);

            ColorPalette(Predefined preset = web) {
                //every preset has to have at least 4 colors, the first color is always the background and the second
                //is always the textcolor/outlinecolor
                switch (preset) {
                    case web:
                        palett = {{0.007, 0.015, 0},
                                  {0.262, 0.525, 0.015},
                                  {0.964, 0.211, 0.011},
                                  {0.984, 0.839, 0.035}};
                        break;
                    case blueish:
                        palett = {{0.258, 0.521, 0},
                                  {0.454, 0.588, 0.050},
                                  {0.780, 0.725, 0.011},
                                  {0.039, 0.549, 0.047}};
                        break;
                    case light:
                        palett = {{0.937, 0.607, 0.058},
                                  {0.494, 0.603, 0.058},
                                  {0.356, 0.474, 0.054},
                                  {0.658, 0.792, 0.058}};
                        break;
                    case dark:
                        palett = {{0.133, 0.513, 0.003},
                                  {0.015, 0.458, 0.054},
                                  {0.164, 0.211, 0.019},
                                  {0.807, 0.807, 0.047}};
                }
            };
            std::vector<fillColor> palett;
        };

        struct Outline {
            Outline()=default;
            Outline(float x,float y,float width,float height){upperLeft.x=x;upperLeft.y=y;size.x=width;size.y=height;};
            glm::fvec2 upperLeft;
            glm::fvec2 size;
            std::array<glm::fvec2,6> getTriangleData()
            {
                return {upperLeft,{upperLeft.x+size.x,upperLeft.y},{upperLeft.x+size.x,upperLeft.y+size.y}
                        ,upperLeft,{upperLeft.x+size.x,upperLeft.y+size.y},{upperLeft.x,upperLeft.y+size.y}};
            }
        };

        class Hookable {
        public:
            Hookable()=default;
            ~Hookable()=default;
            virtual bool check() = 0; //returns true when mouse is over
            virtual void refactor() = 0;

            bool activelyChecked = true;
            bool getsDrawn = true;
            bool close = false;
            Outline outline;
            float transparency = 1.f;

            bool AABB_outline(Outline &first, Outline &sec) {
                return (first.upperLeft.x < sec.upperLeft.x + sec.size.y &&
                        first.upperLeft.x + first.size.y > sec.upperLeft.x &&
                        first.upperLeft.y < sec.upperLeft.y + sec.size.y &&
                        first.upperLeft.y + first.size.x > sec.upperLeft.y);
            };

            bool AABB_point(glm::fvec2 point, Outline& out) {
                return (point.x >= out.upperLeft.x && point.x <= out.upperLeft.x+out.size.x) &&
                       (point.y >= out.upperLeft.y && point.y <= out.upperLeft.y+out.size.y);
            };
        };

        class GUI_Manager : public GameObject {
        public:
            GUI_Manager(MBGL::WindowManager &mgr);

            Outline base_outline; // the base outline is the same as the window outline;
            ColorPalette base_palette = ColorPalette::Predefined::dark;

            void init();

            void update();

            void render() final{
                for(auto& unit:m_hooked_units)
                {
                    unit->outline.getTriangleData();
                }
            };

            void setColorPalette(ColorPalette::Predefined preset);

            void setColorPalette(std::vector<fillColor> palette);

            int hook(Hookable *to_hook){}; //add element that should be managed.
            ~GUI_Manager();
            std::vector<float> GPU_data;
            std::vector<Hookable*> m_hooked_units;
        };

        class Unit : public Hookable {
        public:
            Unit(GUI_Manager& mgr){};
            Unit(Unit &outer_unit, Outline outl){}; // when its the next unit it will be a proportion of the outer base
            void refactor() override {
            }

            bool check() override {
                return false;
            }

            std::vector<float> getUnitsTriangles(){
                std::vector<glm::fvec2> buffer;
                for(auto& el:elements)
                {
                    for(auto& tr:el->outline.getTriangleData())
                        buffer.emplace_back(tr);
                }
            }

            ColorPalette *palette;
            std::vector<std::unique_ptr<Hookable>> elements;
            virtual ~Unit() = default;
        };

        class Widget : public Hookable
        {
        public:
            Widget(Unit& parent,Outline outl):m_parent(parent),m_outline(outline){};
            Outline m_outline;
            fillColor m_color;
            virtual ~Widget()=default;
            Unit& m_parent;
        };
        class Button : public Widget
        {
        public:
            Button(Unit& parent,Outline outl,fillColor color):Widget(parent,outl)
            {
                m_color = color;
                parent.elements.emplace_back(this);
            };
            ~Button(){};

            bool check() override {
                return false;
            }

            void refactor() override {

            }
        };
    }
}



