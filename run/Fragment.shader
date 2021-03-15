#version 330 core							
out vec4 FragColor;							
											
//in vec3 ourColor;							
in vec2 TexCoords;
in vec3 Normal;	
in vec3 FragPos;
in vec4 FragPosLightSpace;

uniform sampler2D shadowMap;
struct Material {
    sampler2D  diffuse;
    sampler2D  specular;
    float shininess;
}; 
uniform Material material;

//定向光
struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  
uniform DirLight dirLight;
//点光源
struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  
struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
};
uniform SpotLight spotLight;
#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];

uniform vec3 lightPos;
uniform vec3 viewPos;



struct LED {
	sampler2D SLED;
	vec3 ld;
	vec3 rd;
	vec3 ru;
    vec3 ins;
};
uniform LED LEDA;
//测试用待删除

//测试完毕
////函数原型
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcLedLight(LED thLED,vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcColor(sampler2D tex,vec2 tx);
float ShadowCalculation(vec4 fragPosLightSpace);
void main()									
{	
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
	vec3 result=vec3(0,0,0);
	// 第一阶段：定向光照
    // result = CalcDirLight(dirLight, norm, viewDir);
    // 第二阶段：点光源
    //for(int i = 0; i < NR_POINT_LIGHTS; i++)
       // result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);  
	//result=CalcLedLight(LEDA,norm,FragPos,viewDir);  
    
{
// 第三阶段：聚光
    //result += CalcSpotLight(spotLight, norm, FragPos, viewDir);    
//	if(gl_FragCoord.x < 400)
//        result = result+0.3;
//    else
//		result = result-0.3;
//if(gl_FrontFacing)
        
//    else
//        FragColor = vec4(1.0, 0.0, 0.0, 1.0);;
}

{
//阴影开始
// calculate shadow
//	vec3 ambient  =  vec3(texture(material.diffuse, TexCoords));
//   // float shadow = ShadowCalculation(FragPosLightSpace);                      
//	float shadow =  ShadowCalculation(FragPosLightSpace) ;                      
//    shadow = min(shadow, 0.75); // reduce shadow strength a little: allow some diffuse/specular light in shadowed regions
//vec3 lighting = ((1.0 - shadow) * (result)) ;    
//	//阴影结束
}
//屏蔽掉其他光照仅留LED
result=CalcLedLight(LEDA,norm, FragPos, viewDir);



//float max=result.x;
//if(max<result.y)
//	max=result.y;
//if(max<result.z)
//	max=result.z;
//result=result/max;
//	 FragColor =vec4((lighting+result), 1);
	FragColor =vec4(result,1);



//FragColor=vec4(vec3(texture(LEDA.SLED,TexCoords)),1);
//FragColor=vec4(vec3(texture(LEDA.SLED, vec2(FragPos.x,FragPos.z))),1);

//FragColor=vec4(vec3(texture(LEDA.SLED, TexCoords)),1);

}	
//阴影
float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(spotLight.position - FragPos);
    //float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    //float shadow = 0.0;
    //vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    //for(int x = -1; x <= 1; ++x)
    //{
    //    for(int y = -1; y <= 1; ++y)
    //    {
    //        float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
    //        shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
    //    }    
    //}
    ////shadow /= 9.0;
    
    //// Keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    //if(projCoords.z > 1.0)
    //    shadow = 0.0;
        //float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
float bias = 0.0005;
float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    return shadow;
    return shadow;
}	
//计算定向光
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // 漫反射着色
    float diff = max(dot(normal, lightDir), 0.0);
    // 镜面光着色
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // 合并结果
	vec3 ambient ;
    //ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    return (ambient + diffuse + specular);

}	
//计算点光源
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // 漫反射着色
    float diff = max(dot(normal, lightDir), 0.0);
    // 镜面光着色
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // 衰减
    float distance    = length(light.position - fragPos)*1.5;	
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
                 light.quadratic * (distance * distance));  
    // 合并结果
	//vec3 ambient=light.ambient   * vec3(texture(material.diffuse, TexCoords));
 //  // ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));
 //   vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
	//vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

vec3 ambient=light.ambient  ;
   // ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse  = light.diffuse  * diff;
	vec3 specular = light.specular * spec;

    //vec3 specular = light.specular * spec * vec3(texture(texture1, TexCoords));
    //ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
