#version 420 core

in vec3 pos;
in vec3 normal;
in vec2 UV;

out vec2 uvOut;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

void main(){
	gl_Position = projectionMatrix * viewMatrix * mat4(1.0) *  vec4(pos,1.0);
}
