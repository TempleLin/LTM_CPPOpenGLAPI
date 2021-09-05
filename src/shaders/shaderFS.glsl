#version 330 core
out vec4 FragColor;
  
uniform vec4 objectColor;

// @Ambient light is the minimum light around. Making it not completely dark.
uniform vec4 ambientColor;
uniform vec4 ambientStrength;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform float lightStrength;

in vec3 FragPos; // @Position of the fragment(vertex) in world space.
in vec2 TexCoord;
in vec3 Normal;
uniform sampler2D ourTexture;
uniform bool enableTexture;

void main()
{
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec4 diffuse = vec4(diff * lightColor, 1);
    if (enableTexture){
        // @Multiply the texture by color mixes the RGB color and texture.
        FragColor = texture(ourTexture, TexCoord) * (ambientColor * objectColor) * (ambientStrength + diffuse * lightStrength);
    } else {
        FragColor = ambientColor * objectColor * (ambientStrength + (diffuse, 1));
    }
}