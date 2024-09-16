#version 330 core
		layout(location = 0) in vec3 aPos;
		layout (location = 1) in vec4 aColor;
		out vec4 Color;
		uniform float _Time;
		void main()
		{
		   Color=aColor;
			vec3 pos= aPos;
			pos.y+=(cos(_Time*1.5+(pos.x*0.5)+(pos.z*0.75))*0.25);
		   gl_Position = vec4(aPos.x, -pos.y, aPos.z, 1.0);
}