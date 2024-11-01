#version 330 core
out vec4 FragColor;

in vec4 Color;
in vec2 TexCoord;
uniform float _Time;
in vec3 Normal;  
in vec3 FragPos;  
  
uniform vec3 lightPos; 
uniform vec3 viewPos; 
uniform vec3 lightColor;
// texture samplers
uniform sampler2D texture1;
/*void main()
{
    float ambientStrength = 1;
    vec3 ambient = ambientStrength * lightColor;
    vec3 result = ambient;
vec4 textColor = texture(texture1, TexCoord);
FragColor = vec4(textColor.rgb * result, 1);return;
} */
void main() {
// ambient
    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * lightColor;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;  
    vec4 textColor =texture(texture1, TexCoord);
    vec3 result = (ambient + diffuse + specular);
    FragColor = vec4(result*textColor.rgb, 1.0);
    }