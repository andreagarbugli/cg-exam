#version 330

#define NR_POINT_LIGHTS 5
#define NR_SPOT_LIGHTS 2

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord0;

// Phong lighting
struct Material {
    sampler2D diffuseTex; // di fatto è la texture
    sampler2D specularTex;

    bool hasTextures;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

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

uniform int numPointLights;
uniform int numSpotLights;

uniform Light pointLigths[NR_POINT_LIGHTS];
uniform Light spotLigths[NR_SPOT_LIGHTS];

uniform vec3 viewPos;

vec3 calcDirectionalLight(Light light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 ambient, diffuse, specular;
    if (material.hasTextures)
    {
        ambient = light.ambient * vec3(texture(material.diffuseTex, TexCoord0));
        diffuse = light.diffuse * diff * vec3(texture(material.diffuseTex, TexCoord0));
        specular = light.specular * spec * vec3(texture(material.specularTex, TexCoord0));
    }
    else
    {
        ambient = light.ambient * material.diffuse;
        diffuse = light.diffuse * diff * material.diffuse;
        specular = light.specular * spec * material.specular;
    }

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

    vec3 ambient, diffuse, specular;
    if (material.hasTextures)
    {
        ambient = light.ambient * vec3(texture(material.diffuseTex, TexCoord0));
        diffuse = light.diffuse * diff * vec3(texture(material.diffuseTex, TexCoord0));
        specular = light.specular * spec * vec3(texture(material.specularTex, TexCoord0));
    }
    else
    {
        ambient = light.ambient * material.diffuse;
        diffuse = light.diffuse * diff * material.diffuse;
        specular = light.specular * spec * material.specular;
    }

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}

vec3 calcSpotLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    float theta     = dot(lightDir, normalize(-light.direction));
    float epsilon   = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance +
                        light.quadratic * (distance * distance));

    vec3 ambient, diffuse, specular;
    if (material.hasTextures)
    {
        ambient = light.ambient * vec3(texture(material.diffuseTex, TexCoord0));
        diffuse = light.diffuse * diff * vec3(texture(material.diffuseTex, TexCoord0));
        specular = light.specular * spec * vec3(texture(material.specularTex, TexCoord0));
    }
    else
    {
        ambient = light.ambient * material.diffuse;
        diffuse = light.diffuse * diff * material.diffuse;
        specular = light.specular * spec * material.specular;
    }

    ambient *= intensity * attenuation;
    diffuse *= intensity * attenuation;
    specular *= intensity * attenuation;

     return (ambient + diffuse + specular);
}

void main()
{
    // If you'd fail to specify an output color in your
    // fragment shader OpenGL will render your object black (or white).

    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = calcDirectionalLight(dirLight, norm, viewDir);

    for (int i = 0; i < NR_POINT_LIGHTS; i++)
    {
        if (i >= numPointLights)
            break;

        result += calcPointLight(pointLigths[i], norm, FragPos, viewDir);
    }

    for (int i = 0; i < NR_SPOT_LIGHTS; i++)
    {
        if (i >= numSpotLights)
           break;

        result += calcSpotLight(spotLigths[i], norm, FragPos, viewDir);
    }

    FragColor = vec4(result, 1.0);
}