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

vec4 pointLight()
{
    vec3 lightVec = lightPos - crntPos;
    float dist = length(lightVec);
    float a = 3.0; float b = 0.7;
    float inten = 1.0f / (a * dist * dist + b * dist + 1.0f);
    float ambient = 0.10f; // lower ambient for toon
    vec3 normal = normalize(Normal);
    vec3 lightDirection = normalize(lightVec);
    float diffuse = max(dot(normal, lightDirection), 0.0f);
    float specularLight = 0.35f;
    vec3 viewDirection = normalize(camPos - crntPos);
    vec3 reflectionDirection = reflect(-lightDirection, normal);
    float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 8);
    float specular = specAmount * specularLight;
    vec4 base = (texture(diffuse0, texCoord) * (diffuse * inten + ambient) + texture(specular0, texCoord).r * specular * inten) * lightColor;
    return base;
}

vec4 direcLight()
{
    float ambient = 0.10f;
    vec3 normal = normalize(Normal);
    vec3 lightDirection = normalize(uLightDir);
    float diffuse = max(dot(normal, lightDirection), 0.0f);
    float specularLight = 0.35f;
    vec3 viewDirection = normalize(camPos - crntPos);
    vec3 reflectionDirection = reflect(-lightDirection, normal);
    float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 8);
    float specular = specAmount * specularLight;
    vec4 base = (texture(diffuse0, texCoord) * (diffuse + ambient) + texture(specular0, texCoord).r * specular) * lightColor;
    return base;
}

vec4 spotLight()
{
    float ambient = 0.10f;
    vec3 normal = normalize(Normal);
    vec3 lightDirection = normalize(lightPos - crntPos);
    float diffuse = max(dot(normal, lightDirection), 0.0f);
    float specularLight = 0.35f;
    vec3 viewDirection = normalize(camPos - crntPos);
    vec3 reflectionDirection = reflect(-lightDirection, normal);
    float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 8);
    float specular = specAmount * specularLight;
    float angle = dot(normalize(uLightDir), -lightDirection);
    float inten = clamp((angle - uSpotOuter) / max(uSpotInner - uSpotOuter, 1e-5), 0.0f, 1.0f);
    vec4 base = (texture(diffuse0, texCoord) * (diffuse * inten + ambient) + texture(specular0, texCoord).r * specular * inten) * lightColor;
    return base;
}

void main()
{
    vec4 col = vec4(1.0);
    if (uLightType == 0)      col = direcLight();
    else if (uLightType == 1) col = pointLight();
    else                      col = spotLight();
    // Toon quantization
    float levels = 4.0;
    col.rgb = floor(col.rgb * levels) / levels;
    FragColor = col;
}

