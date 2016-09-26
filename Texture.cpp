//
// Created by monty on 28/05/16.
//
#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include <iostream>
#include <sstream>
#include <unordered_set>
#include <map>
#include <array>

#include "NativeBitmap.h"
#include "Texture.h"

namespace odb {
	Texture::Texture( unsigned int textureId, std::shared_ptr<NativeBitmap> bitmap ) : mTextureId( textureId ), mNativeBitmap( bitmap ) {
	}
}