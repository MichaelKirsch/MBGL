

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

namespace MBGL {
    namespace GUI {
        enum ColorType {
            Background,
            SecondaryBackground,
            Widget_Primary,
            Widget_Secondary,
            Success,
            Warning,
            Error,
            Text_Primary,
            Text_Inactive
        };

        glm::fvec2 pixelToPercent(WindowManager *m_mgr, int x, int y) {
            return {(1.f / m_mgr->getWindow().getSize().x) * x, 1.0 - ((1.f / m_mgr->getWindow().getSize().y) * y)};
        };

        glm::fvec2 pixelToPercent(WindowManager *m_mgr, sf::Vector2i pos) {
            return {(1.f / m_mgr->getWindow().getSize().x) * pos.x,
                    1.0 - ((1.f / m_mgr->getWindow().getSize().y) * pos.y)};
        };

        typedef glm::vec3 fillColor;

        struct ColorPalette {

            ColorPalette() = default;

            ColorPalette(std::vector<glm::vec3> i_colors) : colors(i_colors) {};

            ColorPalette(std::string path_to_file) {
                loadFromFile(path_to_file);
            };

            void loadFromFile(std::string path);

            glm::vec3 getColor(ColorType type) { return colors[type]; };
        private:
            glm::vec3 getRGBoutOfString(std::string colorString);

            std::vector<glm::vec3> colors;
        };

        struct Outline {
            Outline() = default;

            Outline(float i_x, float i_y, float i_width, float i_height) : x(i_x), y(i_y), width(i_width),
                                                                           height(i_height) {};

            Outline(glm::vec4 input) : x(input.x), y(input.y), width(input.z), height(input.w) {};
            float x = 0.f;
            float y = 0.f;
            float width = 0.f;
            float height = 0.f;

            bool AABB_point(glm::vec2 point);

            bool AABB_outline(Outline &outline);
        };

        class RenderingUnit {
        public:
            RenderingUnit() {
                PROGRAMM = ShaderLoader::createProgram({{"data/shaders/simple_triangle.vert"},
                                                        {"data/shaders/simple_triangle.frag"}});
                glGenVertexArrays(1, &VAO);
                glGenBuffers(1, &VBO);
                glBindVertexArray(VAO);
                glBindBuffer(GL_ARRAY_BUFFER, VBO);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void *) 0);
                glEnableVertexAttribArray(0);
                glBindVertexArray(0);
            };


            void add_data(std::vector<glm::vec3> data_to_render) {
                simple_data.insert(simple_data.end(), data_to_render.begin(), data_to_render.end());
            }

