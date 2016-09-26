//
// Created by monty on 21/02/16.
//
#include <glm/glm.hpp>

#include "Trig.h"
namespace odb {
	Trig::Trig() {
		cachedVertexData = nullptr;
		cachedUVData = nullptr;
		cachedNormalData = nullptr;
	}

	const float *Trig::getUVData() {
		if (cachedUVData == nullptr) {
			cachedUVData = new float[3 * 2];

			cachedUVData[0] = t0.x;
			cachedUVData[1] = t0.y;

			cachedUVData[2] = t1.x;
			cachedUVData[3] = t1.y;

			cachedUVData[4] = t2.x;
			cachedUVData[5] = t2.y;
		}

		return cachedUVData;
	}


	const float *Trig::getVertexData() {
		if (cachedVertexData == nullptr) {
			cachedVertexData = new float[3 * 3];

			cachedVertexData[0] = p0.x;
			cachedVertexData[1] = p0.y;
			cachedVertexData[2] = p0.z;

			cachedVertexData[3] = p1.x;
			cachedVertexData[4] = p1.y;
			cachedVertexData[5] = p1.z;

			cachedVertexData[6] = p2.x;
			cachedVertexData[7] = p2.y;
			cachedVertexData[8] = p2.z;
		}

		return cachedVertexData;
	}

	const float *Trig::getNormalData() {
		if (cachedNormalData == nullptr) {
			cachedNormalData = new float[3 * 3];

			cachedNormalData[0] = n0.x;
			cachedNormalData[1] = n0.y;
			cachedNormalData[2] = n0.z;

			cachedNormalData[3] = n1.x;
			cachedNormalData[4] = n1.y;
			cachedNormalData[5] = n1.z;

			cachedNormalData[6] = n2.x;
			cachedNormalData[7] = n2.y;
			cachedNormalData[8] = n2.z;
		}

		return cachedNormalData;
	}

	const float *Trig::getNormalTangentData() {
		if (cachedNormalTangentData == nullptr) {
			cachedNormalTangentData = new float[3 * 3];

			cachedNormalTangentData[0] = nt0.x;
			cachedNormalTangentData[1] = nt0.y;
			cachedNormalTangentData[2] = nt0.z;

			cachedNormalTangentData[3] = nt1.x;
			cachedNormalTangentData[4] = nt1.y;
			cachedNormalTangentData[5] = nt1.z;

			cachedNormalTangentData[6] = nt2.x;
			cachedNormalTangentData[7] = nt2.y;
			cachedNormalTangentData[8] = nt2.z;
		}

		return cachedNormalTangentData;
	}

	Trig::~Trig() {
		delete cachedUVData;
		delete cachedVertexData;
		delete cachedNormalData;
	}
}