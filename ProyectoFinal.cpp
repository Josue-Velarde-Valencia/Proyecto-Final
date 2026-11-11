#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string>
#include <cmath>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cstdlib>

#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>

#include "Window.h"
#include "Mesh_tn.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Model.h"

#include "Skybox.h"
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"

const float toRadians = 3.14159265f / 180.0f;


Window mainWindow;

std::vector<MeshModel*> meshList;
std::vector<MeshModel*> meshListModel;
std::vector<Shader> shaderList;

std::vector<glm::vec3> puntosCircuito;
std::vector<GLfloat> distanciasCircuito;
GLfloat longitudCircuito = 0.0f;
unsigned int indicePista = 0;

Camera camera;

Model Base;
Model Cupula;
Model Propulsor1;
Model Propulsor2;
Model Propulsor3;
Model Propulsor4;
Model Rigby;
Model RigbyBrazo1;
Model RigbyBrazo2;
Model CasaParque;
Model Iglesia;
Model KuromiCuerpo;
Model KuromiPiernaIzq;
Model KuromiPiernaDer;
Model CasaKuromi;
Model Cementerio;
Model CarruselBase;
Model CarruselSoporte;
Model Caballo1;
Model Caballo2;
Model Caballo3;
Model Caballo4;
Model Caballo5;
Model Caballo6;
Model Caballo7;
Model Caballo8;
Model Plataforma;
Model Cinnamoroll;
Model CarroFruta;
Model CasaArbol;   // Casa del Arbol de Hora de Aventura (hecha con primitivas en Blender)

// Personajes de Hora de Aventura (piezas separadas en Blender para animarlos de forma jerarquica)
Model JakeCuerpo, JakeBrazoIzq, JakeBrazoDer, JakePataIzq, JakePataDer, JakeCola;
Model BmoCuerpo, BmoBrazoIzq, BmoBrazoDer, BmoPataIzq, BmoPataDer;
Model GunterCuerpo, GunterAlaIzq, GunterAlaDer;
Model TreeTrunksCuerpo, TreeTrunksPataDelIzq, TreeTrunksPataDelDer, TreeTrunksPataTrasIzq, TreeTrunksPataTrasDer;
Model TreeTrunksOrejaIzq, TreeTrunksOrejaDer, TreeTrunksTrompa, TreeTrunksCola;
Model NeptrCuerpo;
Model Tienda;
Model Bosque1;
Model Bosque2;
Model Bosque3;
Model Sol;
Model FantasmanoCuerpo;
Model FantasmanoBrazo;
Model FantasmanoMano;

Model Halo_M;
Model Ghost_M;
Model ISS_M;
Model ISS_Panel;
Model Cohete;
Model Rocket_M;
Model Beam_M;
Model Spark_M;

Model Asteroide1;
Model Asteroide2;
Model Asteroide3;
Model cuerposatelite;
Model antena;
Model panel1;
Model panel2;

// Modelos adicionales de la carrera de Halo (version 2)
Model CarroPrincipal;
Model CarroGolfCarrera;
Model CarroBR;
Model CarroRosa;

// Portico de salida y arboles de Halo (Kenney Racing Kit y Nature Kit, CC0)
Model PorticoSalidaHalo;
const int CANTIDAD_ARBOLES_HALO = 13;
Model ArbolesHalo[CANTIDAD_ARBOLES_HALO];
const char* ARCHIVOS_ARBOLES_HALO[CANTIDAD_ARBOLES_HALO] = {
    "halo_tree_default", "halo_tree_oak", "halo_tree_fat", "halo_tree_detailed", "halo_tree_tall", "halo_tree_simple",
    "halo_tree_pinetalla_detailed", "halo_tree_pinetallc_detailed", "halo_tree_pinerounda", "halo_tree_pinedefaulta",
    "halo_tree_oak_fall", "halo_tree_default_fall", "halo_tree_fat_fall" };

Skybox skybox;
Skybox skyboxPlaneta2;
Skybox skyboxEspacioProfundo;
Skybox skyboxAtardecer;

Material Material_brillante;
Material Material_opaco;
Material MaterialPista;

DirectionalLight mainLight;
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

Texture FuegoAzulTexture;
Texture FuegoCoheteTexture;
Texture HumoTexture;
Texture pistaTexture;
Texture bordilloTexture;
Texture concretoTexture;
Texture metaTexture;

MeshModel* plasmaMesh;
MeshModel* mallaCoronaSol = nullptr;

// Diametro en unidades del mundo del resplandor que rodea al Sol.
// El modelo Sol.obj sigue teniendo su escala original x50.
const GLfloat DIAMETRO_CORONA_SOL = 260.0f;


GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

bool carruselActivo = false;
bool teclaCarruselAnterior = false;
GLfloat anguloCarrusel = 0.0f;
GLfloat tiempoCarrusel = 0.0f;
GLfloat tiempoCambioColor = 0.0f;
unsigned int indiceColorCarrusel = 0;

GLfloat tiempoKuromi = 0.0f;
GLfloat velocidadCaminata = 3.0f;
GLfloat velocidadAvance = 0.05f;
GLfloat distanciaX = 0.55f;
GLfloat distanciaZ = 0.33f;

GLfloat anguloHombroRigby = 0.0f;
GLfloat direccionHombroRigby = 1.0f;
GLfloat anguloCodoRigby = 0.0f;
GLfloat direccionCodoRigby = 1.0f;

glm::vec3 posicionFantasmano(-1.0f, 0.7f, 0.0f);
GLfloat direccionFantasmano = 0.0f;
GLfloat anguloCamaraFantasmano = -90.0f;
GLfloat inclinacionCamaraFantasmano = -18.0f;
GLfloat tiempoFantasmano = 0.0f;
GLfloat anguloBrazoFantasmano = 0.0f;
GLfloat anguloManoFantasmano = 0.0f;
GLfloat velocidadFantasmano = 0.18f;

// SISTEMA DE CAMARAS
int modoCamara = 0; // 0 = Fantasmano, 1 = Rocket, 2 = Halo, 3 = Carrera Halo, 4 = Observacion (cohete y estacion), 5 = Seguimiento del cohete
bool teclaMAnterior = false;
bool teclaPAnterior = false;

// Cohete de lanzamiento (plataforma del parque)
// 0 = en la plataforma, 1 = cuenta regresiva, 2 = ascenso, 3 = fuera del parque
int estadoCohete = 0;
bool tecla1Anterior = false;
GLfloat tiempoCohete = 0.0f;

// Paneles solares de la estacion espacial (tecla 2)
bool panelesDesplegados = true;
bool tecla2Anterior = false;
GLfloat factorPanelesISS = 1.0f;

// CAMARA 2: NAVE ESPACIAL

// Posicion inicial de la nave
glm::vec3 posicionRocket(-1250.0f, 0.0f, 400.0f);

// Orientacion de vuelo
GLfloat anguloCamaraRocket = -90.0f;
GLfloat inclinacionCamaraRocket = -8.0f;

// Velocidad de vuelo
GLfloat velocidadRocket = 50.0f;
GLfloat multiplicadorTurboRocket = 3.0f;

// Camara en tercera persona
GLfloat distanciaCamaraRocket = 30.0f;
GLfloat alturaCamaraRocket = 8.0f;

// Apariencia del modelo
GLfloat escalaRocket = 3.0f;

// Ajustes por si el modelo apunta hacia otro eje 
GLfloat ajusteRotacionRocketX = -90.0f;
GLfloat ajusteRotacionRocketY = 0.0f;

// Plasma Rocket: coordenadas locales del modelo (el frente original es +Y)
const glm::vec3 POSICION_PLASMA_ROCKET(0.0f, -2.0f, 0.0f);
const glm::vec3 ESCALA_PLASMA_ROCKET(0.50f, 1.20f, 0.50f);


const glm::vec3 ORIGEN_HALO(-2500.0f, 0.0f, 0.0f);
const glm::vec3 ORIGEN_ISS(-1250.0f, 0.0f, 0.0f);
// Una sola luz de identificacion en la ISS: rojo, verde y blanco, cada 3 segundos.
// Se mezcla durante el ultimo 25 % de cada intervalo para evitar destellos bruscos.
const GLfloat SEGUNDOS_COLOR_LUZ_ISS = 3.0f;
const glm::vec3 COLORES_LUZ_ISS[3] = {
    glm::vec3(1.0f, 0.15f, 0.12f),  // Rojo
    glm::vec3(0.20f, 1.0f, 0.30f),  // Verde
    glm::vec3(1.0f, 1.0f, 1.0f)    // Blanco
};
// Centro geometrico entre SpacePark (X=0) y Halo (X=-2500).
// Coincide con la posicion de la ISS, sin modificar la estacion.
const glm::vec3 CENTRO_ORBITA_SOL(-1250.0f, 0.0f, 0.0f);

// Beam en Halo y reflector azul de piso. Coordenadas locales respecto a ORIGEN_HALO.
// Ajusta estas tres posiciones si quieres mover el reflector o cambiar su punteria.
const glm::vec3 POSICION_BEAM_HALO(70.0f, -5.0f, 0.0f);
const glm::vec3 POSICION_REFLECTOR_BEAM_HALO(58.0f, 0.8f, -12.0f);
const glm::vec3 OBJETIVO_REFLECTOR_BEAM_HALO(70.0f, 12.0f, 0.0f);

// Spark: posicion local en Halo y orientacion de su cara frontal.
// La direccion de mirada usa +Z como frente por defecto; si el modelo
// fue exportado mirando hacia otro eje, modifica solo este angulo.
const glm::vec3 POSICION_SPARK_HALO = POSICION_BEAM_HALO + glm::vec3(-10.0f, 8.0f, 5.0f);
const GLfloat AJUSTE_FRENTE_SPARK_GRADOS = 0.0f;
// Luz celeste emitida desde el cuerpo del personaje (offset vertical local).
const glm::vec3 DESPLAZAMIENTO_LUZ_SPARK_HALO(0.0f, 1.0f, 0.0f);

// Posicion del personaje relativa al origen de Halo
glm::vec3 posicionFantasmanoHalo(40.0f, 2.8f, 0.0f);

GLfloat direccionFantasmanoHalo = 0.0f;
GLfloat anguloCamaraHalo = -90.0f;
GLfloat inclinacionCamaraHalo = -18.0f;

// Configuracion independiente
GLfloat factorVelocidadHalo = 1.0f;
GLfloat distanciaCamaraHalo = 32.0f;
GLfloat alturaCamaraHalo = 2.0f;

// Ajustes visuales del personaje
GLfloat escalaPersonajeHalo = 0.35f;
GLfloat alturaPersonajeHalo = 0.0f;

// MODELO PROVISIONAL
// Cambia esta referencia cuando tengas otro personaje
Model* modeloPersonajeHalo = &FantasmanoCuerpo;


// Hora de Aventura: Casa del Arbol (posicion de referencia en unidades del parque; el frente mira hacia +Z)
const glm::vec3 POSICION_CASA_ARBOL(-0.22f, 0.66f, -0.16f);
const GLfloat ESCALA_CASA_ARBOL = 0.011f;
// La plaza de Hora de Aventura esta en Halo, en la explanada junto al circuito (posicion relativa a ORIGEN_HALO, en unidades de Halo)
const glm::vec3 CASA_ARBOL_HALO(60.0f, 0.0f, -60.0f);

// Hora de Aventura: personajes afuera de la Casa del Arbol (posiciones en el parque)
GLfloat tiempoHoraAventura = 0.0f;
GLfloat anguloRecorridoJake = 0.0f;
const GLfloat velocidadRecorridoJake = 0.45f;
const GLfloat radioRecorridoJake = 0.06f;
const GLfloat distanciaSaludo = 0.16f;                                   // cerca de Fantasmano los personajes reaccionan
const glm::vec3 centroHoraAventura(-0.22f, 0.66f, 0.05f);                // Jake camina en circulo alrededor de este punto
const glm::vec3 posicionBmo(-0.31f, 0.66f, -0.03f);
const glm::vec3 posicionGunter(-0.13f, 0.66f, -0.03f);
const glm::vec3 posicionTreeTrunks(-0.32f, 0.66f, 0.14f);
const glm::vec3 posicionNeptr(-0.12f, 0.66f, 0.14f);
bool baileBmoActivo = false;                                             // tecla 3
bool tecla3Anterior = false;

// Articulaciones: cada pieza esta centrada en su articulacion y gira alrededor de ella
const glm::vec3 pivoteBrazoIzqJake(-0.945f, 1.784f, 0.062f);
const glm::vec3 pivoteBrazoDerJake(0.945f, 1.784f, 0.062f);
const glm::vec3 pivotePataIzqJake(-0.157f, 1.094f, 0.043f);
const glm::vec3 pivotePataDerJake(0.157f, 1.094f, 0.043f);
const glm::vec3 pivoteColaJake(0.0f, 0.937f, -1.538f);
const glm::vec3 pivoteBrazoIzqBmo(-0.382f, 1.266f, -0.01f);
const glm::vec3 pivoteBrazoDerBmo(0.38f, 1.282f, -0.01f);
const glm::vec3 pivotePataIzqBmo(-0.009f, 1.284f, -0.001f);
const glm::vec3 pivotePataDerBmo(0.024f, 1.264f, -0.004f);
const glm::vec3 pivoteAlaIzqGunter(-0.4f, 0.887f, -0.013f);
const glm::vec3 pivoteAlaDerGunter(0.406f, 0.934f, 0.008f);
const glm::vec3 pivotePataDelIzqTreeTrunks(-0.482f, 0.773f, -0.225f);
const glm::vec3 pivotePataDelDerTreeTrunks(0.482f, 0.773f, -0.225f);
const glm::vec3 pivotePataTrasIzqTreeTrunks(-0.482f, 0.773f, -0.86f);
const glm::vec3 pivotePataTrasDerTreeTrunks(0.482f, 0.773f, -0.86f);
const glm::vec3 pivoteOrejaIzqTreeTrunks(-0.79f, 1.803f, -0.236f);
const glm::vec3 pivoteOrejaDerTreeTrunks(0.79f, 1.803f, -0.236f);
const glm::vec3 pivoteTrompaTreeTrunks(0.0f, 1.222f, 0.419f);
const glm::vec3 pivoteColaTreeTrunks(0.0f, 0.196f, -1.588f);

// Variables de Luces (Teclado)
bool lucesSpotTeclado = true;
bool teclaLucesAnterior = false;
// Reflector azul del modelo Beam_M. Tecla H: alterna encendido/apagado.
bool reflectorBeamEncendido = true;
bool teclaHAnterior = false;
GLfloat anguloCicloDia = 0.0f;
// Estado del ciclo solar: true cuando el Sol esta bajo el horizonte.
bool esDeNoche = false;

const glm::vec3 COLORES_CARRUSEL[6] = {
    glm::vec3(1.0f, 0.0f, 0.0f),
    glm::vec3(1.0f, 1.0f, 0.0f),
    glm::vec3(0.0f, 1.0f, 0.0f),
    glm::vec3(0.0f, 1.0f, 1.0f),
    glm::vec3(0.0f, 0.0f, 1.0f),
    glm::vec3(1.0f, 0.0f, 1.0f)
};

// ============================================================
// ESTACION ESPACIAL Y COHETE: ruta, estado y constantes
// ============================================================
// Ruta de la estacion espacial: orbita circular inclinada alrededor del centro entre los dos planetas.
// El cohete sigue esta misma ruta despues de salir del parque.
const GLfloat RADIO_RUTA_ISS = 260.0f;
const GLfloat INCLINACION_RUTA_ISS = 18.0f * toRadians;
const GLfloat VELOCIDAD_RUTA_ISS = 0.05f;
GLfloat anguloRutaISS = 0.0f;

// Vuelo del cohete: despega, cruza hasta la ruta de la ISS, da una vuelta y regresa a la plataforma.
// La trayectoria se calcula al lanzar (curvas de Hermite y un circulo) y se recorre por longitud de arco.
const GLfloat DURACION_CUENTA_REGRESIVA = 4.0f;
const GLfloat INSTANTE_ENCENDIDO = 1.2f;
const GLfloat VELOCIDAD_MAXIMA_ASCENSO = 170.0f;
const GLfloat ACELERACION_ASCENSO = 28.0f;
const GLfloat VELOCIDAD_ANGULAR_COHETE = 0.2f;
const GLfloat VELOCIDAD_MAXIMA_REGRESO = 130.0f;
const GLfloat DISTANCIA_VOLTEO_COHETE = 420.0f;   // distancia a la plataforma a la que empieza el volteo
// Trayectoria y avance del vuelo
std::vector<glm::vec3> trayectoriaCohete;
std::vector<GLfloat> distanciasTrayectoriaCohete;
GLfloat distanciaFinAscenso = 0.0f;
GLfloat distanciaFinVuelta = 0.0f;
GLfloat distanciaRecorridaCohete = 0.0f;
GLfloat velocidadVueloCohete = 0.0f;
GLfloat tiempoRegreso = 0.0f;
// Posicion y orientacion actuales (las usan las camaras 4 y 5, las luces y el dibujo)
glm::vec3 posicionCoheteMundo(0.0f);
glm::vec3 direccionVueloCohete(0.0f, 1.0f, 0.0f);
glm::vec3 ejeDerechaCohete(1.0f, 0.0f, 0.0f);
glm::vec3 direccionHorizontalSeguimiento(0.0f, 0.0f, 1.0f);
glm::mat4 orientacionCohete(1.0f);
// Fuego de los motores y temblor
GLfloat potenciaFuegoCohete = 0.0f;
GLfloat intensidadTemblorCohete = 0.0f;

// Trafico de naves de la estacion: llegan desde un planeta, se acoplan al puerto, salen hacia el otro y vuelven
const glm::vec3 PUERTO_ACOPLAMIENTO_LOCAL = glm::vec3(4.2f, -7.5f, 22.8f) * 3.0f;   // punta de la viga, en el marco de la estacion
const GLfloat DURACION_LLEGADA_NAVE = 12.0f;
const GLfloat DURACION_ACOPLADA_NAVE = 10.0f;
const GLfloat DURACION_SALIDA_NAVE = 12.0f;
const int CANTIDAD_NAVES_TRAFICO = 3;
// Las naves se turnan: cada una ocupa el puerto durante su visita y las demas esperan lejos (sin chocar entre si)
const GLfloat DURACION_VISITA_NAVE = DURACION_LLEGADA_NAVE + DURACION_ACOPLADA_NAVE + DURACION_SALIDA_NAVE;
struct NaveTrafico
{
    Model* modelo;
    GLfloat escala;
    GLfloat holgura;       // distancia del puerto al centro de la nave cuando esta acoplada
    GLfloat desfase;       // DURACION_VISITA_NAVE * (CANTIDAD_NAVES_TRAFICO - posicion en el turno)
    GLfloat ladoLlegada;   // +1 llega desde el parque y sale hacia Halo; -1 al reves
};
struct EstadoNaveTrafico
{
    bool visible;
    int etapa;             // 0 llegando, 1 acoplada, 2 saliendo
    glm::vec3 posicion;
    glm::mat4 orientacion; // columnas: derecha, arriba, frente
};
// Datos de referencia para la camara de observacion (se actualizan cada cuadro)
glm::vec3 puertoAcoplamientoMundo(0.0f);
glm::vec3 salidaAcoplamientoMundo(0.0f, 0.0f, 1.0f);
glm::vec3 lateralEstacionMundo(1.0f, 0.0f, 0.0f);
glm::vec3 objetivoObservacionEstacion(0.0f);

// Humo de los motores
struct ParticulaHumoCohete
{
    glm::vec3 posicion;
    glm::vec3 velocidad;
    GLfloat nacimiento;
    GLfloat vida;
};
std::vector<ParticulaHumoCohete> humoCohete;
GLfloat acumuladoHumoCohete = 0.0f;

// ============================================================
// DECORADO DE LA PISTA DE HALO (Kenney Racing Kit, CC0): piezas y posiciones
// ============================================================
enum PiezaKitId
{
    KIT_POSICIONES_SALIDA, KIT_MURO, KIT_VALLA, KIT_GRADA_TECHADA, KIT_GRADA_TOLDO,
    KIT_BOXES, KIT_BOXES_CERRADOS, KIT_OFICINA, KIT_CARPA_LARGA, KIT_CARPA_CERRADA,
    KIT_FAROLA, KIT_TORRE_BANDERA, KIT_BANDERA_META, KIT_BANDERA_ROJA, KIT_CANTIDAD
};
struct DatosKit
{
    const char* archivo;
    GLfloat desplazaX, desplazaZ;   // para centrar la huella del modelo (su origen esta en una esquina)
};
const DatosKit DATOS_KIT[KIT_CANTIDAD] = {
    { "roadStartPositions", 0.5f, -1.0f }, { "barrierWall", 0.5f, -0.06f }, { "fenceStraight", 0.5f, -0.015f },
    { "grandStandCovered", 0.5f, -0.5f }, { "grandStandAwning", 0.5f, -0.5f },
    { "pitsGarage", 0.5f, -0.455f }, { "pitsGarageClosed", 0.5f, -0.455f }, { "pitsOffice", 0.5f, -0.48f },
    { "tentLong", 1.0f, -0.5f }, { "tentClosedLong", 1.0f, -0.5f },
    { "lightPostLarge", 0.0f, 0.03f }, { "bannerTowerRed", 0.195f, -0.195f },
    { "flagCheckers", 0.1f, -0.02f }, { "flagRed", 0.1f, -0.02f }
};
Model PiezasKit[KIT_CANTIDAD];

struct ColocacionKit
{
    int id;
    GLfloat fase, lateral, giro;   // distancia sobre el circuito, desplazamiento lateral y giro en grados
    glm::vec3 escala;
    GLfloat altura;
};
std::vector<ColocacionKit> decoradoHalo;
unsigned int indiceBordillos = 0, indiceFaldones = 0, indiceMeta = 0;
bool decoradoPistaListo = false;

// Fase (distancia sobre el circuito) del portico de salida y la linea de meta
const GLfloat FASE_META_HALO = 14.0f;
// Farolas del circuito: siete, alternando lados
const int CANTIDAD_FAROLAS_HALO = 7;

static const char* vShader = "shaders/shader_light.vert";
static const char* fShader = "shaders/shader_light.frag";

void CreateShaders()
{
    Shader* shader1 = new Shader();
    shader1->CreateFromFiles(vShader, fShader);
    shaderList.push_back(*shader1);
}

void CreatePlasmaMesh()
{
    unsigned int indices[] = {
        0, 1, 2,  0, 2, 3,
        4, 5, 6,  4, 6, 7
    };
    GLfloat vertices[] = {
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f,   0.0f, 0.0f, 1.0f,
         0.5f, -0.5f, 0.0f,   1.0f, 0.0f,   0.0f, 0.0f, 1.0f,
         0.5f,  0.5f, 0.0f,   1.0f, 1.0f,   0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f, 0.0f,   0.0f, 1.0f,   0.0f, 0.0f, 1.0f,

         0.0f, -0.5f, -0.5f,  0.0f, 0.0f,   1.0f, 0.0f, 0.0f,
         0.0f, -0.5f,  0.5f,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f,
         0.0f,  0.5f,  0.5f,  1.0f, 1.0f,   1.0f, 0.0f, 0.0f,
         0.0f,  0.5f, -0.5f,  0.0f, 1.0f,   1.0f, 0.0f, 0.0f
    };
    plasmaMesh = new MeshModel();
    plasmaMesh->CreateMeshModel(vertices, indices, 64, 12);
}

// Cuadrado plano con UV: el shader crea el degradado radial de la corona.
// Se genera una sola vez y se orienta hacia la camara en cada fotograma.
void CrearMallaCoronaSol()
{
    unsigned int indices[] = { 0, 1, 2, 0, 2, 3 };
    GLfloat vertices[] = {
        // Posicion XYZ     UV      Normal XYZ
        -0.5f, -0.5f, 0.0f,  0.0f, 0.0f,  0.0f, 0.0f, 1.0f,
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f,  0.0f, 0.0f, 1.0f,
         0.5f,  0.5f, 0.0f,  1.0f, 1.0f,  0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f, 0.0f,  0.0f, 1.0f,  0.0f, 0.0f, 1.0f
    };
    mallaCoronaSol = new MeshModel();
    mallaCoronaSol->CreateMeshModel(vertices, indices, 32, 6);
}

glm::mat4 CrearMatrizSpacePark(const char* ruta)
{
    std::ifstream archivo(ruta);
    std::string linea;
    GLfloat minX = 0.0f, maxX = 0.0f;
    GLfloat minZ = 0.0f, maxZ = 0.0f, maxY = 0.0f;
    bool primero = true;

    while (std::getline(archivo, linea))
    {
        if (linea.compare(0, 2, "v ") != 0) continue;
        std::istringstream datos(linea.substr(2));
        GLfloat x, y, z;
        if (!(datos >> x >> y >> z)) continue;

        if (primero)
        {
            minX = maxX = x;
            minZ = maxZ = z;
            maxY = y;
            primero = false;
        }
        else
        {
            minX = std::min(minX, x);
            maxX = std::max(maxX, x);
            minZ = std::min(minZ, z);
            maxZ = std::max(maxZ, z);
            maxY = std::max(maxY, y);
        }
    }

    GLfloat dimension = std::max(maxX - minX, maxZ - minZ);
    if (primero || dimension <= 0.0f)
    {
        printf("No se pudieron obtener las dimensiones de %s\n", ruta);
        return glm::mat4(1.0f);
    }

    GLfloat escala = 600.0f / dimension;
    glm::mat4 matriz = glm::mat4(1.0f);
    matriz = glm::translate(matriz, glm::vec3(0.0f, -1.0f, 0.0f));
    matriz = glm::scale(matriz, glm::vec3(escala));
    matriz = glm::translate(matriz, glm::vec3(-(minX + maxX) * 0.5f, -maxY, -(minZ + maxZ) * 0.5f));
    return matriz;
}

