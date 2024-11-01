//unlit.vert
#version 450
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 vUV;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
	vec3 pos= vPos;
			 vec3 FragPos = vec3(model * vec4(vPos, 1.0));
			 gl_Position = projection * view * vec4(FragPos, 1.0);
}