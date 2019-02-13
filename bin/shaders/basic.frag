#version 330

#define TEXTURE_MAPS
#define NR_POINT_LIGHTS 1

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord0;

// Phong lighting
struct Material {
#ifdef TEXTURE_MAPS
    sampler2D diffuse; // di fatto è la texture
    sampler2D specular;
#else
    vec3 diffuse;
    vec3 specular;
#endif
    float shininess;
};

struct Light {

    // positon per point light
    // direction per directional light
    // con spot light si usano entrambe
    vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    // per point light
    float constant;
    float linear;
    float quadratic;

    // per spot light
    float cutOff;
    float outerCutOff;
};

uniform Material material;

uniform Light dirLight;
uniform Light pointLigths[NR_POINT_LIGHTS];

uniform vec3 viewPos;

uniform vec4 objectColor;

vec3 calcDirectionalLight(Light light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    #ifdef TEXTURE_MAPS
        vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord0));
        vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoord0));
        vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord0));
    #else
        vec3 ambient = light.ambient * material.diffuse;
        vec3 diffuse = light.diffuse * diff * material.diffuse;
        vec3 specular = light.specular * spec * material.specular;
    #endif

    return (ambient + diffuse + specular);
}

vec3 calcPointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance +
                        light.quadratic * (distance * distance));

    #ifdef TEXTURE_MAPS
        vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord0));
        vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoord0));
        vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord0));
    #else
        vec3 ambient = light.ambient * material.diffuse;
        vec3 diffuse = light.diffuse * diff * material.diffuse;
        vec3 specular = light.specular * spec * material.specular;
    #endif

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}

//vec3 calcSpotLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir)
//{
//    vec3 lightDir = normalize(light.position - fragPos);
//
//    // spot light
//    float theta = dot(lightDir, normalize(-light.direction));
//    // altri valori altrimenti la luce della spot light non viene
//    // smussata ma ha dei contorni netti e non reali
//    float epsilon = light.cutOff - light.outerCutOff;
//    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
//
//    // diffuse shading
//    float diff = max(dot(normal, lightDir), 0.0);
//
//    // specular shading
//    vec3 reflectDir = reflect(-lightDir, normal);
//    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
//
//    // attenuation
//    float distance = length(light.position - FragPos);
//    float attenuation = 1.0 / (light.constant + light.linear * distance +
//                        light.quadratic * (distance * distance));
//
//    mat3 lightParams = calcLightParams(light, diff, spec);
//
//    vec3 ambient = lightParams[0] * attenuation;
//    vec3 diffuse = lightParams[1] * intensity * attenuation;
//    vec3 specular = lightParams[2] * intensity * attenuation;
//
//    return (ambient + diffuse + specular);
//}

void main()
{
    // If you'd fail to specify an output color in your
    // fragment shader OpenGL will render your object black (or white).

    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = calcDirectionalLight(dirLight, norm, viewDir);

    for (int i = 0; i < NR_POINT_LIGHTS; i++)
    {
        result += calcPointLight(pointLigths[i], norm, FragPos, viewDir);
    }

    vec4 colorResult = vec4(result, 1.0) * objectColor;

    FragColor = colorResult;
}