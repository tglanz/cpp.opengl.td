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

Geometry *icosahedronGeo, *icosahedronFlatGeo;
Mesh *icosahedronMesh, *icosahedronFlatMesh;

Geometry *boxGeo;
Mesh *boxMesh;

Geometry *cylinderGeo, *cylinderFlatGeo;
Mesh *cylinderMesh, *cylinderFlatMesh;

Geometry *discoBallGeo, *sphereGeo;
Mesh *discoBallMesh, *sphereMesh;

Geometry *pointGeometry;
Mesh *pointMesh, *pointMesh2, *pointMesh3;

Geometry *planeGeo, *planeFlatGeo;
Mesh *planeMesh, *planeFlatMesh;

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

    for (unsigned int idx = 0; idx < defaultProgram.layout()->pointLightBlock().sequentialNumberOfLights; ++idx)
    {
        defaultProgram.layout()->pointLightBlock().lights[idx].attenuation.constant = 1;
        defaultProgram.layout()->pointLightBlock().lights[idx].attenuation.linear = 0;
        defaultProgram.layout()->pointLightBlock().lights[idx].attenuation.quadratic = 0;
    }

    defaultProgram.layout()->remapPointLightBlock();

    planeGeo = geometries::newPlane(6, 64, 6, 6);
    planeMesh = new Mesh(planeGeo, &materialA);
    allGeometries.push_back(planeGeo);
    allMeshes.push_back(planeMesh);
    planeMesh->transformation().translate(-3, -4, 0);

    planeFlatGeo = geometries::newPlaneFlat(6, 64, 6, 6);
    planeFlatMesh = new Mesh(planeFlatGeo, &materialA);
    allGeometries.push_back(planeFlatGeo);
    allMeshes.push_back(planeFlatMesh);
    planeFlatMesh->transformation().translate(3, -4, 0);

    icosahedronGeo = geometries::newIcosahedron(1);
    icosahedronMesh = new Mesh(icosahedronGeo, &materialA);
    icosahedronMesh->transformation().setTranslation(2, 0, 0);
    allGeometries.push_back(icosahedronGeo);
    allMeshes.push_back(icosahedronMesh);

    icosahedronFlatGeo = geometries::newIcosahedronFlat(1);
    icosahedronFlatMesh = new Mesh(icosahedronFlatGeo, &materialA);
    icosahedronFlatMesh->transformation().setTranslation(-2, 0, 0);
    allGeometries.push_back(icosahedronFlatGeo);
    allMeshes.push_back(icosahedronFlatMesh);

    boxGeo = geometries::newBox(1, 1, 1);
    boxMesh = new Mesh(boxGeo, &materialA);
    allGeometries.push_back(boxGeo);
    allMeshes.push_back(boxMesh);

    cylinderGeo = geometries::newCylinder(.5, 1, 1.5, 8);
    cylinderMesh = new Mesh(cylinderGeo, &materialA);
    allGeometries.push_back(cylinderGeo);
    allMeshes.push_back(cylinderMesh);

    cylinderFlatGeo = geometries::newCylinderFlat(.5, 1, 1.5, 8);
    cylinderFlatMesh = new Mesh(cylinderFlatGeo, &materialA);
    allGeometries.push_back(cylinderFlatGeo);
    allMeshes.push_back(cylinderFlatMesh);

    discoBallGeo = geometries::newGeodesicSphereFlat(1, 1);
    discoBallMesh = new Mesh(discoBallGeo, &materialA);
    allGeometries.push_back(discoBallGeo);
    allMeshes.push_back(discoBallMesh);

    sphereGeo = geometries::newGeodesicSphere(1, 6);
    sphereMesh = new Mesh(sphereGeo, &materialA);
    allGeometries.push_back(sphereGeo);
    allMeshes.push_back(sphereMesh);

    pointGeometry = geometries::newIcosahedron(1);
    allGeometries.push_back(pointGeometry);

    for (Geometry* geo : allGeometries)
        setupNewGeometry(geo);

    materialA.specularShininess() = 300.0f;
    materialA.specularColor() = glm::vec3(.7f, .7f, .7f);
    materialA.diffuseColor() = glm::vec3(.2f, .2f, .2f);

    materialB.diffuseColor() = glm::vec3(0, 0, 0);

    pointMesh = new Mesh(pointGeometry, &materialB);
    pointMesh->transformation().scale(.1);
    allMeshes.push_back(pointMesh);

    pointMesh2 = new Mesh(pointGeometry, &materialB);
    pointMesh2->transformation().scale(.1);
    allMeshes.push_back(pointMesh2);

    pointMesh3 = new Mesh(pointGeometry, &materialB);
    pointMesh3->transformation().scale(.1);
    allMeshes.push_back(pointMesh3);

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
    const float speed = 2.0f;

    float toRotate = delta * speed;

    glm::vec4 pointPosition = glm::vec4(-5.0f, 0.0f, 5.0f, 1.0f);
    glm::vec4 pointPosition2 = glm::vec4(0.0f, 0.0f, 5.0f, 1.0f);
    glm::vec4 pointPosition3 = glm::vec4(5.0f, 0.0f, 5.0f, 1.0f);

    defaultProgram.layout()->pointLightBlock().sequentialNumberOfLights = 3;

    defaultProgram.layout()->pointLightBlock().lights[0].position = pointPosition;
    pointMesh->transformation().setTranslation(pointPosition.x, pointPosition.y, pointPosition.z);

    defaultProgram.layout()->pointLightBlock().lights[1].position = pointPosition2;
    pointMesh2->transformation().setTranslation(pointPosition2.x, pointPosition2.y, pointPosition2.z);

    defaultProgram.layout()->pointLightBlock().lights[2].position = pointPosition3;
    pointMesh3->transformation().setTranslation(pointPosition3.x, pointPosition3.y, pointPosition3.z);

    defaultProgram.layout()->remapPointLightBlock();

    icosahedronFlatMesh->transformation().rotateRadiansAroundY(toRotate);
    icosahedronMesh->transformation().rotateRadiansAroundY(toRotate);

    boxMesh->transformation().setTranslation(0, 0, -2.5f);
    boxMesh->transformation().rotateRadiansAroundY(toRotate);

    cylinderMesh->transformation().setTranslation(2, 2, 0);
    cylinderMesh->transformation().rotateRadiansAroundY(toRotate);
    cylinderFlatMesh->transformation().setTranslation(-2, 2, 0);
    cylinderFlatMesh->transformation().rotateRadiansAroundY(toRotate);

    discoBallMesh->transformation().setTranslation(-2, -2, 0);
    discoBallMesh->transformation().rotateRadiansAroundY(toRotate);
    sphereMesh->transformation().setTranslation(2, -2, 0);
    sphereMesh->transformation().rotateRadiansAroundY(toRotate);

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
