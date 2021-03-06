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

        GLuint createProgram(const char *pVertexSource, const char *pFragmentSource);

        GLuint loadShader(GLenum shaderType, const char *pSource);

        const static float cubeVertices[6 * 4 * 12];
        const static unsigned short cubeIndices[6 * 6];

        const static glm::vec4 ambientLightFullColor;
        const static glm::vec4 ambientLightOffColor;

        glm::mat4 cubeTransformMatrix;
        glm::mat4 projectionMatrix;

        GLint tangentVectorShaderPosition;

		GLint vertexAttributePosition;
		GLint modelMatrixAttributePosition;
		GLint samplerUniformPosition;
		GLint normalMapUniformPosition;
		GLint textureCoordinatesAttributePosition;
		GLint projectionMatrixAttributePosition;
		GLint normalAttributePosition;

	    GLint diffuseLightPosition;
        GLuint gProgram;

        GLuint textureId;
	    GLuint normalMapId;

        //VBO stuff
        GLuint vboCubeVertexDataIndex;
        GLuint vboCubeVertexIndicesIndex;

        int *textureData;
        int *normals;
        int textureWidth;
        int textureHeight;
        GLuint currentFilter;
        float cubeRotationAngleYZ;
        float cubeRotationAngleXZ;

        glm::vec4 diffuseLightWorldPosition;
        glm::vec4 diffuseLightColor;
        glm::vec4 ambientLightColor;

        GLuint diffuseLightColorShaderLocation;
        GLuint ambientLightColorShaderLocation;

        float rotationXZSpeed;
        float rotationYZSpeed;
    public:
        GLES2Renderer();

        ~GLES2Renderer();

        bool init(float w, float h, const std::string &vertexShader,
                  const std::string &fragmentShader);

        void setTexture(int *bitmapData, int *normalData, int width, int height, int format);

        void render();

        void shutdown();

        void tick();

        void toggleFiltering();

        void toggleLightning();

        void speedUpXZ();

        void speedDownXZ();

        void speedUpYZ();

        void speedDownYZ();

        void reset();

        void setSpeeds(const glm::vec2 &param);

        void drawTrigBatch( odb::TrigBatch &batch, glm::vec3 translation, float xzAngle, float yzAngle );

	    void printGLString(const char *name, GLenum s);

	    void checkGlError(const char *op);

	    GLuint uploadTextureData(int *pixels, int width, int height);
    };
}
#endif //GLES2RENDERER_GLES2RENDERER_H