void CrearPista()
{
    const GLfloat ANCHO_CIRCUITO = 10.0f;
    const GLfloat ALTURA_BAJA = 2.0f;
    const GLfloat ALTURA_ALTA = 14.0f;

    const glm::vec2 controlPista[][4] = {
        { {154.0f,306.0f}, {240.0f,306.0f}, {320.0f,306.0f}, {399.0f,306.0f} },
        { {399.0f,306.0f}, {428.0f,306.0f}, {445.0f,290.0f}, {445.0f,265.0f} },
        { {445.0f,265.0f}, {445.0f,234.0f}, {435.0f,211.0f}, {399.0f,211.0f} },
        { {399.0f,211.0f}, {335.0f,211.0f}, {270.0f,211.0f}, {214.0f,211.0f} },
        { {214.0f,211.0f}, {192.0f,211.0f}, {181.0f,213.0f}, {181.0f,183.0f} },
        { {181.0f,183.0f}, {181.0f,163.0f}, {184.0f,150.0f}, {203.0f,146.0f} },
        { {203.0f,146.0f}, {270.0f,125.0f}, {338.0f,103.0f}, {402.0f,84.0f} },
        { {402.0f,84.0f}, {420.0f,79.0f}, {430.0f,73.0f}, {429.0f,56.0f} },
        { {429.0f,56.0f}, {428.0f,42.0f}, {419.0f,36.0f}, {396.0f,36.0f} },
        { {396.0f,36.0f}, {303.0f,36.0f}, {212.0f,36.0f}, {120.0f,36.0f} },
        { {120.0f,36.0f}, {72.0f,36.0f}, {49.0f,63.0f}, {49.0f,110.0f} },
        { {49.0f,110.0f}, {49.0f,154.0f}, {49.0f,196.0f}, {49.0f,240.0f} },
        { {49.0f,240.0f}, {49.0f,285.0f}, {75.0f,306.0f}, {122.0f,306.0f} },
        { {122.0f,306.0f}, {132.0f,306.0f}, {143.0f,306.0f}, {154.0f,306.0f} }
    };

    puntosCircuito.clear();
    const unsigned int MUESTRAS_POR_TRAMO = 64;
    for (unsigned int tramo = 0; tramo < 14; tramo++)
    {
        for (unsigned int i = 0; i < MUESTRAS_POR_TRAMO; i++)
        {
            GLfloat u = (GLfloat)i / MUESTRAS_POR_TRAMO;
            GLfloat v = 1.0f - u;
            glm::vec2 punto = v * v * v * controlPista[tramo][0]
                + 3.0f * v * v * u * controlPista[tramo][1]
                + 3.0f * v * u * u * controlPista[tramo][2]
                + u * u * u * controlPista[tramo][3];

            GLfloat altura = ALTURA_BAJA;
            if (tramo == 0)
            {
                GLfloat t = glm::clamp((u - 0.15f) / 0.70f, 0.0f, 1.0f);
                t = t * t * (3.0f - 2.0f * t);
                altura = ALTURA_BAJA + (ALTURA_ALTA - ALTURA_BAJA) * t;
            }
            else if (tramo >= 1 && tramo <= 10) altura = ALTURA_ALTA;
            else if (tramo == 11)
            {
                GLfloat t = glm::clamp((u - 0.15f) / 0.70f, 0.0f, 1.0f);
                t = t * t * (3.0f - 2.0f * t);
                altura = ALTURA_ALTA - (ALTURA_ALTA - ALTURA_BAJA) * t;
            }
            puntosCircuito.push_back(glm::vec3((punto.x - 240.0f) * 0.55f,
                altura, (punto.y - 170.0f) * 0.55f));
        }
    }

    distanciasCircuito.clear();
    distanciasCircuito.push_back(0.0f);
    for (unsigned int i = 0; i < puntosCircuito.size(); i++)
    {
        distanciasCircuito.push_back(distanciasCircuito.back() + glm::length(
            puntosCircuito[(i + 1) % puntosCircuito.size()] - puntosCircuito[i]));
    }
    longitudCircuito = distanciasCircuito.back();

    std::vector<GLfloat> verticesCircuito;
    std::vector<unsigned int> indicesCircuito;
    for (unsigned int i = 0; i <= puntosCircuito.size(); i++)
    {
        unsigned int k = i % (unsigned int)puntosCircuito.size();
        glm::vec3 p = puntosCircuito[k];
        glm::vec3 tangente = glm::normalize(puntosCircuito[(k + 1) % puntosCircuito.size()] -
            puntosCircuito[(k + puntosCircuito.size() - 1) % puntosCircuito.size()]);
        glm::vec3 lateral = glm::normalize(glm::vec3(tangente.z, 0.0f, -tangente.x));
        glm::vec3 normal = glm::normalize(glm::cross(tangente, lateral));
        for (unsigned int j = 0; j < 2; j++)
        {
            glm::vec3 borde = p + lateral * (j == 0 ? ANCHO_CIRCUITO / 2.0f : -ANCHO_CIRCUITO / 2.0f);
            verticesCircuito.insert(verticesCircuito.end(), {
                borde.x, borde.y, borde.z,
                (GLfloat)j, (GLfloat)i / 8.0f,
                normal.x, normal.y, normal.z });
        }
        if (i < puntosCircuito.size())
        {
            unsigned int base = i * 2;
            indicesCircuito.insert(indicesCircuito.end(), { base, base + 1, base + 2,
                base + 1, base + 3, base + 2 });
        }
    }
    MeshModel* pista = new MeshModel();
    pista->CreateMeshModel(verticesCircuito.data(), indicesCircuito.data(),
        (unsigned int)verticesCircuito.size(), (unsigned int)indicesCircuito.size());
    indicePista = (unsigned int)meshList.size();
    meshList.push_back(pista);
}

glm::mat4 MoverNave(GLfloat faseNave, GLfloat desplazamientoLateral, const glm::mat4& matrizPista)
{
    if (puntosCircuito.size() < 2 || distanciasCircuito.size() != puntosCircuito.size() + 1 || longitudCircuito <= 0.0f)
        return matrizPista;

    faseNave = fmod(faseNave, longitudCircuito);
    if (faseNave < 0.0f) faseNave += longitudCircuito;

    unsigned int indiceNave = 0;
    while (indiceNave + 1 < puntosCircuito.size() && faseNave >= distanciasCircuito[indiceNave + 1])
        indiceNave++;

    GLfloat distanciaSegmento = distanciasCircuito[indiceNave + 1] - distanciasCircuito[indiceNave];
    GLfloat uNave = 0.0f;
    if (distanciaSegmento > 0.0001f)
        uNave = (faseNave - distanciasCircuito[indiceNave]) / distanciaSegmento;

    glm::vec3 puntoInicial = puntosCircuito[indiceNave];
    glm::vec3 puntoFinal = puntosCircuito[(indiceNave + 1) % puntosCircuito.size()];
    glm::vec3 posicionNave = glm::mix(puntoInicial, puntoFinal, uNave);

    glm::vec3 anterior = puntosCircuito[(indiceNave + puntosCircuito.size() - 1) % puntosCircuito.size()];
    glm::vec3 siguiente = puntosCircuito[(indiceNave + 1) % puntosCircuito.size()];
    glm::vec3 avanceNave = siguiente - anterior;
    if (glm::length(avanceNave) < 0.0001f) avanceNave = puntoFinal - puntoInicial;
    avanceNave = glm::normalize(avanceNave);

    glm::vec3 derechaNave = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), avanceNave));
    glm::vec3 arribaNave = glm::normalize(glm::cross(avanceNave, derechaNave));

    posicionNave += derechaNave * desplazamientoLateral;

    glm::mat4 orientacionNave(1.0f);
    orientacionNave[0] = glm::vec4(derechaNave, 0.0f);
    orientacionNave[1] = glm::vec4(arribaNave, 0.0f);
    orientacionNave[2] = glm::vec4(avanceNave, 0.0f);

    glm::mat4 matrizNave = matrizPista;
    matrizNave = glm::translate(matrizNave, posicionNave + glm::vec3(0.0f, 9.0f, 0.0f));
    matrizNave = matrizNave * orientacionNave;

    return matrizNave;
}

// ============================================================
// ESTACION ESPACIAL Y COHETE: funciones
// ============================================================
glm::vec3 PosicionRutaISS(GLfloat angulo)
{
    GLfloat x = RADIO_RUTA_ISS * cos(angulo);
    GLfloat z = RADIO_RUTA_ISS * sin(angulo);
    return ORIGEN_ISS + glm::vec3(x, z * sin(INCLINACION_RUTA_ISS), z * cos(INCLINACION_RUTA_ISS));
}

glm::vec3 TangenteRutaISS(GLfloat angulo)
{
    return glm::normalize(glm::vec3(-sin(angulo), cos(angulo) * sin(INCLINACION_RUTA_ISS), cos(angulo) * cos(INCLINACION_RUTA_ISS)));
}

GLfloat SuavizarPaso(GLfloat x)
{
    x = glm::clamp(x, 0.0f, 1.0f);
    return x * x * (3.0f - 2.0f * x);
}

GLfloat AleatorioCohete(GLfloat minimo, GLfloat maximo)
{
    return minimo + (maximo - minimo) * (rand() / (GLfloat)RAND_MAX);
}

// Movimiento irregular: suma de varias senoidales con frecuencias que no coinciden (no se nota el patron)
GLfloat RuidoOrganico(GLfloat t, GLfloat semilla)
{
    return 0.50f * sin(t * 11.3f + semilla)
        + 0.30f * sin(t * 19.7f + semilla * 1.7f + 0.8f * sin(t * 2.3f))
        + 0.20f * sin(t * 37.1f + semilla * 0.6f);
}

glm::vec3 CurvaHermite(const glm::vec3& p0, const glm::vec3& t0, const glm::vec3& p1, const glm::vec3& t1, GLfloat u)
{
    GLfloat u2 = u * u, u3 = u2 * u;
    return (2.0f * u3 - 3.0f * u2 + 1.0f) * p0 + (u3 - 2.0f * u2 + u) * t0
        + (-2.0f * u3 + 3.0f * u2) * p1 + (u3 - u2) * t1;
}

// Ascenso y cruce hacia la ruta, una vuelta por la ruta y regreso a la plataforma
void CrearTrayectoriaCohete(const glm::vec3& posicionPlataforma)
{
    trayectoriaCohete.clear();
    // Punto de entrada a la ruta: un poco detras de donde estara la estacion cuando llegue el cohete
    GLfloat anguloEntrada = anguloRutaISS + VELOCIDAD_RUTA_ISS * 16.0f - 0.35f;
    glm::vec3 puntoEntrada = PosicionRutaISS(anguloEntrada);
    glm::vec3 tangenteEntrada = TangenteRutaISS(anguloEntrada);
    GLfloat distanciaDirecta = glm::length(puntoEntrada - posicionPlataforma);
    const int MUESTRAS_CRUCE = 300, MUESTRAS_VUELTA = 720, MUESTRAS_REGRESO = 300;

    for (int i = 0; i <= MUESTRAS_CRUCE; i++)
        trayectoriaCohete.push_back(CurvaHermite(posicionPlataforma, glm::vec3(0.0f, 0.8f * distanciaDirecta, 0.0f),
            puntoEntrada, tangenteEntrada * 0.9f * distanciaDirecta, (GLfloat)i / MUESTRAS_CRUCE));
    for (int i = 1; i <= MUESTRAS_VUELTA; i++)
        trayectoriaCohete.push_back(PosicionRutaISS(anguloEntrada + 6.2831853f * i / MUESTRAS_VUELTA));
    for (int i = 1; i <= MUESTRAS_REGRESO; i++)
        trayectoriaCohete.push_back(CurvaHermite(puntoEntrada, tangenteEntrada * 0.9f * distanciaDirecta,
            posicionPlataforma, glm::vec3(0.0f, -0.8f * distanciaDirecta, 0.0f), (GLfloat)i / MUESTRAS_REGRESO));

    distanciasTrayectoriaCohete.assign(trayectoriaCohete.size(), 0.0f);
    for (size_t i = 1; i < trayectoriaCohete.size(); i++)
        distanciasTrayectoriaCohete[i] = distanciasTrayectoriaCohete[i - 1] + glm::length(trayectoriaCohete[i] - trayectoriaCohete[i - 1]);
    distanciaFinAscenso = distanciasTrayectoriaCohete[MUESTRAS_CRUCE];
    distanciaFinVuelta = distanciasTrayectoriaCohete[MUESTRAS_CRUCE + MUESTRAS_VUELTA];
}

glm::vec3 PuntoTrayectoriaCohete(GLfloat distancia, glm::vec3& tangente)
{
    const size_t n = trayectoriaCohete.size();
    size_t i = std::upper_bound(distanciasTrayectoriaCohete.begin(), distanciasTrayectoriaCohete.end(), distancia) - distanciasTrayectoriaCohete.begin();
    if (i < 1) i = 1;
    if (i > n - 1) i = n - 1;
    GLfloat tramo = distanciasTrayectoriaCohete[i] - distanciasTrayectoriaCohete[i - 1];
    GLfloat u = (tramo > 0.0001f) ? glm::clamp((distancia - distanciasTrayectoriaCohete[i - 1]) / tramo, 0.0f, 1.0f) : 0.0f;
    glm::vec3 direccion = trayectoriaCohete[std::min(i + 1, n - 1)] - trayectoriaCohete[(i >= 2) ? i - 2 : 0];
    tangente = (glm::length(direccion) > 0.0001f) ? glm::normalize(direccion) : glm::vec3(0.0f, 1.0f, 0.0f);
    return glm::mix(trayectoriaCohete[i - 1], trayectoriaCohete[i], u);
}

glm::vec3 BezierCubica(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, GLfloat u)
{
    const GLfloat v = 1.0f - u;
    return v * v * v * p0 + 3.0f * v * v * u * p1 + 3.0f * v * u * u * p2 + u * u * u * p3;
}

glm::vec3 DerivadaBezierCubica(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, GLfloat u)
{
    const GLfloat v = 1.0f - u;
    return 3.0f * v * v * (p1 - p0) + 6.0f * v * u * (p2 - p1) + 3.0f * u * u * (p3 - p2);
}

// Una nave del trafico en el instante dado. Ciclo continuo: llegada (curva suave que termina de frente al puerto),
// acoplada (fija a la estacion, que sigue girando), salida (retrocede, da la vuelta y se va hacia el otro planeta);
// el resto del ciclo es el turno de las otras naves y esta no se dibuja.
EstadoNaveTrafico CalcularNaveTrafico(const NaveTrafico& nave, GLfloat tiempo, const glm::mat4& matrizISS)
{
    EstadoNaveTrafico estado = { false, 0, glm::vec3(0.0f), glm::mat4(1.0f) };
    const GLfloat ciclo = DURACION_VISITA_NAVE * CANTIDAD_NAVES_TRAFICO;
    const GLfloat t = fmod(tiempo + nave.desfase, ciclo);

    const glm::vec3 puerto = glm::vec3(matrizISS * glm::vec4(PUERTO_ACOPLAMIENTO_LOCAL, 1.0f));
    const glm::vec3 salida = glm::normalize(glm::vec3(matrizISS * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f)));
    const glm::vec3 arriba = glm::normalize(glm::vec3(matrizISS * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f)));
    const glm::vec3 atraque = puerto + salida * nave.holgura;
    const glm::vec3 origenLlegada = ORIGEN_ISS + glm::vec3(650.0f * nave.ladoLlegada, 140.0f, 300.0f);
    const glm::vec3 destinoSalida = ORIGEN_ISS + glm::vec3(-650.0f * nave.ladoLlegada, 110.0f, -260.0f);

    glm::vec3 frente = -salida;
    if (t < DURACION_LLEGADA_NAVE)
    {
        const GLfloat u = SuavizarPaso(t / DURACION_LLEGADA_NAVE);
        const glm::vec3 entrada = atraque + salida * 260.0f;
        const glm::vec3 p1 = origenLlegada + glm::normalize(entrada - origenLlegada) * 420.0f;
        estado.visible = true;
        estado.etapa = 0;
        estado.posicion = BezierCubica(origenLlegada, p1, entrada, atraque, u);
        const glm::vec3 derivada = DerivadaBezierCubica(origenLlegada, p1, entrada, atraque, u);
        frente = (glm::length(derivada) > 0.001f) ? glm::normalize(derivada) : -salida;
    }
    else if (t < DURACION_LLEGADA_NAVE + DURACION_ACOPLADA_NAVE)
    {
        estado.visible = true;
        estado.etapa = 1;
        estado.posicion = atraque;
    }
    else if (t < DURACION_LLEGADA_NAVE + DURACION_ACOPLADA_NAVE + DURACION_SALIDA_NAVE)
    {
        const GLfloat u = SuavizarPaso((t - DURACION_LLEGADA_NAVE - DURACION_ACOPLADA_NAVE) / DURACION_SALIDA_NAVE);
        const glm::vec3 p1 = atraque + salida * 260.0f;
        const glm::vec3 p2 = destinoSalida - glm::normalize(destinoSalida - p1) * 420.0f;
        estado.visible = true;
        estado.etapa = 2;
        estado.posicion = BezierCubica(atraque, p1, p2, destinoSalida, u);
        glm::vec3 derivada = DerivadaBezierCubica(atraque, p1, p2, destinoSalida, u);
        derivada = (glm::length(derivada) > 0.001f) ? glm::normalize(derivada) : salida;
        // Empieza de frente a la estacion (retrocede) y gira sobre la vertical hasta apuntar hacia donde va
        const GLfloat giro = 3.1415927f * (1.0f - SuavizarPaso(u / 0.3f));
        frente = glm::vec3(glm::rotate(glm::mat4(1.0f), giro, arriba) * glm::vec4(derivada, 0.0f));
    }
    else
        return estado;

    const glm::vec3 derecha = glm::normalize(glm::cross(arriba, frente));
    const glm::vec3 arribaNave = glm::cross(frente, derecha);
    estado.orientacion[0] = glm::vec4(derecha, 0.0f);
    estado.orientacion[1] = glm::vec4(arribaNave, 0.0f);
    estado.orientacion[2] = glm::vec4(frente, 0.0f);
    return estado;
}

// Vuelo del cohete: actualiza el estado (cuenta regresiva, ascenso, ruta de la ISS y regreso) y devuelve su posicion
glm::vec3 ActualizarVueloCohete(const glm::vec3& posicionPlataformaMundo)
{
    GLfloat potenciaObjetivoFuego = 0.0f;
    glm::vec3 posicionCohete = posicionPlataformaMundo;
    intensidadTemblorCohete = 0.0f;
    GLfloat temblorAterrizaje = 0.0f;

    if (estadoCohete == 0)
    {
        ejeDerechaCohete = glm::vec3(1.0f, 0.0f, 0.0f);
        orientacionCohete = glm::mat4(1.0f);
        direccionVueloCohete = glm::vec3(0.0f, 1.0f, 0.0f);
    }
    else if (estadoCohete == 1)
    {
        // Cuenta regresiva: ronroneo suave, encendido de los motores y temblor cada vez mas fuerte
        tiempoCohete += deltaTime;
        if (tiempoCohete < INSTANTE_ENCENDIDO)
            intensidadTemblorCohete = 0.06f + 0.10f * (tiempoCohete / INSTANTE_ENCENDIDO);
        else
        {
            GLfloat avanceMotores = glm::clamp((tiempoCohete - INSTANTE_ENCENDIDO) / (DURACION_CUENTA_REGRESIVA - INSTANTE_ENCENDIDO), 0.0f, 1.0f);
            intensidadTemblorCohete = 0.16f + 0.84f * SuavizarPaso(avanceMotores);
            potenciaObjetivoFuego = 0.25f + 0.75f * avanceMotores;
        }
        if (tiempoCohete >= DURACION_CUENTA_REGRESIVA)
        {
            estadoCohete = 2;
            tiempoCohete = 0.0f;
            CrearTrayectoriaCohete(posicionPlataformaMundo);
            distanciaRecorridaCohete = 0.0f;
            velocidadVueloCohete = 0.0f;
            tiempoRegreso = 0.0f;
        }
    }
    if (estadoCohete == 2)
    {
        tiempoCohete += deltaTime;
        const GLfloat velocidadOrbitalCohete = RADIO_RUTA_ISS * VELOCIDAD_ANGULAR_COHETE;
        const GLfloat longitudTotalCohete = distanciasTrayectoriaCohete.back();
        if (distanciaRecorridaCohete < distanciaFinAscenso)
        {
            // Ascenso: acelera y frena al llegar para entrar a la ruta a velocidad orbital
            velocidadVueloCohete = glm::min(velocidadVueloCohete + ACELERACION_ASCENSO * deltaTime, VELOCIDAD_MAXIMA_ASCENSO);
            velocidadVueloCohete = glm::min(velocidadVueloCohete, velocidadOrbitalCohete + 0.4f * (distanciaFinAscenso - distanciaRecorridaCohete));
            potenciaObjetivoFuego = 1.0f;
        }
        else if (distanciaRecorridaCohete < distanciaFinVuelta)
        {
            velocidadVueloCohete += (velocidadOrbitalCohete - velocidadVueloCohete) * glm::min(1.0f, 2.0f * deltaTime);
            potenciaObjetivoFuego = 0.5f;
        }
        else
        {
            // Regreso: baja con la nariz por delante y los motores apagados; cerca del planeta se voltea,
            // enciende los motores hacia abajo y frena para posarse en la plataforma
            GLfloat distanciaRestanteRegreso = longitudTotalCohete - distanciaRecorridaCohete;
            if (distanciaRestanteRegreso <= DISTANCIA_VOLTEO_COHETE) tiempoRegreso += deltaTime;
            velocidadVueloCohete = glm::min(velocidadVueloCohete + 20.0f * deltaTime, VELOCIDAD_MAXIMA_REGRESO);
            velocidadVueloCohete = glm::min(velocidadVueloCohete, 6.0f + 0.22f * distanciaRestanteRegreso);
            if (tiempoRegreso > 1.8f)
            {
                potenciaObjetivoFuego = 0.9f;
                temblorAterrizaje = 0.25f + 0.35f * SuavizarPaso(1.0f - distanciaRestanteRegreso / 200.0f);
            }
        }
        distanciaRecorridaCohete += velocidadVueloCohete * deltaTime;

        // El temblor del despegue no se corta: sigue al encender y se apaga poco a poco al ganar altura
        GLfloat avanceDespegue = glm::clamp(tiempoCohete / 10.0f, 0.0f, 1.0f);
        intensidadTemblorCohete = exp(-0.45f * tiempoCohete) * (1.0f - SuavizarPaso(avanceDespegue));
        intensidadTemblorCohete = glm::max(intensidadTemblorCohete, temblorAterrizaje);

        if (distanciaRecorridaCohete >= longitudTotalCohete)
        {
            estadoCohete = 0;
            tiempoCohete = 0.0f;
            distanciaRecorridaCohete = 0.0f;
            velocidadVueloCohete = 0.0f;
            potenciaObjetivoFuego = 0.0f;
            ejeDerechaCohete = glm::vec3(1.0f, 0.0f, 0.0f);
            orientacionCohete = glm::mat4(1.0f);
            direccionVueloCohete = glm::vec3(0.0f, 1.0f, 0.0f);
        }
        else
        {
            glm::vec3 tangenteVuelo;
            posicionCohete = PuntoTrayectoriaCohete(distanciaRecorridaCohete, tangenteVuelo);
            direccionVueloCohete = tangenteVuelo;
            glm::vec3 narizCohete = tangenteVuelo;
            if (tiempoRegreso > 0.0f)
            {
                // Cerca del planeta el cohete gira 180 grados sobre su eje transversal para bajar con la cola por delante
                GLfloat giroVolteo = 3.1415927f * SuavizarPaso(tiempoRegreso / 3.0f);
                glm::vec3 ejeVolteo = ejeDerechaCohete - tangenteVuelo * glm::dot(ejeDerechaCohete, tangenteVuelo);
                ejeVolteo = (glm::length(ejeVolteo) > 0.001f) ? glm::normalize(ejeVolteo) : glm::vec3(1.0f, 0.0f, 0.0f);
                narizCohete = glm::normalize(tangenteVuelo * cos(giroVolteo) + glm::cross(ejeVolteo, tangenteVuelo) * sin(giroVolteo));
            }
            // El giro sobre su propio eje se hereda del cuadro anterior para que no de saltos
            ejeDerechaCohete = ejeDerechaCohete - narizCohete * glm::dot(ejeDerechaCohete, narizCohete);
            if (glm::length(ejeDerechaCohete) < 0.001f) ejeDerechaCohete = glm::cross(narizCohete, glm::vec3(0.0f, 0.0f, 1.0f));
            ejeDerechaCohete = glm::normalize(ejeDerechaCohete);
            // Al acercarse a la plataforma recupera su giro original
            GLfloat distanciaRestante = longitudTotalCohete - distanciaRecorridaCohete;
            if (distanciaRestante < 250.0f)
            {
                glm::vec3 derechaOriginal = glm::vec3(1.0f, 0.0f, 0.0f) - narizCohete * glm::dot(glm::vec3(1.0f, 0.0f, 0.0f), narizCohete);
                if (glm::length(derechaOriginal) > 0.001f)
                    ejeDerechaCohete = glm::normalize(glm::mix(ejeDerechaCohete, glm::normalize(derechaOriginal), SuavizarPaso(1.0f - distanciaRestante / 250.0f)));
            }
            orientacionCohete[0] = glm::vec4(ejeDerechaCohete, 0.0f);
            orientacionCohete[1] = glm::vec4(narizCohete, 0.0f);
            orientacionCohete[2] = glm::vec4(glm::cross(ejeDerechaCohete, narizCohete), 0.0f);
        }
    }
    posicionCoheteMundo = posicionCohete;
    potenciaFuegoCohete += (potenciaObjetivoFuego - potenciaFuegoCohete) * glm::min(1.0f, 3.0f * deltaTime);
    return posicionCohete;
}

// Matriz del cohete en el mundo, con el temblor organico (desplazamiento y cabeceo irregulares) encima de su posicion
glm::mat4 CalcularMatrizCohete(const glm::vec3& posicionCohete, GLfloat tiempoRuido)
{
    glm::vec3 desplazamientoTemblor(
        0.9f * intensidadTemblorCohete * RuidoOrganico(tiempoRuido, 0.0f),
        0.2f * intensidadTemblorCohete * RuidoOrganico(tiempoRuido * 1.9f, 4.2f),
        0.9f * intensidadTemblorCohete * RuidoOrganico(tiempoRuido * 1.07f, 2.1f));
    GLfloat cabeceoTemblorX = 0.016f * intensidadTemblorCohete * RuidoOrganico(tiempoRuido * 0.9f, 1.3f);
    GLfloat cabeceoTemblorZ = 0.016f * intensidadTemblorCohete * RuidoOrganico(tiempoRuido * 0.8f, 3.7f);
    glm::mat4 matrizCohete = glm::translate(glm::mat4(1.0f), posicionCohete + desplazamientoTemblor) * orientacionCohete;
    matrizCohete = glm::rotate(matrizCohete, cabeceoTemblorX, glm::vec3(1.0f, 0.0f, 0.0f));
    matrizCohete = glm::rotate(matrizCohete, cabeceoTemblorZ, glm::vec3(0.0f, 0.0f, 1.0f));
    return matrizCohete;
}

