#version 330 core
		layout(location = 0) in vec3 aPos;
		layout (location = 1) in vec4 aColor;
		layout(location = 2) in vec2 aTexCoord;
		layout(location = 3) in vec3 aNormal;
		out vec3 FragPos;
		out vec3 Normal;
		out vec4 Color;
		uniform vec3 lightPos; 
		uniform float _Time;
		uniform mat4 model;
		uniform mat4 view;
		uniform mat4 projection;
		out vec2 TexCoord;
		void main()
		{
		   Color=aColor;
			vec3 pos= aPos;
			 FragPos = vec3(model * vec4(aPos, 1.0));
			 Normal = mat3(transpose(inverse(model))) * aNormal;  
			 TexCoord = aTexCoord;
			 gl_Position = projection * view * vec4(FragPos, 1.0);
		  
}