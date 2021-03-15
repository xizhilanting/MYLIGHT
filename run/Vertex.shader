#version 330 core							 
layout (location = 0) in vec3 aPos;			 
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (std140) uniform Matrices
{
    mat4 projection;
    mat4 view;
};
uniform mat4 model;

out vec3 Normal;
out vec3 FragPos; 
//out vec3 ourColor;							 
out vec2 TexCoords;	
//“ı”∞
//uniform mat4 lightSpaceMatrix;	
//out vec4 FragPosLightSpace;
//“ı”∞Ω· ¯								 
void main()									 
{											 
    gl_Position = projection*view*model*vec4(aPos, 1.0);			 
    //ourColor = aColor;			
	TexCoords=aTexCoord;
	Normal=mat3(transpose(inverse(model))) * aNormal;
	FragPos=vec3(model * vec4(aPos, 1.0));	
	//FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);	 
}											 