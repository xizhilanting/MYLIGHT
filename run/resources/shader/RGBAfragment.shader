#version 330 core							
out vec4 FragColor;							
											
//in vec3 ourColor;							
in vec2 TexCoords;
in vec3 Normal;	
in vec3 FragPos;
struct Material {
    sampler2D  diffuse;
    sampler2D  specular;
    float shininess;
}; 
uniform Material material;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
//�����
struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  
uniform DirLight dirLight;
//���Դ
struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  
#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];

uniform vec3 lightPos;
uniform vec3 viewPos;
////����ԭ��
vec4 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec4 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
void main()									
{	
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
	// ��һ�׶Σ��������
    vec4 result = CalcDirLight(dirLight, norm, viewDir);
    // �ڶ��׶Σ����Դ
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);    
    // �����׶Σ��۹�
    //result += CalcSpotLight(spotLight, norm, FragPos, viewDir);    
	if(result.a<0.1)
		discard;
  FragColor =vec4(result);
	//FragColor=texture(texture1, TexCoords);

}		
//���㶨���
vec4 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // ��������ɫ
    float diff = max(dot(normal, lightDir), 0.0);
    // �������ɫ
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // �ϲ����
    vec4 ambient  = vec4(light.ambient,1)  * vec4(texture(material.diffuse, TexCoords));
    vec4 diffuse  = vec4(light.diffuse,1)  * diff * vec4(texture(material.diffuse, TexCoords));
    vec4 specular = vec4(light.specular,1) * spec * vec4(texture(material.specular, TexCoords));
    return (ambient + diffuse + specular);
}	
//������Դ
vec4 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // ��������ɫ
    float diff = max(dot(normal, lightDir), 0.0);
    // �������ɫ
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // ˥��
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
                 light.quadratic * (distance * distance));  
    // �ϲ����
    vec4 ambient  = vec4(light.ambient,1)  * vec4(texture(material.diffuse, TexCoords));
    vec4 diffuse  = vec4(light.diffuse,1)  * diff * vec4(texture(material.diffuse, TexCoords));
	vec4 specular = vec4(light.specular,1) * spec * vec4(texture(material.specular, TexCoords));

    //vec3 specular = light.specular * spec * vec3(texture(texture1, TexCoords));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}							