//if(distance>15)	return vec3(0,0,0);
    return ambient +( diffuse + specular)/2;

}		
// calculates the color when using a spot light.
vec3 CalcSpotLight1(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    if(theta > light.cutOff) // remember that we're working with angles as cosines instead of degrees so a '>' is used.
    {    
        // ambient
		vec3 ambient;
        ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
        
        // diffuse 
        vec3 norm = normalize(Normal);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;  
        
        // specular
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);  
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;  
        
        // attenuation
        //float distance    = length(light.position - FragPos);
        //float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

        //// ambient  *= attenuation; // remove attenuation from ambient, as otherwise at large distances the light would be darker inside than outside the spotlight due the ambient term in the else branche
        //diffuse   *= attenuation;
        //specular *= attenuation;   
            
        vec3 result =  ambient+diffuse + specular;
		return result;
       // FragColor = vec4(result, 1.0);
    }
    else 
    {
vec3 ambient;
        ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
        // else, use ambient light so scene isn't completely dark outside the spotlight.
        vec3 result = (light.ambient * texture(material.diffuse, TexCoords).rgb);
		return result;
    }
    //return (ambient + diffuse + specular);	
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
     //lightDir = normalize(light.position - fragPos);
	vec3 lightDir=-light.direction;
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0),material.shininess); //
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // spotlight intensity
    float theta = dot(light.direction, normalize(fragPos-light.position));
    //float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    //float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    float intensity = smoothstep(light.outerCutOff, light.cutOff, theta);

    //intensity = 0.01;
    // combine results
    vec3 ambient = light.ambient;// * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse ;//* diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec;// * vec3(texture(material.specular, TexCoords));
    ambient *=   attenuation *intensity;//
    diffuse *=   attenuation *intensity;//
    specular *=  attenuation *intensity;//
    return (diffuse + specular);
    //return (ambient + diffuse + specular);

}

//LED

vec3 CalcLedLight(LED thLED,vec3 normal, vec3 fragPos, vec3 viewDir)
{
//通过pos降采返回vec3
//通过颜色，和两个pos计算颜色
int step = 50;//各分50步
vec3 result;
	//vec3 thbegin = thLED.ld;
	//float zlen = thLED.rd.z - thLED.ld.z;
	//float ylen = thLED.ru.y - thLED.rd.y;
	vec3 thbegin = LEDA.ld;
	float zlen = LEDA.rd.z - LEDA.ld.z;
	float ylen = LEDA.ru.y - LEDA.rd.y;
	float minv = 0.001;
	float stepL = 0.998/(step-1);
	for (int i =0; i < step; i++)
	{
		for (int j = 0; j < step; j+=2)
		{
			//vec3 now = thbegin + vec3(0, 1.0*i*ylen / step, 1.0*j*zlen / step);
			//vec2 tx = vec2(j*stepL+minv, i*stepL+minv);
			vec3 now = thbegin + vec3(0, 1.0*j/step*ylen,1.0*i/step*zlen);
			vec2 tx = vec2(i*stepL+minv, j * stepL + minv);
			SpotLight thSLight;
			thSLight.position=now;
			//float distance    = length(thSLight.position - fragPos)*1.5;
			thSLight.cutOff=0.8061;
			thSLight.outerCutOff=0.5002;
			//float theta = dot(normalize(thSLight.position - fragPos), normalize(-vec3(1,0,0))); 
			//float epsilon = thSLight.cutOff - thSLight.outerCutOff;
			//float intensity = clamp((theta - thSLight.outerCutOff) / epsilon, 0.0, 1.0);
			//if(intensity>0.0001&&distance<55)
			//if()	 
			{
				vec3 color=CalcColor(thLED.SLED,tx);
				thSLight.ambient=color;
				thSLight.diffuse=color;
				thSLight.specular=color;
				thSLight.direction=vec3(1,0,0);
				/*thSLight.constant = 1.0f;
				thSLight.linear = 2.0;
				thSLight.quadratic = 1.592;*/
                thSLight.constant = thLED.ins.x;
                thSLight.linear = thLED.ins.y;
                thSLight.quadratic = thLED.ins.z;
                
                //*************1,4.9,1.4配合0.7，0.5效果不错******************
                // //20,9.7,2.4
                //thSLight.linear = 13;
                //thSLight.quadratic = 15.92;


				//CalcPointLight(thSLight, normal, fragPos, viewDir);
				result+=CalcSpotLight(thSLight,  normal,  fragPos,  viewDir);
			}
		}
	}
return result;
//return result/step/step*10;

}
const float offset = 1.0 / 300.0;  
vec2 offsets[9] = vec2[](
			vec2(-offset,  offset), // 左上
			vec2( 0.0f,    offset), // 正上
			vec2( offset,  offset), // 右上
			vec2(-offset,  0.0f),   // 左
			vec2( 0.0f,    0.0f),   // 中
			vec2( offset,  0.0f),   // 右
			vec2(-offset, -offset), // 左下
			vec2( 0.0f,   -offset), // 正下
			vec2( offset, -offset)  // 右下
		);
	float kernel[9] = float[](//边缘检测
			1.0/9, 1.0/9, 1.0/9,
			1.0/9, 1.0/9, 1.0/9,
			1.0/9, 1.0/9, 1.0/9
		);

vec3 CalcColor(sampler2D tex,vec2 tx)
{
	
		vec3 sampleTex[9];
		for(int i=0;i<9;i++)
			sampleTex[i]=vec3(texture(LEDA.SLED,tx+offsets[i]));//通过传参数调用就出问题
		vec3 col=vec3(0.0f);

		for(int i=0;i<9;i++)
			col+=sampleTex[i]*kernel[i];
col=vec3(texture(LEDA.SLED,tx));

		return  col;


}
