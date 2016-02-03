
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

Geometry *planeGeo, *icosahedronGeo, *temporaryGeo;
Mesh *meshA, *meshB, *meshC, *meshD;

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

    for (unsigned int idx = 0; idx < allGeometries.size(); ++idx)
    {
        setupNewGeometry(allGeometries[idx]);
    }

    materialA.specularShininess() = 1.5f;
    materialA.specularColor() = glm::vec3(1.0f, 0.7f, 0.7f);

    materialB.specularShininess() = 1.5f;

    meshA = new Mesh(icosahedronGeo, &materialA);
    allMeshes.push_back(meshA);

    meshB = new Mesh(icosahedronGeo, &materialA);
    meshB->transformation().translate(2.0f, 0, 0);
    allMeshes.push_back(meshB);

    meshC = new Mesh(icosahedronGeo, &materialA);
    meshC->transformation().translate(-2.0f, 0, 0);
    allMeshes.push_back(meshC);

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

    float rotSpeed = 30;

    meshA->transformation().rotateDegreesAroundZ(delta * rotSpeed);
    meshB->transformation().rotateDegreesAroundX(delta * rotSpeed);
    meshC->transformation().rotateDegreesAroundY(delta * rotSpeed);

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

    layout.setAmbientUniform(glm::vec3(.05, 0.01, .01));
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
