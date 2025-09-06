#version 330 core

// Outputs colors in RGBA
out vec4 FragColor;

// Imports the current position from the Vertex Shader
in vec3 crntPos;
// Imports the normal from the Vertex Shader
in vec3 Normal;
// Imports the color from the Vertex Shader
in vec3 color;
// Imports the texture coordinates from the Vertex Shader
in vec2 texCoord;



// Gets the Texture Units from the main function
uniform sampler2D diffuse0;
uniform sampler2D specular0;
// Gets the color and parameters of the light from the main function
uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;
uniform int  uLightType;  // 0=Directional,1=Point,2=Spot
uniform vec3 uLightDir;   // For directional & spot
uniform float uSpotInner; // [0..1]
uniform float uSpotOuter; // [0..1], <= inner
uniform float uAttenA;    // quadratic term
uniform float uAttenB;    // linear term
uniform float uAttenC;    // constant term


vec4 pointLight()
{	
	// used in two variables so I calculate it here to not have to do it twice
	vec3 lightVec = lightPos - crntPos;

	// intensity of light with respect to distance
    float dist = length(lightVec);
    float inten = 1.0f / (uAttenA * dist * dist + uAttenB * dist + max(uAttenC, 1e-5));

	// ambient lighting
	float ambient = uAmbient;

	// diffuse lighting
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightVec);
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	// specular lighting
	float specularLight = uSpecularStrength;
	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), max(uShininess, 1.0));
	float specular = specAmount * specularLight;

	return (texture(diffuse0, texCoord) * (diffuse * inten + ambient) + texture(specular0, texCoord).r * specular * inten) * lightColor;
}

vec4 direcLight()
{
    float ambient = uAmbient;
    vec3 normal = normalize(Normal);
    vec3 lightDirection = normalize(uLightDir);
    float diffuse = max(dot(normal, lightDirection), 0.0f);
    float specularLight = uSpecularStrength;
    vec3 viewDirection = normalize(camPos - crntPos);
    vec3 reflectionDirection = reflect(-lightDirection, normal);
    float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), max(uShininess, 1.0));
    float specular = specAmount * specularLight;
    return (texture(diffuse0, texCoord) * (diffuse + ambient) + texture(specular0, texCoord).r * specular) * lightColor;
}

vec4 spotLight()
{
    float ambient = uAmbient;
    vec3 normal = normalize(Normal);
    vec3 lightDirection = normalize(lightPos - crntPos);
    float diffuse = max(dot(normal, lightDirection), 0.0f);
    float specularLight = uSpecularStrength;
    vec3 viewDirection = normalize(camPos - crntPos);
    vec3 reflectionDirection = reflect(-lightDirection, normal);
    float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), max(uShininess, 1.0));
    float specular = specAmount * specularLight;
    // angle to center of cone
    float angle = dot(normalize(uLightDir), -lightDirection);
    float inten = clamp((angle - uSpotOuter) / max(uSpotInner - uSpotOuter, 1e-5), 0.0f, 1.0f);
    return (texture(diffuse0, texCoord) * (diffuse * inten + ambient) + texture(specular0, texCoord).r * specular * inten) * lightColor;
}


void main()
{
    vec4 col;
    if (uLightType == 0)      col = direcLight();
    else if (uLightType == 1) col = pointLight();
    else                      col = spotLight();
    col.rgb *= uTint;
    float g = dot(col.rgb, vec3(0.299, 0.587, 0.114));
    col.rgb = mix(col.rgb, vec3(g), clamp(uGrayAmount, 0.0, 1.0));
    if (uToonLevels >= 1.5) {
        float L = max(1.0, uToonLevels);
        col.rgb = floor(col.rgb * L) / L;
    }
    FragColor = col;
}

