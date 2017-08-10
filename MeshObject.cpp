//
// Created by monty on 24/09/16.
//
#include <string>
#include "glm/glm.hpp"
#include <memory>
#include <EASTL/vector.h>
#include <EASTL/array.h>

using eastl::vector;
using eastl::array;


#include "NativeBitmap.h"
#include "Material.h"
#include "Trig.h"
#include "TrigBatch.h"
#include "MeshObject.h"


namespace odb {
	const MeshObject::Id MeshObject::NullId = std::string{"-"};
}
