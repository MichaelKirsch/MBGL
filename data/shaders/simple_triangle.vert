#version 330

layout (location = 0) in vec3 data;

out float color_float;

void main() {
    float x_pos =  (data.x-0.5)*2.0;
    float y_pos = (data.y-0.5)*2.0;
    gl_Position = vec4(x_pos,y_pos,0.0, 1.0);
    color_float = data.z;
}
