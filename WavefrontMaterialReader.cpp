//
// Created by monty on 24/09/16.
//

#include <glm/glm.hpp>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <memory>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <iterator>
#include <unordered_set>

#include <map>

#include <array>
#include "Common.h"
#include "NativeBitmap.h"
#include "Texture.h"
#include "Material.h"
#include "Trig.h"
#include "TrigBatch.h"
#include "MeshObject.h"
#include "MaterialList.h"
#include "Scene.h"
#include "WavefrontMaterialReader.h"



glm::vec3 readRGB( std::vector<std::string>::iterator &position,  std::vector<std::string>::iterator &end ) {

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

std::shared_ptr<odb::Material> readMaterial( std::vector<std::string>::iterator &position,  std::vector<std::string>::iterator &end ) {
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

	std::vector<std::string> tokenList{std::istream_iterator<std::string>(materialData),
	                                   std::istream_iterator<std::string>{}};
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