//
// Created by monty on 25/02/16.
//

#ifndef LESSON10_TRIGBATCH_H
#define LESSON10_TRIGBATCH_H

namespace odb {
	class TrigBatch {
		float *vertex;
		float *uv;
		float *normal;
		float *normaltangent;
		int vertexCount;
		std::shared_ptr<Material> material;
	public:
		TrigBatch(const std::vector<Trig> &triangles);

		void draw(unsigned int vertexAttributePosition, unsigned int textureCoordinatesAttributePosition, unsigned int normalAttributePosition, unsigned int tangentVectorShaderPosition);
	};
}

#endif //LESSON10_TRIGBATCH_H
