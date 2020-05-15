

#pragma once

#include <map>
#include "GameObject.h"
#include "glm/glm.hpp"
#include "SFML/Graphics.hpp"
#include "WindowManager.h"
#include "ShaderLoader.h"
#include <memory>
#include <iostream>
#include <list>
#include <fstream>

//namespace MBGL {
//    namespace GUI {
//
//
//
//        class GUI : public GameObject, public Hook //GUI will be the base class.
//        {
//        public:
//            GUI(WindowManager &mgr) {
//                m_mgr=&mgr;
//            };
//
//            ~GUI() {};
//
//            void init() {};
//
//            void update() {};
//
//            void render() {
//                //std::cout << "GUI MAIN RENDER " << std::endl;
//                for (auto &ch:children)
//                    ch->render(&r_unit);
//                r_unit.display();
//            };
//            Outline main_outline;
//
//
//
//            ColorPalette colorPalette;
//        private:
//            RenderingUnit r_unit;
//            WindowManager* m_mgr;
//        };
//
//        class Unit : public Widget, public Hook {
//        public:
//            Unit(GUI *parent, Outline outline) {
//                m_outline = outline; //this sets the size according to the window;
//                parent->hook(this);
//                m_col_pal = &parent->colorPalette;
//                setColor(ColorPalette::Background); //when the gui is the parent then make it the primary background
//                is_hooked_to_gui= true;
//                m_gui_parent = parent;
//            };
//
//            Unit(Unit *parent, Outline outline) {
//                parent->hook(this);
//                m_outline = getOutlineFromOutline(parent->m_outline,outline);
//                originalOutline = outline;
//                m_parent = parent;
//                m_col_pal = parent->m_col_pal;//when a unit is the parent then make it the secondary background
//                setColor(ColorPalette::SecondaryBackground);
//            };
//
//            void render(RenderingUnit *r_unit) {
//                for (auto &ch:children)
//                    ch->render(r_unit);
//                if(!is_hooked_to_gui)
//                    m_outline = getOutlineFromOutline(m_parent->m_outline,originalOutline);
//                r_unit->add_data(generateGPUData());
//            };
//
//            ~Unit() {
//                if (m_parent != nullptr)
//                    m_parent->unhook(this);
//                if(is_hooked_to_gui)
//                    m_gui_parent->unhook(this);
//            };
//        private:
//            Outline originalOutline;
//            bool is_hooked_to_gui=false;
//            Unit *m_parent = nullptr;
//            GUI* m_gui_parent = nullptr;
//        };
//
//        class Button : public Widget {
//        public:
//            Button(Unit *parent, Outline outline) {
//                parent->hook(this);
//                m_parent = parent;
//                originalOutline = outline;
//                m_col_pal = parent->m_col_pal;
//                setColor(ColorPalette::Widget_Primary);
//            };
//            void render(RenderingUnit *r_unit) {
//                m_outline = getOutlineFromOutline(m_parent->m_outline,originalOutline);
//                r_unit->add_data(generateGPUData());
//            };
//
//            ~Button() {
//                if (m_parent != nullptr)
//                    m_parent->unhook(this);
//            };
//
//        private:
//            Unit *m_parent = nullptr;
//            Outline originalOutline;
//        };
//
//    }
//}



