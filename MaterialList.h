//
// Created by monty on 24/09/16.
//

#ifndef DUNGEONSOFNOUDAR_NDK_MATERIALLIST_H
#define DUNGEONSOFNOUDAR_NDK_MATERIALLIST_H

namespace odb {
	class MaterialList {
	public:
		std::map< std::string, std::shared_ptr<odb::Material>> materials;
	};
}

#endif //DUNGEONSOFNOUDAR_NDK_MATERIALLIST_H