// Humo del cohete: nace en las toberas, se esparce cerca del suelo y se desvanece
void ActualizarHumoCohete(const glm::mat4& matrizCohete, const glm::vec3& posicionPlataformaMundo, GLfloat tiempoRuido)
{
    const glm::vec3 posicionToberaCohete = glm::vec3(matrizCohete * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    const glm::vec3 haciaAtrasCohete = -glm::normalize(glm::vec3(matrizCohete * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f)));
    // En el espacio no hay aire: el humo solo sale cerca del planeta y se apaga al ganar altura
    const GLfloat atmosferaCohete = 1.0f - SuavizarPaso((glm::length(posicionToberaCohete - posicionPlataformaMundo) - 150.0f) / 500.0f);
    if (potenciaFuegoCohete > 0.05f && atmosferaCohete > 0.01f)
    {
        acumuladoHumoCohete += 45.0f * potenciaFuegoCohete * atmosferaCohete * deltaTime;
        while (acumuladoHumoCohete >= 1.0f)
        {
            acumuladoHumoCohete -= 1.0f;
            ParticulaHumoCohete particula;
            particula.posicion = posicionToberaCohete + glm::vec3(AleatorioCohete(-3.0f, 3.0f), AleatorioCohete(-3.0f, 3.0f), AleatorioCohete(-3.0f, 3.0f));
            particula.velocidad = haciaAtrasCohete * AleatorioCohete(8.0f, 18.0f)
                + glm::vec3(AleatorioCohete(-5.0f, 5.0f), AleatorioCohete(-2.0f, 3.0f), AleatorioCohete(-5.0f, 5.0f));
            // Cerca de la plataforma el humo se abre hacia los lados, como la nube de un lanzamiento
            if (posicionToberaCohete.y - posicionPlataformaMundo.y < 90.0f)
                particula.velocidad += glm::vec3(AleatorioCohete(-30.0f, 30.0f), AleatorioCohete(0.0f, 4.0f), AleatorioCohete(-30.0f, 30.0f));
            particula.nacimiento = tiempoRuido;
            particula.vida = 3.5f;
            humoCohete.push_back(particula);
        }
    }
    for (size_t i = 0; i < humoCohete.size(); i++)
    {
        humoCohete[i].posicion += humoCohete[i].velocidad * deltaTime;
        humoCohete[i].velocidad *= (1.0f - 0.6f * deltaTime);
    }
    humoCohete.erase(std::remove_if(humoCohete.begin(), humoCohete.end(),
        [&](const ParticulaHumoCohete& particula) { return tiempoRuido - particula.nacimiento >= particula.vida; }), humoCohete.end());
}

// ============================================================
// DECORADO DE LA PISTA DE HALO: funciones
// ============================================================
// Posicion y tangente de un punto del circuito (en coordenadas de la pista, sin la altura extra de MoverNave)
void PuntoPista(GLfloat fase, glm::vec3& posicion, glm::vec3& tangente)
{
    const unsigned int n = (unsigned int)puntosCircuito.size();
    fase = fmod(fase, longitudCircuito);
    if (fase < 0.0f) fase += longitudCircuito;
    unsigned int i = 0;
    while (i + 1 < n && fase >= distanciasCircuito[i + 1]) i++;
    const GLfloat tramo = distanciasCircuito[i + 1] - distanciasCircuito[i];
    const GLfloat u = tramo > 0.0001f ? (fase - distanciasCircuito[i]) / tramo : 0.0f;
    posicion = glm::mix(puntosCircuito[i], puntosCircuito[(i + 1) % n], u);
    tangente = glm::normalize(puntosCircuito[(i + 1) % n] - puntosCircuito[(i + n - 1) % n]);
}

static void AnadirCuadroPista(std::vector<GLfloat>& v, std::vector<unsigned int>& idx,
    const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const glm::vec3& d, const glm::vec3& n,
    const glm::vec2& uvA, const glm::vec2& uvB, const glm::vec2& uvC, const glm::vec2& uvD)
{
    const unsigned int base = (unsigned int)(v.size() / 8);
    const glm::vec3 p[4] = { a, b, c, d };
    const glm::vec2 t[4] = { uvA, uvB, uvC, uvD };
    for (int i = 0; i < 4; i++)
        v.insert(v.end(), { p[i].x, p[i].y, p[i].z, t[i].x, t[i].y, n.x, n.y, n.z });
    idx.insert(idx.end(), { base, base + 1, base + 2, base, base + 2, base + 3 });
}

// Caja con cuatro caras laterales y base (para las columnas del puente)
static void AnadirCajaPista(std::vector<GLfloat>& v, std::vector<unsigned int>& idx, const glm::vec3& centro,
    const glm::vec3& ejeX, const glm::vec3& ejeZ, GLfloat mitadX, GLfloat mitadZ, GLfloat y0, GLfloat y1)
{
    auto esquina = [&](int sx, int sz, GLfloat y)
        {
            glm::vec3 p = centro + ejeX * (mitadX * sx) + ejeZ * (mitadZ * sz);
            p.y = y;
            return p;
        };
    const glm::vec2 uv(0.5f, 0.5f);
    AnadirCuadroPista(v, idx, esquina(-1, -1, y0), esquina(1, -1, y0), esquina(1, -1, y1), esquina(-1, -1, y1), -ejeZ, uv, uv, uv, uv);
    AnadirCuadroPista(v, idx, esquina(1, -1, y0), esquina(1, 1, y0), esquina(1, 1, y1), esquina(1, -1, y1), ejeX, uv, uv, uv, uv);
    AnadirCuadroPista(v, idx, esquina(1, 1, y0), esquina(-1, 1, y0), esquina(-1, 1, y1), esquina(1, 1, y1), ejeZ, uv, uv, uv, uv);
    AnadirCuadroPista(v, idx, esquina(-1, 1, y0), esquina(-1, -1, y0), esquina(-1, -1, y1), esquina(-1, 1, y1), -ejeX, uv, uv, uv, uv);
    AnadirCuadroPista(v, idx, esquina(-1, -1, y0), esquina(1, -1, y0), esquina(1, 1, y0), esquina(-1, 1, y0), glm::vec3(0.0f, -1.0f, 0.0f), uv, uv, uv, uv);
}

// Bordillos rojos y blancos, hombros de concreto, faldones del puente, columnas y linea de meta a cuadros
void CrearDecoradoPista()
{
    const unsigned int n = (unsigned int)puntosCircuito.size();
    if (n < 2 || distanciasCircuito.size() != n + 1) return;
    const GLfloat BORDE_PISTA = 5.0f, BORDE_BORDILLO = 5.9f, BORDE_HOMBRO = 7.0f, GROSOR_PUENTE = 2.2f;
    const glm::vec3 arriba(0.0f, 1.0f, 0.0f);

    auto lateralEn = [&](unsigned int k)
        {
            glm::vec3 tg = glm::normalize(puntosCircuito[(k + 1) % n] - puntosCircuito[(k + n - 1) % n]);
            return glm::normalize(glm::vec3(tg.z, 0.0f, -tg.x));
        };

    std::vector<GLfloat> vBordillo, vConcreto, vMeta;
    std::vector<unsigned int> iBordillo, iConcreto, iMeta;

    for (int lado = -1; lado <= 1; lado += 2)
    {
        for (unsigned int i = 0; i < n; i++)
        {
            const unsigned int k1 = (i + 1) % n;
            const glm::vec3 p0 = puntosCircuito[i], p1 = puntosCircuito[k1];
            const glm::vec3 d0 = lateralEn(i) * (GLfloat)lado, d1 = lateralEn(k1) * (GLfloat)lado;
            const GLfloat s0 = distanciasCircuito[i] / 6.0f, s1 = distanciasCircuito[i + 1] / 6.0f;
            const glm::vec3 elev(0.0f, 0.15f, 0.0f);

            // bordillo rojo y blanco
            AnadirCuadroPista(vBordillo, iBordillo,
                p0 + d0 * BORDE_PISTA + elev, p0 + d0 * BORDE_BORDILLO + elev,
                p1 + d1 * BORDE_BORDILLO + elev, p1 + d1 * BORDE_PISTA + elev, arriba,
                glm::vec2(0.0f, s0), glm::vec2(1.0f, s0), glm::vec2(1.0f, s1), glm::vec2(0.0f, s1));
            // hombro de concreto
            const glm::vec3 elevH(0.0f, 0.10f, 0.0f);
            AnadirCuadroPista(vConcreto, iConcreto,
                p0 + d0 * BORDE_BORDILLO + elevH, p0 + d0 * BORDE_HOMBRO + elevH,
                p1 + d1 * BORDE_HOMBRO + elevH, p1 + d1 * BORDE_BORDILLO + elevH, arriba,
                glm::vec2(0.5f), glm::vec2(0.5f), glm::vec2(0.5f), glm::vec2(0.5f));
            // faldon vertical por fuera del hombro
            AnadirCuadroPista(vConcreto, iConcreto,
                p0 + d0 * BORDE_HOMBRO + elevH, p1 + d1 * BORDE_HOMBRO + elevH,
                p1 + d1 * BORDE_HOMBRO - glm::vec3(0.0f, GROSOR_PUENTE, 0.0f), p0 + d0 * BORDE_HOMBRO - glm::vec3(0.0f, GROSOR_PUENTE, 0.0f), d0,
                glm::vec2(0.5f), glm::vec2(0.5f), glm::vec2(0.5f), glm::vec2(0.5f));
        }
    }
    // cara inferior del puente
    for (unsigned int i = 0; i < n; i++)
    {
        const unsigned int k1 = (i + 1) % n;
        const glm::vec3 d0 = lateralEn(i), d1 = lateralEn(k1);
        const glm::vec3 baja(0.0f, -GROSOR_PUENTE, 0.0f);
        AnadirCuadroPista(vConcreto, iConcreto,
            puntosCircuito[i] + d0 * BORDE_HOMBRO + baja, puntosCircuito[i] - d0 * BORDE_HOMBRO + baja,
            puntosCircuito[k1] - d1 * BORDE_HOMBRO + baja, puntosCircuito[k1] + d1 * BORDE_HOMBRO + baja, -arriba,
            glm::vec2(0.5f), glm::vec2(0.5f), glm::vec2(0.5f), glm::vec2(0.5f));
    }
    // columnas con viga transversal bajo el tramo elevado
    for (GLfloat fase = 140.0f; fase < 760.0f; fase += 45.0f)
    {
        glm::vec3 p, tg;
        PuntoPista(fase, p, tg);
        if (p.y < 11.0f) continue;
        const glm::vec3 lat = glm::normalize(glm::vec3(tg.z, 0.0f, -tg.x));
        AnadirCajaPista(vConcreto, iConcreto, p, lat, tg, BORDE_HOMBRO - 0.6f, 1.3f, p.y - GROSOR_PUENTE - 1.4f, p.y - GROSOR_PUENTE);
        AnadirCajaPista(vConcreto, iConcreto, p, lat, tg, 1.2f, 1.2f, -4.0f, p.y - GROSOR_PUENTE - 1.4f);
    }
    // linea de meta a cuadros
    {
        glm::vec3 p, tg;
        PuntoPista(FASE_META_HALO, p, tg);
        const glm::vec3 lat = glm::normalize(glm::vec3(tg.z, 0.0f, -tg.x));
        const glm::vec3 elev(0.0f, 0.22f, 0.0f);
        AnadirCuadroPista(vMeta, iMeta,
            p + lat * BORDE_PISTA - tg * 1.25f + elev, p - lat * BORDE_PISTA - tg * 1.25f + elev,
            p - lat * BORDE_PISTA + tg * 1.25f + elev, p + lat * BORDE_PISTA + tg * 1.25f + elev, arriba,
            glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec2(0.0f, 1.0f));
    }

    auto crear = [&](std::vector<GLfloat>& v, std::vector<unsigned int>& idx, unsigned int& indice)
        {
            MeshModel* malla = new MeshModel();
            malla->CreateMeshModel(v.data(), idx.data(), (unsigned int)v.size(), (unsigned int)idx.size());
            indice = (unsigned int)meshList.size();
            meshList.push_back(malla);
        };
    crear(vBordillo, iBordillo, indiceBordillos);
    crear(vConcreto, iConcreto, indiceFaldones);
    crear(vMeta, iMeta, indiceMeta);
    decoradoPistaListo = true;
}

// Posiciones de las piezas del Racing Kit (gradas, boxes, carpas, farolas, vallas, banderas...)
void PrepararDecoradoHalo()
{
    decoradoHalo.clear();
    const GLfloat LADO_PISTA = 6.4f;   // lateral de los postes sobre el hombro de concreto
    auto poner = [&](int id, GLfloat fase, GLfloat lateral, GLfloat giro, const glm::vec3& escala, GLfloat altura)
        {
            decoradoHalo.push_back({ id, fase, lateral, giro, escala, altura });
        };
    // El frente de cada modelo del kit mira hacia -Z: con +90 queda mirando al lado izquierdo de la pista
    const GLfloat MIRA_A_PISTA_DESDE_DERECHA = 90.0f, MIRA_A_PISTA_DESDE_IZQUIERDA = -90.0f;

    // Zona de salida: marcas de la parrilla, gradas en el interior y boxes en el exterior
    poner(KIT_POSICIONES_SALIDA, 0.0f, 0.0f, 0.0f, glm::vec3(10.0f), 0.2f);
    poner(KIT_GRADA_TECHADA, -10.0f, 15.5f, MIRA_A_PISTA_DESDE_DERECHA, glm::vec3(10.0f), 0.0f);
    poner(KIT_GRADA_TOLDO, 0.0f, 15.5f, MIRA_A_PISTA_DESDE_DERECHA, glm::vec3(10.0f), 0.0f);
    poner(KIT_GRADA_TECHADA, 10.0f, 15.5f, MIRA_A_PISTA_DESDE_DERECHA, glm::vec3(10.0f), 0.0f);
    poner(KIT_BOXES, -8.0f, -11.5f, MIRA_A_PISTA_DESDE_IZQUIERDA, glm::vec3(8.0f), 0.0f);
    poner(KIT_BOXES_CERRADOS, 0.0f, -11.5f, MIRA_A_PISTA_DESDE_IZQUIERDA, glm::vec3(8.0f), 0.0f);
    poner(KIT_BOXES, 8.0f, -11.5f, MIRA_A_PISTA_DESDE_IZQUIERDA, glm::vec3(8.0f), 0.0f);
    poner(KIT_OFICINA, 18.0f, -11.5f, MIRA_A_PISTA_DESDE_IZQUIERDA, glm::vec3(8.0f), 0.0f);
    poner(KIT_CARPA_LARGA, -4.0f, 30.0f, MIRA_A_PISTA_DESDE_DERECHA, glm::vec3(8.0f), 0.0f);
    poner(KIT_CARPA_CERRADA, 14.0f, 30.0f, MIRA_A_PISTA_DESDE_DERECHA, glm::vec3(8.0f), 0.0f);
    poner(KIT_TORRE_BANDERA, 24.0f, 17.0f, MIRA_A_PISTA_DESDE_DERECHA, glm::vec3(8.0f), 0.0f);
    for (int i = 0; i < 4; i++)
    {
        poner(i % 2 == 0 ? KIT_BANDERA_META : KIT_BANDERA_ROJA, 26.0f + 9.0f * i, (i % 2 == 0 ? 1.0f : -1.0f) * LADO_PISTA,
            0.0f, glm::vec3(8.0f), 0.0f);
    }

    // Farolas: mismas posiciones que los focos de luz
    for (int i = 0; i < CANTIDAD_FAROLAS_HALO; i++)
    {
        const GLfloat fase = (i + 0.5f) * longitudCircuito / CANTIDAD_FAROLAS_HALO;
        const GLfloat lado = (i % 2 == 0) ? -1.0f : 1.0f;
        poner(KIT_FAROLA, fase, lado * LADO_PISTA, 0.0f, glm::vec3(10.0f), 0.0f);
    }

    // Muros en el exterior de las curvas y vallas en las rectas, a lo largo de la rampa y del tramo elevado
    for (GLfloat fase = 30.0f; fase < 770.0f; fase += 10.0f)
    {
        glm::vec3 p0, t0, p1, t1;
        PuntoPista(fase + 5.0f, p0, t0);
        PuntoPista(fase + 15.0f, p1, t1);
        if (p0.y < 3.5f) continue;
        const GLfloat giroCurva = t0.z * t1.x - t0.x * t1.z;   // > 0: curva hacia la derecha
        for (int lado = -1; lado <= 1; lado += 2)
        {
            const bool exteriorCurva = (lado > 0 && giroCurva < -0.07f) || (lado < 0 && giroCurva > 0.07f);
            if (exteriorCurva)
                poner(KIT_MURO, fase + 5.0f, lado * 6.6f, -90.0f, glm::vec3(10.0f, 14.0f, 10.0f), 0.0f);
            else
                poner(KIT_VALLA, fase + 5.0f, lado * 6.9f, -90.0f, glm::vec3(10.0f, 5.0f, 10.0f), 0.0f);
        }
    }
}

// ============================================================
// HORA DE AVENTURA: funciones
// ============================================================
// Pieza articulada: va a la articulacion del padre y gira ahi
glm::mat4 CrearMatrizArticulacion(const glm::mat4& matrizPadre, glm::vec3 articulacion, GLfloat grados, glm::vec3 eje)
{
    glm::mat4 matriz = glm::translate(matrizPadre, articulacion);
    return glm::rotate(matriz, glm::radians(grados), eje);
}

// Personaje fijo de Hora de Aventura: en su lugar y mirando hacia Fantasmano
glm::mat4 CrearMatrizPersonajeHoraAventura(const glm::mat4& matrizPlaneta, glm::vec3 posicion, glm::vec3 objetivo, GLfloat escala)
{
    glm::vec3 haciaObjetivo = objetivo - posicion;
    glm::mat4 matriz = glm::translate(matrizPlaneta, posicion);
    matriz = glm::rotate(matriz, atan2(haciaObjetivo.x, haciaObjetivo.z), glm::vec3(0.0f, 1.0f, 0.0f));
    return glm::scale(matriz, glm::vec3(escala));
}

