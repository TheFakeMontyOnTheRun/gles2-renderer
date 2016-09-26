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

#include "NativeBitmap.h"
#include "Texture.h"
#include "Material.h"
#include "Trig.h"
#include "TrigBatch.h"
#include "MeshObject.h"
#include "MaterialList.h"
#include "Scene.h"
#include "WavefrontOBJReader.h"

std::vector<std::string> consolidateLines(std::vector<char> vector);

float floatFrom(std::string str) {
	std::istringstream buffer(str);
	float temp;
	buffer >> temp;

	return temp;
}

int intFrom(std::string str) {
	std::istringstream buffer(str);
	int temp;
	buffer >> temp;

	return temp;
}

std::string readToEndOfLine(std::vector<std::string>::iterator &position,  std::vector<std::string>::iterator &end ) {
	std::stringstream toReturn;

	while ( position != end && *position != "\n") {
		toReturn << " " << *position;
	}

	return toReturn.str();
}

std::string filterComments(std::string input) {
	bool reading = true;
	std::stringstream output;

	for (auto &character : input) {

		if (character == '/') {
			reading = false;
		} else if (character == '\n') {
			reading = true;
		}

		if (reading) {
			output << character;
		}
	}

	return output.str();
}

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

glm::vec3 readVec3( std::vector<std::string>::iterator &position,  std::vector<std::string>::iterator &end ) {

	glm::vec3 toReturn;

	std::string x = *position;
	++position;
	std::string y = *position;
	++position;
	std::string z = *position;
	++position;

	toReturn.x = floatFrom( x );
	toReturn.y = floatFrom( y );
	toReturn.z = floatFrom( z );

	return toReturn;
}

glm::vec3 readVec2( std::vector<std::string>::iterator &position,  std::vector<std::string>::iterator &end ) {

	glm::vec3 toReturn;

	std::string x = *position;
	++position;
	std::string y = *position;
	++position;

	toReturn.x = floatFrom( x );
	toReturn.y = floatFrom( y );

	return toReturn;
}

std::vector<int> parseProperties( std::string vertexRawInformation ) {
	std::vector<int> toReturn;

	std::stringstream buffer;

	for ( auto c : vertexRawInformation ) {
		if ( c == '/' ) {
			toReturn.push_back( intFrom( buffer.str() ) );
			buffer.str(std::string());
		} else {
			buffer << c;
		}
	}

	toReturn.push_back( intFrom( buffer.str() ) );

	return toReturn;
}

