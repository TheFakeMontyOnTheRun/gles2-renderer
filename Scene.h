//
// Created by monty on 24/09/16.
//

#ifndef DUNGEONSOFNOUDAR_NDK_SCENE_H
#define DUNGEONSOFNOUDAR_NDK_SCENE_H

namespace odb {
	class Scene {
	public:
		std::map< std::string, std::shared_ptr<MeshObject>> meshObjects;
		MaterialList materialList;
	};
}

#endif //DUNGEONSOFNOUDAR_NDK_SCENE_H
