#version 330

#define NR_POINT_LIGHTS 5
#define NR_SPOT_LIGHTS 2

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord0;

// Phong lighting
struct Terrain {
    sampler2D back;
    sampler2D rock;
    sampler2D street;// di fatto è la texture
    sampler2D blendMap;

    sampler2D specular;

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

uniform sampler2D shadowMap;

uniform Terrain terrain;

uniform Light dirLight;

uniform int numPointLights;
uniform int numSpotLights;

uniform Light pointLigths[NR_POINT_LIGHTS];
uniform Light spotLigths[NR_SPOT_LIGHTS];

uniform vec3 viewPos;

vec3 calcDirectionalLight(Light light, vec3 terrainDiffuse, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), terrain.shininess);

    vec3 ambient = light.ambient * terrainDiffuse;
    vec3 diffuse = light.diffuse * diff * terrainDiffuse;
    vec3 specular = light.specular * spec * vec3(texture(terrain.specular, TexCoord0));

    return (ambient + diffuse + specular);
}

vec3 calcPointLight(Light light, vec3 terrainDiffuse, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), terrain.shininess);

    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance +
    light.quadratic * (distance * distance));

    vec3 ambient = light.ambient * terrainDiffuse;
    vec3 diffuse = light.diffuse * diff * terrainDiffuse;
    vec3 specular = light.specular * spec * vec3(texture(terrain.specular, TexCoord0));

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}

vec3 calcSpotLight(Light light, vec3 terrainDiffuse, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    float theta     = dot(lightDir, normalize(-light.direction));
    float epsilon   = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);


    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), terrain.shininess);

    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance +
    light.quadratic * (distance * distance));

    vec3 ambient = light.ambient * terrainDiffuse;
    vec3 diffuse = light.diffuse * diff * terrainDiffuse;
    vec3 specular = light.specular * spec * vec3(texture(terrain.specular, TexCoord0));

    ambient *= intensity * attenuation;
    diffuse *= intensity * attenuation;
    specular *= intensity * attenuation;

    return (ambient + diffuse + specular);
}


void main()
{
    // If you'd fail to specify an output color in your
    // fragment shader OpenGL will render your object black (or white).

    vec4 blendMapColour = texture(terrain.blendMap, TexCoord0);

    float backTextureAmmount = 1 - (blendMapColour.r + blendMapColour.g);

    vec2 titledCoords = TexCoord0 * 250.0;

    vec4 backTextureColor = texture(terrain.back, titledCoords) * backTextureAmmount;
    vec4 rockTextureColor = texture(terrain.rock, titledCoords) * blendMapColour.r;
    vec4 streetTextureColor = texture(terrain.street, titledCoords) * blendMapColour.g;

    vec3 totalColor = vec3(backTextureColor + rockTextureColor + streetTextureColor);

    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result =  calcDirectionalLight(dirLight, totalColor, norm, viewDir);

    for (int i = 0; i < NR_POINT_LIGHTS; i++)
    {
        if (i >= numPointLights)
            break;

        result +=calcPointLight(pointLigths[i], totalColor, norm, FragPos, viewDir);
    }

    for (int i = 0; i < NR_SPOT_LIGHTS; i++)
    {
        if (i >= numSpotLights)
            break;

        result += calcSpotLight(spotLigths[i], totalColor, norm, FragPos, viewDir);
    }

    FragColor = vec4(result, 1.0);
}