std::shared_ptr<odb::MeshObject> readObjectFrom( std::vector<std::string>::iterator &line,  std::vector<std::string>::iterator &end, odb::MaterialList &materialList) {
	std::shared_ptr<odb::MeshObject> toReturn = std::make_shared<odb::MeshObject>();

	std::map< std::string, std::function<void(std::vector<std::string>)>> meshTokenHandlers;
	std::vector< glm::vec3 > vertices;
    std::vector< glm::vec3 > normals;
	std::vector< glm::vec2 > uvs;
	std::vector< odb::Trig > trigsInThisBatch;
	std::shared_ptr<odb::Material> currentMaterial;


	meshTokenHandlers[ "v" ] = [&](std::vector<std::string> stringLine) {
		auto position = stringLine.begin();
		++position;
		vertices.push_back( readVec3( position, end ) );
	};

	meshTokenHandlers[ "vt" ] = [&](std::vector<std::string> stringLine) {
		auto position = stringLine.begin();
		++position;
		uvs.push_back( readVec2( position, end ) );
	};

    meshTokenHandlers[ "vn" ] = [&](std::vector<std::string> stringLine) {
        auto position = stringLine.begin();
        ++position;
        normals.push_back( readVec3( position, end ) );
    };

    meshTokenHandlers[ "usemtl" ] = [&](std::vector<std::string> stringLine) {
		auto position = stringLine.begin();
		++position;
		std::string currentMaterialName = *position;
		currentMaterial = materialList.materials[ currentMaterialName ];
	};

	meshTokenHandlers[ "f" ] = [&](std::vector<std::string> stringLine) {

		odb::Trig trig;

		std::stringstream buffer;
		buffer << *line;

		std::vector<std::string> tokenList{std::istream_iterator<std::string>(buffer),
		                                   std::istream_iterator<std::string>{}};

		std::vector<int> vertexProperties;

		std::string p0 = tokenList[ 1 ];
		vertexProperties = parseProperties( p0 );
		trig.p0 = vertices[ vertexProperties[ 0 ] - 1 ];

		if ( vertexProperties.size() > 1 ) {
			trig.t0 = uvs[ vertexProperties[ 1 ] - 1 ];
		}

        if ( vertexProperties.size() > 2 ) {
            trig.n0 = normals[ vertexProperties[ 2 ] - 1 ];
            trig.nt0 = glm::vec3( 0.0f, -1.0f, 0.0f );
        }

        std::string p1 = tokenList[ 2 ];
		vertexProperties = parseProperties( p1 );
		trig.p1 = vertices[ vertexProperties[ 0 ] - 1 ];

		if ( vertexProperties.size() > 1 ) {
			trig.t1 = uvs[ vertexProperties[ 1 ] - 1 ];
		}

        if ( vertexProperties.size() > 2 ) {
            trig.n1 = normals[ vertexProperties[ 2 ] - 1 ];
            trig.nt1 = glm::vec3( 0.0f, -1.0f, 0.0f );
        }

        std::string p2 = tokenList[ 3 ];
		vertexProperties = parseProperties( p2 );
		trig.p2 = vertices[ vertexProperties[ 0 ] - 1 ];

		if ( vertexProperties.size() > 1 ) {
			trig.t2 = uvs[ vertexProperties[ 1 ] - 1 ];
		}

        if ( vertexProperties.size() > 2 ) {
            trig.n2 = normals[ vertexProperties[ 2 ] - 1 ];
            trig.nt2 = glm::vec3( 0.0f, -1.0f, 0.0f );
        }

        trigsInThisBatch.push_back( trig );
	};


	while ( line != end ) {
		std::string stringLine = *line;

		if ( stringLine[ 0 ] == 'o' ) {
			return toReturn;
		}

		std::stringstream buffer;
		buffer << filterComments(stringLine);

		std::vector<std::string> tokenList{std::istream_iterator<std::string>(buffer),
		                                   std::istream_iterator<std::string>{}};

		auto handler = meshTokenHandlers[ tokenList[ 0 ] ];

		if ( handler != nullptr ) {
			handler(tokenList);
		}
		++line;
	}

	odb::TrigBatch batch( trigsInThisBatch );
	toReturn->trigBatches.push_back( batch );

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

odb::MaterialList readMaterialsFrom( std::string materialData ) {
	odb::MaterialList toReturn;

	std::stringstream buffer;
	buffer << filterComments(materialData);
	std::vector<std::string> tokenList{std::istream_iterator<std::string>(buffer),
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

std::vector<std::string> consolidateLines(std::string data) {
	std::vector<std::string> lines;

	std::stringstream buffer;

	for ( auto c : data ) {
		if ( c == '\n' ) {
			lines.push_back( buffer.str() );
			buffer.str(std::string());
		} else {
			buffer << c;
		}
	}

	return lines;
}

void populateSceneWith( std::string meshData, std::shared_ptr<odb::Scene> scene ) {
	odb::MaterialList toReturn;
	std::vector<std::string> tokenList = consolidateLines( meshData );
	auto it = tokenList.begin();
	auto end = tokenList.end();

	while ( it != end ) {

		std::string stringLine = *it;

		if ( stringLine[ 0 ] ==  'o' ) {
			std::string id = stringLine;
			++it;
			auto object = readObjectFrom( it, end, scene->materialList );
			scene->meshObjects[ id ] = object;
		} else {
			++it;
		}
	}
}



std::shared_ptr<odb::Scene> readScene(std::string objFileContents, std::string materialFileContents) {

	std::shared_ptr<odb::Scene> scene = std::make_shared<odb::Scene>();
	scene->materialList = readMaterialsFrom(materialFileContents);
	populateSceneWith( objFileContents, scene );

	return scene;
}