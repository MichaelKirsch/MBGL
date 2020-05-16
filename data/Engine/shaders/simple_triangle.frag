#version 330

in float color_float;

const uint mask8bit = 255;

void main() {

    uint colors = floatBitsToUint(color_float);
    uint color_red = colors>>(24) & mask8bit;
    uint color_green = colors>>(16) & mask8bit;
    uint color_blue = colors>>(8) & mask8bit;
    uint transparency = colors>>(0) & mask8bit;
    float factor = 1.0/255.0;
    gl_FragColor = vec4(color_red*factor, color_green*factor, color_blue*factor, transparency*factor);
}
