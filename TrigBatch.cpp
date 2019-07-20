//
// Created by monty on 25/02/16.
//
#include "glm/glm.hpp"


#if defined(__ANDROID__ ) || defined(__EMSCRIPTEN__) || defined(MESA_GLES2) || defined(TARGET_IOS)
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
#endif


#include <memory>

#include <iostream>
#include <sstream>
#include <unordered_set>
#include <map>
#include <vector>
#include <array>

using std::vector;
using std::array;


#include "NativeBitmap.h"
#include "Material.h"
#include "Trig.h"
#include "TrigBatch.h"

namespace odb {

	void TrigBatch::draw(unsigned int vertexAttributePosition,
	                     unsigned int textureCoordinatesAttributePosition) {
#if defined(__ANDROID__ ) || defined(__EMSCRIPTEN__) || defined(MESA_GLES2) || defined(TARGET_IOS)
		glVertexAttribPointer(vertexAttributePosition, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
		                      vertex);

		glVertexAttribPointer(textureCoordinatesAttributePosition, 2, GL_FLOAT, GL_FALSE,
		                      2 * sizeof(float), uv);

		glDrawArrays(GL_TRIANGLES, 0, vertexCount);
#endif
	}

	void TrigBatch::draw(unsigned int vertexAttributePosition,
	                     unsigned int textureCoordinatesAttributePosition, unsigned int normalAttributePosition, unsigned int tangentVectorShaderPosition) {
#if defined(__ANDROID__ ) || defined(__EMSCRIPTEN__) || defined(MESA_GLES2) || defined(TARGET_IOS)
		glVertexAttribPointer(vertexAttributePosition, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
		                      vertex);

		glVertexAttribPointer(textureCoordinatesAttributePosition, 2, GL_FLOAT, GL_FALSE,
		                      2 * sizeof(float), uv);

		glVertexAttribPointer(normalAttributePosition, 3, GL_FLOAT, GL_TRUE,
							  sizeof(float) * 3, normal);
		glVertexAttribPointer(tangentVectorShaderPosition, 3, GL_FLOAT, GL_FALSE,
							  sizeof(float) * 3, normaltangent);

		glDrawArrays(GL_TRIANGLES, 0, vertexCount);
#endif
	}

	TrigBatch::TrigBatch(const vector<Trig> &triangles) {
		size_t size = triangles.size();

		cachedMeshData = new float[5 * 3 * size];
		vertex = new float[3 * 3 * size];
		normal = new float[3 * 3 * size];
		normaltangent = new float[3 * 3 * size];
		uv = new float[3 * 2 * size];
		int vertexIndex = 0;
		int normalIndex = 0;
		int normalTangentIndex = 0;
		int uvIndex = 0;
		int meshIndex = 0;
		vertexCount = 0;
		for (auto &trig : triangles) {
			cachedMeshData[ meshIndex++ ] = trig.p0.x;
			cachedMeshData[ meshIndex++ ] = trig.p0.y;
			cachedMeshData[ meshIndex++ ] = trig.p0.z;
			cachedMeshData[ meshIndex++ ] = trig.t0.x;
			cachedMeshData[ meshIndex++ ] = trig.t0.y;

			cachedMeshData[ meshIndex++ ] = trig.p1.x;
			cachedMeshData[ meshIndex++ ] = trig.p1.y;
			cachedMeshData[ meshIndex++ ] = trig.p1.z;
			cachedMeshData[ meshIndex++ ] = trig.t1.x;
			cachedMeshData[ meshIndex++ ] = trig.t1.y;

			cachedMeshData[ meshIndex++ ] = trig.p2.x;
			cachedMeshData[ meshIndex++ ] = trig.p2.y;
			cachedMeshData[ meshIndex++ ] = trig.p2.z;
			cachedMeshData[ meshIndex++ ] = trig.t2.x;
			cachedMeshData[ meshIndex++ ] = trig.t2.y;

			vertex[vertexIndex++] = trig.p0.x;
			vertex[vertexIndex++] = trig.p0.y;
			vertex[vertexIndex++] = trig.p0.z;
			normal[normalIndex++] = trig.n0.x;
			normal[normalIndex++] = trig.n0.y;
			normal[normalIndex++] = trig.n0.z;
			normaltangent[normalTangentIndex++] = trig.nt0.x;
			normaltangent[normalTangentIndex++] = trig.nt0.y;
			normaltangent[normalTangentIndex++] = trig.nt0.z;
			uv[uvIndex++] = trig.t0.x;
			uv[uvIndex++] = trig.t0.y;
			++vertexCount;

			vertex[vertexIndex++] = trig.p1.x;
			vertex[vertexIndex++] = trig.p1.y;
			vertex[vertexIndex++] = trig.p1.z;
			normal[normalIndex++] = trig.n1.x;
			normal[normalIndex++] = trig.n1.y;
			normal[normalIndex++] = trig.n1.z;
			normaltangent[normalTangentIndex++] = trig.nt1.x;
			normaltangent[normalTangentIndex++] = trig.nt1.y;
			normaltangent[normalTangentIndex++] = trig.nt1.z;
			uv[uvIndex++] = trig.t1.x;
			uv[uvIndex++] = trig.t1.y;
			++vertexCount;

			vertex[vertexIndex++] = trig.p2.x;
			vertex[vertexIndex++] = trig.p2.y;
			vertex[vertexIndex++] = trig.p2.z;
			normal[normalIndex++] = trig.n2.x;
			normal[normalIndex++] = trig.n2.y;
			normal[normalIndex++] = trig.n2.z;
			normaltangent[normalTangentIndex++] = trig.nt2.x;
			normaltangent[normalTangentIndex++] = trig.nt2.y;
			normaltangent[normalTangentIndex++] = trig.nt2.z;
			uv[uvIndex++] = trig.t2.x;
			uv[uvIndex++] = trig.t2.y;
			++vertexCount;
		}
	}

	float *TrigBatch::getVertexData() {
		return this->cachedMeshData;
	}

	int TrigBatch::getVertexCount() {
		return this->vertexCount;
	}

	float *TrigBatch::getUVData() {
		return this->uv;
	}

	float *TrigBatch::getNormalData() {
		return this->normal;
	}

	unsigned short *TrigBatch::getIndices() {
		unsigned short *toReturn = new unsigned short[ vertexCount ];

		for ( unsigned short c = 0; c < vertexCount; ++c ) {
			toReturn[ c ] = c;
		}
		return toReturn;
	}

	int TrigBatch::getIndicesCount() {
		return vertexCount;
	}
}
