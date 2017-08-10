//
// Created by monty on 24/09/16.
//

#include "glm/glm.hpp"

#include <memory>

#include <string>
#include <iostream>
#include <sstream>
#include <iterator>
#include <unordered_set>
#include <functional>
#include <map>
#include <EASTL/vector.h>
#include <EASTL/array.h>

using eastl::vector;
using eastl::array;

#include "Common.h"
#include "NativeBitmap.h"
#include "Material.h"
#include "Trig.h"
#include "TrigBatch.h"
#include "MeshObject.h"
#include "MaterialList.h"
#include "Scene.h"
#include "WavefrontMaterialReader.h"

using Knights::floatFrom;
using Knights::intFrom;
using Knights::filterComments;

glm::vec3 readRGB( vector<std::string>::iterator &position,  vector<std::string>::iterator &end ) {

	glm::vec3 toReturn;

	std::string r = *position;
	++position;
	std::string g = *position;
	++position;
	std::string b = *position;
	++position;

	toReturn.r = floatFrom( r );
	toReturn.b = floatFrom( g );
	toReturn.b = floatFrom( b );

	return toReturn;
}

std::shared_ptr<odb::Material> readMaterial( vector<std::string>::iterator &position,  vector<std::string>::iterator &end ) {
	std::shared_ptr<odb::Material> toReturn = std::make_shared<odb::Material>();

	std::map< std::string, std::function<void(void)>> materialTokenHandlers;

	materialTokenHandlers[ "Ns" ] = [&]() {
		++position;
		std::string parameter = *position;
		++position;
		toReturn->specularExponent = floatFrom(parameter);
	};

	materialTokenHandlers[ "Ka" ] = [&]() {
		++position;
		toReturn->ambientColour = readRGB( position, end );
		++position;
	};

	materialTokenHandlers[ "Kd" ] = [&]() {
		++position;
		toReturn->diffuseColour = readRGB( position, end );
		++position;
	};


	materialTokenHandlers[ "map_Kd" ] = [&]() {
		++position;
		toReturn->diffuseMapFilename = *position;//readToEndOfLine( position, end );
		++position;
	};

	materialTokenHandlers[ "map_bump" ] = [&]() {
		++position;
		toReturn->normalMapFilename = *position;//readToEndOfLine( position, end );
		++position;
	};

	while ( position != end && *position != "newmtl" ) {

		auto handler = materialTokenHandlers[ *position ];

		if ( handler != nullptr ) {
			handler();
		} else {
			++position;
		}
	}
	return toReturn;
}

odb::MaterialList readMaterialsFrom( std::istream& materialData ) {
	odb::MaterialList toReturn;

	vector<std::string> tokenList;

	while ( materialData.good() ) {
		std::string tmp;
		materialData >> tmp;
		tokenList.push_back(tmp);
	}

	auto it = tokenList.begin();
	auto end = tokenList.end();

	while ( it != end ) {
		if ( *it ==  "newmtl" ) {
			++it;
			std::string id = *it;
			++it;
			auto material = readMaterial( it, end );
			toReturn.materials[ id ] = material;
		} else {
			++it;
		}
	}

	return toReturn;
}
