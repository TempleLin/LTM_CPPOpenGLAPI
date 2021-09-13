#version 330 core
out vec4 FragColor;
  
struct Colors{
    vec3 objectColor;
    vec3 ambientColor;
    vec3 lightColor;
};

struct Strengths{
    float ambientStrength;
    float lightStrength;
    float specularStrength;
    float shininess;
};

uniform vec3 cameraViewPos;
uniform vec3 objectColor;
// @Ambient light is the minimum light around. Making it not completely dark.
uniform vec3 ambientColor;
uniform float ambientStrength;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform float lightStrength;
uniform float objectOpacity;

in vec3 FragPos; // @Position of the fragment(vertex) in world space.
in vec2 TexCoord;
in vec3 Normal;
uniform sampler2D ourTexture;
uniform bool enableTexture;

void main()
{
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos); // @Normalize to prevent diffuse value higher than unit.
    // @The use of max() is for putting all negative result as 0.
    // @The more perpendicular the light and fragment angle, the higher the brightness.
    float diff = max(dot(norm, lightDir), 0.0); 

    vec3 diffuse = diff * lightColor * lightStrength;

    vec3 ambient = ambientColor * ambientStrength;

    float specularStrength = 0.5; //temp

    // @Vector direction from view to fragment.
    vec3 viewDir = normalize(cameraViewPos - FragPos);
    // @Vectoyr direction of reflection from light passing with normal as reflection point.
    vec3 reflectDir = reflect(-lightDir, norm);

    // @Using power, the more center of reflection point, the stronger the specular.
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor * lightStrength;  

    if (enableTexture){
        // @Multiply the texture by color mixes the RGB color and texture.
        FragColor = texture(ourTexture, TexCoord) * vec4(((ambient + diffuse + specular) * objectColor), 1);
    } else {
        FragColor = vec4(((ambient + diffuse + specular) * objectColor), 1);
    }
    FragColor.a = objectOpacity;
}