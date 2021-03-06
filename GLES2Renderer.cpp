//
// Created by monty on 23/11/15.
//
#include <iostream>
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <memory>
#include <map>
#include <EASTL/vector.h>
#include <EASTL/array.h>

using eastl::vector;
using eastl::array;



#ifdef __APPLE__
#if TARGET_IOS
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>
#else
#import <OpenGL/OpenGL.h>
#import <OpenGL/gl3.h>
#endif
#else
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <EGL/egl.h>
#endif

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <iostream>


#include <string>

#include "NativeBitmap.h"
#include "Material.h"
#include "Trig.h"
#include "TrigBatch.h"
#include "MeshObject.h"
#include "MaterialList.h"
#include "Scene.h"
#include "Logger.h"
#include "GLES2Renderer.h"

namespace odb {
//Counter Clockwise
    const float GLES2Renderer::cubeVertices[]{
//    4________5
//    /|       /|
//   / |      / |
// 0/__|___1_/  |
//  | 7|____|___|6
//  |  /    |  /
//  | /     | /
// 3|/______|/2
//x, y, z, u, v, nx, ny, nz, nw, tangent1, tangent2

		    //FLOOR
		    1.0f, -1.0f, -1.0f, 1.0f,    1.0f, 1.0f,   -1.0f, 0.0f, 0.0f,     0.0, -1.0, 0.0,   //6 - 0
		    -1.0f, -1.0f, -1.0f, 1.0f,   0.0f, 1.0f,   -1.0f, 0.0f, 0.0f,     0.0, -1.0, 0.0, //7 - 1
		    1.0f, -1.0f, 1.0f, 1.0f,     1.0f, 0.0f,   -1.0f, 0.0f, 0.0f,     0.0, -1.0, 0.0, //2 - 2
		    -1.0f, -1.0f, 1.0f, 1.0f,    0.0f, 0.0f,   -1.0f, 0.0f, 0.0f,     0.0, -1.0, 0.0, //3 - 3
		    //CEILING
		    -1.0f, 1.0f, 1.0f, 1.0f,     0.0f, 1.0f,    1.0f, 0.0f, 0.0f,     0.0, -1.0, 0.0, //0 - 4
		    1.0f, 1.0f, 1.0f, 1.0f,      1.0f, 1.0f,    1.0f, 0.0f, 0.0f,     0.0, -1.0, 0.0, //1 - 5
		    -1.0f, 1.0f, -1.0f, 1.0f,    0.0f, 0.0f,    1.0f, 0.0f, 0.0f,     0.0, -1.0, 0.0,//4 - 6
		    1.0f, 1.0f, -1.0f, 1.0f,     1.0f, 0.0f,    1.0f, 0.0f, 0.0f,     0.0, -1.0, 0.0,//5 - 7
		    //SOUTH
		    -1.0f, 1.0f, 1.0f, 1.0f,     0.0f, 1.0f,    0.0f, 0.0f, 1.0f,     0.0, -1.0, 0.0,  //0 - 8
		    1.0f, 1.0f, 1.0f, 1.0f,      1.0f, 1.0f,    0.0f, 0.0f, 1.0f,     0.0, -1.0, 0.0,   //1 - 9
		    1.0f, -1.0f, 1.0f, 1.0f,     1.0f, 0.0f,    0.0f, 0.0f, 1.0f,     0.0, -1.0, 0.0,  //2 - 10
		    -1.0f, -1.0f, 1.0f, 1.0f,    0.0f, 0.0f,    0.0f, 0.0f, 1.0f,     0.0, -1.0, 0.0, //3 - 11
		    //NORTH
		    -1.0f, 1.0f, -1.0f, 1.0f,    0.0f, 0.0f,    0.0f, 0.0f, -1.0f,    0.0, -1.0, 0.0,//4 - 12
		    1.0f, 1.0f, -1.0f, 1.0f,     1.0f, 0.0f,    0.0f, 0.0f, -1.0,     0.0, -1.0, 0.0,//5 - 13
		    1.0f, -1.0f, -1.0f, 1.0f,    1.0f, 1.0f,    0.0f, 0.0f, -1.0f,    0.0, -1.0, 0.0,//6 - 14
		    -1.0f, -1.0f, -1.0f, 1.0f,   0.0f, 1.0f,    0.0f, 0.0f, -1.0f,    0.0, -1.0, 0.0,//7 -15
		    //EAST
		    1.0f, 1.0f, 1.0f, 1.0f,      0.0f, 1.0f,    1.0f, 0.0f, 0.0f,     0.0, -1.0, 0.0, //1 - 16
		    1.0f, -1.0f, 1.0f, 1.0f,     0.0f, 0.0f,    1.0f, 0.0f, 0.0f,     0.0, -1.0, 0.0, //2 - 17
		    1.0f, 1.0f, -1.0f, 1.0f,     1.0f, 1.0f,    1.0f, 0.0f, 0.0f,     0.0, -1.0, 0.0,  //5 - 18
		    1.0f, -1.0f, -1.0f, 1.0f,    1.0f, 0.0f,    1.0f, 0.0f, 0.0f,     0.0, -1.0, 0.0, //6 - 19
		    //WEST
		    -1.0f, 1.0f, 1.0f, 1.0f,     1.0f, 1.0f,   -1.0f, 0.0f, 0.0f,     0.0, -1.0, 0.0,//0 - 20
		    -1.0f, -1.0f, 1.0f, 1.0f,    1.0f, 0.0f,   -1.0f, 0.0f, 0.0f,     0.0, -1.0, 0.0,//3 - 21
		    -1.0f, 1.0f, -1.0f, 1.0f,    0.0f, 1.0f,   -1.0f, 0.0f, 0.0f,     0.0, -1.0, 0.0,//4 - 22
		    -1.0f, -1.0f, -1.0f, 1.0f,   0.0f, 0.0f,   -1.0f, 0.0f, 0.0f,     0.0, -1.0, 0.0//7 - 23
    };


