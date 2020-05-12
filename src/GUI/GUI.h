

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

        struct ColorPalette {
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

            ColorPalette() {};

            ColorPalette(std::vector<glm::vec3> i_colors) : colors(i_colors) {};

            ColorPalette(std::string path_to_file) {
                loadFromFile(path_to_file);
            };

            void loadFromFile(std::string path) {
                //load all the colors from a specified file
                std::ifstream file;
                file.open(path);
                if (file.is_open()) {
                    std::string line;
                    while (std::getline(file, line)) {

                        colors.emplace_back(getRGBoutOfString(line.substr(0,6)));
                    }
                    file.close();
                }
            }

            glm::vec3 getColor(ColorType type) { return colors[type]; };
        private:
            glm::vec3 getRGBoutOfString(std::string colorString) {
                float r = (1.f / 255.f) * std::stoi(colorString.substr(0, 2), nullptr, 16);
                float g = (1.f / 255.f) * std::stoi(colorString.substr(2, 2), nullptr, 16);
                float b = (1.f / 255.f) * std::stoi(colorString.substr(4, 2), nullptr, 16);
                return {r, g, b};
            }

            std::vector<glm::vec3> colors;
        };

        struct Outline {
            Outline() {};

            Outline(float i_x, float i_y, float i_width, float i_height) : x(i_x), y(i_y), width(i_width),
                                                                           height(i_height) {};

            Outline(glm::vec4 input) : x(input.x), y(input.y), width(input.z), height(input.w) {};
            float x = 0.f;
            float y = 0.f;
            float width = 0.f;
            float height = 0.f;
        };

        class RectangleUnit {
            RectangleUnit(std::string vertex_shader_path, std::string fragment_shader_path);

            std::vector<float> vbo_data;
        };


        class RenderingUnit {
        public:
            RenderingUnit(){
                PROGRAMM = ShaderLoader::createProgram({{"data/shaders/simple_triangle.vert"},{"data/shaders/simple_triangle.frag"}});
                glGenVertexArrays(1,&VAO);
                glGenBuffers(1,&VBO);
                glBindVertexArray(VAO);
                glBindBuffer(GL_ARRAY_BUFFER,VBO);
                glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(glm::vec3),(void*)0);
                glEnableVertexAttribArray(0);
                glBindVertexArray(0);
            };


            void add_data(std::vector<glm::vec3> data_to_render) {
                simple_data.insert(simple_data.end(),data_to_render.begin(),data_to_render.end());
                std::cout << "added data to vbo" << std::endl;
            }
            void display() {
                // we will need to upload all the data to the gpu and then render it
                ShaderLoader::useProgramm(PROGRAMM);
                glBindVertexArray(VAO);
                glBindBuffer(GL_ARRAY_BUFFER,VBO);
                glBufferData(GL_ARRAY_BUFFER,simple_data.size()*sizeof(glm::vec3),simple_data.data(),GL_DYNAMIC_DRAW);
                glDrawArrays(GL_TRIANGLES,0,simple_data.size());
                glBindVertexArray(0);
                std::cout << "FINAL DISPLAY OF GUI" << std::endl;
                simple_data.clear();
            };
        private:
            unsigned int PROGRAMM,VBO,VAO;
            std::vector<glm::vec3> simple_data;
        };

        struct Widget {
            virtual void render(RenderingUnit *r_unit) = 0;
            void setColor(ColorPalette::ColorType type){
                m_color = m_col_pal->getColor(type);
            };
            glm::vec3 getColor(){return m_color;};
            RenderingUnit *r_unit;
            ColorPalette* m_col_pal;
            Outline m_outline;
            float transparency = 1.f;
        protected:
            Outline getOutlineFromOutline(Outline& units_outline, Outline& newOutline)
            {
                Outline to_return;
                to_return.height = units_outline.height * newOutline.height;
                to_return.width = units_outline.width * newOutline.width;
                to_return.x = units_outline.x + (units_outline.width*newOutline.x);
                to_return.y = units_outline.y + (units_outline.height*newOutline.y);
                return to_return;
            }
            glm::vec3 m_color = {0.858, 0, 0.823};

            float vec3colortofloat(glm::vec3& to_convert)
            {
                int x= to_convert.x*255;
                int y =to_convert.y*255;
                int z = to_convert.z*255;
                int w = transparency*255;
                glm::uint32 buffer=x;
                buffer = (buffer<<8)+y;
                buffer = (buffer<<8)+z;
                buffer = (buffer<<8)+w;
                return glm::uintBitsToFloat(buffer);
            }

            std::vector<glm::vec3> generateGPUData()
                    {
                        auto col = vec3colortofloat(m_color);
                        //we need to generate 2 triangles
                        std::vector<glm::vec3> to_return;
                        //triangle 1
                        to_return.emplace_back(glm::vec3(m_outline.x,m_outline.y,col));
                        to_return.emplace_back(glm::vec3(m_outline.x+m_outline.width,m_outline.y,col));
                        to_return.emplace_back(glm::vec3(m_outline.x,m_outline.y+m_outline.height,col));
                        //triangle 2
                        to_return.emplace_back(glm::vec3(m_outline.x+m_outline.width,m_outline.y,col));
                        to_return.emplace_back(glm::vec3(m_outline.x+m_outline.width,m_outline.y+m_outline.height,col));
                        to_return.emplace_back(glm::vec3(m_outline.x,m_outline.y+m_outline.height,col));
                        return to_return;
                    };
        };

        struct Hook {
            void hook(Widget *to_hook) {
                std::cout << "HOOK " << std::endl;
                children.push_back(to_hook);
            };

            void unhook(Widget *to_unhook) {
                std::cout << "UNHOOK " << std::endl;
                children.remove(to_unhook);
            }

            std::list<Widget *> children;
        };

        class GUI : public GameObject, public Hook //GUI will be the base class.
        {
        public:
            GUI(WindowManager &mgr) {
                auto &window = mgr.getWindow();
            };

            ~GUI() {};

            void init() {};

            void update() {};

            void render() {
                std::cout << "GUI MAIN RENDER " << std::endl;
                for (auto &ch:children)
                    ch->render(&r_unit);
                r_unit.display();
            };
            Outline main_outline;
            ColorPalette colorPalette;
        private:
            RenderingUnit r_unit;
        };

        class Unit : public Widget, public Hook {
        public:
            Unit(GUI *parent, Outline outline) {
                m_outline = outline; //this sets the size according to the window;
                parent->hook(this);
                m_col_pal = &parent->colorPalette;
                setColor(ColorPalette::Background); //when the gui is the parent then make it the primary background
            };

            Unit(Unit *parent, Outline outline) {
                parent->hook(this);
                m_outline = getOutlineFromOutline(parent->m_outline,outline);
                m_parent = parent;
                m_col_pal = parent->m_col_pal;//when a unit is the parent then make it the secondary background
                setColor(ColorPalette::SecondaryBackground);
            };

            void render(RenderingUnit *r_unit) {
                r_unit->add_data(generateGPUData());
                for (auto &ch:children)
                    ch->render(r_unit);
            };

            ~Unit() {
                if (m_parent != nullptr)
                    m_parent->unhook(this);
            };
        private:
            Unit *m_parent = nullptr;
        };

        class Button : public Widget {
        public:
            Button(Unit *parent, Outline outline) {
                m_outline = getOutlineFromOutline(parent->m_outline,outline);
                parent->hook(this);
                m_parent = parent;
                m_col_pal = parent->m_col_pal;
                setColor(ColorPalette::Widget_Primary);
            };
            void render(RenderingUnit *r_unit) {
                r_unit->add_data(generateGPUData());
            };

            ~Button() {
                if (m_parent != nullptr)
                    m_parent->unhook(this);
            };

        private:
            Unit *m_parent = nullptr;
        };

    }
}



