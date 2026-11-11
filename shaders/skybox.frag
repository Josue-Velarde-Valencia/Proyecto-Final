#version 330 core

in vec3 TexCoords;
out vec4 FragColor;

uniform samplerCube skybox;
uniform samplerCube skybox2;
uniform float mezcla;

void main()
{
    vec4 colorPrincipal = texture(skybox, TexCoords);
    vec4 colorSecundario = texture(skybox2, TexCoords);

    FragColor = mix(colorPrincipal, colorSecundario, clamp(mezcla, 0.0, 1.0));
}