    const unsigned short GLES2Renderer::cubeIndices[]{
		    20, 21, 23,
		    22, 20, 23,
		    16, 18, 19,
		    16, 19, 17,
		    13, 12, 14,
		    14, 12, 15,
		    8, 9, 10,
		    8, 10, 11,
		    6, 7, 5,
		    6, 5, 4,
		    3, 2, 1,
		    1, 0, 2
    };


    const glm::vec4 GLES2Renderer::ambientLightFullColor = glm::vec4(0.15f, 0.15f, 0.15f, 1.0f);

    const glm::vec4 GLES2Renderer::ambientLightOffColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

    GLuint GLES2Renderer::uploadTextureData(int *pixels, int width, int height) {
	    // Texture object handle
	    GLuint textureId = 0;

	    //Generate texture storage
	    glGenTextures(1, &textureId);

	    //specify what we want for that texture
	    glBindTexture(GL_TEXTURE_2D, textureId);

	    //upload the data
	    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
	                 pixels);

	    return textureId;
    }


    void GLES2Renderer::printGLString(const char *name, GLenum s) {
	    const char *v = (const char *) glGetString(s);
	    Logger::log("GL %s = %s\n", name, v);
    }

    void GLES2Renderer::checkGlError(const char *op) {
	    for (GLint error = glGetError(); error; error = glGetError()) {
		    Logger::log("after %s() glError (0x%x)\n", op, error);
	    }
    }

    GLuint GLES2Renderer::loadShader(GLenum shaderType, const char *pSource) {
	    auto shader = glCreateShader(shaderType);
	    if (shader) {
		    glShaderSource(shader, 1, &pSource, NULL);
		    glCompileShader(shader);
		    GLint compiled = 0;
		    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
		    if (!compiled) {
			    GLint infoLen = 0;
			    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
			    if (infoLen) {
				    char *buf = (char *) malloc(infoLen);
				    if (buf) {
					    glGetShaderInfoLog(shader, infoLen, NULL, buf);
					    Logger::log("Could not compile shader %d:\n%s\n", shaderType, buf);
					    free(buf);
				    }
				    glDeleteShader(shader);
				    shader = 0;
			    }
		    }
	    }
	    return shader;
    }

    GLuint GLES2Renderer::createProgram(const char *pVertexSource, const char *pFragmentSource) {
	    auto vertexShader = loadShader(GL_VERTEX_SHADER, pVertexSource);
	    if (!vertexShader) {
		    return 0;
	    }

	    auto pixelShader = loadShader(GL_FRAGMENT_SHADER, pFragmentSource);
	    if (!pixelShader) {
		    return 0;
	    }

	    auto program = glCreateProgram();
	    if (program) {
		    glAttachShader(program, vertexShader);
		    checkGlError("glAttachShader");
		    glAttachShader(program, pixelShader);
		    checkGlError("glAttachShader");
		    glLinkProgram(program);
		    GLint linkStatus = GL_FALSE;
		    glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
		    if (linkStatus != GL_TRUE) {
			    GLint bufLength = 0;
			    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
			    if (bufLength) {
				    char *buf = (char *) malloc(bufLength);
				    if (buf) {
					    glGetProgramInfoLog(program, bufLength, NULL, buf);
					    Logger::log("Could not link program:\n%s\n", buf);
					    free(buf);
				    }
			    }
			    glDeleteProgram(program);
			    program = 0;
		    }
	    }
	    return program;
    }

    void GLES2Renderer::printVerboseDriverInformation() {
	    printGLString("Version", GL_VERSION);
	    printGLString("Vendor", GL_VENDOR);
	    printGLString("Renderer", GL_RENDERER);
	    printGLString("Extensions", GL_EXTENSIONS);
    }

    GLES2Renderer::GLES2Renderer() {
//start off as identity - late we will init it with proper values.
	    cubeTransformMatrix = glm::mat4(1.0f);
	    projectionMatrix = glm::mat4(1.0f);
	    textureData = nullptr;
	    vertexAttributePosition = 0;
	    modelMatrixAttributePosition = 0;
	    projectionMatrixAttributePosition = 0;
	    gProgram = 0;
	    currentFilter = GL_LINEAR;

	    ambientLightColor = ambientLightFullColor;
	    diffuseLightWorldPosition = glm::normalize(glm::vec4(0.0f, 0.0f, -6.0f, 1.0f));
	    diffuseLightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	    reset();
    }

    GLES2Renderer::~GLES2Renderer() {
	    deleteVBOs();
	    glDeleteTextures(1, &textureId);
	    glDeleteTextures(1, &normalMapId);
    }

    bool GLES2Renderer::init(float w, float h, const std::string &vertexShader,
                           const std::string &fragmentShader) {

	    printVerboseDriverInformation();

	    gProgram = createProgram(vertexShader.c_str(), fragmentShader.c_str());

	    if (!gProgram) {
		    Logger::log("Could not create program.");
		    return false;
	    }

	    fetchShaderLocations();

	    glViewport(0, 0, w, h);
	    checkGlError("glViewport");

	    projectionMatrix = glm::perspective(45.0f, w / h, 0.1f, 100.0f);

	    createVBOs();

	    glActiveTexture(GL_TEXTURE0);
	    textureId = uploadTextureData(textureData, textureWidth, textureHeight);
	    glActiveTexture(GL_TEXTURE1);
	    normalMapId = uploadTextureData(normals, textureWidth, textureHeight);

	    glEnable(GL_DEPTH_TEST);
	    glDepthFunc(GL_LEQUAL);
	    glFrontFace(GL_CW);
	    glDepthMask(true);
	    return true;
    }

    glm::mat4 GLES2Renderer::resetTransformMatrices(glm::vec3 translate) {
	    glm::mat4 identity = glm::mat4(1.0f);
	    glm::vec3 xAxis = glm::vec3(1.0f, 0.0f, 0.0f);
	    glm::vec3 yAxis = glm::vec3(0.0f, 1.0f, 0.0f);
	    glm::mat4 translated = glm::translate(identity, translate);
	    glm::mat4 rotatedAroundXAxis = glm::rotate(translated, cubeRotationAngleYZ, xAxis);
	    glm::mat4 rotatedAroundYAxis = glm::rotate(rotatedAroundXAxis, cubeRotationAngleXZ, yAxis);
	    return rotatedAroundYAxis;
    }

    void GLES2Renderer::fetchShaderLocations() {

	    vertexAttributePosition = glGetAttribLocation(gProgram, "aPosition");
	    modelMatrixAttributePosition = glGetUniformLocation(gProgram, "uModel");
	    projectionMatrixAttributePosition = glGetUniformLocation(gProgram, "uProjection");
	    samplerUniformPosition = glGetUniformLocation(gProgram, "sTexture");
	    normalMapUniformPosition = glGetUniformLocation(gProgram, "sNormalMap");
	    textureCoordinatesAttributePosition = glGetAttribLocation(gProgram, "aTexCoord");

	    ambientLightColorShaderLocation = glGetUniformLocation(gProgram, "uAmbientLightColor");
	    diffuseLightColorShaderLocation = glGetUniformLocation(gProgram, "uDiffuseLightColor");

	    normalAttributePosition = glGetAttribLocation(gProgram, "aVertexNormal");

	    diffuseLightPosition = glGetUniformLocation(gProgram, "uDiffuseLightPosition");

	    checkGlError("all other positions");
	    tangentVectorShaderPosition = glGetAttribLocation(gProgram, "aTangent");
    }

    void GLES2Renderer::drawGeometry(const int vertexVbo, const int indexVbo, int vertexCount,
                                   const glm::mat4 &transform) {

	    glBindBuffer(GL_ARRAY_BUFFER, vertexVbo);
	    glEnableVertexAttribArray(vertexAttributePosition);
	    glEnableVertexAttribArray(textureCoordinatesAttributePosition);
	    glEnableVertexAttribArray(normalAttributePosition);
	    glEnableVertexAttribArray(tangentVectorShaderPosition);


	    glUniform4fv(ambientLightColorShaderLocation, 1, &ambientLightColor[0]);

	    glUniformMatrix4fv(modelMatrixAttributePosition, 1, false, &transform[0][0]);

	    glVertexAttribPointer(vertexAttributePosition, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 12,
	                          (void *) (sizeof(float) * 0));
	    glVertexAttribPointer(textureCoordinatesAttributePosition, 2, GL_FLOAT, GL_TRUE,
	                          sizeof(float) * 12, (void *) (sizeof(float) * 4));
	    glVertexAttribPointer(normalAttributePosition, 3, GL_FLOAT, GL_TRUE, sizeof(float) * 12,
	                          (void *) (sizeof(float) * 6));
	    glVertexAttribPointer(tangentVectorShaderPosition, 3, GL_FLOAT, GL_FALSE,
	                          sizeof(float) * 12, (void *) (sizeof(float) * 9));


	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVbo);
	    glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_SHORT, 0);

	    glDisableVertexAttribArray(tangentVectorShaderPosition);
	    glDisableVertexAttribArray(vertexAttributePosition);
	    glDisableVertexAttribArray(textureCoordinatesAttributePosition);
	    glDisableVertexAttribArray(normalAttributePosition);

	    glBindBuffer(GL_ARRAY_BUFFER, 0);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void GLES2Renderer::deleteVBOs() {
	    glDeleteBuffers(1, &vboCubeVertexDataIndex);
	    glDeleteBuffers(1, &vboCubeVertexIndicesIndex);
    }

    void GLES2Renderer::createVBOs() {
	    glGenBuffers(1, &vboCubeVertexDataIndex);
	    glBindBuffer(GL_ARRAY_BUFFER, vboCubeVertexDataIndex);
	    glBufferData(GL_ARRAY_BUFFER, 4 * 6 * sizeof(float) * 12, cubeVertices, GL_STATIC_DRAW);
	    glBindBuffer(GL_ARRAY_BUFFER, 0);

	    glGenBuffers(1, &vboCubeVertexIndicesIndex);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboCubeVertexIndicesIndex);
	    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(GLushort), cubeIndices, GL_STATIC_DRAW);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void GLES2Renderer::clearBuffers() {
	    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	    glClearDepthf(1.0f);
	    checkGlError("glClearColor");
	    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	    checkGlError("glClear");
    }

    void GLES2Renderer::setPerspective() {
	    glUniformMatrix4fv(projectionMatrixAttributePosition, 1, false, &projectionMatrix[0][0]);
    }

    void GLES2Renderer::prepareShaderProgram() {
	    glUseProgram(gProgram);
	    checkGlError("glUseProgram");

	    glUniform4fv(diffuseLightColorShaderLocation, 1, &diffuseLightColor[0]);
	    glUniform4fv(ambientLightColorShaderLocation, 1, &ambientLightColor[0]);
	    glUniform4fv(diffuseLightPosition, 1, &diffuseLightWorldPosition[0]);

	    glUniform1i(samplerUniformPosition, 0);
	    glActiveTexture(GL_TEXTURE0);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, currentFilter);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, currentFilter);

	    glUniform1i(normalMapUniformPosition, 1);
	    glActiveTexture(GL_TEXTURE1);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, currentFilter);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, currentFilter);
    }

    void GLES2Renderer::render() {
	    clearBuffers();
	    prepareShaderProgram();
	    setPerspective();

//	    drawGeometry(vboCubeVertexDataIndex,
//	                 vboCubeVertexIndicesIndex,
//	                 36,
//	                 resetTransformMatrices(glm::vec3(2.0f, 0.0f, -6.0f))
//	    );
//
//
//	    drawGeometry(vboCubeVertexDataIndex,
//	                 vboCubeVertexIndicesIndex,
//	                 36,
//	                 glm::translate(glm::mat4(1.0f),
//	                                glm::vec3(-2.0f, -2.0f, -10.0f))//cubeTransformMatrix
//	    );
//
//	    drawGeometry(vboCubeVertexDataIndex,
//	                 vboCubeVertexIndicesIndex,
//	                 36,
//	                 glm::translate(glm::mat4(1.0f),
//	                                glm::vec3(2.0f, -2.0f, -10.0f))//cubeTransformMatrix
//	    );
//
//
//	    drawGeometry(vboCubeVertexDataIndex,
//	                 vboCubeVertexIndicesIndex,
//	                 36,
//	                 glm::translate(glm::mat4(1.0f),
//	                                glm::vec3(2.0f, -2.0f, -4.0f))//cubeTransformMatrix
//	    );
//
//	    drawGeometry(vboCubeVertexDataIndex,
//	                 vboCubeVertexIndicesIndex,
//	                 36,
//	                 glm::translate(glm::mat4(1.0f),
//	                                glm::vec3(-2.0f, 2.0f, -5.0f))//cubeTransformMatrix
//	    );

    }

    void GLES2Renderer::setTexture(int *bitmapData, int *normalData, int width, int height,
                                 int format) {
	    textureData = bitmapData;
	    normals = normalData;
	    textureWidth = width;
	    textureHeight = height;
    }

    void GLES2Renderer::tick() {
	    cubeRotationAngleYZ += rotationYZSpeed;
	    cubeRotationAngleXZ += rotationXZSpeed;
    }

    void GLES2Renderer::shutdown() {
	    delete textureData;
	    Logger::log("Shutdown!\n");
    }

    void GLES2Renderer::toggleFiltering() {
//        if (currentFilter == GL_NEAREST) {
//            currentFilter = GL_LINEAR;
//            LOGI("Using GL_LINEAR\n");
//        } else {
//            currentFilter = GL_NEAREST;
//            LOGI("Using GL_NEAREST\n");
//        }
    }

    void GLES2Renderer::toggleLightning() {
//        if (ambientLightColor == ambientLightFullColor) {
//            ambientLightColor = ambientLightOffColor;
//        } else {
//            ambientLightColor = ambientLightFullColor;
//        }
    }

    void GLES2Renderer::speedUpXZ() {
	    rotationXZSpeed += 0.125f;
    }

    void GLES2Renderer::speedDownXZ() {
	    rotationXZSpeed -= 0.125f;
    }

    void GLES2Renderer::speedUpYZ() {
	    rotationYZSpeed += 0.125f;
    }

    void GLES2Renderer::speedDownYZ() {
	    rotationYZSpeed -= 0.125f;
    }

    void GLES2Renderer::reset() {
	    cubeRotationAngleYZ = 0.0f;
	    rotationYZSpeed = 0.0f;
	    cubeRotationAngleXZ = 0.0f;
	    rotationXZSpeed = 0.0f;
    }

    void GLES2Renderer::setSpeeds(const glm::vec2 &velocity) {
	    rotationXZSpeed = -velocity.x;
	    rotationYZSpeed = -velocity.y;
    }

	void GLES2Renderer::drawTrigBatch( odb::TrigBatch &batch, glm::vec3 translation, float xzAngle, float yzAngle ) {
		prepareShaderProgram();
		setPerspective();

		cubeRotationAngleYZ = yzAngle;
		cubeRotationAngleXZ = xzAngle;

		glm::mat4 trans = resetTransformMatrices(translation);
                //glm::rotate( glm::translate( glm::mat4(1.0f), glm::vec3( 0.0f, 0.0f, -5.0f ) ), cubeRotationAngleXZ, glm::vec3( 0.0f, 1.0f, 0.0f ) );

		glUniformMatrix4fv(modelMatrixAttributePosition, 1, false, &trans[0][0]);
		checkGlError("before drawing");

        glEnableVertexAttribArray(vertexAttributePosition);
        glEnableVertexAttribArray(textureCoordinatesAttributePosition);
        glEnableVertexAttribArray(normalAttributePosition);
        glEnableVertexAttribArray(tangentVectorShaderPosition);


        glUniform4fv(ambientLightColorShaderLocation, 1, &ambientLightColor[0]);
    	batch.draw(vertexAttributePosition, textureCoordinatesAttributePosition, normalAttributePosition , tangentVectorShaderPosition);

		glDisableVertexAttribArray(vertexAttributePosition);
		glDisableVertexAttribArray(textureCoordinatesAttributePosition);
        glDisableVertexAttribArray(normalAttributePosition);
        glDisableVertexAttribArray(tangentVectorShaderPosition);

	}
}
