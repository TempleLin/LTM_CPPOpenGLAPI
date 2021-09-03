#version 330 core
out vec4 FragColor;
  
uniform vec4 objectColor = vec4(0, 0, 0, 1);

/* 
 * @Ambient light is the minimum light around. Making it not completely dark.
 */
uniform vec4 ambientColor = vec4(1, 1, 1, 1);
uniform vec4 ambientStrength = vec4(.1, .1, .1, 1);

in vec2 TexCoord;
uniform sampler2D ourTexture;

void main()
{
    // @Multiply the texture by color mixes the RGB color and texture.
    FragColor = texture(ourTexture, TexCoord) * (ambientColor * objectColor) * ambientStrength;
}