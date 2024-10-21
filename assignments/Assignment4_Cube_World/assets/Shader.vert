#version 330 core
		layout(location = 0) in vec3 aPos;
		layout (location = 1) in vec4 aColor;
		layout(location = 2) in vec2 aTexCoord;
		out vec4 Color;
		uniform float _Time;
		uniform mat4 model;
		uniform mat4 view;
		uniform mat4 projection;
		out vec2 TexCoord;
		void main()
		{
		   Color=aColor;
			vec3 pos= aPos;
			//pos.y+=(cos(_Time*1.5+(pos.x*0.5)+(pos.z*0.75))*0.25);
		   //gl_Position = vec4(aPos.x, -pos.y, aPos.z, 1.0);
		   gl_Position=projection * view * model * vec4(aPos, 1.0f);
		   TexCoord = vec2(aTexCoord.x,aTexcoord.y);
}