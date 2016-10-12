//
// Created by monty on 24/09/16.
//

#ifndef DUNGEONSOFNOUDAR_NDK_MESHOBJECT_H
#define DUNGEONSOFNOUDAR_NDK_MESHOBJECT_H

namespace odb {
	class MeshObject {
	private:
	public:

		using Id = std::string;
		static const Id NullId;

		std::vector<TrigBatch> trigBatches;
		Id mId;
	};
}

#endif //DUNGEONSOFNOUDAR_NDK_MESHOBJECT_H