int main()
{
    mainWindow = Window(1366, 768);
    mainWindow.Initialise();
    CreateShaders();
    CreatePlasmaMesh();
    CrearMallaCoronaSol();
    CrearPista();
    CrearDecoradoPista();
    PrepararDecoradoHalo();

    Shader shaderPlasma;
    shaderPlasma.CreateFromString(
        R"GLSL(#version 330 core
layout (location = 0) in vec3 posicion;
layout (location = 1) in vec2 coordenadaUV;
out vec2 UV;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
void main()
{
    UV = coordenadaUV;
    gl_Position = projection * view * model * vec4(posicion, 1.0);
}
)GLSL",
R"GLSL(#version 330 core
in vec2 UV;
out vec4 colorFinal;
uniform sampler2D texturaFuego;
void main()
{
    colorFinal = texture(texturaFuego, UV);
}
)GLSL");

    shaderPlasma.UseShader();
    GLint programaPlasma = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, &programaPlasma);
    GLint estadoPlasma = GL_FALSE;
    if (programaPlasma != 0) glGetProgramiv(programaPlasma, GL_LINK_STATUS, &estadoPlasma);

    bool plasmaDisponible = (programaPlasma != 0 && estadoPlasma == GL_TRUE);
    GLint plasmaModel = plasmaDisponible ? (GLint)shaderPlasma.GetModelLocation() : -1;
    GLint plasmaView = plasmaDisponible ? (GLint)shaderPlasma.GetViewLocation() : -1;
    GLint plasmaProjection = plasmaDisponible ? (GLint)shaderPlasma.GetProjectionLocation() : -1;
    GLint plasmaTextura = plasmaDisponible ? glGetUniformLocation(programaPlasma, "texturaFuego") : -1;
    glUseProgram(0);

    const char* rutaFuego = "Textures/fuego.tga";
    FILE* archivoFuego = nullptr;
    fopen_s(&archivoFuego, rutaFuego, "rb");
    if (!archivoFuego)
    {
        rutaFuego = "Textures/fuego.tga";
        fopen_s(&archivoFuego, rutaFuego, "rb");
    }

    Shader shaderSolEmisivo;
    shaderSolEmisivo.CreateFromString(
        R"SOL_VERT(#version 330 core
layout (location = 0) in vec3 posicion;
layout (location = 1) in vec2 coordenadaUV;
out vec2 UV;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
void main()
{
    UV = coordenadaUV;
    gl_Position = projection * view * model * vec4(posicion, 1.0);
}
)SOL_VERT",
R"SOL_FRAG(#version 330 core
in vec2 UV;
out vec4 colorFinal;
uniform sampler2D texturaSol;
void main()
{
    vec3 textura = texture(texturaSol, UV).rgb;
    float detalle = clamp(dot(textura, vec3(0.30, 0.59, 0.11)), 0.0, 1.0);
    vec3 rosaBase = vec3(1.0, 0.22, 0.68);
    vec3 rosaClaro = vec3(1.0, 0.66, 0.93);
    colorFinal = vec4(mix(rosaBase, rosaClaro, 0.25 + 0.65 * detalle), 1.0);
}
)SOL_FRAG");
    shaderSolEmisivo.UseShader();
    GLint programaSolEmisivo = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, &programaSolEmisivo);
    GLint estadoSolEmisivo = GL_FALSE;
    if (programaSolEmisivo != 0)
        glGetProgramiv(programaSolEmisivo, GL_LINK_STATUS, &estadoSolEmisivo);
    const bool solEmisivoDisponible =
        (programaSolEmisivo != 0 && estadoSolEmisivo == GL_TRUE);
    const GLint solModel = solEmisivoDisponible ? glGetUniformLocation(programaSolEmisivo, "model") : -1;
    const GLint solView = solEmisivoDisponible ? glGetUniformLocation(programaSolEmisivo, "view") : -1;
    const GLint solProjection = solEmisivoDisponible ? glGetUniformLocation(programaSolEmisivo, "projection") : -1;
    const GLint solTextura = solEmisivoDisponible ? glGetUniformLocation(programaSolEmisivo, "texturaSol") : -1;
    glUseProgram(0);

    Shader shaderCoronaSol;
    shaderCoronaSol.CreateFromString(
        R"CORONA_VERT(#version 330 core
layout (location = 0) in vec3 posicion;
layout (location = 1) in vec2 coordenadaUV;
out vec2 UV;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
void main()
{
    UV = coordenadaUV;
    gl_Position = projection * view * model * vec4(posicion, 1.0);
}
)CORONA_VERT",
R"CORONA_FRAG(#version 330 core
in vec2 UV;
out vec4 colorFinal;
void main()
{
    vec2 uvCentrado = (UV - vec2(0.5)) * 2.0;
    float radio = length(uvCentrado);
    float brillo = pow(max(1.0 - radio, 0.0), 2.0);
    colorFinal = vec4(1.0, 0.20, 0.68, brillo * 0.78);
}
)CORONA_FRAG");
    shaderCoronaSol.UseShader();
    GLint programaCoronaSol = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, &programaCoronaSol);
    GLint estadoCoronaSol = GL_FALSE;
    if (programaCoronaSol != 0)
        glGetProgramiv(programaCoronaSol, GL_LINK_STATUS, &estadoCoronaSol);
    const bool coronaSolDisponible =
        (programaCoronaSol != 0 && estadoCoronaSol == GL_TRUE);
    const GLint coronaModel = coronaSolDisponible ? glGetUniformLocation(programaCoronaSol, "model") : -1;
    const GLint coronaView = coronaSolDisponible ? glGetUniformLocation(programaCoronaSol, "view") : -1;
    const GLint coronaProjection = coronaSolDisponible ? glGetUniformLocation(programaCoronaSol, "projection") : -1;
    glUseProgram(0);

    bool tieneTexturaFuego = (archivoFuego != nullptr);
    if (archivoFuego)
    {
        fclose(archivoFuego);
        FuegoAzulTexture = Texture(rutaFuego);
        FuegoAzulTexture.LoadTextureA();
    }

    Shader shaderHumo;
    shaderHumo.CreateFromString(
        R"HUMO_VERT(#version 330 core
layout (location = 0) in vec3 posicion;
layout (location = 1) in vec2 coordenadaUV;
out vec2 UV;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
void main()
{
    UV = coordenadaUV;
    gl_Position = projection * view * model * vec4(posicion, 1.0);
}
)HUMO_VERT",
R"HUMO_FRAG(#version 330 core
in vec2 UV;
out vec4 colorFinal;
uniform sampler2D texturaHumo;
uniform float opacidad;
uniform vec3 tinte;
void main()
{
    vec4 textura = texture(texturaHumo, UV);
    colorFinal = vec4(tinte * textura.rgb, textura.a * opacidad);
}
)HUMO_FRAG");
    shaderHumo.UseShader();
    GLint programaHumo = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, &programaHumo);
    GLint estadoHumo = GL_FALSE;
    if (programaHumo != 0)
        glGetProgramiv(programaHumo, GL_LINK_STATUS, &estadoHumo);
    const bool humoDisponible = (programaHumo != 0 && estadoHumo == GL_TRUE);
    const GLint humoModel = humoDisponible ? glGetUniformLocation(programaHumo, "model") : -1;
    const GLint humoView = humoDisponible ? glGetUniformLocation(programaHumo, "view") : -1;
    const GLint humoProjection = humoDisponible ? glGetUniformLocation(programaHumo, "projection") : -1;
    const GLint humoTextura = humoDisponible ? glGetUniformLocation(programaHumo, "texturaHumo") : -1;
    const GLint humoOpacidad = humoDisponible ? glGetUniformLocation(programaHumo, "opacidad") : -1;
    const GLint humoTinte = humoDisponible ? glGetUniformLocation(programaHumo, "tinte") : -1;
    glUseProgram(0);
    HumoTexture = Texture("Textures/humo.png");
    HumoTexture.LoadTextureA();

    // Llama del cohete de lanzamiento (naranja, con el nucleo claro)
    FuegoCoheteTexture = Texture("Textures/fuego_cohete.png");
    FuegoCoheteTexture.LoadTextureA();

    camera = Camera(glm::vec3(0.0f, 200.0f, 400.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, -13.0f, 50.0f, 0.5f);

    pistaTexture = Texture("Textures/pista_kit.png");
    pistaTexture.LoadTextureA();
    bordilloTexture = Texture("Textures/pista_curb.png");
    bordilloTexture.LoadTextureA();
    concretoTexture = Texture("Textures/pista_concreto.png");
    concretoTexture.LoadTextureA();
    metaTexture = Texture("Textures/pista_meta.png");
    metaTexture.LoadTextureA();

    Base = Model();
    Base.LoadModel("Models/spacepark.obj");
    Cupula = Model();
    Cupula.LoadModel("Models/cupula.obj");
    Propulsor1 = Model();
    Propulsor1.LoadModel("Models/turbina1spacepark.obj");
    Propulsor2 = Model();
    Propulsor2.LoadModel("Models/turbina2spacepark.obj");
    Propulsor3 = Model();
    Propulsor3.LoadModel("Models/turbina3spacepark.obj");
    Propulsor4 = Model();
    Propulsor4.LoadModel("Models/turbina4spacepark.obj");
    Rigby = Model();
    Rigby.LoadModel("Models/rigby.obj");
    RigbyBrazo1 = Model();
    RigbyBrazo1.LoadModel("Models/rigbybrazo1.obj");
    RigbyBrazo2 = Model();
    RigbyBrazo2.LoadModel("Models/rigbybrazo2.obj");
    CasaParque = Model();
    CasaParque.LoadModel("Models/casaparque.obj");
    Iglesia = Model();
    Iglesia.LoadModel("Models/iglesia.obj");
    KuromiCuerpo = Model();
    KuromiCuerpo.LoadModel("Models/kuromicuerpo.obj");
    KuromiPiernaIzq = Model();
    KuromiPiernaIzq.LoadModel("Models/kuromipiernaizq.obj");
    KuromiPiernaDer = Model();
    KuromiPiernaDer.LoadModel("Models/kuromipiernader.obj");
    CasaKuromi = Model();
    CasaKuromi.LoadModel("Models/casakuromi.obj");
    Cementerio = Model();
    Cementerio.LoadModel("Models/cementerio.obj");
    CarruselBase = Model();
    CarruselBase.LoadModel("Models/carruselbase.obj");
    CarruselSoporte = Model();
    CarruselSoporte.LoadModel("Models/carruselsoporte.obj");
    Caballo1 = Model();
    Caballo1.LoadModel("Models/caballo1.obj");
    Caballo2 = Model();
    Caballo2.LoadModel("Models/caballo2.obj");
    Caballo3 = Model();
    Caballo3.LoadModel("Models/caballo3.obj");
    Caballo4 = Model();
    Caballo4.LoadModel("Models/caballo4.obj");
    Caballo5 = Model();
    Caballo5.LoadModel("Models/caballo5.obj");
    Caballo6 = Model();
    Caballo6.LoadModel("Models/caballo6.obj");
    Caballo7 = Model();
    Caballo7.LoadModel("Models/caballo7.obj");
    Caballo8 = Model();
    Caballo8.LoadModel("Models/caballo8.obj");
    Plataforma = Model();
    Plataforma.LoadModel("Models/plataforma.obj");
    Cinnamoroll = Model();
    Cinnamoroll.LoadModel("Models/cinnamoroll.obj");
    CarroFruta = Model();
    CarroFruta.LoadModel("Models/carrofruta.obj");

    CasaArbol = Model();
    CasaArbol.LoadModel("Models/casa_arbol.obj");

    // Personajes de Hora de Aventura
    {
        struct CargaModelo { Model* modelo; const char* archivo; };
        const CargaModelo cargas[] = {
            { &JakeCuerpo, "jakecuerpo" }, { &JakeBrazoIzq, "jakebrazoizq" }, { &JakeBrazoDer, "jakebrazoder" },
            { &JakePataIzq, "jakepataizq" }, { &JakePataDer, "jakepatader" }, { &JakeCola, "jakecola" },
            { &BmoCuerpo, "bmocuerpo" }, { &BmoBrazoIzq, "bmobrazoizq" }, { &BmoBrazoDer, "bmobrazoder" },
            { &BmoPataIzq, "bmopataizq" }, { &BmoPataDer, "bmopatader" },
            { &GunterCuerpo, "guntercuerpo" }, { &GunterAlaIzq, "gunteralaizq" }, { &GunterAlaDer, "gunteralader" },
            { &TreeTrunksCuerpo, "treetrunkscuerpo" }, { &TreeTrunksPataDelIzq, "treetrunkspatadelizq" },
            { &TreeTrunksPataDelDer, "treetrunkspatadelder" }, { &TreeTrunksPataTrasIzq, "treetrunkspatatrasizq" },
            { &TreeTrunksPataTrasDer, "treetrunkspatatrasder" }, { &TreeTrunksOrejaIzq, "treetrunksorejaizq" },
            { &TreeTrunksOrejaDer, "treetrunksorejader" }, { &TreeTrunksTrompa, "treetrunkstrompa" },
            { &TreeTrunksCola, "treetrunkscola" }, { &NeptrCuerpo, "neptrcuerpo" } };
        for (const CargaModelo& carga : cargas)
        {
            *carga.modelo = Model();
            carga.modelo->LoadModel(std::string("Models/") + carga.archivo + ".obj");
        }
    }
    Tienda = Model();
    Tienda.LoadModel("Models/tienda.obj");
    Bosque1 = Model();
    Bosque1.LoadModel("Models/bosque1.obj");
    Bosque2 = Model();
    Bosque2.LoadModel("Models/bosque2.obj");
    Bosque3 = Model();
    Bosque3.LoadModel("Models/bosque3.obj");
    Sol = Model();
    Sol.LoadModel("Models/sol.obj");
    FantasmanoCuerpo = Model();
    FantasmanoCuerpo.LoadModel("Models/fantasmanocuerpo.obj");
    FantasmanoBrazo = Model();
    FantasmanoBrazo.LoadModel("Models/fantasmanobrazo.obj");
    FantasmanoMano = Model();
    FantasmanoMano.LoadModel("Models/fantasmanomano.obj");

    cuerposatelite = Model();
    cuerposatelite.LoadModel("Models/cuerposatelite.obj");
    antena = Model();
    antena.LoadModel("Models/antena.obj");
    panel1 = Model();
    panel1.LoadModel("Models/panel1.obj");
    panel2 = Model();
    panel2.LoadModel("Models/panel2.obj");
    Asteroide1 = Model();
    Asteroide1.LoadModel("Models/asteroide1.obj");
    Asteroide2 = Model();
    Asteroide2.LoadModel("Models/asteroide2.obj");
    Asteroide3 = Model();
    Asteroide3.LoadModel("Models/asteroide3.obj");

    Halo_M = Model();
    Halo_M.LoadModel("Models/Halo.gltf");
    Ghost_M = Model();
    Ghost_M.LoadModel("Models/Ghost.gltf");
    ISS_M = Model();
    ISS_M.LoadModel("Models/issbase.obj");
    ISS_Panel = Model();
    ISS_Panel.LoadModel("Models/isspanel.obj");
    Cohete = Model();
    Cohete.LoadModel("Models/cohete.obj");
    Rocket_M = Model();
    Rocket_M.LoadModel("Models/Rocket.gltf");
    Beam_M = Model();
    Beam_M.LoadModel("Models/Beam.gltf");
    Spark_M = Model();
    Spark_M.LoadModel("Models/Spark.gltf");


    CarroPrincipal = Model();
    CarroPrincipal.LoadModel("Models/CarroPrincipal.obj");
    CarroGolfCarrera = Model();
    CarroGolfCarrera.LoadModel("Models/CarroGolfCarrera.obj");
    CarroRosa = Model();
    CarroRosa.LoadModel("Models/CarroRosa.obj");

    PorticoSalidaHalo = Model();
    PorticoSalidaHalo.LoadModel("Models/halo_portico_salida.obj");
    for (int i = 0; i < KIT_CANTIDAD; i++)
    {
        PiezasKit[i] = Model();
        PiezasKit[i].LoadModel(std::string("Models/kit_") + DATOS_KIT[i].archivo + ".obj");
    }
    for (int i = 0; i < CANTIDAD_ARBOLES_HALO; i++)
    {
        ArbolesHalo[i] = Model();
        ArbolesHalo[i].LoadModel(std::string("Models/") + ARCHIVOS_ARBOLES_HALO[i] + ".obj");
    }
    CarroBR = Model();
    CarroBR.LoadModel("Models/carrobr.obj");

    std::vector<std::string> skyboxFaces;
    skyboxFaces.push_back("Textures/Skybox/space_rt.tga");
    skyboxFaces.push_back("Textures/Skybox/space_lf.tga");
    skyboxFaces.push_back("Textures/Skybox/space_up.tga");
    skyboxFaces.push_back("Textures/Skybox/space_dn.tga");
    skyboxFaces.push_back("Textures/Skybox/space_bk.tga");
    skyboxFaces.push_back("Textures/Skybox/space_ft.tga");
    stbi_set_flip_vertically_on_load(false);
    skybox.CargarSkybox(skyboxFaces);

    // Skybox del Planeta 2 (parque): fondo violeta y estrellas de otro tono
    std::vector<std::string> skyboxFacesPlaneta2;
    skyboxFacesPlaneta2.push_back("Textures/Skybox2/space_rt.tga");
    skyboxFacesPlaneta2.push_back("Textures/Skybox2/space_lf.tga");
    skyboxFacesPlaneta2.push_back("Textures/Skybox2/space_up.tga");
    skyboxFacesPlaneta2.push_back("Textures/Skybox2/space_dn.tga");
    skyboxFacesPlaneta2.push_back("Textures/Skybox2/space_bk.tga");
    skyboxFacesPlaneta2.push_back("Textures/Skybox2/space_ft.tga");
    skyboxPlaneta2.CargarSkybox(skyboxFacesPlaneta2);

    // Skybox del espacio profundo (zona de la ISS): nebulosa y estrellas
    std::vector<std::string> skyboxFacesEspacioProfundo;
    skyboxFacesEspacioProfundo.push_back("Textures/Skybox3/space_rt.tga");
    skyboxFacesEspacioProfundo.push_back("Textures/Skybox3/space_lf.tga");
    skyboxFacesEspacioProfundo.push_back("Textures/Skybox3/space_up.tga");
    skyboxFacesEspacioProfundo.push_back("Textures/Skybox3/space_dn.tga");
    skyboxFacesEspacioProfundo.push_back("Textures/Skybox3/space_bk.tga");
    skyboxFacesEspacioProfundo.push_back("Textures/Skybox3/space_ft.tga");
    skyboxEspacioProfundo.CargarSkybox(skyboxFacesEspacioProfundo);

    // Skybox del amanecer y el atardecer: se mezcla con el cielo del planeta cuando el Sol esta cerca del horizonte
    std::vector<std::string> skyboxFacesAtardecer;
    skyboxFacesAtardecer.push_back("Textures/Skybox4/space_rt.tga");
    skyboxFacesAtardecer.push_back("Textures/Skybox4/space_lf.tga");
    skyboxFacesAtardecer.push_back("Textures/Skybox4/space_up.tga");
    skyboxFacesAtardecer.push_back("Textures/Skybox4/space_dn.tga");
    skyboxFacesAtardecer.push_back("Textures/Skybox4/space_bk.tga");
    skyboxFacesAtardecer.push_back("Textures/Skybox4/space_ft.tga");
    skyboxAtardecer.CargarSkybox(skyboxFacesAtardecer);

    Material_brillante = Material(4.0f, 256);
    Material_opaco = Material(0.3f, 4);

    unsigned int pointLightCount = 0;
    unsigned int spotLightCount = 0;

    spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
        0.3f, 1.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        1.0f, 0.01f, 0.01f,
        15.0f);
    spotLightCount = 1;

    GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0;
    GLuint uniformEyePosition = 0, uniformSpecularIntensity = 0, uniformShininess = 0;
    GLuint uniformColor = 0;

    glm::mat4 projection = glm::perspective(glm::radians(45.0f),
        (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(),
        0.1f, 12000.0f);

    glm::mat4 model(1.0f);
    glm::mat4 modelaux(1.0f);
    glm::mat4 matrizSpacePark = CrearMatrizSpacePark("Models/spacepark.obj");
    const GLfloat escalaPlaneta = glm::length(glm::vec3(matrizSpacePark[0]));
    glm::vec3 color(1.0f, 1.0f, 1.0f);
    shaderList[0].UseShader();

    GLint programa = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, &programa);
    GLint uniformOpacidad = glGetUniformLocation((GLuint)programa, "opacidad");
    glUseProgram(0);

    bool carreraIniciada = false;

    // Salida de la carrera en Halo: semaforo de tres luces rojas que pasan a verde
    bool cuentaSalidaActiva = false;
    GLfloat tiempoSalidaHalo = 0.0f;
    const GLfloat DURACION_CUENTA_SALIDA = 3.0f;
    const GLfloat DURACION_LUZ_VERDE = 2.5f;

    GLfloat fasePrincipal = 0.0f;
    GLfloat lateralPrincipal = 0.0f;
    GLfloat alturaPrincipal = 0.6f;

    GLfloat faseNave1 = 0.0f;
    GLfloat lateralNave1 = -3.5f;
    GLfloat alturaNave1 = 2.5f;
    GLfloat velNave1 = 80.0f;

    GLfloat faseNave2 = 0.0f;
    GLfloat lateralNave2 = 3.5f;
    GLfloat alturaNave2 = 3.0f;
    GLfloat velNave2 = 73.0f;

    GLfloat faseNave3 = 0.0f;
    GLfloat lateralNave3 = -3.5f;
    GLfloat alturaNave3 = -1.0f;
    GLfloat velNave3 = 78.0f;

    GLfloat faseNave4 = 0.0f;
    GLfloat lateralNave4 = 3.5f;
    GLfloat alturaNave4 = -1.0f;
    GLfloat velNave4 = 72.0f;

    glm::mat4 matrizBasePista = glm::translate(glm::mat4(1.0f), ORIGEN_HALO);
    matrizBasePista = glm::translate(matrizBasePista, glm::vec3(-100.0f, -3.5f, 0.0f));

    GLfloat faseNave = 0.0f;
    glm::vec2 posicionSatelite(60.0f, 0.0f);
    GLfloat giroSatelite = 0.0f;
    GLfloat articulacionesSatelite = 0.0f;
    bool subeArticulacionesSatelite = true;

    lastTime = glfwGetTime();
    while (!mainWindow.getShouldClose())
    {
        GLfloat now = glfwGetTime();
        deltaTime = now - lastTime;
        lastTime = now;

        glfwPollEvents();
        GLFWwindow* ventana = glfwGetCurrentContext();

        bool teclaM = glfwGetKey(ventana, GLFW_KEY_M) == GLFW_PRESS;

        if (teclaM && !teclaMAnterior)
        {
            modoCamara = (modoCamara + 1) % 6;
        }

        teclaMAnterior = teclaM;

        // P: cambiar de planeta (parque <-> Halo); el skybox cambia con el planeta
        bool teclaP = glfwGetKey(ventana, GLFW_KEY_P) == GLFW_PRESS;
        if (teclaP && !teclaPAnterior)
        {
            modoCamara = (modoCamara == 2 || modoCamara == 3) ? 0 : 2;
        }
        teclaPAnterior = teclaP;

        // 1: lanzamiento del cohete
        bool tecla1 = glfwGetKey(ventana, GLFW_KEY_1) == GLFW_PRESS;
        if (tecla1 && !tecla1Anterior && (estadoCohete == 0 || estadoCohete == 3))
        {
            estadoCohete = 1;
            tiempoCohete = 0.0f;
        }
        tecla1Anterior = tecla1;

        // 2: desplegar / recoger los paneles solares de la estacion
        bool tecla2 = glfwGetKey(ventana, GLFW_KEY_2) == GLFW_PRESS;
        if (tecla2 && !tecla2Anterior) panelesDesplegados = !panelesDesplegados;
        tecla2Anterior = tecla2;

        // 3: baile de BMO
        bool tecla3 = glfwGetKey(ventana, GLFW_KEY_3) == GLFW_PRESS;
        if (tecla3 && !tecla3Anterior) baileBmoActivo = !baileBmoActivo;
        tecla3Anterior = tecla3;
        GLfloat metaPanelesISS = panelesDesplegados ? 1.0f : 0.05f;
        factorPanelesISS += glm::clamp(metaPanelesISS - factorPanelesISS, -0.6f * deltaTime, 0.6f * deltaTime);

        // ========================================
        // COHETE
        // ========================================
        // Vuelo (cuenta regresiva, ascenso, ruta de la ISS y regreso), temblor y humo
        const glm::vec3 posicionPlataformaMundo = glm::vec3(matrizSpacePark * glm::vec4(-0.9f, 0.81f, -0.42f, 1.0f));
        const GLfloat tiempoRuido = (GLfloat)glfwGetTime();
        const glm::vec3 posicionCohete = ActualizarVueloCohete(posicionPlataformaMundo);
        const glm::mat4 matrizCohete = CalcularMatrizCohete(posicionCohete, tiempoRuido);
        const GLfloat escalaMundoCohete = escalaPlaneta * 0.08f;
        ActualizarHumoCohete(matrizCohete, posicionPlataformaMundo, tiempoRuido);

        // ========================================
        // SALIDA DE LA CARRERA DE HALO
        // ========================================
        // R inicia la cuenta regresiva del semaforo; al terminar arranca la carrera
        if (modoCamara == 3 && glfwGetKey(ventana, GLFW_KEY_R) == GLFW_PRESS && !carreraIniciada && !cuentaSalidaActiva)
        {
            cuentaSalidaActiva = true;
            tiempoSalidaHalo = 0.0f;
        }
        if (cuentaSalidaActiva || carreraIniciada)
        {
            tiempoSalidaHalo += deltaTime;
            if (cuentaSalidaActiva && tiempoSalidaHalo >= DURACION_CUENTA_SALIDA)
            {
                cuentaSalidaActiva = false;
                carreraIniciada = true;
            }
        }

        GLfloat velocidadPrincipal = 50.0f;
        bool acelerandoW = false;

        if (carreraIniciada)
        {
            if (modoCamara == 3)
            {
                if (glfwGetKey(ventana, GLFW_KEY_W) == GLFW_PRESS)
                {
                    velocidadPrincipal = 90.0f;
                    acelerandoW = true;
                }
                if (glfwGetKey(ventana, GLFW_KEY_A) == GLFW_PRESS)
                {
                    lateralPrincipal -= 15.0f * deltaTime;
                }
                if (glfwGetKey(ventana, GLFW_KEY_D) == GLFW_PRESS)
                {
                    lateralPrincipal += 15.0f * deltaTime;
                }
            }

            lateralPrincipal = glm::clamp(lateralPrincipal, -4.0f, 4.0f);

            fasePrincipal += velocidadPrincipal * deltaTime;
            faseNave1 += velNave1 * deltaTime;
            faseNave2 += velNave2 * deltaTime;
            faseNave3 += velNave3 * deltaTime;
            faseNave4 += velNave4 * deltaTime;
        }

        glm::mat4 matNavePrincipal = MoverNave(fasePrincipal, lateralPrincipal, matrizBasePista);
        matNavePrincipal = glm::translate(matNavePrincipal, glm::vec3(0.0f, -6.0f + alturaPrincipal, 0.0f));

        GLfloat movimientoMouseX = mainWindow.getXChange();
        GLfloat movimientoMouseY = mainWindow.getYChange();

        bool plasmaRocketActivo = false;

        if (modoCamara == 0)
        {
            anguloCamaraFantasmano += movimientoMouseX * 0.5f;
            inclinacionCamaraFantasmano += movimientoMouseY * 0.5f;
            inclinacionCamaraFantasmano = glm::clamp(inclinacionCamaraFantasmano, -80.0f, 80.0f);

            float anguloRad = glm::radians(anguloCamaraFantasmano);
            glm::vec3 adelante = glm::normalize(glm::vec3(cos(anguloRad), 0.0f, sin(anguloRad)));
            glm::vec3 derecha = glm::normalize(glm::cross(adelante, glm::vec3(0.0f, 1.0f, 0.0f)));

            glm::vec3 movimientoFantasmano(0.0f);

            if (glfwGetKey(ventana, GLFW_KEY_W) == GLFW_PRESS) movimientoFantasmano += adelante;
            if (glfwGetKey(ventana, GLFW_KEY_S) == GLFW_PRESS) movimientoFantasmano -= adelante;
            if (glfwGetKey(ventana, GLFW_KEY_A) == GLFW_PRESS) movimientoFantasmano -= derecha;
            if (glfwGetKey(ventana, GLFW_KEY_D) == GLFW_PRESS) movimientoFantasmano += derecha;

            if (glm::length(movimientoFantasmano) > 0.0f)
            {
                movimientoFantasmano = glm::normalize(movimientoFantasmano);
                posicionFantasmano += movimientoFantasmano * velocidadFantasmano * deltaTime;
                direccionFantasmano = glm::degrees(atan2(movimientoFantasmano.x, movimientoFantasmano.z));
            }

            posicionFantasmano.x = glm::clamp(posicionFantasmano.x, -1.35f, 1.35f);
            posicionFantasmano.z = glm::clamp(posicionFantasmano.z, -1.2f, 1.35f);

            glm::vec3 posicionRealFantasmano = glm::vec3(matrizSpacePark * glm::vec4(posicionFantasmano, 1.0f));
            GLfloat distanciaCamara = 42.0f;
            GLfloat inclinacionOrbita = glm::min(inclinacionCamaraFantasmano, 0.0f);

            glm::vec3 direccionVista(
                cos(glm::radians(inclinacionOrbita)) * cos(glm::radians(anguloCamaraFantasmano)),
                sin(glm::radians(inclinacionOrbita)),
                cos(glm::radians(inclinacionOrbita)) * sin(glm::radians(anguloCamaraFantasmano))
            );

            glm::vec3 posicionCamara = posicionRealFantasmano - direccionVista * distanciaCamara;

            camera = Camera(posicionCamara, glm::vec3(0.0f, 1.0f, 0.0f), anguloCamaraFantasmano, inclinacionCamaraFantasmano, 100.0f, 0.5f);
        }
        else if (modoCamara == 1)
        {
            anguloCamaraRocket += movimientoMouseX * 0.5f;
            inclinacionCamaraRocket += movimientoMouseY * 0.5f;
            inclinacionCamaraRocket = glm::clamp(inclinacionCamaraRocket, -89.0f, 89.0f);

            glm::vec3 adelanteRocket(
                cos(glm::radians(inclinacionCamaraRocket)) * cos(glm::radians(anguloCamaraRocket)),
                sin(glm::radians(inclinacionCamaraRocket)),
                cos(glm::radians(inclinacionCamaraRocket)) * sin(glm::radians(anguloCamaraRocket))
            );
            adelanteRocket = glm::normalize(adelanteRocket);
            glm::vec3 derechaRocket = glm::normalize(glm::cross(adelanteRocket, glm::vec3(0.0f, 1.0f, 0.0f)));

            glm::vec3 movimientoRocket(0.0f);
            if (glfwGetKey(ventana, GLFW_KEY_W) == GLFW_PRESS) movimientoRocket += adelanteRocket;
            if (glfwGetKey(ventana, GLFW_KEY_S) == GLFW_PRESS) movimientoRocket -= adelanteRocket;
            if (glfwGetKey(ventana, GLFW_KEY_A) == GLFW_PRESS) movimientoRocket -= derechaRocket;
            if (glfwGetKey(ventana, GLFW_KEY_D) == GLFW_PRESS) movimientoRocket += derechaRocket;

            plasmaRocketActivo = glm::dot(movimientoRocket, movimientoRocket) > 0.0f;

            if (glfwGetKey(ventana, GLFW_KEY_SPACE) == GLFW_PRESS) movimientoRocket.y += 1.0f;
            if (glfwGetKey(ventana, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) movimientoRocket.y -= 1.0f;

            GLfloat velocidadActualRocket = velocidadRocket;
            if (glfwGetKey(ventana, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) velocidadActualRocket *= multiplicadorTurboRocket;

            if (glm::length(movimientoRocket) > 0.0f)
            {
                movimientoRocket = glm::normalize(movimientoRocket);
                posicionRocket += movimientoRocket * velocidadActualRocket * deltaTime;
            }

            glm::vec3 posicionCamaraRocket = posicionRocket - adelanteRocket * distanciaCamaraRocket + glm::vec3(0.0f, alturaCamaraRocket, 0.0f);
            camera = Camera(posicionCamaraRocket, glm::vec3(0.0f, 1.0f, 0.0f), anguloCamaraRocket, inclinacionCamaraRocket, 100.0f, 0.5f);
        }
        else if (modoCamara == 2)
        {
            anguloCamaraHalo += movimientoMouseX * 2.0f;
            inclinacionCamaraHalo += movimientoMouseY * 0.7f;
            inclinacionCamaraHalo = glm::clamp(inclinacionCamaraHalo, -80.0f, 80.0f);

            float anguloRadHalo = glm::radians(anguloCamaraHalo);
            glm::vec3 adelanteHalo = glm::normalize(glm::vec3(cos(anguloRadHalo), 0.0f, sin(anguloRadHalo)));
            glm::vec3 derechaHalo = glm::normalize(glm::cross(adelanteHalo, glm::vec3(0.0f, 1.0f, 0.0f)));

            glm::vec3 movimientoHalo(0.0f);
            if (glfwGetKey(ventana, GLFW_KEY_W) == GLFW_PRESS) movimientoHalo += adelanteHalo;
            if (glfwGetKey(ventana, GLFW_KEY_S) == GLFW_PRESS) movimientoHalo -= adelanteHalo;
            if (glfwGetKey(ventana, GLFW_KEY_A) == GLFW_PRESS) movimientoHalo -= derechaHalo;
            if (glfwGetKey(ventana, GLFW_KEY_D) == GLFW_PRESS) movimientoHalo += derechaHalo;

            if (glm::length(movimientoHalo) > 0.0f)
            {
                movimientoHalo = glm::normalize(movimientoHalo);
                GLfloat escalaSpacePark = glm::length(glm::vec3(matrizSpacePark[0]));
                GLfloat velocidadRealHalo = velocidadFantasmano * escalaSpacePark * factorVelocidadHalo;
                posicionFantasmanoHalo += movimientoHalo * velocidadRealHalo * deltaTime;
                direccionFantasmanoHalo = glm::degrees(atan2(movimientoHalo.x, movimientoHalo.z));
            }

            posicionFantasmanoHalo.x = glm::clamp(posicionFantasmanoHalo.x, -225.0f, 225.0f);
            posicionFantasmanoHalo.z = glm::clamp(posicionFantasmanoHalo.z, -125.0f, 125.0f);

            glm::vec3 posicionRealHalo = ORIGEN_HALO + posicionFantasmanoHalo;
            glm::vec3 objetivoCamaraHalo = posicionRealHalo + glm::vec3(0.0f, alturaCamaraHalo, 0.0f);
            GLfloat inclinacionOrbitaHalo = glm::min(inclinacionCamaraHalo, 0.0f);
            glm::vec3 direccionVistaHalo(
                cos(glm::radians(inclinacionOrbitaHalo)) * cos(glm::radians(anguloCamaraHalo)),
                sin(glm::radians(inclinacionOrbitaHalo)),
                cos(glm::radians(inclinacionOrbitaHalo)) * sin(glm::radians(anguloCamaraHalo))
            );
            glm::vec3 posicionCamaraHalo = objetivoCamaraHalo - direccionVistaHalo * distanciaCamaraHalo;
            camera = Camera(posicionCamaraHalo, glm::vec3(0.0f, 1.0f, 0.0f), anguloCamaraHalo, inclinacionCamaraHalo, 150.0f, 0.5f);
        }
        else if (modoCamara == 3)
        {
            const GLfloat distanciaAtrasCarrera = 15.0f;
            const GLfloat alturaCamaraCarrera = 4.0f;
            glm::vec3 posicionCamaraCarrera = glm::vec3(matNavePrincipal * glm::vec4(0.0f, alturaCamaraCarrera, -distanciaAtrasCarrera, 1.0f));
            glm::vec3 objetivoCamaraCarrera = glm::vec3(matNavePrincipal * glm::vec4(0.0f, 0.5f, 15.0f, 1.0f));

            glm::vec3 direccionVistaCarrera = glm::normalize(objetivoCamaraCarrera - posicionCamaraCarrera);
            GLfloat pitchCarrera = glm::degrees(asin(glm::clamp(direccionVistaCarrera.y, -1.0f, 1.0f)));
            GLfloat yawCarrera = glm::degrees(atan2(direccionVistaCarrera.z, direccionVistaCarrera.x));

            camera = Camera(posicionCamaraCarrera, glm::vec3(0.0f, 1.0f, 0.0f), yawCarrera, pitchCarrera, 100.0f, 0.5f);
        }
        else if (modoCamara == 4)
        {
            // Camara de observacion: durante el lanzamiento va fija junto a la plataforma y sigue al cohete;
            // el resto del tiempo mira desde un costado del puerto de la estacion a la nave que llega, se acopla o sale
            glm::vec3 posicionObservador = glm::vec3(matrizSpacePark * glm::vec4(-0.35f, 0.85f, 0.05f, 1.0f));
            glm::vec3 objetivoObservador = posicionCoheteMundo + glm::vec3(0.0f, 8.0f, 0.0f);
            if (estadoCohete == 0 && glm::length(objetivoObservacionEstacion) > 0.0f)
            {
                posicionObservador = puertoAcoplamientoMundo + salidaAcoplamientoMundo * 85.0f + lateralEstacionMundo * 95.0f + glm::vec3(0.0f, 35.0f, 0.0f);
                objetivoObservador = objetivoObservacionEstacion;
            }

            glm::vec3 direccionObservador = glm::normalize(objetivoObservador - posicionObservador);
            GLfloat pitchObservador = glm::degrees(asin(glm::clamp(direccionObservador.y, -1.0f, 1.0f)));
            GLfloat yawObservador = glm::degrees(atan2(direccionObservador.z, direccionObservador.x));

            camera = Camera(posicionObservador, glm::vec3(0.0f, 1.0f, 0.0f), yawObservador, pitchObservador, 100.0f, 0.5f);
        }
        else if (modoCamara == 5)
        {
            // Camara de seguimiento: va detras del cohete y un poco arriba mientras vuela
            glm::vec3 horizontalVuelo(direccionVueloCohete.x, 0.0f, direccionVueloCohete.z);
            if (estadoCohete == 2 && glm::length(horizontalVuelo) > 0.2f)
            {
                horizontalVuelo = -glm::normalize(horizontalVuelo);
                direccionHorizontalSeguimiento = glm::normalize(glm::mix(direccionHorizontalSeguimiento, horizontalVuelo, glm::min(1.0f, 1.5f * deltaTime)));
            }
            else if (estadoCohete != 2)
            {
                direccionHorizontalSeguimiento = glm::vec3(0.0f, 0.0f, 1.0f);
            }
            glm::vec3 posicionSeguimiento = posicionCoheteMundo + direccionHorizontalSeguimiento * 200.0f + glm::vec3(0.0f, 70.0f, 0.0f);
            glm::vec3 objetivoSeguimiento = posicionCoheteMundo + glm::vec3(0.0f, 20.0f, 0.0f);

            glm::vec3 direccionSeguimiento = glm::normalize(objetivoSeguimiento - posicionSeguimiento);
            GLfloat pitchSeguimiento = glm::degrees(asin(glm::clamp(direccionSeguimiento.y, -1.0f, 1.0f)));
            GLfloat yawSeguimiento = glm::degrees(atan2(direccionSeguimiento.z, direccionSeguimiento.x));

            camera = Camera(posicionSeguimiento, glm::vec3(0.0f, 1.0f, 0.0f), yawSeguimiento, pitchSeguimiento, 100.0f, 0.5f);
        }

        tiempoFantasmano += deltaTime;
        GLfloat flotacionFantasmano = sin(tiempoFantasmano * 3.5f) * 0.01f;
        anguloBrazoFantasmano = sin(tiempoFantasmano * 5.0f) * 25.0f;
        anguloManoFantasmano = sin(tiempoFantasmano * 5.0f) * 15.0f;

        bool teclaC = glfwGetKey(ventana, GLFW_KEY_C) == GLFW_PRESS;
        if (teclaC && !teclaCarruselAnterior) carruselActivo = !carruselActivo;
        teclaCarruselAnterior = teclaC;

        bool teclaL = glfwGetKey(ventana, GLFW_KEY_L) == GLFW_PRESS;
        if (teclaL && !teclaLucesAnterior) lucesSpotTeclado = !lucesSpotTeclado;
        teclaLucesAnterior = teclaL;
        GLfloat intSpotTeclado = lucesSpotTeclado ? 3.0f : 0.0f;

        // Una pulsacion de H cambia el estado una sola vez (no cada fotograma).
        const bool teclaH = glfwGetKey(ventana, GLFW_KEY_H) == GLFW_PRESS;
        if (teclaH && !teclaHAnterior) reflectorBeamEncendido = !reflectorBeamEncendido;
        teclaHAnterior = teclaH;

        anguloCicloDia += glm::radians(3.0f) * deltaTime;
        if (anguloCicloDia > glm::radians(360.0f)) anguloCicloDia -= glm::radians(360.0f);
        GLfloat alturaSol = sin(anguloCicloDia);
        // Se evalua ANTES de limitar la altura a cero, para diferenciar dia y noche.
        esDeNoche = (alturaSol < 0.0f);
        if (alturaSol < 0.0f) alturaSol = 0.0f;
        glm::vec3 direccionDia = glm::vec3(cos(anguloCicloDia), -sin(anguloCicloDia), 0.0f);

        mainLight = DirectionalLight(1.0f, 0.4f, 0.8f,
            0.15f + (0.35f * alturaSol),
            0.1f + (0.7f * alturaSol),
            direccionDia.x, direccionDia.y, direccionDia.z);

        if (carruselActivo)
        {
            anguloCarrusel += glm::radians(18.0f) * deltaTime;
            anguloCarrusel = std::fmod(anguloCarrusel, 6.283185f);
            tiempoCarrusel += deltaTime;
            tiempoCambioColor += deltaTime;
            while (tiempoCambioColor >= 2.0f)
            {
                tiempoCambioColor -= 2.0f;
                indiceColorCarrusel = (indiceColorCarrusel + 1) % 6;
            }
        }

        posicionSatelite.x -= posicionSatelite.y * 0.18f * deltaTime;
        posicionSatelite.y += posicionSatelite.x * 0.18f * deltaTime;
        posicionSatelite = glm::normalize(posicionSatelite) * 60.0f;
        giroSatelite += 0.18f * deltaTime;
        if (giroSatelite >= 6.2831853f)
            giroSatelite -= 6.2831853f;

        // Estacion Espacial: avanza por su ruta y gira lentamente sobre si misma
        anguloRutaISS += VELOCIDAD_RUTA_ISS * deltaTime;
        if (anguloRutaISS >= 6.2831853f) anguloRutaISS -= 6.2831853f;
        glm::vec3 posicionISS = PosicionRutaISS(anguloRutaISS);
        glm::mat4 matrizISS = glm::translate(glm::mat4(1.0f), posicionISS);
        matrizISS = glm::rotate(matrizISS, glm::radians((GLfloat)glfwGetTime() * 3.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        matrizISS = glm::rotate(matrizISS, glm::radians(8.0f), glm::vec3(0.0f, 0.0f, 1.0f));

        // Trafico de la estacion: naves que llegan, se acoplan al puerto y salen hacia el otro planeta
        const NaveTrafico navesTrafico[CANTIDAD_NAVES_TRAFICO] = {
            { &Ghost_M, 6.0f, 16.0f, DURACION_VISITA_NAVE * 3.0f, 1.0f },
            { &CarroPrincipal, 1.8f, 14.0f, DURACION_VISITA_NAVE * 2.0f, -1.0f },
            { &CarroRosa, 5.0f, 16.0f, DURACION_VISITA_NAVE * 1.0f, 1.0f } };
        EstadoNaveTrafico estadosTrafico[CANTIDAD_NAVES_TRAFICO];
        const GLfloat tiempoTrafico = (GLfloat)glfwGetTime();
        puertoAcoplamientoMundo = glm::vec3(matrizISS * glm::vec4(PUERTO_ACOPLAMIENTO_LOCAL, 1.0f));
        salidaAcoplamientoMundo = glm::normalize(glm::vec3(matrizISS * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f)));
        lateralEstacionMundo = glm::normalize(glm::vec3(matrizISS * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f)));
        objetivoObservacionEstacion = puertoAcoplamientoMundo;
        int naveActivaTrafico = -1;
        for (int i = 0; i < CANTIDAD_NAVES_TRAFICO; i++)
        {
            estadosTrafico[i] = CalcularNaveTrafico(navesTrafico[i], tiempoTrafico, matrizISS);
            if (estadosTrafico[i].visible) naveActivaTrafico = i;
        }
        // La camara de observacion sigue a la nave que esta usando el puerto
        if (naveActivaTrafico >= 0) objetivoObservacionEstacion = estadosTrafico[naveActivaTrafico].posicion;

        if (subeArticulacionesSatelite)
        {
            articulacionesSatelite += 1.0f * deltaTime;
            if (articulacionesSatelite >= 1.0f)
            {
                articulacionesSatelite = 1.0f;
                subeArticulacionesSatelite = false;
            }
        }
        else
        {
            articulacionesSatelite -= 1.0f * deltaTime;
            if (articulacionesSatelite <= -1.0f)
            {
                articulacionesSatelite = -1.0f;
                subeArticulacionesSatelite = true;
            }
        }

        GLfloat perimetro = 2.0f * (distanciaX + distanciaZ);
        tiempoKuromi += deltaTime * velocidadAvance;
        GLfloat distanciaActual = std::fmod(tiempoKuromi, perimetro);
        glm::vec3 posicionMundoKuromi(0.0f);
        GLfloat anguloRotacionMundoKuromi = 0.0f;
        glm::vec3 centroRecorridoKuromi = glm::vec3(-0.75f, 0.66f, 0.69f);

        if (distanciaActual < distanciaX) {
            GLfloat progreso = distanciaActual / distanciaX;
            posicionMundoKuromi = centroRecorridoKuromi + glm::vec3(-distanciaX / 2.0f + progreso * distanciaX, 0.0f, -distanciaZ / 2.0f);
            anguloRotacionMundoKuromi = 90.0f;
        }
        else if (distanciaActual < distanciaX + distanciaZ) {
            GLfloat progreso = (distanciaActual - distanciaX) / distanciaZ;
            posicionMundoKuromi = centroRecorridoKuromi + glm::vec3(distanciaX / 2.0f, 0.0f, -distanciaZ / 2.0f + progreso * distanciaZ);
            anguloRotacionMundoKuromi = 0.0f;
        }
        else if (distanciaActual < 2.0f * distanciaX + distanciaZ) {
            GLfloat progreso = (distanciaActual - (distanciaX + distanciaZ)) / distanciaX;
            posicionMundoKuromi = centroRecorridoKuromi + glm::vec3(distanciaX / 2.0f - progreso * distanciaX, 0.0f, distanciaZ / 2.0f);
            anguloRotacionMundoKuromi = -90.0f;
        }
        else {
            GLfloat progreso = (distanciaActual - (2.0f * distanciaX + distanciaZ)) / distanciaZ;
            posicionMundoKuromi = centroRecorridoKuromi + glm::vec3(-distanciaX / 2.0f, 0.0f, distanciaZ / 2.0f - progreso * distanciaZ);
            anguloRotacionMundoKuromi = 180.0f;
        }
        GLfloat anguloPiernas = sin(glfwGetTime() * velocidadCaminata * 3.14159f) * 25.0f;
        glm::mat4 matrizKuromi = matrizSpacePark;
        matrizKuromi = glm::translate(matrizKuromi, posicionMundoKuromi);
        matrizKuromi = glm::rotate(matrizKuromi, glm::radians(anguloRotacionMundoKuromi), glm::vec3(0.0f, 1.0f, 0.0f));
        matrizKuromi = glm::scale(matrizKuromi, glm::vec3(0.005f, 0.005f, 0.005f));
        glm::mat4 matrizCuerpoKuromi = matrizKuromi;
        glm::mat4 matrizPiernaIzqKuromi = matrizKuromi;
        matrizPiernaIzqKuromi = glm::rotate(matrizPiernaIzqKuromi, glm::radians(anguloPiernas), glm::vec3(1.0f, 0.0f, 0.0f));
        glm::mat4 matrizPiernaDerKuromi = matrizKuromi;
        matrizPiernaDerKuromi = glm::rotate(matrizPiernaDerKuromi, glm::radians(-anguloPiernas), glm::vec3(1.0f, 0.0f, 0.0f));

        anguloHombroRigby += 45.0f * deltaTime * direccionHombroRigby;
        if (anguloHombroRigby > 25.0f) { anguloHombroRigby = 25.0f; direccionHombroRigby = -1.0f; }
        else if (anguloHombroRigby < -20.0f) { anguloHombroRigby = -20.0f; direccionHombroRigby = 1.0f; }
        anguloCodoRigby += 60.0f * deltaTime * direccionCodoRigby;
        if (anguloCodoRigby > 90.0f) { anguloCodoRigby = 90.0f; direccionCodoRigby = -1.0f; }
        else if (anguloCodoRigby < 0.0f) { anguloCodoRigby = 0.0f; direccionCodoRigby = 1.0f; }

        glm::mat4 matrizFantasmano = matrizSpacePark;
        matrizFantasmano = glm::translate(matrizFantasmano, posicionFantasmano + glm::vec3(0.0f, flotacionFantasmano, 0.0f));
        matrizFantasmano = glm::rotate(matrizFantasmano, glm::radians(direccionFantasmano), glm::vec3(0.0f, 1.0f, 0.0f));
        matrizFantasmano = glm::scale(matrizFantasmano, glm::vec3(0.002f));
        glm::mat4 matrizBrazoFantasmano = matrizFantasmano;
        glm::vec3 pivoteBrazoFantasmano(0.0f, 0.0f, 0.0f);
        matrizBrazoFantasmano = glm::translate(matrizBrazoFantasmano, glm::vec3(0.0f, 3.0f, -0.5f));
        matrizBrazoFantasmano = glm::translate(matrizBrazoFantasmano, pivoteBrazoFantasmano);
        matrizBrazoFantasmano = glm::rotate(matrizBrazoFantasmano, glm::radians(anguloBrazoFantasmano), glm::vec3(1.0f, 0.0f, 0.0f));
        matrizBrazoFantasmano = glm::translate(matrizBrazoFantasmano, -pivoteBrazoFantasmano);
        glm::mat4 matrizManoFantasmano = matrizBrazoFantasmano;
        matrizManoFantasmano = glm::translate(matrizManoFantasmano, glm::vec3(0.0f, 1.4f, 0.0f));
        matrizManoFantasmano = glm::rotate(matrizManoFantasmano, glm::radians(anguloManoFantasmano), glm::vec3(0.0f, 1.0f, 0.0f));

        // ========================================
        // HORA DE AVENTURA
        // ========================================
        // La plaza esta en Halo: las posiciones de los personajes (en unidades del parque) se llevan a Halo con esta matriz
        const glm::vec3 origenPlazaHalo = ORIGEN_HALO + glm::vec3(CASA_ARBOL_HALO.x, posicionFantasmanoHalo.y, CASA_ARBOL_HALO.z);
        const glm::mat4 matrizPlazaHalo = glm::translate(glm::mat4(1.0f), origenPlazaHalo) * glm::scale(glm::mat4(1.0f), glm::vec3(escalaPlaneta)) *
            glm::translate(glm::mat4(1.0f), -POSICION_CASA_ARBOL);
        glm::mat4 matrizCasaArbolHalo = glm::translate(matrizPlazaHalo, POSICION_CASA_ARBOL);
        matrizCasaArbolHalo = glm::rotate(matrizCasaArbolHalo, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        matrizCasaArbolHalo = glm::scale(matrizCasaArbolHalo, glm::vec3(ESCALA_CASA_ARBOL));
        // Posicion del avatar de Halo en las mismas unidades del parque (los personajes lo miran y reaccionan si se acerca)
        const glm::vec3 posicionAvatarPlaza = glm::vec3(posicionFantasmanoHalo.x - CASA_ARBOL_HALO.x, 0.0f, posicionFantasmanoHalo.z - CASA_ARBOL_HALO.z) / escalaPlaneta + POSICION_CASA_ARBOL;
        tiempoHoraAventura += deltaTime;
        const GLfloat escalaHoraAventura = 0.005f;
        const glm::vec2 posicionPlanoFantasmano(posicionAvatarPlaza.x, posicionAvatarPlaza.z);
        const bool cercaBmo = glm::distance(posicionPlanoFantasmano, glm::vec2(posicionBmo.x, posicionBmo.z)) < distanciaSaludo;
        const bool cercaGunter = glm::distance(posicionPlanoFantasmano, glm::vec2(posicionGunter.x, posicionGunter.z)) < distanciaSaludo;
        const bool cercaTreeTrunks = glm::distance(posicionPlanoFantasmano, glm::vec2(posicionTreeTrunks.x, posicionTreeTrunks.z)) < distanciaSaludo;
        const bool cercaNeptr = glm::distance(posicionPlanoFantasmano, glm::vec2(posicionNeptr.x, posicionNeptr.z)) < distanciaSaludo;

        // Jake camina en circulo frente a la casa: patas, brazos y cola con la misma fase
        anguloRecorridoJake += velocidadRecorridoJake * deltaTime;
        if (anguloRecorridoJake > 6.283185f) anguloRecorridoJake -= 6.283185f;
        const glm::vec3 posicionJake = centroHoraAventura + glm::vec3(radioRecorridoJake * cos(anguloRecorridoJake), 0.0f, radioRecorridoJake * sin(anguloRecorridoJake));
        const GLfloat rumboJake = atan2(-sin(anguloRecorridoJake), cos(anguloRecorridoJake));
        const GLfloat pasoJake = sin(tiempoHoraAventura * 9.0f);
        const GLfloat balanceoJake = 38.0f * pasoJake;
        glm::mat4 matrizJake = matrizPlazaHalo;
        matrizJake = glm::translate(matrizJake, posicionJake);
        matrizJake = glm::rotate(matrizJake, rumboJake, glm::vec3(0.0f, 1.0f, 0.0f));
        matrizJake = glm::scale(matrizJake, glm::vec3(escalaHoraAventura));
        matrizJake = glm::translate(matrizJake, glm::vec3(0.0f, 0.1f * fabs(pasoJake), 0.0f));
        const glm::mat4 matrizBrazoIzqJake = CrearMatrizArticulacion(matrizJake, pivoteBrazoIzqJake, -balanceoJake, glm::vec3(1.0f, 0.0f, 0.0f));
        const glm::mat4 matrizBrazoDerJake = CrearMatrizArticulacion(matrizJake, pivoteBrazoDerJake, balanceoJake, glm::vec3(1.0f, 0.0f, 0.0f));
        const glm::mat4 matrizPataIzqJake = CrearMatrizArticulacion(matrizJake, pivotePataIzqJake, balanceoJake, glm::vec3(1.0f, 0.0f, 0.0f));
        const glm::mat4 matrizPataDerJake = CrearMatrizArticulacion(matrizJake, pivotePataDerJake, -balanceoJake, glm::vec3(1.0f, 0.0f, 0.0f));
        const glm::mat4 matrizColaJake = CrearMatrizArticulacion(matrizJake, pivoteColaJake, 25.0f * sin(tiempoHoraAventura * 13.5f), glm::vec3(0.0f, 1.0f, 0.0f));

        // BMO baila con la tecla 3 (o cuando Fantasmano se acerca): brinca, gira y mueve brazos y piernas
        const bool bailaBmo = baileBmoActivo || cercaBmo;
        const GLfloat baileBmo = sin(7.0f * tiempoHoraAventura);
        glm::mat4 matrizBmo = CrearMatrizPersonajeHoraAventura(matrizPlazaHalo, posicionBmo, posicionAvatarPlaza, escalaHoraAventura);
        if (bailaBmo)
        {
            matrizBmo = glm::translate(matrizBmo, glm::vec3(0.0f, 0.25f * fabs(baileBmo), 0.0f));
            matrizBmo = glm::rotate(matrizBmo, glm::radians(18.0f * sin(3.5f * tiempoHoraAventura)), glm::vec3(0.0f, 1.0f, 0.0f));
        }
        const glm::mat4 matrizBrazoIzqBmo = CrearMatrizArticulacion(matrizBmo, pivoteBrazoIzqBmo, bailaBmo ? -(70.0f + 45.0f * baileBmo) : 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
        const glm::mat4 matrizBrazoDerBmo = CrearMatrizArticulacion(matrizBmo, pivoteBrazoDerBmo, bailaBmo ? (70.0f - 45.0f * baileBmo) : 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
        const glm::mat4 matrizPataIzqBmo = CrearMatrizArticulacion(matrizBmo, pivotePataIzqBmo, bailaBmo ? 30.0f * baileBmo : 0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
        const glm::mat4 matrizPataDerBmo = CrearMatrizArticulacion(matrizBmo, pivotePataDerBmo, bailaBmo ? -30.0f * baileBmo : 0.0f, glm::vec3(1.0f, 0.0f, 0.0f));

        // Gunter aletea cuando Fantasmano se acerca
        const GLfloat aleteoGunter = cercaGunter ? 40.0f + 30.0f * sin(10.0f * tiempoHoraAventura) : 5.0f;
        const glm::mat4 matrizGunter = CrearMatrizPersonajeHoraAventura(matrizPlazaHalo, posicionGunter, posicionAvatarPlaza, escalaHoraAventura);
        const glm::mat4 matrizAlaIzqGunter = CrearMatrizArticulacion(matrizGunter, pivoteAlaIzqGunter, -aleteoGunter, glm::vec3(0.0f, 0.0f, 1.0f));
        const glm::mat4 matrizAlaDerGunter = CrearMatrizArticulacion(matrizGunter, pivoteAlaDerGunter, aleteoGunter, glm::vec3(0.0f, 0.0f, 1.0f));

        // Tree Trunks camina en su lugar y mueve orejas, trompa y cola cuando Fantasmano se acerca
        const GLfloat paseoTreeTrunks = sin(6.0f * tiempoHoraAventura);
        const GLfloat caminataTreeTrunks = cercaTreeTrunks ? 28.0f * paseoTreeTrunks : 0.0f;
        const GLfloat aleteoOrejaTreeTrunks = cercaTreeTrunks ? 25.0f * sin(8.0f * tiempoHoraAventura) : 4.0f * sin(1.5f * tiempoHoraAventura);
        glm::mat4 matrizTreeTrunks = CrearMatrizPersonajeHoraAventura(matrizPlazaHalo, posicionTreeTrunks, posicionAvatarPlaza, escalaHoraAventura);
        if (cercaTreeTrunks) matrizTreeTrunks = glm::translate(matrizTreeTrunks, glm::vec3(0.0f, 0.06f * fabs(paseoTreeTrunks), 0.0f));
        const glm::mat4 matrizPataDelIzqTreeTrunks = CrearMatrizArticulacion(matrizTreeTrunks, pivotePataDelIzqTreeTrunks, caminataTreeTrunks, glm::vec3(1.0f, 0.0f, 0.0f));
        const glm::mat4 matrizPataTrasDerTreeTrunks = CrearMatrizArticulacion(matrizTreeTrunks, pivotePataTrasDerTreeTrunks, caminataTreeTrunks, glm::vec3(1.0f, 0.0f, 0.0f));
        const glm::mat4 matrizPataDelDerTreeTrunks = CrearMatrizArticulacion(matrizTreeTrunks, pivotePataDelDerTreeTrunks, -caminataTreeTrunks, glm::vec3(1.0f, 0.0f, 0.0f));
        const glm::mat4 matrizPataTrasIzqTreeTrunks = CrearMatrizArticulacion(matrizTreeTrunks, pivotePataTrasIzqTreeTrunks, -caminataTreeTrunks, glm::vec3(1.0f, 0.0f, 0.0f));
        const glm::mat4 matrizOrejaIzqTreeTrunks = CrearMatrizArticulacion(matrizTreeTrunks, pivoteOrejaIzqTreeTrunks, aleteoOrejaTreeTrunks, glm::vec3(0.0f, 1.0f, 0.0f));
        const glm::mat4 matrizOrejaDerTreeTrunks = CrearMatrizArticulacion(matrizTreeTrunks, pivoteOrejaDerTreeTrunks, -aleteoOrejaTreeTrunks, glm::vec3(0.0f, 1.0f, 0.0f));
        const glm::mat4 matrizTrompaTreeTrunks = CrearMatrizArticulacion(matrizTreeTrunks, pivoteTrompaTreeTrunks, 15.0f * sin((cercaTreeTrunks ? 5.0f : 1.2f) * tiempoHoraAventura), glm::vec3(1.0f, 0.0f, 0.0f));
        const glm::mat4 matrizColaTreeTrunks = CrearMatrizArticulacion(matrizTreeTrunks, pivoteColaTreeTrunks, 30.0f * sin(4.0f * tiempoHoraAventura), glm::vec3(0.0f, 0.0f, 1.0f));

        // NEPTR avanza, retrocede y gira de un lado a otro cuando Fantasmano se acerca
        glm::mat4 matrizNeptr = CrearMatrizPersonajeHoraAventura(matrizPlazaHalo, posicionNeptr, posicionAvatarPlaza, escalaHoraAventura);
        if (cercaNeptr)
        {
            matrizNeptr = glm::translate(matrizNeptr, glm::vec3(0.0f, 0.0f, 0.8f * sin(2.0f * tiempoHoraAventura)));
            matrizNeptr = glm::rotate(matrizNeptr, glm::radians(35.0f * sin(3.0f * tiempoHoraAventura)), glm::vec3(0.0f, 1.0f, 0.0f));
        }

        glm::vec3 dirAbajo = glm::vec3(0.0f, -1.0f, 0.0f);
        glm::vec3 posLuzCamara = camera.getCameraPosition();
        posLuzCamara.y -= 0.3f;
        spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
            lucesSpotTeclado ? 0.3f : 0.0f, lucesSpotTeclado ? 3.5f : 0.0f,
            posLuzCamara.x, posLuzCamara.y, posLuzCamara.z,
            camera.getCameraDirection().x, camera.getCameraDirection().y, camera.getCameraDirection().z,
            1.0f, 0.01f, 0.01f, 15.0f);
        spotLights[0].SetFlash(posLuzCamara, camera.getCameraDirection());

        glm::mat4 matrizBase = matrizSpacePark;
        matrizBase = glm::translate(matrizBase, glm::vec3(0.78f, 0.675f, -0.67f));
        matrizBase = glm::scale(matrizBase, glm::vec3(0.007f, 0.007f, 0.007f));
        matrizBase = glm::rotate(matrizBase, 34.4f, glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 matrizSoporte = matrizSpacePark;
        matrizSoporte = glm::translate(matrizSoporte, glm::vec3(0.78f, 0.675f, -0.67f));
        matrizSoporte = glm::scale(matrizSoporte, glm::vec3(0.007f, 0.007f, 0.007f));
        matrizSoporte = glm::rotate(matrizSoporte, 34.4f + anguloCarrusel, glm::vec3(0.0f, 1.0f, 0.0f));
        float amplitud = 0.010f;
        float velocidad = 3.0f;
        float desfase = 3.14159f / 4.0f;
        glm::mat4 matricesCaballos[8];
        for (int i = 0; i < 8; i++) {
            float offsetY = sin(tiempoCarrusel * velocidad + (i * desfase)) * amplitud;
            matricesCaballos[i] = matrizSpacePark;
            matricesCaballos[i] = glm::translate(matricesCaballos[i], glm::vec3(0.78f, 0.683f + offsetY, -0.67f));
            matricesCaballos[i] = glm::scale(matricesCaballos[i], glm::vec3(0.007f, 0.007f, 0.007f));
            matricesCaballos[i] = glm::rotate(matricesCaballos[i], 34.4f + anguloCarrusel, glm::vec3(0.0f, 1.0f, 0.0f));
        }
        glm::vec3 colorFaro = COLORES_CARRUSEL[indiceColorCarrusel];
        glm::vec3 posicionLuzCarrusel = glm::vec3(matrizBase * glm::vec4(0.0f, 20.0f, 0.0f, 1.0f));
        glm::vec3 direccionLuzCarrusel = glm::vec3(0.0f, -1.0f, 0.0f);
        spotLights[1] = SpotLight(
            colorFaro.r, colorFaro.g, colorFaro.b,
            0.2f, carruselActivo ? 4.0f : 0.0f,
            posicionLuzCarrusel.x, posicionLuzCarrusel.y, posicionLuzCarrusel.z,
            direccionLuzCarrusel.x, direccionLuzCarrusel.y, direccionLuzCarrusel.z,
            1.0f, 0.02f, 0.01f, 60.0f
        );
        spotLights[1].SetFlash(posicionLuzCarrusel, direccionLuzCarrusel);

        glm::vec3 posicionLuzLinterna = glm::vec3(matrizCuerpoKuromi * glm::vec4(1.0f, 1.0f, 1.5f, 1.0f));
        glm::vec3 direccionLuzLinterna = glm::normalize(glm::vec3(matrizCuerpoKuromi * glm::vec4(0.0f, -0.5f, 1.0f, 0.0f)));
        spotLights[2] = SpotLight(1.0f, 1.0f, 1.0f, lucesSpotTeclado ? 0.2f : 0.0f, lucesSpotTeclado ? 2.0f : 0.0f, posicionLuzLinterna.x, posicionLuzLinterna.y, posicionLuzLinterna.z,
            direccionLuzLinterna.x, direccionLuzLinterna.y, direccionLuzLinterna.z, 1.0f, 0.09f, 0.032f, 20.0f);
        spotLights[2].SetFlash(posicionLuzLinterna, direccionLuzLinterna);

        spotLightCount = 3;

        glm::mat4 matrizCasaParque = matrizSpacePark;
        matrizCasaParque = glm::translate(matrizCasaParque, glm::vec3(0.42f, 0.625f, 0.92f));
        matrizCasaParque = glm::scale(matrizCasaParque, glm::vec3(0.006f));

        glm::mat4 matrizTienda = matrizSpacePark; matrizTienda = glm::translate(matrizTienda, glm::vec3(0.8f, 0.66f, 0.48f));
        glm::mat4 matrizPlataforma = matrizSpacePark; matrizPlataforma = glm::translate(matrizPlataforma, glm::vec3(-0.9f, 0.66f, -0.42f));
        glm::mat4 matrizIglesia = matrizSpacePark; matrizIglesia = glm::translate(matrizIglesia, glm::vec3(0.0f, 0.66f, -0.49f));

        glm::vec3 centroP1 = glm::vec3(0.0f, -1.1f, 1.15f); glm::vec3 luzProp1 = glm::vec3(matrizSpacePark * glm::vec4(centroP1, 1.0f));
        glm::vec3 centroP2 = glm::vec3(0.0f, -1.1f, -1.15f); glm::vec3 luzProp2 = glm::vec3(matrizSpacePark * glm::vec4(centroP2, 1.0f));
        glm::vec3 centroP3 = glm::vec3(1.15f, -1.1f, 0.0f); glm::vec3 luzProp3 = glm::vec3(matrizSpacePark * glm::vec4(centroP3, 1.0f));
        glm::vec3 centroP4 = glm::vec3(-1.15f, -1.1f, 0.0f); glm::vec3 luzProp4 = glm::vec3(matrizSpacePark * glm::vec4(centroP4, 1.0f));
        pointLights[0] = PointLight(0.12f, 0.50f, 1.0f, 0.6f, 4.0f, luzProp1.x, luzProp1.y, luzProp1.z, 1.0f, 0.05f, 0.01f);
        pointLights[1] = PointLight(0.12f, 0.50f, 1.0f, 0.6f, 4.0f, luzProp2.x, luzProp2.y, luzProp2.z, 1.0f, 0.05f, 0.01f);
        pointLights[2] = PointLight(0.12f, 0.50f, 1.0f, 0.6f, 4.0f, luzProp3.x, luzProp3.y, luzProp3.z, 1.0f, 0.05f, 0.01f);
        pointLights[3] = PointLight(0.12f, 0.50f, 1.0f, 0.6f, 4.0f, luzProp4.x, luzProp4.y, luzProp4.z, 1.0f, 0.05f, 0.01f);

        // Luz iglesia
        glm::mat4 matrizLuzIglesia = glm::scale(matrizIglesia, glm::vec3(0.015f, 0.015f, 0.015f));
        glm::vec3 posicionLuzIglesia = glm::vec3(matrizLuzIglesia * glm::vec4(-7.0f, 6.0f, 0.0f, 1.0f));
        glm::vec3 direccionLuzIglesia = glm::vec3(0.0f, -1.0f, 0.0f);
        spotLights[3] = SpotLight(
            1.0f, 0.9f, 0.6f,
            esDeNoche ? 0.8f : 0.0f, esDeNoche ? 1.0f : 0.0f,
            posicionLuzIglesia.x, posicionLuzIglesia.y, posicionLuzIglesia.z,
            direccionLuzIglesia.x, direccionLuzIglesia.y, direccionLuzIglesia.z,
            1.0f, 0.01f, 0.005f,
            60.0f
        );
        spotLights[3].SetFlash(posicionLuzIglesia, direccionLuzIglesia);
        spotLightCount = 4;

        // Luz Plataforma
        glm::vec3 posicionLuzPlataforma = glm::vec3(matrizLuzIglesia * glm::vec4(-60.0f, 6.0f, 5.0f, 1.0f));
        glm::vec3 direccionLuzPlataforma = glm::vec3(0.0f, -1.0f, 0.0f);
        spotLights[4] = SpotLight(
            1.0f, 0.0f, 0.0f,
            esDeNoche ? 3.0f : 0.0f, esDeNoche ? 1.5f : 0.0f,
            posicionLuzPlataforma.x, posicionLuzPlataforma.y, posicionLuzPlataforma.z,
            direccionLuzPlataforma.x, direccionLuzPlataforma.y, direccionLuzPlataforma.z,
            1.0f, 0.005f, 0.001f,
            90.0f
        );
        spotLights[4].SetFlash(posicionLuzPlataforma, direccionLuzPlataforma);
        spotLightCount = 5;

        // Luz Tienda
        glm::vec3 posicionLuzTienda = glm::vec3(matrizLuzIglesia * glm::vec4(50.0f, 4.0f, 65.0f, 1.0f));
        glm::vec3 direccionLuzTienda = glm::vec3(0.0f, -1.0f, 0.0f);
        spotLights[5] = SpotLight(
            1.0f, 0.0f, 1.0f,
            esDeNoche ? 0.6f : 0.0f, esDeNoche ? 2.0f : 0.0f,
            posicionLuzTienda.x, posicionLuzTienda.y, posicionLuzTienda.z,
            direccionLuzTienda.x, direccionLuzTienda.y, direccionLuzTienda.z,
            1.0f, 0.005f, 0.001f,
            50.0f
        );
        spotLights[5].SetFlash(posicionLuzTienda, direccionLuzTienda);
        spotLightCount = 6;


        glm::vec3 posicionLuzCasa = glm::vec3(matrizCasaParque * glm::vec4(-25.0f, 15.0f, -10.0f, 1.0f));
        pointLights[4] = PointLight(1.0f, 1.0f, 1.0f,
            lucesSpotTeclado ? 0.6f : 0.0f, intSpotTeclado,
            posicionLuzCasa.x, posicionLuzCasa.y, posicionLuzCasa.z, 1.0f, 0.05f, 0.01f);

        glm::mat4 matrizRenderSol = glm::mat4(1.0f);
        const GLfloat radioOrbitaSol = 10000.0f;
        const glm::vec3 posicionFisicaSol = CENTRO_ORBITA_SOL - direccionDia * radioOrbitaSol;
        matrizRenderSol = glm::translate(matrizRenderSol, posicionFisicaSol);
        matrizRenderSol = glm::scale(matrizRenderSol, glm::vec3(50.0f, 50.0f, 50.0f));

        pointLightCount = 5;


        // La ISS y los asteroides se iluminan solo con las luces normales de la escena.
        // Sin luces puntuales de navegacion intermitentes (roja, verde ni blanca).

        if (carreraIniciada && acelerandoW && pointLightCount < MAX_POINT_LIGHTS)
        {
            glm::vec3 posicionLuzNave = glm::vec3(matNavePrincipal * glm::vec4(0.0f, 0.5f, -1.0f, 1.0f));
            pointLights[pointLightCount] = PointLight(
                0.12f, 0.50f, 1.0f, 0.8f, 5.0f,
                posicionLuzNave.x, posicionLuzNave.y, posicionLuzNave.z,
                1.0f, 0.05f, 0.01f
            );
            ++pointLightCount;
        }



        if (plasmaRocketActivo && pointLightCount < MAX_POINT_LIGHTS)
        {
            const glm::vec3 avanceRocket(
                cos(glm::radians(inclinacionCamaraRocket)) * cos(glm::radians(anguloCamaraRocket)),
                sin(glm::radians(inclinacionCamaraRocket)),
                cos(glm::radians(inclinacionCamaraRocket)) * sin(glm::radians(anguloCamaraRocket))
            );
            const glm::vec3 posicionLuzRocket = posicionRocket -
                glm::normalize(avanceRocket) * (escalaRocket * -POSICION_PLASMA_ROCKET.y);

            pointLights[pointLightCount] = PointLight(
                0.12f, 0.50f, 1.0f, 0.8f, 5.0f,
                posicionLuzRocket.x, posicionLuzRocket.y, posicionLuzRocket.z,
                1.0f, 0.05f, 0.01f
            );
            ++pointLightCount;
        }

        // Luz naranja del motor del cohete mientras se enciende o asciende
        // Luz naranja de los motores del cohete: sigue a la tobera y depende de la potencia del fuego
        if (potenciaFuegoCohete > 0.02f && pointLightCount < MAX_POINT_LIGHTS)
        {
            glm::vec3 posicionLuzCohete = glm::vec3(matrizCohete * glm::vec4(0.0f, -2.0f, 0.0f, 1.0f));
            pointLights[pointLightCount] = PointLight(
                1.0f, 0.55f, 0.15f, 0.8f, 6.0f * potenciaFuegoCohete,
                posicionLuzCohete.x, posicionLuzCohete.y, posicionLuzCohete.z,
                1.0f, 0.02f, 0.002f
            );
            ++pointLightCount;
        }

        // ========================================
        // LUCES DE HALO
        // ========================================
        // Estado del semaforo de salida: luces rojas que se encienden una a una y luego verde
        const GLfloat FASE_SEMAFORO_HALO = FASE_META_HALO;
        const glm::mat4 matSemaforoHalo = MoverNave(FASE_SEMAFORO_HALO, 0.0f, matrizBasePista);
        int rojasSemaforo = 0;
        bool verdeSemaforo = false;
        if (cuentaSalidaActiva) rojasSemaforo = glm::min(3, (int)floor(tiempoSalidaHalo) + 1);
        else if (carreraIniciada && tiempoSalidaHalo < DURACION_CUENTA_SALIDA + DURACION_LUZ_VERDE) verdeSemaforo = true;
        const glm::vec3 lamparasSemaforo[3] = {
            glm::vec3(matSemaforoHalo * glm::vec4(-3.2f, -2.4f, 0.0f, 1.0f)),
            glm::vec3(matSemaforoHalo * glm::vec4(0.0f, -2.4f, 0.0f, 1.0f)),
            glm::vec3(matSemaforoHalo * glm::vec4(3.2f, -2.4f, 0.0f, 1.0f)) };

        // Solo cuando la camara esta en el planeta: las luces del parque quedan a 2500 unidades,
        // asi que se liberan sus espacios para las luces propias de Halo.
        if (glm::length(camera.getCameraPosition() - ORIGEN_HALO) < 900.0f)
        {
            const GLfloat tiempoHalo = (GLfloat)glfwGetTime();
            // 0 de dia, 1 de noche: las farolas se apagan casi por completo con el Sol alto
            const GLfloat oscuridadHalo = 1.0f - glm::clamp(alturaSol / 0.35f, 0.0f, 1.0f);

            pointLightCount = 0;
            spotLightCount = 1;   // se conserva la linterna de la camara (spotLights[0])

            // Luces de la Casa del Arbol: farol del porche y ventanas calidas. Se encienden al atardecer y se apagan de dia.
            {
                const GLfloat nocheCasaArbol = 1.0f - glm::clamp(alturaSol / 0.35f, 0.0f, 1.0f);
                const GLfloat parpadeoCasaArbol = 0.93f + 0.07f * sin(tiempoHoraAventura * 9.0f);
                const glm::vec3 posPorcheCasa = glm::vec3(matrizCasaArbolHalo * glm::vec4(0.0f, 3.9f, -7.2f, 1.0f));
                const glm::vec3 posSalaBajaCasa = glm::vec3(matrizCasaArbolHalo * glm::vec4(0.0f, 3.0f, -3.2f, 1.0f));
                const glm::vec3 posSalaAltaCasa = glm::vec3(matrizCasaArbolHalo * glm::vec4(0.0f, 7.4f, -2.6f, 1.0f));
                pointLights[pointLightCount++] = PointLight(1.0f, 0.78f, 0.40f, 0.0f, 3.2f * nocheCasaArbol * parpadeoCasaArbol,
                    posPorcheCasa.x, posPorcheCasa.y, posPorcheCasa.z, 1.0f, 0.05f, 0.004f);
                pointLights[pointLightCount++] = PointLight(1.0f, 0.65f, 0.30f, 0.0f, 2.4f * nocheCasaArbol,
                    posSalaBajaCasa.x, posSalaBajaCasa.y, posSalaBajaCasa.z, 1.0f, 0.06f, 0.006f);
                pointLights[pointLightCount++] = PointLight(1.0f, 0.65f, 0.30f, 0.0f, 2.0f * nocheCasaArbol,
                    posSalaAltaCasa.x, posSalaAltaCasa.y, posSalaAltaCasa.z, 1.0f, 0.06f, 0.006f);
            }

            // Farolas sobre la pista: siete luminarias puntuales repartidas por el circuito; se encienden de noche y se apagan de dia
            for (int i = 0; i < CANTIDAD_FAROLAS_HALO && puntosCircuito.size() > 0 && pointLightCount < MAX_POINT_LIGHTS; i++)
            {
                // El foco cuelga del brazo de la farola del Racing Kit, del lado de la pista
                const GLfloat fasePoste = (i + 0.5f) * longitudCircuito / CANTIDAD_FAROLAS_HALO;
                const GLfloat ladoPoste = (i % 2 == 0) ? -1.0f : 1.0f;
                const glm::mat4 marcoPoste = MoverNave(fasePoste, ladoPoste * 4.6f, matrizBasePista);
                const glm::vec3 posPoste = glm::vec3(marcoPoste * glm::vec4(0.0f, -9.0f + 8.3f, 0.0f, 1.0f));
                // Colores alternados cian y rosa, como la pista
                const bool rosa = (i % 2) == 1;
                // Cada farola vibra apenas distinto para que no parezcan sincronizadas
                const GLfloat titileo = 0.92f + 0.08f * sin(tiempoHalo * 7.0f + i * 2.1f);
                pointLights[pointLightCount++] = PointLight(
                    rosa ? 1.0f : 0.55f, rosa ? 0.45f : 0.95f, rosa ? 0.85f : 1.0f,
                    0.0f, 3.4f * oscuridadHalo * titileo,
                    posPoste.x, posPoste.y, posPoste.z, 1.0f, 0.05f, 0.006f);
            }

            // Reflector de la linea de salida
            {
                const glm::vec3 posSalida = glm::vec3(matSemaforoHalo * glm::vec4(0.0f, 24.0f, 0.0f, 1.0f));
                spotLights[spotLightCount] = SpotLight(1.0f, 0.95f, 0.8f,
                    0.0f, 0.2f + 1.3f * oscuridadHalo,
                    posSalida.x, posSalida.y, posSalida.z, 0.0f, -1.0f, 0.0f,
                    1.0f, 0.01f, 0.0005f, 45.0f);
                ++spotLightCount;
            }

            // Faros delanteros de la nave del jugador
            {
                const glm::vec3 posFaros = glm::vec3(matNavePrincipal * glm::vec4(0.0f, 1.0f, 3.5f, 1.0f));
                const glm::vec3 dirFaros = glm::normalize(glm::vec3(matNavePrincipal * glm::vec4(0.0f, -0.12f, 1.0f, 0.0f)));
                spotLights[spotLightCount] = SpotLight(1.0f, 0.95f, 0.85f,
                    0.0f, 0.4f + 3.0f * oscuridadHalo,
                    posFaros.x, posFaros.y, posFaros.z, dirFaros.x, dirFaros.y, dirFaros.z,
                    1.0f, 0.02f, 0.002f, 22.0f);
                ++spotLightCount;
            }

            // Semaforo de salida
            {
                GLfloat rojo = 0.0f, verde = 0.0f;
                if (rojasSemaforo > 0) rojo = 0.6f + 0.4f * (rojasSemaforo / 3.0f);
                if (verdeSemaforo) verde = 1.0f;
                if ((rojo > 0.0f || verde > 0.0f) && pointLightCount < MAX_POINT_LIGHTS)
                {
                    pointLights[pointLightCount++] = PointLight(
                        verdeSemaforo ? 0.1f : 1.0f, verdeSemaforo ? 1.0f : 0.1f, verdeSemaforo ? 0.2f : 0.1f,
                        0.0f, 9.0f * (rojo + verde),
                        lamparasSemaforo[1].x, lamparasSemaforo[1].y - 2.0f, lamparasSemaforo[1].z,
                        1.0f, 0.03f, 0.002f);
                }
            }

            // Resplandor frio del anillo de Halo que se curva sobre el cielo: respira lentamente
            for (int i = 0; i < 1 && pointLightCount < MAX_POINT_LIGHTS; i++)
            {
                const GLfloat respiracion = 0.65f + 0.35f * sin(tiempoHalo * 0.7f + i * 2.1f);
                const GLfloat angulo = i * 2.0944f + 0.6f;
                const glm::vec3 posAnillo = ORIGEN_HALO + glm::vec3(cos(angulo) * 170.0f, 220.0f, sin(angulo) * 90.0f);
                pointLights[pointLightCount++] = PointLight(
                    0.25f, 0.75f, 1.0f, 0.0f, (0.25f + 2.6f * oscuridadHalo) * respiracion,
                    posAnillo.x, posAnillo.y, posAnillo.z, 1.0f, 0.004f, 0.00004f);
            }

            // Luz azul de la nave del jugador cuando acelera
            if (carreraIniciada && acelerandoW && pointLightCount < MAX_POINT_LIGHTS)
            {
                const glm::vec3 posicionLuzNaveHalo = glm::vec3(matNavePrincipal * glm::vec4(0.0f, 0.5f, -1.0f, 1.0f));
                pointLights[pointLightCount++] = PointLight(
                    0.12f, 0.50f, 1.0f, 0.8f, 5.0f,
                    posicionLuzNaveHalo.x, posicionLuzNaveHalo.y, posicionLuzNaveHalo.z,
                    1.0f, 0.05f, 0.01f);
            }
        }

        // Reflector azul al pie de Beam_M, desde un lateral y apuntando hacia arriba.
        const glm::vec3 posicionReflectorBeam = ORIGEN_HALO + POSICION_REFLECTOR_BEAM_HALO;
        const glm::vec3 objetivoReflectorBeam = ORIGEN_HALO + OBJETIVO_REFLECTOR_BEAM_HALO;
        const glm::vec3 direccionReflectorBeam = glm::normalize(objetivoReflectorBeam - posicionReflectorBeam);
        SpotLight luzReflectorBeam(
            0.15f, 0.50f, 1.0f,                 // Azul
            0.0f, reflectorBeamEncendido ? 7.0f : 0.0f,
            posicionReflectorBeam.x, posicionReflectorBeam.y, posicionReflectorBeam.z,
            direccionReflectorBeam.x, direccionReflectorBeam.y, direccionReflectorBeam.z,
            1.0f, 0.02f, 0.001f, 55.0f
        );
        luzReflectorBeam.SetFlash(posicionReflectorBeam, direccionReflectorBeam);

        // Se agrega a las luces de la escena si existe un espacio disponible.
        // En Halo ya hay hasta diez SpotLight; si el arreglo esta lleno,
        // mas abajo se aplica el reflector solo al render de Beam_M sin quitar farolas.
        bool reflectorBeamEnLucesGlobales = false;
        if (reflectorBeamEncendido && spotLightCount < MAX_SPOT_LIGHTS)
        {
            spotLights[spotLightCount] = luzReflectorBeam;
            ++spotLightCount;
            reflectorBeamEnLucesGlobales = true;
        }

        // Luz puntual azul claro que nace del propio Spark. Se registra DESPUES
        // de seleccionar las luces del planeta para evitar que Halo la borre.
        const glm::vec3 posicionSparkMundo = ORIGEN_HALO + POSICION_SPARK_HALO;
        const glm::vec3 posicionLuzSpark = posicionSparkMundo + DESPLAZAMIENTO_LUZ_SPARK_HALO;
        PointLight luzPuntualSpark(
            0.60f, 0.85f, 1.0f,  // Azul claro / celeste
            0.40f, 1.0f,        // Intensidades ambiente y difusa
            posicionLuzSpark.x, posicionLuzSpark.y, posicionLuzSpark.z,
            1.0f, 0.03f, 0.004f // Atenuacion local: no ilumina el otro planeta
        );
        bool luzSparkEnLucesGlobales = false;
        if (pointLightCount < MAX_POINT_LIGHTS)
        {
            pointLights[pointLightCount++] = luzPuntualSpark;
            luzSparkEnLucesGlobales = true;
        }

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // El skybox depende de donde esta la camara: cerca del Halo, cerca del parque o en el espacio profundo (zona de la ISS)
        const glm::vec3 posicionCamaraCielo = camera.getCameraPosition();
        const GLfloat distanciaAlHalo = glm::length(posicionCamaraCielo - ORIGEN_HALO);
        const GLfloat distanciaAlParque = glm::length(posicionCamaraCielo);
        const GLfloat distanciaAlPlaneta = glm::min(distanciaAlHalo, distanciaAlParque);
        // El amanecer y el atardecer se mezclan con el cielo del planeta cuando el Sol esta cerca del horizonte
        const GLfloat alturaSolCielo = sin(anguloCicloDia);
        const GLfloat mezclaAtardecer = 0.9f * exp(-pow(alturaSolCielo / 0.30f, 2.0f)) * (1.0f - SuavizarPaso((distanciaAlPlaneta - 500.0f) / 200.0f));
        // Al alejarse del planeta el cielo pasa poco a poco al espacio profundo
        const GLfloat mezclaEspacio = SuavizarPaso((distanciaAlPlaneta - 700.0f) / 400.0f);
        Skybox& skyboxPlaneta = (distanciaAlHalo < distanciaAlParque) ? skybox : skyboxPlaneta2;
        if (mezclaEspacio >= 0.999f)
            skyboxEspacioProfundo.DrawSkybox(camera.calculateViewMatrix(), projection);
        else if (mezclaEspacio > 0.0f)
            skyboxPlaneta.DrawSkyboxMezcla(camera.calculateViewMatrix(), projection, skyboxEspacioProfundo, mezclaEspacio);
        else
            skyboxPlaneta.DrawSkyboxMezcla(camera.calculateViewMatrix(), projection, skyboxAtardecer, mezclaAtardecer);

        shaderList[0].UseShader();
        uniformModel = shaderList[0].GetModelLocation();
        uniformProjection = shaderList[0].GetProjectionLocation();
        uniformView = shaderList[0].GetViewLocation();
        uniformEyePosition = shaderList[0].GetEyePositionLocation();
        uniformColor = shaderList[0].getColorLocation();
        uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
        uniformShininess = shaderList[0].GetShininessLocation();

        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
        glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        glUniform1f(uniformOpacidad, 1.0f);

        shaderList[0].SetDirectionalLight(&mainLight);
        shaderList[0].SetPointLights(pointLights, pointLightCount);
        shaderList[0].SetSpotLights(spotLights, spotLightCount);

        if (solEmisivoDisponible)
        {
            shaderSolEmisivo.UseShader();
            glUniformMatrix4fv(solModel, 1, GL_FALSE, glm::value_ptr(matrizRenderSol));
            glUniformMatrix4fv(solView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
            glUniformMatrix4fv(solProjection, 1, GL_FALSE, glm::value_ptr(projection));
            glUniform1i(solTextura, 0);
            Sol.RenderModel();
            shaderList[0].UseShader();
        }
        else
        {
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(matrizRenderSol));
            const glm::vec3 colorRosa(1.0f, 0.4f, 0.8f);
            glUniform3fv(uniformColor, 1, glm::value_ptr(colorRosa));
            Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
            Sol.RenderModel();
            const glm::vec3 colorBlanco(1.0f, 1.0f, 1.0f);
            glUniform3fv(uniformColor, 1, glm::value_ptr(colorBlanco));
        }

        if (coronaSolDisponible && mallaCoronaSol != nullptr)
        {
            const glm::vec3 haciaCamara = camera.getCameraPosition() - posicionFisicaSol;
            if (glm::dot(haciaCamara, haciaCamara) > 0.0001f)
            {
                const glm::vec3 frente = glm::normalize(haciaCamara);
                const glm::vec3 vertical = (fabs(frente.y) > 0.98f) ? glm::vec3(0.0f, 0.0f, 1.0f) : glm::vec3(0.0f, 1.0f, 0.0f);
                const glm::vec3 derecha = glm::normalize(glm::cross(vertical, frente));
                const glm::vec3 arriba = glm::normalize(glm::cross(frente, derecha));

                glm::mat4 orientacionCorona(1.0f);
                orientacionCorona[0] = glm::vec4(derecha, 0.0f);
                orientacionCorona[1] = glm::vec4(arriba, 0.0f);
                orientacionCorona[2] = glm::vec4(frente, 0.0f);

                glm::mat4 matrizCorona = glm::translate(glm::mat4(1.0f), posicionFisicaSol);
                matrizCorona = matrizCorona * orientacionCorona;
                matrizCorona = glm::scale(matrizCorona, glm::vec3(DIAMETRO_CORONA_SOL, DIAMETRO_CORONA_SOL, 1.0f));

                const GLboolean cullingAnteriorSol = glIsEnabled(GL_CULL_FACE);
                const GLboolean blendAnteriorSol = glIsEnabled(GL_BLEND);
                GLboolean escrituraProfundidadAnteriorSol = GL_TRUE;
                glGetBooleanv(GL_DEPTH_WRITEMASK, &escrituraProfundidadAnteriorSol);
                GLint mezclaFuenteRGB = GL_SRC_ALPHA, mezclaDestinoRGB = GL_ONE_MINUS_SRC_ALPHA;
                GLint mezclaFuenteAlpha = GL_SRC_ALPHA, mezclaDestinoAlpha = GL_ONE_MINUS_SRC_ALPHA;
                glGetIntegerv(GL_BLEND_SRC_RGB, &mezclaFuenteRGB);
                glGetIntegerv(GL_BLEND_DST_RGB, &mezclaDestinoRGB);
                glGetIntegerv(GL_BLEND_SRC_ALPHA, &mezclaFuenteAlpha);
                glGetIntegerv(GL_BLEND_DST_ALPHA, &mezclaDestinoAlpha);

                glDisable(GL_CULL_FACE);
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE);
                glDepthMask(GL_FALSE);

                shaderCoronaSol.UseShader();
                glUniformMatrix4fv(coronaModel, 1, GL_FALSE, glm::value_ptr(matrizCorona));
                glUniformMatrix4fv(coronaView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
                glUniformMatrix4fv(coronaProjection, 1, GL_FALSE, glm::value_ptr(projection));
                mallaCoronaSol->RenderMeshModel();

                glDepthMask(escrituraProfundidadAnteriorSol);
                glBlendFuncSeparate(mezclaFuenteRGB, mezclaDestinoRGB, mezclaFuenteAlpha, mezclaDestinoAlpha);
                if (!blendAnteriorSol) glDisable(GL_BLEND);
                if (cullingAnteriorSol) glEnable(GL_CULL_FACE);
                shaderList[0].UseShader();
            }
        }

        model = matrizSpacePark;
        modelaux = model;
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
        Base.RenderModel();

        model = modelaux;
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
        Propulsor1.RenderModel();
        Propulsor2.RenderModel();
        Propulsor3.RenderModel();
        Propulsor4.RenderModel();

        if (plasmaDisponible && tieneTexturaFuego)
        {
            GLboolean cullingAnterior = glIsEnabled(GL_CULL_FACE);
            GLboolean blendAnterior = glIsEnabled(GL_BLEND);
            GLboolean profundidadAnterior;
            glGetBooleanv(GL_DEPTH_WRITEMASK, &profundidadAnterior);
            glDisable(GL_CULL_FACE);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE);
            glDepthMask(GL_FALSE);
            shaderPlasma.UseShader();
            glUniformMatrix4fv(plasmaProjection, 1, GL_FALSE, glm::value_ptr(projection));
            glUniformMatrix4fv(plasmaView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
            glUniform1i(plasmaTextura, 0);
            glActiveTexture(GL_TEXTURE0);
            FuegoAzulTexture.UseTexture();
            GLfloat anguloRotacionPlasma = glfwGetTime() * 150.0f;
            glm::vec3 escalaFuego = glm::vec3(0.6f, 0.9f, 0.6f);

            auto dibujarFuego = [&](glm::vec3 posLocal) {
                glm::mat4 modelFuego = matrizSpacePark;
                modelFuego = glm::translate(modelFuego, posLocal);
                modelFuego = glm::rotate(modelFuego, glm::radians(anguloRotacionPlasma), glm::vec3(0.0f, 1.0f, 0.0f));
                modelFuego = glm::scale(modelFuego, escalaFuego);
                glUniformMatrix4fv(plasmaModel, 1, GL_FALSE, glm::value_ptr(modelFuego));
                plasmaMesh->RenderMeshModel();
                };
            dibujarFuego(centroP1);
            dibujarFuego(centroP2);
            dibujarFuego(centroP3);
            dibujarFuego(centroP4);
            glDepthMask(profundidadAnterior);
            if (!blendAnterior) glDisable(GL_BLEND);
            if (cullingAnterior) glEnable(GL_CULL_FACE);
            shaderList[0].UseShader();
        }

        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(matrizFantasmano));
        Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
        FantasmanoCuerpo.RenderModel();
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(matrizBrazoFantasmano));
        FantasmanoBrazo.RenderModel();
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(matrizManoFantasmano));
        FantasmanoMano.RenderModel();

        model = matrizTienda;
        model = glm::scale(model, glm::vec3(0.027f, 0.027f, 0.027f));
        model = glm::rotate(model, 34.4f, glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Tienda.RenderModel();
        model = modelaux;
        model = glm::translate(model, glm::vec3(0.92f, 0.66f, 0.95f));
        model = glm::scale(model, glm::vec3(0.035f, 0.035f, 0.035f));
        model = glm::rotate(model, 1.4f, glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Bosque2.RenderModel();
        model = modelaux;
        model = glm::translate(model, glm::vec3(1.1f, 0.66f, -0.1f));
        model = glm::scale(model, glm::vec3(0.035f, 0.035f, 0.035f));
        model = glm::rotate(model, 1.4f, glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Bosque2.RenderModel();
        model = modelaux;
        model = glm::translate(model, glm::vec3(1.05f, 0.66f, 0.22f));
        model = glm::scale(model, glm::vec3(0.035f, 0.035f, 0.040f));
        model = glm::rotate(model, 1.4f, glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Bosque1.RenderModel();

        model = matrizIglesia;
        model = glm::scale(model, glm::vec3(0.015f, 0.015f, 0.015f));
        model = glm::rotate(model, 34.4f, glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Iglesia.RenderModel();
        model = modelaux;
        model = glm::translate(model, glm::vec3(0.58f, 0.66f, 0.1f));
        model = glm::scale(model, glm::vec3(0.039f, 0.035f, 0.033f));
        model = glm::rotate(model, 34.4f, glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
        Bosque3.RenderModel();

        model = matrizBase;
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        CarruselBase.RenderModel();
        model = matrizSoporte;
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        CarruselSoporte.RenderModel();
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(matricesCaballos[0]));
        Caballo1.RenderModel();
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(matricesCaballos[1]));
        Caballo2.RenderModel();
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(matricesCaballos[2]));
        Caballo3.RenderModel();
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(matricesCaballos[3]));
        Caballo4.RenderModel();
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(matricesCaballos[4]));
        Caballo5.RenderModel();
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(matricesCaballos[5]));
        Caballo6.RenderModel();
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(matricesCaballos[6]));
        Caballo7.RenderModel();
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(matricesCaballos[7]));
        Caballo8.RenderModel();

        glm::mat4 matrizCuerpoRigby = modelaux;
        matrizCuerpoRigby = glm::translate(matrizCuerpoRigby, glm::vec3(0.17f, 0.662f, 0.8f));
        matrizCuerpoRigby = glm::scale(matrizCuerpoRigby, glm::vec3(0.004f, 0.004f, 0.004f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(matrizCuerpoRigby));
        Rigby.RenderModel();
        glm::mat4 matrizBrazo1Rigby = matrizCuerpoRigby;
        glm::vec3 pivoteHombro = glm::vec3(0.0f, 0.0f, 0.0f);
        matrizBrazo1Rigby = glm::translate(matrizBrazo1Rigby, glm::vec3(-0.8f, 3.6f, 0.65f));
        matrizBrazo1Rigby = glm::translate(matrizBrazo1Rigby, pivoteHombro);
        matrizBrazo1Rigby = glm::rotate(matrizBrazo1Rigby, glm::radians(anguloHombroRigby), glm::vec3(-1.0f, 0.0f, 0.0f));
        matrizBrazo1Rigby = glm::translate(matrizBrazo1Rigby, -pivoteHombro);
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(matrizBrazo1Rigby));
        RigbyBrazo1.RenderModel();
        glm::mat4 matrizBrazo2Rigby = matrizBrazo1Rigby;
        glm::vec3 pivoteCodo = glm::vec3(0.0f, 0.0f, 0.0f);
        matrizBrazo2Rigby = glm::translate(matrizBrazo2Rigby, glm::vec3(-0.2f, 0.0f, 0.8f));
        matrizBrazo2Rigby = glm::translate(matrizBrazo2Rigby, pivoteCodo);
        matrizBrazo2Rigby = glm::rotate(matrizBrazo2Rigby, glm::radians(anguloCodoRigby), glm::vec3(-1.0f, 0.0f, 0.0f));
        matrizBrazo2Rigby = glm::translate(matrizBrazo2Rigby, -pivoteCodo);
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(matrizBrazo2Rigby));
        RigbyBrazo2.RenderModel();

        model = matrizCasaParque;
        model = glm::rotate(model, glm::radians(-0.15f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        CasaParque.RenderModel();
        model = modelaux;
        model = glm::translate(model, glm::vec3(0.5f, 0.66f, 0.59f));
        model = glm::scale(model, glm::vec3(0.038f, 0.035f, 0.035f));
        model = glm::rotate(model, 34.4f, glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Bosque2.RenderModel();
        model = modelaux;
        model = glm::translate(model, glm::vec3(0.38f, 0.66f, 1.21f));
        model = glm::scale(model, glm::vec3(0.035f, 0.035f, 0.035f));
        model = glm::rotate(model, 34.4f, glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Bosque2.RenderModel();
        model = modelaux;
        model = glm::translate(model, glm::vec3(0.1f, 0.66f, 1.355f));
        model = glm::scale(model, glm::vec3(0.039f, 0.035f, 0.035f));
        model = glm::rotate(model, 34.4f, glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Bosque2.RenderModel();
        model = modelaux;
        model = glm::translate(model, glm::vec3(-0.67f, 0.66f, 1.235f));
        model = glm::scale(model, glm::vec3(0.039f, 0.035f, 0.035f));
        model = glm::rotate(model, 34.4f, glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Bosque1.RenderModel();
        model = modelaux;
        model = glm::translate(model, glm::vec3(0.67f, 0.66f, 1.115f));
        model = glm::scale(model, glm::vec3(0.039f, 0.035f, 0.035f));
        model = glm::rotate(model, 1.4f, glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Bosque2.RenderModel();

        model = matrizCuerpoKuromi;
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        KuromiCuerpo.RenderModel();
        model = matrizPiernaIzqKuromi;
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        KuromiPiernaIzq.RenderModel();
        model = matrizPiernaDerKuromi;
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        KuromiPiernaDer.RenderModel();

        model = modelaux;
        model = glm::translate(model, glm::vec3(-0.90f, 0.66f, 0.67f));
        model = glm::scale(model, glm::vec3(0.055f, 0.05f, 0.055f));
        model = glm::rotate(model, -45.7f, glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        CasaKuromi.RenderModel();

        model = modelaux;
        model = glm::translate(model, glm::vec3(-0.65f, 0.67f, 0.70f));
        model = glm::scale(model, glm::vec3(0.013f, 0.013f, 0.013f));
        model = glm::rotate(model, 15.6f, glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Cementerio.RenderModel();
        model = modelaux;
        model = glm::translate(model, glm::vec3(-0.33f, 0.67f, 0.75f));
        model = glm::scale(model, glm::vec3(0.035f, 0.035f, 0.040f));
        model = glm::rotate(model, 1.4f, glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Bosque1.RenderModel();
        model = modelaux;
        model = glm::translate(model, glm::vec3(-0.37f, 0.66f, 0.45f));
        model = glm::scale(model, glm::vec3(0.039f, 0.035f, 0.035f));
        model = glm::rotate(model, 34.4f, glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Bosque2.RenderModel();
        model = modelaux;
        model = glm::translate(model, glm::vec3(-0.47f, 0.66f, 1.02f));
        model = glm::scale(model, glm::vec3(0.039, 0.035f, 0.035f));
        model = glm::rotate(model, 34.4f, glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Bosque2.RenderModel();

        model = matrizPlataforma;
        model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
        model = glm::rotate(model, 34.4f, glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Plataforma.RenderModel();

        // Cohete de lanzamiento: despega de la plataforma, cruza hasta la ruta de la ISS, da una vuelta y regresa
        if (estadoCohete != 3)
        {
            model = glm::scale(matrizCohete, glm::vec3(escalaMundoCohete));
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
            Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
            Cohete.RenderModel();

            // Fuego del motor: varias capas de plasma que se estiran con la potencia
            if (potenciaFuegoCohete > 0.02f && plasmaDisponible && tieneTexturaFuego)
            {
                GLboolean cullingAnteriorCohete = glIsEnabled(GL_CULL_FACE);
                GLboolean blendAnteriorCohete = glIsEnabled(GL_BLEND);
                GLboolean profundidadAnteriorCohete;
                glGetBooleanv(GL_DEPTH_WRITEMASK, &profundidadAnteriorCohete);
                glDisable(GL_CULL_FACE);
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE);
                glDepthMask(GL_FALSE);
                shaderPlasma.UseShader();
                glUniformMatrix4fv(plasmaProjection, 1, GL_FALSE, glm::value_ptr(projection));
                glUniformMatrix4fv(plasmaView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
                glUniform1i(plasmaTextura, 0);
                glActiveTexture(GL_TEXTURE0);
                FuegoCoheteTexture.UseTexture();

                GLfloat parpadeoFuego = 1.0f + 0.10f * RuidoOrganico(tiempoRuido * 1.7f, 5.5f);
                for (int i = 0; i < 3; i++)
                {
                    GLfloat largoFuego = (0.10f + 0.12f * i) * potenciaFuegoCohete * parpadeoFuego * escalaPlaneta;
                    GLfloat anchoFuego = (0.12f - 0.025f * i) * escalaPlaneta;
                    glm::mat4 modelFuegoCohete = glm::translate(matrizCohete, glm::vec3(0.0f, -largoFuego * 0.5f, 0.0f));
                    modelFuegoCohete = glm::rotate(modelFuegoCohete, glm::radians((GLfloat)glfwGetTime() * 400.0f + i * 60.0f), glm::vec3(0.0f, 1.0f, 0.0f));
                    modelFuegoCohete = glm::scale(modelFuegoCohete, glm::vec3(anchoFuego, largoFuego, anchoFuego));
                    glUniformMatrix4fv(plasmaModel, 1, GL_FALSE, glm::value_ptr(modelFuegoCohete));
                    plasmaMesh->RenderMeshModel();
                }

                glDepthMask(profundidadAnteriorCohete);
                if (!blendAnteriorCohete) glDisable(GL_BLEND);
                if (cullingAnteriorCohete) glEnable(GL_CULL_FACE);
                shaderList[0].UseShader();
            }
        }

        // Humo del cohete: cuadros que miran a la camara, con transparencia; los mas lejanos primero
        if (humoDisponible && !humoCohete.empty())
        {
            const glm::mat4 vistaHumo = camera.calculateViewMatrix();
            const glm::vec3 posicionCamaraHumo = camera.getCameraPosition();
            std::sort(humoCohete.begin(), humoCohete.end(), [&](const ParticulaHumoCohete& a, const ParticulaHumoCohete& b)
                { return glm::distance(a.posicion, posicionCamaraHumo) > glm::distance(b.posicion, posicionCamaraHumo); });

            GLboolean cullingAnteriorHumo = glIsEnabled(GL_CULL_FACE);
            GLboolean blendAnteriorHumo = glIsEnabled(GL_BLEND);
            GLboolean profundidadAnteriorHumo;
            glGetBooleanv(GL_DEPTH_WRITEMASK, &profundidadAnteriorHumo);
            glDisable(GL_CULL_FACE);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glDepthMask(GL_FALSE);

            shaderHumo.UseShader();
            glUniformMatrix4fv(humoView, 1, GL_FALSE, glm::value_ptr(vistaHumo));
            glUniformMatrix4fv(humoProjection, 1, GL_FALSE, glm::value_ptr(projection));
            glUniform1i(humoTextura, 0);
            glActiveTexture(GL_TEXTURE0);
            HumoTexture.UseTexture();
            // El humo se ve mas oscuro cuando es de noche
            const GLfloat claridadHumo = 0.40f + 0.50f * alturaSol;
            glUniform3f(humoTinte, 0.95f * claridadHumo, 0.92f * claridadHumo, 1.0f * claridadHumo);
            const glm::mat4 giroCamaraHumo = glm::mat4(glm::transpose(glm::mat3(vistaHumo)));
            for (size_t i = 0; i < humoCohete.size(); i++)
            {
                GLfloat edadHumo = (tiempoRuido - humoCohete[i].nacimiento) / humoCohete[i].vida;
                GLfloat tamanoHumo = 18.0f + 55.0f * edadHumo;
                glm::mat4 modelHumo = glm::translate(glm::mat4(1.0f), humoCohete[i].posicion) * giroCamaraHumo;
                modelHumo = glm::scale(modelHumo, glm::vec3(tamanoHumo));
                glUniformMatrix4fv(humoModel, 1, GL_FALSE, glm::value_ptr(modelHumo));
                glUniform1f(humoOpacidad, 0.5f * (1.0f - edadHumo));
                mallaCoronaSol->RenderMeshModel();
            }

            glDepthMask(profundidadAnteriorHumo);
            if (!blendAnteriorHumo) glDisable(GL_BLEND);
            if (cullingAnteriorHumo) glEnable(GL_CULL_FACE);
            shaderList[0].UseShader();
        }
        model = modelaux;
        model = glm::translate(model, glm::vec3(-1.2f, 0.66f, 0.43f));
        model = glm::scale(model, glm::vec3(0.035f, 0.035f, 0.035f));
        model = glm::rotate(model, 1.4f, glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Bosque2.RenderModel();
        model = modelaux;
        model = glm::translate(model, glm::vec3(-1.25f, 0.66f, 0.77f));
        model = glm::scale(model, glm::vec3(0.035f, 0.035f, 0.035f));
        model = glm::rotate(model, 1.4f, glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Bosque1.RenderModel();
        model = modelaux;
        model = glm::translate(model, glm::vec3(-1.4f, 0.66f, 0.49f));
        model = glm::scale(model, glm::vec3(0.035f, 0.035f, 0.035f));
        model = glm::rotate(model, 1.4f, glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Bosque2.RenderModel();
        model = modelaux;
        model = glm::translate(model, glm::vec3(-1.255f, 0.66f, -0.235f));
        model = glm::scale(model, glm::vec3(0.035f, 0.035f, 0.035f));
        model = glm::rotate(model, 1.4f, glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Bosque2.RenderModel();

        model = modelaux;
        model = glm::translate(model, glm::vec3(-0.133f, 0.69f, -0.997f));
        model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
        model = glm::rotate(model, 34.4f, glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Cinnamoroll.RenderModel();

        model = modelaux;
        model = glm::translate(model, glm::vec3(-0.15f, 0.66f, -1.0f));
        model = glm::scale(model, glm::vec3(0.032f, 0.032f, 0.032f));
        model = glm::rotate(model, 34.4f, glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        CarroFruta.RenderModel();

        model = modelaux;
        model = glm::translate(model, glm::vec3(-0.20f, 0.66f, -0.67f));
        model = glm::scale(model, glm::vec3(0.035f, 0.035f, 0.035f));
        model = glm::rotate(model, 34.45f, glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Bosque1.RenderModel();
        model = modelaux;
        model = glm::translate(model, glm::vec3(-0.183f, 0.66f, -0.845f));
        model = glm::scale(model, glm::vec3(0.035f, 0.035f, 0.035f));
        model = glm::rotate(model, 34.45f, glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Bosque1.RenderModel();
        model = modelaux;
        model = glm::translate(model, glm::vec3(0.52f, 0.66f, -1.07f));
        model = glm::scale(model, glm::vec3(0.039f, 0.035f, 0.035f));
        model = glm::rotate(model, 34.45f, glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Bosque2.RenderModel();
        model = modelaux;
        model = glm::translate(model, glm::vec3(-0.29f, 0.66f, -1.17f));
        model = glm::scale(model, glm::vec3(0.039f, 0.035f, 0.035f));
        model = glm::rotate(model, 34.5f, glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Bosque1.RenderModel();

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDepthMask(GL_FALSE);
        glUniform1f(uniformOpacidad, 0.2f);
        model = modelaux;
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
        Cupula.RenderModel();
        glDepthMask(GL_TRUE);
        glDisable(GL_BLEND);
        glUniform1f(uniformOpacidad, 1.0f);


        // ==============================================================
        // SATÉLITE 1
        // ==============================================================
   /*     model = glm::translate(glm::mat4(1.0f), posicionISS);
        model = glm::translate(model, glm::vec3(posicionSatelite.x * 2.5f, 50.0f, posicionSatelite.y * 2.5f));
        model = glm::rotate(model, -giroSatelite, glm::vec3(0.0f, 1.0f, 0.0f));
        modelaux = model;
        model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
        color = glm::vec3(1.0f, 1.0f, 1.0f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        cuerposatelite.RenderModel();
        model = modelaux;
        model = glm::translate(model, glm::vec3(-3.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
        model = glm::rotate(model, glm::radians(30.0f * articulacionesSatelite), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(160.0f * articulacionesSatelite), glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        antena.RenderModel();
        model = modelaux;
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 3.3f));
        model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
        model = glm::rotate(model, glm::radians(35.0f * articulacionesSatelite), glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        panel1.RenderModel();
        model = modelaux;
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.3f));
        model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
        model = glm::rotate(model, glm::radians(-35.0f * articulacionesSatelite), glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        panel2.RenderModel();*/


        // ==============================================================
        // CINTURÓN DE ASTEROIDES
        // ==============================================================
        // Solo modelos con sus texturas, sin luces propias ni reflejos exagerados.
        Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
        glUniform3f(uniformColor, 1.0f, 1.0f, 1.0f);
        for (int i = 0; i < 15; i++) {
            float astAngulo = glfwGetTime() * (0.05f + i * 0.01f) + (i * 3.14159f / 7.5f);
            float astRadio = 350.0f + (i % 5) * 60.0f;
            float astAltura = sin(glfwGetTime() * 0.2f + i) * 80.0f;

            glm::mat4 matrizAsteroide = glm::translate(glm::mat4(1.0f), ORIGEN_ISS);
            matrizAsteroide = glm::translate(matrizAsteroide, glm::vec3(cos(astAngulo) * astRadio, astAltura, sin(astAngulo) * astRadio));
            matrizAsteroide = glm::rotate(matrizAsteroide, (float)glfwGetTime() * 0.5f + i, glm::vec3(1.0f, 1.0f, 0.0f));
            matrizAsteroide = glm::scale(matrizAsteroide, glm::vec3(5.0f + (i % 4) * 2.0f));

            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(matrizAsteroide));

            if (i % 3 == 0) Asteroide1.RenderModel();
            else if (i % 3 == 1) Asteroide2.RenderModel();
            else Asteroide3.RenderModel();
        }

        // ========================================
        // HALO
        // ========================================
        model = glm::mat4(1.0);
        model = glm::translate(model, ORIGEN_HALO);
        modelaux = model;
        model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Halo_M.RenderModel();

        // Spark permanece en su lugar y gira solo alrededor del eje vertical Y.
        // Asi siempre presenta la misma cara (+Z local) hacia Fantasmano Halo,
        // independientemente de la camara activa o de la direccion del jugador.
        model = glm::translate(modelaux, POSICION_SPARK_HALO);
        const glm::vec3 haciaFantasmanoSpark = posicionFantasmanoHalo - POSICION_SPARK_HALO;
        const GLfloat distanciaHorizontalSpark2 =
            haciaFantasmanoSpark.x * haciaFantasmanoSpark.x +
            haciaFantasmanoSpark.z * haciaFantasmanoSpark.z;
        if (distanciaHorizontalSpark2 > 0.0001f)
        {
            const GLfloat anguloMiradaSpark = atan2(haciaFantasmanoSpark.x, haciaFantasmanoSpark.z);
            model = glm::rotate(model,
                anguloMiradaSpark + glm::radians(AJUSTE_FRENTE_SPARK_GRADOS),
                glm::vec3(0.0f, 1.0f, 0.0f));
        }
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

        // Si todas las ranuras PointLight estan ocupadas, el modelo mantiene
        // su luz usando una ranura prestada solo durante este RenderModel().
        const bool luzSparkSoloModelo = !luzSparkEnLucesGlobales && pointLightCount > 0;
        PointLight luzPuntualGuardadaSpark;
        if (luzSparkSoloModelo)
        {
            luzPuntualGuardadaSpark = pointLights[pointLightCount - 1];
            pointLights[pointLightCount - 1] = luzPuntualSpark;
            shaderList[0].SetPointLights(pointLights, pointLightCount);
        }
        Spark_M.RenderModel();
        if (luzSparkSoloModelo)
        {
            pointLights[pointLightCount - 1] = luzPuntualGuardadaSpark;
            shaderList[0].SetPointLights(pointLights, pointLightCount);
        }

        model = modelaux;
        model = glm::translate(model, POSICION_BEAM_HALO);
        model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

        // Respaldo si el shader ya ocupa el maximo de SpotLight (p. ej., diez
        // focos al estar cerca de Halo): durante Beam_M se presta el ultimo
        // slot al reflector azul, y se restaura inmediatamente despues.
        // Asi se ilumina Beam sin desactivar ninguna luz del resto de la escena.
        const bool reflectorBeamSoloModelo = reflectorBeamEncendido &&
            !reflectorBeamEnLucesGlobales && spotLightCount > 0;
        SpotLight luzTemporalGuardada;
        if (reflectorBeamSoloModelo)
        {
            luzTemporalGuardada = spotLights[spotLightCount - 1];
            spotLights[spotLightCount - 1] = luzReflectorBeam;
            shaderList[0].SetSpotLights(spotLights, spotLightCount);
        }
        Beam_M.RenderModel();
        if (reflectorBeamSoloModelo)
        {
            spotLights[spotLightCount - 1] = luzTemporalGuardada;
            shaderList[0].SetSpotLights(spotLights, spotLightCount);
        }

        glm::vec3 posicionRealPersonajeHalo = ORIGEN_HALO + posicionFantasmanoHalo;
        glm::mat4 matrizPersonajeHalo = glm::mat4(1.0f);
        GLfloat escalaSpaceParkFlotacion = glm::length(glm::vec3(matrizSpacePark[1]));
        GLfloat flotacionHalo = flotacionFantasmano * escalaSpaceParkFlotacion;

        matrizPersonajeHalo = glm::translate(matrizPersonajeHalo, posicionRealPersonajeHalo + glm::vec3(0.0f, alturaPersonajeHalo + flotacionHalo, 0.0f));
        matrizPersonajeHalo = glm::rotate(matrizPersonajeHalo, glm::radians(direccionFantasmanoHalo), glm::vec3(0.0f, 1.0f, 0.0f));
        matrizPersonajeHalo = glm::scale(matrizPersonajeHalo, glm::vec3(escalaPersonajeHalo));

        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(matrizPersonajeHalo));
        Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
        modeloPersonajeHalo->RenderModel();

        if (modeloPersonajeHalo == &FantasmanoCuerpo)
        {
            glm::mat4 matrizBrazoHalo = matrizPersonajeHalo;
            matrizBrazoHalo = glm::translate(matrizBrazoHalo, glm::vec3(0.0f, 3.0f, -0.5f));
            matrizBrazoHalo = glm::rotate(matrizBrazoHalo, glm::radians(anguloBrazoFantasmano), glm::vec3(1.0f, 0.0f, 0.0f));
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(matrizBrazoHalo));
            FantasmanoBrazo.RenderModel();

            glm::mat4 matrizManoHalo = matrizBrazoHalo;
            matrizManoHalo = glm::translate(matrizManoHalo, glm::vec3(0.0f, 1.4f, 0.0f));
            matrizManoHalo = glm::rotate(matrizManoHalo, glm::radians(anguloManoFantasmano), glm::vec3(0.0f, 1.0f, 0.0f));
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(matrizManoHalo));
            FantasmanoMano.RenderModel();
        }

        // Arboles de Halo (Kenney Nature Kit)
        auto RenderizarArbolHalo = [&](Model& modeloArbol, GLfloat posX, GLfloat posZ,
            GLfloat escala, GLfloat giro)
            {
                glm::mat4 matArbol = glm::translate(modelaux, glm::vec3(posX, 0.0f, posZ));
                // Viento: balanceo lento con rafagas que cruzan el bosque; la base del tronco queda fija
                const GLfloat tiempoViento = (GLfloat)glfwGetTime();
                const GLfloat rafaga = 0.6f + 0.4f * sin(tiempoViento * 0.35f + posX * 0.02f);
                const GLfloat balanceoX = sin(tiempoViento * 1.3f + posX * 0.045f + posZ * 0.03f) * 1.6f * rafaga;
                const GLfloat balanceoZ = sin(tiempoViento * 1.0f + posZ * 0.05f + 1.7f) * 1.2f * rafaga;
                matArbol = glm::rotate(matArbol, glm::radians(balanceoX), glm::vec3(1.0f, 0.0f, 0.0f));
                matArbol = glm::rotate(matArbol, glm::radians(balanceoZ), glm::vec3(0.0f, 0.0f, 1.0f));
                matArbol = glm::rotate(matArbol, glm::radians(giro), glm::vec3(0.0f, 1.0f, 0.0f));
                matArbol = glm::scale(matArbol, glm::vec3(escala));
                glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(matArbol));
                modeloArbol.RenderModel();
            };

        Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
        // Un poco mas de brillo en el color para que el follaje no se pierda con la luz rosa
        color = glm::vec3(1.7f, 1.7f, 1.7f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));

        // Pseudoaleatorio fijo: los arboles quedan siempre en el mismo lugar
        auto AleatorioHalo = [](int n)
            {
                GLfloat valor = sin(n * 12.9898f) * 43758.5453f;
                return valor - floor(valor);
            };

        // Zonas libres de la pista: derecha, izquierda y franjas al frente y al fondo
        const GLfloat zonasArbolesHalo[3][6] = {
            //  xIni    xFin    zIni     zFin   pasoX  pasoZ
            {   75.0f, 210.0f, -105.0f, 105.0f, 22.0f, 22.0f },
            { -215.0f,-185.0f, -105.0f, 105.0f, 22.0f, 22.0f },
            { -140.0f,  40.0f, -105.0f, 105.0f, 22.0f, 210.0f }
        };

        // Puntos de la pista en las coordenadas de los arboles (la pista se dibuja 100 unidades a la izquierda del origen de Halo)
        static std::vector<glm::vec2> muestrasPistaHalo;
        if (muestrasPistaHalo.empty())
            for (size_t i = 0; i < puntosCircuito.size(); i += 3)
                muestrasPistaHalo.push_back(glm::vec2(puntosCircuito[i].x - 100.0f, puntosCircuito[i].z));
        auto muyCercaDePistaHalo = [&](GLfloat x, GLfloat z, GLfloat margen)
            {
                for (size_t i = 0; i < muestrasPistaHalo.size(); i++)
                    if (glm::distance(glm::vec2(x, z), muestrasPistaHalo[i]) < margen) return true;
                return false;
            };

        int semillaArbol = 0;
        for (int zona = 0; zona < 3; zona++)
        {
            for (GLfloat x = zonasArbolesHalo[zona][0]; x <= zonasArbolesHalo[zona][1]; x += zonasArbolesHalo[zona][4])
            {
                for (GLfloat z = zonasArbolesHalo[zona][2]; z <= zonasArbolesHalo[zona][3]; z += zonasArbolesHalo[zona][5])
                {
                    semillaArbol++;
                    if (AleatorioHalo(semillaArbol) < 0.22f) continue;

                    GLfloat posX = x + (AleatorioHalo(semillaArbol + 100) - 0.5f) * 20.0f;
                    GLfloat posZ = z + (AleatorioHalo(semillaArbol + 200) - 0.5f) * 20.0f;
                    // Nunca sobre la pista ni sobre la zona de salida (gradas, boxes y carpas)
                    if (muyCercaDePistaHalo(posX, posZ, 17.0f)) continue;
                    if (posX > -180.0f && posX < -95.0f && posZ > 30.0f && posZ < 120.0f) continue;
                    if (glm::distance(glm::vec2(posX, posZ), glm::vec2(CASA_ARBOL_HALO.x, CASA_ARBOL_HALO.z)) < 45.0f) continue;   // explanada de la Casa del Arbol

                    GLfloat giro = AleatorioHalo(semillaArbol + 400) * 360.0f;
                    GLfloat variedad = AleatorioHalo(semillaArbol + 300);

                    // Arboledas: una onda suave sobre el terreno decide el tipo de bosque de cada zona
                    const GLfloat ondaBosque = sin(posX * 0.021f + 1.3f) * sin(posZ * 0.027f + 0.7f);
                    int primero, cantidad;
                    GLfloat escalaBase;
                    if (ondaBosque > 0.30f) { primero = 6; cantidad = 4; escalaBase = 12.5f; }         // pinos oscuros
                    else if (ondaBosque < -0.42f) { primero = 10; cantidad = 3; escalaBase = 11.5f; }  // arboles de otono
                    else { primero = 0; cantidad = 6; escalaBase = 11.5f; }                             // arboles verdes
                    const int indiceArbol = primero + (int)(AleatorioHalo(semillaArbol + 500) * cantidad) % cantidad;
                    const GLfloat escala = escalaBase * (0.75f + 0.65f * variedad);

                    RenderizarArbolHalo(ArbolesHalo[indiceArbol], posX, posZ, escala, giro);
                }
            }
        }

        color = glm::vec3(1.0f, 1.0f, 1.0f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));

        // Casa del Arbol de Hora de Aventura y sus personajes, en la explanada de Halo junto al circuito
        {
            struct PiezaDibujo { const glm::mat4* matriz; Model* modelo; };
            const PiezaDibujo piezas[] = {
                { &matrizCasaArbolHalo, &CasaArbol },
                { &matrizJake, &JakeCuerpo }, { &matrizBrazoIzqJake, &JakeBrazoIzq }, { &matrizBrazoDerJake, &JakeBrazoDer },
                { &matrizPataIzqJake, &JakePataIzq }, { &matrizPataDerJake, &JakePataDer }, { &matrizColaJake, &JakeCola },
                { &matrizBmo, &BmoCuerpo }, { &matrizBrazoIzqBmo, &BmoBrazoIzq }, { &matrizBrazoDerBmo, &BmoBrazoDer },
                { &matrizPataIzqBmo, &BmoPataIzq }, { &matrizPataDerBmo, &BmoPataDer },
                { &matrizGunter, &GunterCuerpo }, { &matrizAlaIzqGunter, &GunterAlaIzq }, { &matrizAlaDerGunter, &GunterAlaDer },
                { &matrizTreeTrunks, &TreeTrunksCuerpo }, { &matrizPataDelIzqTreeTrunks, &TreeTrunksPataDelIzq },
                { &matrizPataDelDerTreeTrunks, &TreeTrunksPataDelDer }, { &matrizPataTrasIzqTreeTrunks, &TreeTrunksPataTrasIzq },
                { &matrizPataTrasDerTreeTrunks, &TreeTrunksPataTrasDer }, { &matrizOrejaIzqTreeTrunks, &TreeTrunksOrejaIzq },
                { &matrizOrejaDerTreeTrunks, &TreeTrunksOrejaDer }, { &matrizTrompaTreeTrunks, &TreeTrunksTrompa },
                { &matrizColaTreeTrunks, &TreeTrunksCola }, { &matrizNeptr, &NeptrCuerpo } };
            Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
            for (const PiezaDibujo& pieza : piezas)
            {
                glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(*pieza.matriz));
                pieza.modelo->RenderModel();
            }
        }

        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(matrizBasePista));
        pistaTexture.UseTexture();
        MaterialPista.UseMaterial(uniformSpecularIntensity, uniformShininess);
        meshList[indicePista]->RenderMeshModel();
        if (decoradoPistaListo)
        {
            bordilloTexture.UseTexture();
            meshList[indiceBordillos]->RenderMeshModel();
            concretoTexture.UseTexture();
            meshList[indiceFaldones]->RenderMeshModel();
            metaTexture.UseTexture();
            meshList[indiceMeta]->RenderMeshModel();
        }

        // Decorado de la pista con piezas del Racing Kit
        Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
        for (size_t i = 0; i < decoradoHalo.size(); i++)
        {
            const ColocacionKit& pieza = decoradoHalo[i];
            glm::mat4 matPieza = MoverNave(pieza.fase, pieza.lateral, matrizBasePista);
            matPieza = glm::translate(matPieza, glm::vec3(0.0f, -9.0f + pieza.altura, 0.0f));
            matPieza = glm::rotate(matPieza, glm::radians(pieza.giro), glm::vec3(0.0f, 1.0f, 0.0f));
            matPieza = glm::scale(matPieza, pieza.escala);
            matPieza = glm::translate(matPieza, glm::vec3(DATOS_KIT[pieza.id].desplazaX, 0.0f, DATOS_KIT[pieza.id].desplazaZ));
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(matPieza));
            PiezasKit[pieza.id].RenderModel();
        }

        auto RenderizarNaveCarrera = [&](Model& modeloNave, GLfloat fase,
            GLfloat lateral, GLfloat alturaAdicional,
            const glm::vec3& escala)
            {
                glm::mat4 matNave = MoverNave(fase, lateral, matrizBasePista);
                // Las naves flotan sobre su colchon de energia: sube y baja suave y un ligero alabeo
                const GLfloat tiempoFlotacion = (GLfloat)glfwGetTime();
                const GLfloat flotacionNave = sin(tiempoFlotacion * 2.2f + lateral * 1.7f + alturaAdicional * 3.0f) * 0.18f;
                const GLfloat alabeoNave = sin(tiempoFlotacion * 1.5f + lateral * 0.9f + alturaAdicional * 2.0f) * 2.5f;
                matNave = glm::translate(matNave, glm::vec3(0.0f, -6.0f + alturaAdicional + flotacionNave, 0.0f));
                matNave = glm::rotate(matNave, glm::radians(alabeoNave), glm::vec3(0.0f, 0.0f, 1.0f));
                matNave = glm::rotate(matNave, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
                matNave = glm::scale(matNave, escala);
                glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(matNave));
                modeloNave.RenderModel();
            };

        RenderizarNaveCarrera(CarroPrincipal, fasePrincipal, lateralPrincipal, alturaPrincipal, glm::vec3(0.3f));
        RenderizarNaveCarrera(CarroGolfCarrera, faseNave1, lateralNave1, alturaNave1, glm::vec3(0.02f));
        RenderizarNaveCarrera(CarroRosa, faseNave2, lateralNave2, alturaNave2, glm::vec3(0.9f));
        RenderizarNaveCarrera(Ghost_M, faseNave3, lateralNave3, alturaNave3, glm::vec3(1.0f));
        RenderizarNaveCarrera(CarroBR, faseNave4, lateralNave4, alturaNave4, glm::vec3(0.5f));

        // Portico del semaforo de salida (modelo del Racing Kit) con sus tres lamparas
        {
            auto dibujarPieza = [&](const glm::vec3& posLocal, const glm::vec3& escala, const glm::vec3& colorPieza)
                {
                    glm::mat4 matPieza = glm::translate(matSemaforoHalo, posLocal);
                    matPieza = glm::scale(matPieza, escala);
                    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(matPieza));
                    glUniform3fv(uniformColor, 1, glm::value_ptr(colorPieza));
                    Sol.RenderModel();
                };
            Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
            // Portico (Kenney Racing Kit): el modelo mide 1.26 de ancho con el origen en un pilar y la viga
            // un poco hacia atras, asi que se centra sobre la pista y se apoya en el suelo
            const GLfloat escalaPortico = 13.8f;
            glm::mat4 matPortico = glm::translate(matSemaforoHalo, glm::vec3(0.5f * escalaPortico, -9.0f, 0.12f * escalaPortico));
            matPortico = glm::scale(matPortico, glm::vec3(escalaPortico));
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(matPortico));
            glUniform3f(uniformColor, 1.0f, 1.0f, 1.0f);
            PorticoSalidaHalo.RenderModel();
            // Lamparas bajo la viga: apagadas son casi negras para que se note el cambio de color
            for (int i = 0; i < 3; i++)
            {
                glm::vec3 colorLampara(0.10f, 0.02f, 0.02f);
                if (verdeSemaforo) colorLampara = glm::vec3(0.0f, 1.0f, 0.1f) * 1.8f;
                else if (i < rojasSemaforo) colorLampara = glm::vec3(1.0f, 0.08f, 0.08f) * 5.0f;
                dibujarPieza(glm::vec3(-3.2f + 3.2f * i, -2.4f, 0.0f), glm::vec3(0.075f), colorLampara);
            }
            color = glm::vec3(1.0f, 1.0f, 1.0f);
            glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        }

        if (carreraIniciada && acelerandoW && plasmaDisponible && tieneTexturaFuego)
        {
            GLboolean cullingAnterior = glIsEnabled(GL_CULL_FACE);
            GLboolean blendAnterior = glIsEnabled(GL_BLEND);
            GLboolean profundidadAnterior;
            glGetBooleanv(GL_DEPTH_WRITEMASK, &profundidadAnterior);

            glDisable(GL_CULL_FACE);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glDepthMask(GL_FALSE);

            shaderPlasma.UseShader();
            glUniformMatrix4fv(plasmaProjection, 1, GL_FALSE, glm::value_ptr(projection));
            glUniformMatrix4fv(plasmaView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
            glUniform1i(plasmaTextura, 0);
            glActiveTexture(GL_TEXTURE0);
            FuegoAzulTexture.UseTexture();

            glm::mat4 modelFuegoNave = matNavePrincipal;
            modelFuegoNave = glm::translate(modelFuegoNave, glm::vec3(0.0f, 0.2f, -3.0f));
            modelFuegoNave = glm::rotate(modelFuegoNave, 1.57f, glm::vec3(1.0f, 0.0f, 0.0f));
            GLfloat anguloRotacionPlasma = glfwGetTime() * 360.0f;
            modelFuegoNave = glm::rotate(modelFuegoNave, glm::radians(anguloRotacionPlasma * 100.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            modelFuegoNave = glm::scale(modelFuegoNave, glm::vec3(1.5f, 2.0f, 1.5f));
            glUniformMatrix4fv(plasmaModel, 1, GL_FALSE, glm::value_ptr(modelFuegoNave));
            plasmaMesh->RenderMeshModel();

            glDepthMask(profundidadAnterior);
            if (!blendAnterior) glDisable(GL_BLEND);
            if (cullingAnterior) glEnable(GL_CULL_FACE);
            shaderList[0].UseShader();
        }

        // Luz puntual exclusiva de la ISS y sus paneles. No se agrega a las luces
        // generales: los asteroides mantienen su iluminacion sin cambios de color.
        const GLfloat faseColorLuzISS = std::fmod((GLfloat)glfwGetTime() / SEGUNDOS_COLOR_LUZ_ISS, 3.0f);
        const unsigned int indiceColorLuzISS = (unsigned int)faseColorLuzISS;
        const GLfloat avanceColorLuzISS = faseColorLuzISS - (GLfloat)indiceColorLuzISS;
        const GLfloat transicionColorLuzISS = SuavizarPaso((avanceColorLuzISS - 0.75f) / 0.25f);
        const glm::vec3 colorLuzISS = glm::mix(
            COLORES_LUZ_ISS[indiceColorLuzISS],
            COLORES_LUZ_ISS[(indiceColorLuzISS + 1) % 3], transicionColorLuzISS);
        // La fuente acompana tanto la traslacion como la rotacion de la ISS.
        const glm::vec3 posicionLuzISS = glm::vec3(matrizISS * glm::vec4(0.0f, 12.0f, 15.0f, 1.0f));
        PointLight luzPuntualISS(
            colorLuzISS.r, colorLuzISS.g, colorLuzISS.b,
            0.8f, 2.0f,                       // Brillo moderado: evita sobreexposicion
            posicionLuzISS.x, posicionLuzISS.y, posicionLuzISS.z,
            1.0f, 0.018f, 0.0008f            // Atenuacion en torno a la ISS
        );
        shaderList[0].SetPointLights(&luzPuntualISS, 1);

        // Estacion Espacial: sigue su ruta (la matriz se calcula antes de las luces)
        model = matrizISS;
        model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
        color = glm::vec3(1.0f, 1.0f, 1.0f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        ISS_M.RenderModel();
        color = glm::vec3(1.0f, 1.0f, 1.0f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));

        // Paneles solares: giran sobre su eje para quedar de frente al Sol
        const glm::vec3 pivotesPanelesISS[8] = {
            glm::vec3(2.829f,   0.295f, -21.075f), glm::vec3(2.829f,   0.295f, -14.781f),
            glm::vec3(2.829f,   0.295f,  14.781f), glm::vec3(2.829f,   0.295f,  21.075f),
            glm::vec3(2.829f, -15.789f, -21.075f), glm::vec3(2.829f, -15.789f, -14.781f),
            glm::vec3(2.829f, -15.789f,  14.781f), glm::vec3(2.829f, -15.789f,  21.075f)
        };
        // Direccion hacia el Sol en las coordenadas del modelo (la estacion gira y se inclina)
        glm::vec3 haciaSolISS = glm::vec3(glm::inverse(matrizISS) * glm::vec4(glm::normalize(-direccionDia), 0.0f));
        glm::vec2 proyeccionSolISS(haciaSolISS.x, haciaSolISS.z);
        GLfloat anguloPanelesISS = 0.0f;
        if (glm::length(proyeccionSolISS) > 0.0001f)
        {
            proyeccionSolISS = glm::normalize(proyeccionSolISS);
            if (proyeccionSolISS.x < 0.0f) proyeccionSolISS = -proyeccionSolISS;
            anguloPanelesISS = atan2(-proyeccionSolISS.y, proyeccionSolISS.x);
        }
        // Material opaco para que los paneles conserven su azul y no se laven con la luz rosa
        Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
        color = glm::vec3(1.0f, 1.0f, 1.0f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        for (int i = 0; i < 8; i++)
        {
            glm::mat4 matrizPanelISS = glm::translate(matrizISS, pivotesPanelesISS[i] * 3.0f);
            matrizPanelISS = glm::rotate(matrizPanelISS, anguloPanelesISS, glm::vec3(0.0f, 1.0f, 0.0f));
            matrizPanelISS = glm::scale(matrizPanelISS, glm::vec3(1.0f, factorPanelesISS, 1.0f));
            matrizPanelISS = glm::scale(matrizPanelISS, glm::vec3(3.0f, 3.0f, 3.0f));
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(matrizPanelISS));
            ISS_Panel.RenderModel();
        }
        color = glm::vec3(1.0f, 1.0f, 1.0f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));

        // Restaurar las luces normales ANTES de dibujar los siguientes modelos.
        // En particular, el cinturon de asteroides nunca recibira esta luz de color.
        shaderList[0].SetPointLights(pointLights, pointLightCount);

        // Sin esferas luminosas parpadeantes en la ISS.

        glUniform3f(uniformColor, 1.0f, 1.0f, 1.0f);
        // Naves del trafico de la estacion (llegan, se acoplan y salen)
        Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
        for (int i = 0; i < CANTIDAD_NAVES_TRAFICO; i++)
        {
            if (!estadosTrafico[i].visible) continue;
            glm::mat4 matNaveTrafico = glm::translate(glm::mat4(1.0f), estadosTrafico[i].posicion) * estadosTrafico[i].orientacion;
            matNaveTrafico = glm::rotate(matNaveTrafico, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
            matNaveTrafico = glm::scale(matNaveTrafico, glm::vec3(navesTrafico[i].escala));
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(matNaveTrafico));
            navesTrafico[i].modelo->RenderModel();
        }

        glm::vec3 frenteRocket(
            cos(glm::radians(inclinacionCamaraRocket)) * cos(glm::radians(anguloCamaraRocket)),
            sin(glm::radians(inclinacionCamaraRocket)),
            cos(glm::radians(inclinacionCamaraRocket)) * sin(glm::radians(anguloCamaraRocket))
        );
        frenteRocket = glm::normalize(frenteRocket);
        glm::vec3 derechaModeloRocket = glm::normalize(glm::cross(frenteRocket, glm::vec3(0.0f, 1.0f, 0.0f)));
        glm::vec3 arribaModeloRocket = glm::normalize(glm::cross(derechaModeloRocket, frenteRocket));

        glm::mat4 orientacionRocket(1.0f);
        orientacionRocket[0] = glm::vec4(derechaModeloRocket, 0.0f);
        orientacionRocket[1] = glm::vec4(arribaModeloRocket, 0.0f);
        orientacionRocket[2] = glm::vec4(-frenteRocket, 0.0f);

        glm::mat4 matrizRocket(1.0f);
        matrizRocket = glm::translate(matrizRocket, posicionRocket);
        matrizRocket = matrizRocket * orientacionRocket;
        matrizRocket = glm::rotate(matrizRocket, glm::radians(ajusteRotacionRocketY), glm::vec3(0.0f, 1.0f, 0.0f));
        matrizRocket = glm::rotate(matrizRocket, glm::radians(ajusteRotacionRocketX), glm::vec3(1.0f, 0.0f, 0.0f));
        matrizRocket = glm::scale(matrizRocket, glm::vec3(escalaRocket));

        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(matrizRocket));
        Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
        Rocket_M.RenderModel();

        if (plasmaRocketActivo && plasmaDisponible && tieneTexturaFuego)
        {
            const GLboolean cullingAnterior = glIsEnabled(GL_CULL_FACE);
            const GLboolean blendAnterior = glIsEnabled(GL_BLEND);
            GLboolean profundidadAnterior = GL_TRUE;
            glGetBooleanv(GL_DEPTH_WRITEMASK, &profundidadAnterior);

            glDisable(GL_CULL_FACE);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glDepthMask(GL_FALSE);

            shaderPlasma.UseShader();
            glUniformMatrix4fv(plasmaProjection, 1, GL_FALSE, glm::value_ptr(projection));
            glUniformMatrix4fv(plasmaView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
            glUniform1i(plasmaTextura, 0);
            glActiveTexture(GL_TEXTURE0);
            FuegoAzulTexture.UseTexture();

            glm::mat4 matrizFuegoRocket = matrizRocket;
            matrizFuegoRocket = glm::translate(matrizFuegoRocket, POSICION_PLASMA_ROCKET);
            const GLfloat anguloPlasmaRocket = (GLfloat)glfwGetTime() * 360.0f;
            matrizFuegoRocket = glm::rotate(matrizFuegoRocket, glm::radians(anguloPlasmaRocket * 100.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            matrizFuegoRocket = glm::scale(matrizFuegoRocket, ESCALA_PLASMA_ROCKET);

            glUniformMatrix4fv(plasmaModel, 1, GL_FALSE, glm::value_ptr(matrizFuegoRocket));
            plasmaMesh->RenderMeshModel();

            glDepthMask(profundidadAnterior);
            if (!blendAnterior) glDisable(GL_BLEND);
            if (cullingAnterior) glEnable(GL_CULL_FACE);
            shaderList[0].UseShader();
        }

        glUseProgram(0);
        mainWindow.swapBuffers();
    }
    return 0;
}