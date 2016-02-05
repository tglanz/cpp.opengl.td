
#include <iostream>
#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "td/Display.h"
#include "td/log.h"

#include "td/gpu/Program.h"
#include "td/gpu/ProgramLayout.h"

#include "td/gpu/Mesh.h"
#include "td/gpu/Material.h"
#include "td/gpu/Vertex.h"
#include "td/gpu/Geometry.h"
#include "td/gpu/geometries.h"

#include "td/gpu/Transformations.h"

#include "td/cameras/Camera.h"
#include "td/cameras/FirstPersonCamera.h"

using namespace std;

using namespace td;
using namespace gpu;
using namespace cameras;

const float RESOLUTION_WIDTH = 1024.0f;
const float RESOLUTION_HEIGHT = 780.0f;
const float CLIP_NEAR = 1.0f;
const float CLIP_FAR = 1000.0f;
const float FOV = 120.0f;

bool init();
void finalize();
void render();
void update();
void checkError(const char* message);

void setupNewGeometry(Geometry * geo);

td::MouseState mouse(&Display::instance());
td::KeyboardState keyboard(&Display::instance());

td::gpu::Program defaultProgram = td::gpu::Program(
    "resources/shaders/color/color.vert",
    "resources/shaders/color/color.frag"
);

td::cameras::FirstPersonCamera camera = td::cameras::FirstPersonCamera(&mouse, &keyboard);

std::vector<Geometry*> allGeometries;
std::vector<Mesh*> allMeshes;

Material materialA(&defaultProgram.layout()), materialB(&defaultProgram.layout());

Geometry *planeGeo, *icosahedronGeo, *temporaryGeo, *cylinderGeo;
Mesh *meshA, *meshB, *meshC, *meshD, *meshE, *meshF;

glm::vec4 clearColor(0x00 / 255.0f, 0xC0 / 255.0f, 0x00 / 255.0f, 1.0f);

int main(void)
{
    log::info("Application start");
    log::lineBreak();

    Display display = Display::instance();

    if (display.initialize(RESOLUTION_WIDTH, RESOLUTION_HEIGHT, "TD") == false)
    {
        log::error("Unable to initialize display");
        return -1;
    }

    if (init() == false)
    {
        log::error("Unable to initialize program");
        return -1;
    }

    while (display.shouldClose() == false)
    {
        display.clearBuffers();

        update();
        render();

        display.update();
    }

    finalize();

    return 1;
}

bool init()
{
    if (defaultProgram.compile() == false)
    {
        return false;
    }

    defaultProgram.acquireLayoutLocations();

    camera.setPosition(glm::vec3(0.0f, 0.0f, 15.0f));
    camera.lookAtTarget(glm::vec3(0.0f, 0, 0));

    defaultProgram.use();

    planeGeo = geometries::newPlane(20, 20, 2, 2);
    allGeometries.push_back(planeGeo);

    temporaryGeo = geometries::newTemporary(0, 0);
    allGeometries.push_back(temporaryGeo);

    icosahedronGeo = geometries::newIcosahedron();
    allGeometries.push_back(icosahedronGeo);

    cylinderGeo = geometries::newCylinder(1.0f, .5f, 2.0f, 64.0f);
    allGeometries.push_back(cylinderGeo);

    for (unsigned int idx = 0; idx < allGeometries.size(); ++idx)
    {
        setupNewGeometry(allGeometries[idx]);
    }

    materialA.specularShininess() = 30.0f;
    materialA.specularColor() = glm::vec3(1.0f, 1.0f, 1.0f);

    materialB.specularShininess() = 1.5f;

    meshD = new Mesh(cylinderGeo, &materialA);
    meshD->transformation().translate(3.0, 0.0, 0.0);
    allMeshes.push_back(meshD);

    meshE = new Mesh(cylinderGeo, &materialA);
    meshE->transformation().translate(0.0, 0.0, 0.0);
    allMeshes.push_back(meshE);

    meshF = new Mesh(cylinderGeo, &materialA);
    meshF->transformation().translate(-3.0, 0.0, 0.0);
    allMeshes.push_back(meshF);


    meshA = new Mesh(icosahedronGeo, &materialA);
    meshA->transformation().translate(2, 3, 0);
    allMeshes.push_back(meshA);

    meshB = new Mesh(icosahedronGeo, &materialA);
    meshB->transformation().translate(0.0f, 3, 0);
    allMeshes.push_back(meshB);

    meshC = new Mesh(icosahedronGeo, &materialA);
    meshC->transformation().translate(-2.0f, 3, 0);
    allMeshes.push_back(meshC);

    meshA->transformation().rotateDegreesAroundZ(45);
    meshB->transformation().rotateDegreesAroundX(45);
    meshC->transformation().rotateDegreesAroundY(45);


    return true;
}

void finalize()
{
    for (unsigned int idx = 0; idx < allGeometries.size(); ++idx)
    {
        delete allGeometries[idx];
    }

    allGeometries.clear();

    for (unsigned int idx = 0; idx < allMeshes.size(); ++idx)
    {
        delete allMeshes[idx];
    }

    allMeshes.clear();
}

void update()
{
    mouse.update();

    static float lastTime = glfwGetTime();
    float now = glfwGetTime();
    float delta = now - lastTime;


    static float rotSpeed = 100;


    meshA->transformation().rotateDegreesAroundZ(delta * rotSpeed);
    meshB->transformation().rotateDegreesAroundX(delta * rotSpeed);
    meshC->transformation().rotateDegreesAroundY(delta * rotSpeed);

    meshD->transformation().rotateDegreesAroundZ(delta * rotSpeed);
    meshE->transformation().rotateDegreesAroundX(delta * rotSpeed);
    meshF->transformation().rotateDegreesAroundY(delta * rotSpeed);

    camera.timeFactor() = delta;
    camera.update();

    lastTime = now;
}

void render()
{
    glClearBufferfv(GL_COLOR, 0, glm::value_ptr(clearColor));

    defaultProgram.use();

    ProgramLayout layout = defaultProgram.layout();

    glm::mat4 perspective = glm::perspective(FOV, RESOLUTION_WIDTH / RESOLUTION_HEIGHT, CLIP_NEAR, CLIP_FAR);
    glm::mat4 view = camera.getViewMatrix();

    glUniformMatrix4fv(10, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));

    layout.setViewPerspectiveProjectionsUniforms(view, perspective);

    layout.setAmbientUniform(glm::vec3(.05f, .05f, .05f));
    layout.setEyePositionUniform(camera.getPosition());

    for (unsigned int idx = 0; idx < allMeshes.size(); ++idx)
    {
        allMeshes[idx]->draw();
    }
}

void setupNewGeometry(Geometry * geo)
{
    geo->setupGpuBindings();
    geo->bind();
    defaultProgram.layout().declareVertexAtribPointers();
    geo->unbind();
}

void checkError(const char * message)
{
    GLenum error = glGetError();

    printf("glGetError at %s : %d\n", message, error);
}