            void display() {
                // we will need to upload all the data to the gpu and then render it
                ShaderLoader::useProgramm(PROGRAMM);
                glBindVertexArray(VAO);
                glBindBuffer(GL_ARRAY_BUFFER, VBO);
                glBufferData(GL_ARRAY_BUFFER, simple_data.size() * sizeof(glm::vec3), simple_data.data(),
                             GL_DYNAMIC_DRAW);
                glDrawArrays(GL_TRIANGLES, 0, simple_data.size());
                glBindVertexArray(0);
                //std::cout << "FINAL DISPLAY OF GUI" << std::endl;
                simple_data.clear();
            };
        private:
            unsigned int PROGRAMM, VBO, VAO;
            std::vector<glm::vec3> simple_data;
        };

        struct Widget {
            Widget(ColorPalette *plt, Outline outl) : m_palette(plt) {
                private_outline = outl;
            };

            void setPrimaryColor(fillColor color) {
                primaryColor = color;
            }

            void setPrimaryColor(ColorType type) {
                primaryColor = m_palette->getColor(type);
            }

            void setSecondaryColor(fillColor color) {
                secondaryColor = color;
            }

            void setSecondaryColor(ColorType type) {
                secondaryColor = m_palette->getColor(type);
            }


            fillColor getColor() {
                return m_color;
            }

            void setTransparency(float n_trans) { transparency = n_trans; };

            virtual void generateData(RenderingUnit *r_unit) {};

            virtual void update(glm::vec2 mouse_pos);

            virtual ~Widget() = default;

            bool resizeable = false;
            bool moveable = false;
            bool please_delete = false;
            bool has_unit_as_parent = false;
            Outline global_outline;
            Outline private_outline;
            ColorPalette *m_palette;
            bool mouse_is_over = false;
        protected:
            float transparency = 1.f;

            float vec3colortofloat(glm::vec3 &to_convert);

            std::vector<glm::vec3> generateGPUData();

            std::vector<glm::vec3> generateGPUData(Outline outl, fillColor color);

            Outline getOutlineFromOutline(Outline &units_outline, Outline &newOutline);

            fillColor m_color = {1, 0.078, 0.925}; //make it pink to see elements that need their color set
            fillColor primaryColor;
            fillColor secondaryColor;
        };

        struct Parent : public Widget {
            Parent(ColorPalette *plt, Outline outl) : Widget(plt, outl) {};

            virtual ~Parent() = default;

            void hook(Widget *to_hook) {
                hooked_children.push_back(to_hook);
            };

            void unhook(Widget *to_unhook) {
                std::cout << "Unhooked" << std::endl;
                hooked_children.remove(to_unhook);
            };
            std::list<Widget *> hooked_children;
        };

        struct Child : public Widget {
            Child(Parent *parent, ColorPalette *plt, Outline Outline) : Widget(plt, Outline) {
                hook_into_parent(parent);
            };

            virtual ~Child() {
                unhook_from_parent();
            };

            void hook_into_parent(Parent *prt) {
                //hooked_at_this_parent->unhook(this);
                prt->hook(this);
                hooked_at_this_parent = prt;
            }

            void unhook_from_parent() {
                hooked_at_this_parent->unhook(this);
            }

        protected:
            Parent *hooked_at_this_parent = nullptr;
        };

        class GUI : public Parent {
        public:
            GUI(WindowManager &mgr, ColorPalette &palette) : Parent(&palette, {0.0, 0.0, 1.0, 1.0}) {
                global_outline = private_outline;
                m_mgr = &mgr;
            };

            //GUI class will be the main funktion for all gui elements
            ~GUI() {
                std::cout << "Final Close of GUI" << std::endl;
            };

            void generateData(RenderingUnit *r_unit) override;

            void update(sf::Mouse &mouse);

            void render() {
                for (auto &el:hooked_children)
                    el->generateData(&r_unit);
                r_unit.display();
            };


        private:
            WindowManager *m_mgr;
            RenderingUnit r_unit;
        };

        struct Button : public Child {
            Button(Parent *prt, Outline outl) : Child(prt, prt->m_palette, outl) {
                global_outline = getOutlineFromOutline(hooked_at_this_parent->global_outline, private_outline);
                setPrimaryColor(Widget_Primary);
                setSecondaryColor(Widget_Secondary);
            };

            void generateData(RenderingUnit *r_unit) override {
                global_outline = getOutlineFromOutline(hooked_at_this_parent->global_outline, private_outline);
                r_unit->add_data(generateGPUData());
            }

            void update(glm::vec2 mouse_pos) override {
                Widget::update(mouse_pos);
            }

        };

        struct FreeSlider : public Child {
            FreeSlider(Parent *prt, Outline outl) : Child(prt, prt->m_palette, outl) {
                global_outline = getOutlineFromOutline(hooked_at_this_parent->global_outline, private_outline);
                setPrimaryColor(Warning);
                setSecondaryColor(Error);
            }

            void generateData(RenderingUnit *r_unit) override {
                r_unit->add_data(generateGPUData());
                r_unit->add_data(generateGPUData(slide, m_palette->getColor(Text_Primary)));
                r_unit->add_data(generateGPUData(button, m_palette->getColor(Text_Inactive)));
            }

            void update(glm::vec2 mouse_pos) override {
                Widget::update(mouse_pos);
                if (mouse_is_over && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                    auto max = global_outline.width;
                    auto min = global_outline.x;
                    auto zeroed_out = global_outline.x-mouse_pos.x;

                    value = -1.f*((1.0f/max)*zeroed_out);

                    std::cout << "Value: " << std::to_string(value)<< std::endl;
                }
            }

            float getValue() {
                return value;
            }

            void setValue(float to_Set) {
                value = to_Set;
            }

        private:
            float value = 0.f;
            Outline slide, button;
        };


        struct Unit : public Parent {
            Unit(GUI *par, Outline outl) : Parent(par->m_palette, outl) {
                generateMovingButton();
                setStartColor();
                m_parent = par;
                m_parent->hook(this);
                show_moving_button = true;
            };

            Unit(Unit *par, Outline outl) : Parent(par->m_palette, outl) {
                generateMovingButton();
                setStartColor();
                m_parent = par;
                m_parent->hook(this);
            }

            ~Unit() {
                delete moving_button;
                m_parent->unhook(this);
            }

            void generateData(RenderingUnit *r_unit) override {
                global_outline = getOutlineFromOutline(m_parent->global_outline, private_outline);
                for (auto &el:hooked_children)
                    el->generateData(r_unit);
                r_unit->add_data(generateGPUData());

            }

            void update(glm::vec2 mouse_pos) override {
                Widget::update(mouse_pos);
                for (auto &e:hooked_children) {
                    e->update(mouse_pos);
                }
                if (show_moving_button) {

                }
                if (moveable)
                    if (mouse_is_over && sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
                        private_outline.x = mouse_pos.x - (private_outline.width / 2);
                        private_outline.y = mouse_pos.y - (private_outline.height / 2);
                    }

            }

            bool show_moving_button = false;
        private:
            void setStartColor() {
                setPrimaryColor(Background);
                setSecondaryColor(SecondaryBackground);
            }

            void generateMovingButton() {
                moving_button = new Button(this, {0.85, 0.0, 0.15, 0.15});
                moving_button->setPrimaryColor(SecondaryBackground);
            }

            Parent *m_parent = nullptr;
            Button *moving_button;
        };
    }
}



