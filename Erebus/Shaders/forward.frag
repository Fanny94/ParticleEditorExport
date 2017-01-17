#version 420 core

in vec2 uvOut;

out vec4 finalColor;

uniform sampler2D tex;

void main (){
	finalColor = vec4(1.0, 1.0, 1.0, 1.0);
}
