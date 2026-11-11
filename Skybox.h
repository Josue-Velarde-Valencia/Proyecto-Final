#pragma once

#include <vector>
#include <string>
#include <glew.h>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>

#include "CommonValues.h"
#include "Shader_light.h"

class Skybox
{
public:
    Skybox();
    explicit Skybox(const std::vector<std::string>& faceLocations);

    // IMPORTANTE: carga en la instancia existente. No se debe hacer
    // skybox = Skybox(caras), pues Shader no admite copias seguras.
    void CargarSkybox(const std::vector<std::string>& faceLocations);

    // Evita la copia accidental de IDs GPU, especialmente del Shader.
    Skybox(const Skybox&) = delete;
    Skybox& operator=(const Skybox&) = delete;
    Skybox(Skybox&&) = delete;
    Skybox& operator=(Skybox&&) = delete;

    void DrawSkybox(glm::mat4 viewMatrix, glm::mat4 projectionMatrix);
    void DrawSkyboxMezcla(glm::mat4 viewMatrix, glm::mat4 projectionMatrix,
                          const Skybox& otro, GLfloat mezcla);

private:
    void DibujarSkybox(glm::mat4 viewMatrix, glm::mat4 projectionMatrix,
                       GLuint texturaOtro, GLfloat mezcla);

    Shader skyShader;
    GLuint uniformProjection = 0;
    GLuint uniformView = 0;
    GLuint textureId = 0;
    GLuint skyVAO = 0;
    GLuint skyVBO = 0;
    GLuint skyIBO = 0;
    GLsizei indexCount = 0;
};
