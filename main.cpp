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

Material materialA(defaultProgram.layout());
Material materialB(defaultProgram.layout());

Geometry *planeGeo, *icosahedronGeo, *temporaryGeo, *cylinderGeo, *boxGeo, *sphereGeo;
Mesh *meshA, *meshB, *meshC, *meshD, *meshE, *meshF;
Mesh *pointMesh, *pointMesh2, *pointMesh3;
Mesh *boxMeshA, *boxMeshB, *boxMeshC;

glm::vec4 clearColor(0x00 / 255.0f, 0x90 / 255.0f, 0x00 / 255.0f, 1.0f);

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

    camera.setPosition(glm::vec3(0.0f, 0.0f, 20.0f));
    camera.lookAtTarget(glm::vec3(0.0f, 0, 0));

    defaultProgram.use();

    defaultProgram.layout()->directionalLightBlock().lights[0].direction = glm::vec4(0, 0, -1, 0);
    defaultProgram.layout()->directionalLightBlock().lights[0].intensities = glm::vec4(0, 0, 1, 0);
    defaultProgram.layout()->directionalLightBlock().lights[1].direction = glm::vec4(-1, 0, 0, 0);
    defaultProgram.layout()->directionalLightBlock().lights[1].intensities = glm::vec4(1, 0, 0, 0);
    defaultProgram.layout()->directionalLightBlock().sequentialNumberOfLights = 0;
    defaultProgram.layout()->remapDirectionalLightBlock();

    defaultProgram.layout()->pointLightBlock().lights[0].position = glm::vec4(0, 0, 15, 0);
    defaultProgram.layout()->pointLightBlock().lights[0].intensities = glm::vec4(0.0, 0.0, 1.0, 0.0);
    defaultProgram.layout()->pointLightBlock().lights[1].position = glm::vec4(0, 0, 15, 0);
    defaultProgram.layout()->pointLightBlock().lights[1].intensities = glm::vec4(1.0, 0.0, 0.0, 0.0);;
    defaultProgram.layout()->pointLightBlock().lights[2].position = glm::vec4(0, 0, 15, 0);
    defaultProgram.layout()->pointLightBlock().lights[2].intensities = glm::vec4(0.0, 1.0, 0.0, 0.0);;
    defaultProgram.layout()->pointLightBlock().sequentialNumberOfLights = 3;

    for (int idx = 0; idx < defaultProgram.layout()->pointLightBlock().sequentialNumberOfLights; ++idx)
    {
        defaultProgram.layout()->pointLightBlock().lights[idx].attenuation.constant = 1;
        defaultProgram.layout()->pointLightBlock().lights[idx].attenuation.linear = 0;
        defaultProgram.layout()->pointLightBlock().lights[idx].attenuation.quadratic = 0;
    }

    defaultProgram.layout()->remapPointLightBlock();

    planeGeo = geometries::newPlane(20, 20, 2, 2);
    allGeometries.push_back(planeGeo);

    temporaryGeo = geometries::newTemporary(0, 0);
    allGeometries.push_back(temporaryGeo);

    icosahedronGeo = geometries::newIcosahedron(1);
    allGeometries.push_back(icosahedronGeo);

    cylinderGeo = geometries::newCylinder(1.0f, .5f, 2.0f, 6.0f);
    allGeometries.push_back(cylinderGeo);

    boxGeo = geometries::newBox(1.0f, 1.0f, 2.0f);
    allGeometries.push_back(boxGeo);

    sphereGeo = geometries::newGeodesicSphere(1, 4);
    allGeometries.push_back(sphereGeo);
    //sphereGeo->printPositions();


    for (unsigned int idx = 0; idx < allGeometries.size(); ++idx)
    {
        setupNewGeometry(allGeometries[idx]);
    }

    materialA.specularShininess() = 300.0f;
    materialA.specularColor() = glm::vec3(.7f, .7f, .7f);
    materialA.diffuseColor() = glm::vec3(.2f, .2f, .2f);

    materialB.diffuseColor() = glm::vec3(0, 0, 0);

    boxMeshA = new Mesh(boxGeo, &materialA);
    boxMeshA->transformation().translate(-2, .5, 0);
    allMeshes.push_back(boxMeshA);

    boxMeshB = new Mesh(boxGeo, &materialA);
    boxMeshB->transformation().translate(0, .5, 0);
    allMeshes.push_back(boxMeshB);

    boxMeshC = new Mesh(boxGeo, &materialA);
    boxMeshC->transformation().translate(2, .5, 0);
    allMeshes.push_back(boxMeshC);

    meshA = new Mesh(sphereGeo, &materialA);
    meshA->transformation().translate(2, 3, 0);
    allMeshes.push_back(meshA);

    meshB = new Mesh(sphereGeo, &materialA);
    meshB->transformation().translate(0.0f, 3, 0);
    allMeshes.push_back(meshB);

    meshC = new Mesh(sphereGeo, &materialA);
    meshC->transformation().translate(-2.0f, 3, 0);
    allMeshes.push_back(meshC);

    meshD = new Mesh(cylinderGeo, &materialA);
    meshD->transformation().translate(3.0, -2.0, 0.0);
    allMeshes.push_back(meshD);

    meshE = new Mesh(cylinderGeo, &materialA);
    meshE->transformation().translate(0.0, -2.0, 0.0);
    allMeshes.push_back(meshE);

    meshF = new Mesh(cylinderGeo, &materialA);
    meshF->transformation().translate(-3.0, -2.0, 0.0);
    allMeshes.push_back(meshF);

    pointMesh = new Mesh(icosahedronGeo, &materialB);
    pointMesh->transformation().scale(.2);
    allMeshes.push_back(pointMesh);

    pointMesh2 = new Mesh(icosahedronGeo, &materialB);
    pointMesh2->transformation().scale(.2);
    allMeshes.push_back(pointMesh2);

    pointMesh3 = new Mesh(icosahedronGeo, &materialB);
    pointMesh3->transformation().scale(.2);
    allMeshes.push_back(pointMesh3);

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
    static float lightPosSpeed = 1;
    static float lightPosRadius = 5;

    glm::vec4 pointPosition = glm::vec4(cos(now * lightPosSpeed) * lightPosRadius, 0, sin(now * lightPosSpeed) * lightPosRadius, 0);
    glm::vec4 pointPosition2 = glm::vec4(cos( - now * 2 * lightPosSpeed) * lightPosRadius, 0, sin( - now * 2 * lightPosSpeed) * lightPosRadius, 0);
    glm::vec4 pointPosition3 = glm::vec4(0, cos( - now * lightPosSpeed) * lightPosRadius, sin( - now * lightPosSpeed) * lightPosRadius, 0);

    defaultProgram.layout()->pointLightBlock().sequentialNumberOfLights = 3;

    defaultProgram.layout()->pointLightBlock().lights[0].position = pointPosition;
    defaultProgram.layout()->pointLightBlock().lights[1].position = pointPosition2;
    defaultProgram.layout()->pointLightBlock().lights[2].position = pointPosition3;
    defaultProgram.layout()->remapPointLightBlock();

    pointMesh->transformation().setTranslation(pointPosition.x, pointPosition.y, pointPosition.z);
    pointMesh2->transformation().setTranslation(pointPosition2.x, pointPosition2.y, pointPosition2.z);
    pointMesh3->transformation().setTranslation(pointPosition3.x, pointPosition3.y, pointPosition3.z);

    meshA->transformation().rotateDegreesAroundZ(delta * rotSpeed);
    meshB->transformation().rotateDegreesAroundX(delta * rotSpeed);
    meshC->transformation().rotateDegreesAroundY(delta * rotSpeed);

    meshD->transformation().rotateDegreesAroundZ(delta * rotSpeed);
    meshE->transformation().rotateDegreesAroundX(delta * rotSpeed);
    meshF->transformation().rotateDegreesAroundY(delta * rotSpeed);

    boxMeshA->transformation().rotateDegreesAroundZ(delta * rotSpeed);
    boxMeshB->transformation().rotateDegreesAroundX(delta * rotSpeed);
    boxMeshC->transformation().rotateDegreesAroundY(delta * rotSpeed);

    camera.timeFactor() = delta;
    camera.update();

    lastTime = now;
}

void render()
{
    glClearBufferfv(GL_COLOR, 0, glm::value_ptr(clearColor));

    defaultProgram.use();

    glm::mat4 perspective = glm::perspective(FOV, RESOLUTION_WIDTH / RESOLUTION_HEIGHT, CLIP_NEAR, CLIP_FAR);
    glm::mat4 view = camera.getViewMatrix();

    glUniformMatrix4fv(10, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));

    ProgramLayout * layout = defaultProgram.layout();

    layout->viewBlock().viewProjection = view;
    layout->viewBlock().perspectiveProjection = perspective;
    layout->viewBlock().eyePosition = camera.getPosition();
    layout->remapViewBlockUBO();

    layout->setAmbientUniform(glm::vec3(.07f, .07f, .03f));

    for (unsigned int idx = 0; idx < allMeshes.size(); ++idx)
    {
        allMeshes[idx]->draw();
    }
}

void setupNewGeometry(Geometry * geo)
{
    geo->setupGpuBindings();
    geo->bind();
    defaultProgram.layout()->declareVertexAtribPointers();
    geo->unbind();
}

void checkError(const char * message)
{
    GLenum error = glGetError();

    printf("glGetError at %s : %d\n", message, error);
}
