#version 330 core

out vec4 FragColor;

in vec3 crntPos;
in vec3 Normal;
in vec3 color;
in vec2 texCoord;

uniform sampler2D diffuse0;
uniform sampler2D specular0;
uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;
uniform int  uLightType;
uniform vec3 uLightDir;
uniform float uSpotInner;
uniform float uSpotOuter;
uniform float uAttenA;
uniform float uAttenB;
uniform float uAttenC;

vec4 pointLight()
{
    vec3 lightVec = lightPos - crntPos;
    float dist = length(lightVec);
    float inten = 1.0f / (uAttenA * dist * dist + uAttenB * dist + max(uAttenC, 1e-5));
    float ambient = 0.20f;
    vec3 normal = normalize(Normal);
    vec3 lightDirection = normalize(lightVec);
    float diffuse = max(dot(normal, lightDirection), 0.0f);
    float specularLight = 0.50f;
    vec3 viewDirection = normalize(camPos - crntPos);
    vec3 reflectionDirection = reflect(-lightDirection, normal);
    float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
    float specular = specAmount * specularLight;
    return (texture(diffuse0, texCoord) * (diffuse * inten + ambient) + texture(specular0, texCoord).r * specular * inten) * lightColor;
}

vec4 direcLight()
{
    float ambient = 0.20f;
    vec3 normal = normalize(Normal);
    vec3 lightDirection = normalize(uLightDir);
    float diffuse = max(dot(normal, lightDirection), 0.0f);
    float specularLight = 0.50f;
    vec3 viewDirection = normalize(camPos - crntPos);
    vec3 reflectionDirection = reflect(-lightDirection, normal);
    float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
    float specular = specAmount * specularLight;
    return (texture(diffuse0, texCoord) * (diffuse + ambient) + texture(specular0, texCoord).r * specular) * lightColor;
}

vec4 spotLight()
{
    float ambient = 0.20f;
    vec3 normal = normalize(Normal);
    vec3 lightDirection = normalize(lightPos - crntPos);
    float diffuse = max(dot(normal, lightDirection), 0.0f);
    float specularLight = 0.50f;
    vec3 viewDirection = normalize(camPos - crntPos);
    vec3 reflectionDirection = reflect(-lightDirection, normal);
    float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
    float specular = specAmount * specularLight;
    float angle = dot(normalize(uLightDir), -lightDirection);
    float inten = clamp((angle - uSpotOuter) / max(uSpotInner - uSpotOuter, 1e-5), 0.0f, 1.0f);
    return (texture(diffuse0, texCoord) * (diffuse * inten + ambient) + texture(specular0, texCoord).r * specular * inten) * lightColor;
}

void main()
{
    vec4 col = vec4(1.0);
    if (uLightType == 0)      col = direcLight();
    else if (uLightType == 1) col = pointLight();
    else                      col = spotLight();
    // Cool tint
    col.rgb *= vec3(0.85, 0.95, 1.10);
    FragColor = col;
}
