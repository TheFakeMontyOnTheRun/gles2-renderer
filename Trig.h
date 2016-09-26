//
// Created by monty on 21/02/16.
//

#ifndef LESSON10_TRIG_H
#define LESSON10_TRIG_H

namespace odb {
	class Trig {
		float *cachedVertexData;
		float *cachedUVData;
		float *cachedNormalData;
		float *cachedNormalTangentData;
	public:
		glm::vec3 p0;
		glm::vec2 t0;
		glm::vec3 nt0;
		glm::vec3 n0;

		glm::vec3 p1;
		glm::vec2 t1;
		glm::vec3 nt1;
		glm::vec3 n1;

		glm::vec3 p2;
		glm::vec2 t2;
		glm::vec3 nt2;
		glm::vec3 n2;

		Trig();

		const float *getVertexData();

		const float *getUVData();

		const float *getNormalData();

		const float *getNormalTangentData();

		virtual ~Trig();
	};
}

#endif //LESSON10_TRIG_H
