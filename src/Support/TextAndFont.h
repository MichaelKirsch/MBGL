#pragma once
#include "SFML/Graphics.hpp"
#include "glm/glm.hpp"

namespace MBGL
{
    namespace SUPPORT
    {
        struct Character {
            unsigned int TextureID;  // ID handle of the glyph texture
            glm::ivec2   Size;       // Size of glyph
            glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
            unsigned int Advance;    // Offset to advance to next glyph
        };

        class Font
        {
        public:
            Font();
            Font(std::string filepath)
            {
                loadFromFile(filepath);
            }
            ~Font();
            void loadFromFile(std::string file, int fontsize=50,bool bold = false)
            {
                if(!m_font.loadFromFile(file))
                    throw std::runtime_error("Path to font: " + file + " is incorrect!");
                for(int x =0;x<ASCII_SET_LENGTH;x++)
                {
                    sf::Glyph buffer =  m_font.getGlyph(x,fontsize,bold);
                    m_characters.insert(std::make_pair(x,buffer));
                }

                m_fontTexture =  m_font.getTexture(fontsize);
            }

        private:
            sf::Texture m_fontTexture;
            int ASCII_SET_LENGTH = 128;
            sf::Font m_font;
            std::map<char,sf::Glyph> m_characters;
        };
    }
}
