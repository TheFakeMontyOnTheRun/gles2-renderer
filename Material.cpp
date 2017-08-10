//
// Created by monty on 24/09/16.
//
#include "glm/glm.hpp"

#include <iostream>
#include <memory>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <unordered_set>
#include <map>
#include <cstdint>
#include <string>
#include <tuple>
#include <utility>
#include <stdio.h>
#include <cmath>
#include <EASTL/vector.h>
#include <EASTL/array.h>

using eastl::vector;
using eastl::array;


#include "NativeBitmap.h"
#include "Material.h"

namespace odb {
    const Material::Id Material::NullId = std::string{"-"};
}
