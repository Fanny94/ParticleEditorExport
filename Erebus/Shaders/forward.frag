#version 420 core

in vec2 uvOut;

out vec4 finalColor;

uniform sampler2D tex;

void main (){
	vec4 tempColor = texture(tex, uvOut);
	finalColor = tempColor;
}
