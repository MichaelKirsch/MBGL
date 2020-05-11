

#pragma once

#include <map>
#include "Meta/GameObject.h"
#include "glm/glm.hpp"
#include "SFML/Graphics.hpp"
#include <memory>
namespace MBGL{

    typedef glm::fvec3 fillColor;

    struct ColorPalette
    {
        enum Predefined
        {
            web,dark,light,blueish
        };
        ColorPalette(std::vector<fillColor> custom_colors);
        ColorPalette(Predefined preset=web)
        {
            //every preset has to have at least 4 colors, the first color is always the background and the second
            //is always the textcolor/outlinecolor
            switch (preset) {
                case web:
                    palett = {{0.007, 0.015, 0},{0.262, 0.525, 0.015},{0.964, 0.211, 0.011},{0.984, 0.839, 0.035}};
                    break;
                case blueish:
                    palett = {{0.258, 0.521, 0},{0.454, 0.588, 0.050},{0.780, 0.725, 0.011},{0.039, 0.549, 0.047}};
                    break;
                case light:
                    palett ={{0.937, 0.607, 0.058},{0.494, 0.603, 0.058},{0.356, 0.474, 0.054},{0.658, 0.792, 0.058}};
                    break;
                case dark:
                    palett ={{0.133, 0.513, 0.003},{0.015, 0.458, 0.054},{0.164, 0.211, 0.019},{0.807, 0.807, 0.047}};
            }
        };
        std::vector<fillColor> palett;
    };

    struct Outline
    {
        glm::fvec2 upperLeft;
        glm::fvec2 size;
    };

    class Hookable
    {
    public:
        virtual bool check()=0; //returns true when mouse is over
        virtual void refactor()=0;
        bool activelyChecked=true;
        bool getsDrawn=true;
        bool close=false;
        Outline outline;
        bool AABB_outline(Outline& first, Outline& sec){
            return (first.upperLeft.x < sec.upperLeft.x + sec.size.y &&
                    first.upperLeft.x + first.size.y > sec.upperLeft.x &&
                    first.upperLeft.y < sec.upperLeft.y + sec.size.y &&
                    first.upperLeft.y + first.size.x > sec.upperLeft.y);
        };
        bool AABB_point(glm::fvec2 point_to_check)
        {

        }
    };

    class Entity : public Hookable
    {
    public:
        Entity(Outline outline,fillColor color);
        Outline m_outline;
        fillColor m_fillcolor;
    };


    class GUI_Base : public GameObject{
    public:
        GUI_Base(sf::Window& window);
        Outline base_outline; // the base outline is the same as the window outline;
        ColorPalette base_palette = ColorPalette::Predefined::dark;
        virtual void init();
        virtual void update();
        virtual void render();
        void setColorPalette(ColorPalette::Predefined preset);
        void setColorPalette(std::vector<fillColor> palette);
        int hook(Hookable* to_hook); //add element that should be managed.
        ~GUI_Base();
        std::map<int,Hookable*> m_hooked_widgets;
    };

    class Unit : public Hookable
    {
    public:
        Unit(GUI_Base& base); // when this is the first unit it will be the size of the GUI_Base
        Unit(Unit& outer_unit,glm::fvec2 upper_left, glm::fvec2 size); // when its the next unit it will be a proportion of the outer base
        void refactor() override {
        }
        bool check() override {
            return false;
        }
        ColorPalette* palette;
        std::vector<std::unique_ptr<Hookable>> elements;
    };
}



