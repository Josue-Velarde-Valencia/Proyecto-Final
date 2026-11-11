#include "Skybox.h"
#include "stb_image.h" // Solo declaraciones. STB_IMAGE_IMPLEMENTATION esta en el main.

#include <cstdio>

Skybox::Skybox() = default;

Skybox::Skybox(const std::vector<std::string>& faceLocations)
    : Skybox()
{
    CargarSkybox(faceLocations);
}

void Skybox::CargarSkybox(const std::vector<std::string>& faceLocations)
{
    if (textureId != 0 || skyVAO != 0)
    {
        printf("Skybox: este objeto ya fue inicializado.\n");
        return;
    }
    if (faceLocations.size() != 6)
    {
        printf("Skybox: se requieren exactamente 6 caras, se recibieron %zu.\n",
               faceLocations.size());
        return;
    }

    // La instancia Shader vive dentro de ESTE objeto Skybox; no se copia
    // ni se destruye un Shader temporal luego de la carga.
    skyShader.CreateFromFiles("shaders/skybox.vert", "shaders/skybox.frag");
    uniformProjection = skyShader.GetProjectionLocation();
    uniformView = skyShader.GetViewLocation();

    skyShader.UseShader();
    GLint programa = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, &programa);
    if (programa == 0 || !glIsProgram(static_cast<GLuint>(programa)))
    {
        printf("Skybox: el programa GLSL no es valido. Revisa skybox.vert y skybox.frag.\n");
        glUseProgram(0);
        return;
    }
    GLint enlazado = GL_FALSE;
    glGetProgramiv(static_cast<GLuint>(programa), GL_LINK_STATUS, &enlazado);
    if (enlazado != GL_TRUE)
    {
        printf("Skybox: fallo el enlace del shader.\n");
        glUseProgram(0);
        return;
    }
    glUseProgram(0);

    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);

    int dimensionCara = -1;
    for (size_t i = 0; i < 6; ++i)
    {
        int w = 0, h = 0, componentes = 0;
        unsigned char* datos = stbi_load(faceLocations[i].c_str(),
                                         &w, &h, &componentes, STBI_rgb_alpha);
        if (!datos)
        {
            printf("Skybox: no se pudo cargar la cara %zu: %s\n",
                   i, faceLocations[i].c_str());
            continue;
        }

        // Una textura cubica incompleta tampoco puede muestrearse correctamente.
        if (w != h || (dimensionCara > 0 && w != dimensionCara))
        {
            printf("Skybox: dimensiones incompatibles en %s (%dx%d). "
                   "Todas las caras deben ser cuadradas e iguales.\n",
                   faceLocations[i].c_str(), w, h);
        }
        if (dimensionCara < 0) dimensionCara = w;

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + static_cast<GLenum>(i),
                     0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, datos);
        stbi_image_free(datos);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    const unsigned int indices[] = {
        0, 1, 2, 2, 1, 3,
        2, 3, 5, 5, 3, 7,
        5, 7, 4, 4, 7, 6,
        4, 6, 0, 0, 6, 1,
        4, 0, 5, 5, 0, 2,
        1, 6, 3, 3, 6, 7
    };
    const GLfloat vertices[] = {
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    indexCount = 36;
    glGenVertexArrays(1, &skyVAO);
    glBindVertexArray(skyVAO);

    glGenBuffers(1, &skyIBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glGenBuffers(1, &skyVBO);
    glBindBuffer(GL_ARRAY_BUFFER, skyVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Skybox::DrawSkybox(glm::mat4 viewMatrix, glm::mat4 projectionMatrix)
{
    DibujarSkybox(viewMatrix, projectionMatrix, textureId, 0.0f);
}

void Skybox::DrawSkyboxMezcla(glm::mat4 viewMatrix, glm::mat4 projectionMatrix,
                              const Skybox& otro, GLfloat mezcla)
{
    DibujarSkybox(viewMatrix, projectionMatrix, otro.textureId, mezcla);
}

void Skybox::DibujarSkybox(glm::mat4 viewMatrix, glm::mat4 projectionMatrix,
                           GLuint texturaOtro, GLfloat mezcla)
{
    if (skyVAO == 0 || textureId == 0)
        return;

    viewMatrix = glm::mat4(glm::mat3(viewMatrix));
    glDepthMask(GL_FALSE);
    skyShader.UseShader();

    GLint programaSkybox = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, &programaSkybox);
    if (programaSkybox == 0)
    {
        static bool avisoMostrado = false;
        if (!avisoMostrado)
        {
            printf("Skybox: no hay programa GLSL activo al dibujar.\n");
            avisoMostrado = true;
        }
        glDepthMask(GL_TRUE);
        return;
    }

    glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Es importante ligar AMBOS samplers de cubemap con sus unidades.
    glUniform1i(glGetUniformLocation(static_cast<GLuint>(programaSkybox), "skybox"), 0);
    glUniform1i(glGetUniformLocation(static_cast<GLuint>(programaSkybox), "skybox2"), 1);
    glUniform1f(glGetUniformLocation(static_cast<GLuint>(programaSkybox), "mezcla"),
                glm::clamp(mezcla, 0.0f, 1.0f));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texturaOtro);

    glBindVertexArray(skyVAO);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    glDepthMask(GL_TRUE);
}
