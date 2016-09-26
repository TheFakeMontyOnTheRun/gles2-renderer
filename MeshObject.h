//
// Created by monty on 24/09/16.
//

#ifndef DUNGEONSOFNOUDAR_NDK_MESHOBJECT_H
#define DUNGEONSOFNOUDAR_NDK_MESHOBJECT_H

namespace odb {
	class MeshObject {
	private:
	public:
		std::vector<TrigBatch> trigBatches;
		std::string id;
	};
}

#endif //DUNGEONSOFNOUDAR_NDK_MESHOBJECT_H
