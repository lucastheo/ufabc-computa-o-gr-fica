#version 410

layout( location = 0 ) in vec4 vPosition;
layout( location = 1 ) in vec4 vColors;

out vec4 v2fcolor;

uniform float scaling;
uniform float color1;
uniform float color2;
uniform float color3;
uniform vec4 translation;
void main(){
    gl_Position = vPosition*vec4(scaling,scaling,scaling,1) + translation;
    v2fcolor[0] = (vColors[0])*0.5f + color1*0.5f;
    v2fcolor[1] = (vColors[1])*0.5f + color2*0.5f;
    v2fcolor[2] = (vColors[2])*0.5f + color3*0.5f;
}
