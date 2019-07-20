//
// Created by monty on 23/11/15.
//

#ifndef GLES2RENDERER_GLES2RENDERER_H
#define GLES2RENDERER_GLES2RENDERER_H

namespace odb {

    class GLES2Renderer {

        void fetchShaderLocations();

        void setPerspective();

        void prepareShaderProgram();

        void clearBuffers();

        glm::mat4 resetTransformMatrices( glm::vec3 translate );

        void printVerboseDriverInformation();

        void createVBOs();

        void deleteVBOs();

        void drawGeometry(const int vertexVbo, const int indexVbo, int vertexCount,
                          const glm::mat4 &transform);

        uint createProgram(const char *pVertexSource, const char *pFragmentSource);

        uint loadShader(GLenum shaderType, const char *pSource);

        const static float cubeVertices[6 * 4 * 12];
        const static unsigned short cubeIndices[6 * 6];

        const static glm::vec4 ambientLightFullColor;
        const static glm::vec4 ambientLightOffColor;

        glm::mat4 cubeTransformMatrix;
        glm::mat4 projectionMatrix;

		int vertexAttributePosition;
		int modelMatrixAttributePosition;
		int samplerUniformPosition;
		int normalMapUniformPosition;
		int textureCoordinatesAttributePosition;
		int projectionMatrixAttributePosition;
		int normalAttributePosition;

	    int diffuseLightPosition;
        uint gProgram;

        uint textureId;
	    uint normalMapId;

        //VBO stuff
        uint vboCubeVertexDataIndex;
        uint vboCubeVertexIndicesIndex;

        int *textureData;
        int *normals;
        int textureWidth;
        int textureHeight;
        uint currentFilter;
        float cubeRotationAngleYZ;
        float cubeRotationAngleXZ;

        glm::vec4 diffuseLightWorldPosition;
        glm::vec4 diffuseLightColor;
        glm::vec4 ambientLightColor;

        uint diffuseLightColorShaderLocation;
        uint ambientLightColorShaderLocation;

        float rotationXZSpeed;
        float rotationYZSpeed;
    public:
        GLES2Renderer();

        ~GLES2Renderer();

        bool init(float w, float h, const std::string &vertexShader,
                  const std::string &fragmentShader);

        void render();

        void reset();

	    void printGLString(const char *name, GLenum s);

	    void checkGlError(const char *op);

	    uint uploadTextureData(int *pixels, int width, int height);
    };
}
#endif //GLES2RENDERER_GLES2RENDERER_H
