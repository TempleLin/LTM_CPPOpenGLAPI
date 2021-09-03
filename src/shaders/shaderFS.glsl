#version 330 core
out vec4 FragColor;
  
uniform vec4 objectColor;

// @Ambient light is the minimum light around. Making it not completely dark.
uniform vec4 ambientColor;
uniform vec4 ambientStrength;

in vec2 TexCoord;
uniform sampler2D ourTexture;
uniform bool enableTexture;

void main()
{
    if (enableTexture){
        // @Multiply the texture by color mixes the RGB color and texture.
        FragColor = texture(ourTexture, TexCoord) * (ambientColor * objectColor) * ambientStrength;
    } else {
        FragColor = ambientColor * objectColor * ambientStrength;
    }
}