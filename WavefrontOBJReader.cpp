//
// Created by monty on 24/09/16.
//

#include "glm/glm.hpp"

#include <iostream>
#include <functional>
#include <memory>

#include <string>
#include <sstream>
#include <iterator>
#include <unordered_set>
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
#include "IFileLoaderDelegate.h"
#include "WavefrontMaterialReader.h"
#include "WavefrontOBJReader.h"

using Knights::floatFrom;
using Knights::intFrom;
using Knights::filterComments;

glm::vec3 readVec3( vector<std::string>::iterator &position,  vector<std::string>::iterator &end ) {

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

glm::vec2 readVec2( vector<std::string>::iterator &position,  vector<std::string>::iterator &end ) {

	glm::vec2 toReturn;

	std::string x = *position;
	++position;
	std::string y = *position;
	++position;

	toReturn.x = floatFrom( x );
	toReturn.y = 1.0f - floatFrom( y );

	return toReturn;
}

vector<int> parseProperties( std::string vertexRawInformation ) {
	vector<int> toReturn;

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

std::shared_ptr<odb::MeshObject> readObjectFrom( vector<std::string>::iterator &line,  vector<std::string>::iterator &end, odb::MaterialList &materialList) {
	std::shared_ptr<odb::MeshObject> toReturn = std::make_shared<odb::MeshObject>();

	std::map< std::string, std::function<void(vector<std::string>)>> meshTokenHandlers;
	vector< glm::vec3 > vertices;
    vector< glm::vec3 > normals;
	vector< glm::vec2 > uvs;
	vector< odb::Trig > trigsInThisBatch;
	std::shared_ptr<odb::Material> currentMaterial;


	meshTokenHandlers[ "v" ] = [&](vector<std::string> stringLine) {
		auto position = stringLine.begin();
		++position;
		vertices.push_back( readVec3( position, end ) );
	};

	meshTokenHandlers[ "vt" ] = [&](vector<std::string> stringLine) {
		auto position = stringLine.begin();
		++position;
		uvs.push_back( readVec2( position, end ) );
	};

    meshTokenHandlers[ "vn" ] = [&](vector<std::string> stringLine) {
        auto position = stringLine.begin();
        ++position;
        normals.push_back( readVec3( position, end ) );
    };

    meshTokenHandlers[ "usemtl" ] = [&](vector<std::string> stringLine) {
		auto position = stringLine.begin();
		++position;
		std::string currentMaterialName = *position;
		currentMaterial = materialList.materials[ currentMaterialName ];
	};

	meshTokenHandlers[ "f" ] = [&](vector<std::string> stringLine) {

		odb::Trig trig;

		std::stringstream buffer;
		buffer << *line;

		vector<std::string> tokenList;

		while ( buffer.good() ) {
			std::string tmp;
			buffer >> tmp;
			tokenList.push_back(tmp);
		}

		vector<int> vertexProperties;

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

		vector<std::string> tokenList;

		while ( buffer.good() ) {
			std::string tmp;
			buffer >> tmp;
			tokenList.push_back(tmp);
		}

		auto handler = meshTokenHandlers[ tokenList[ 0 ] ];

		if ( handler != nullptr ) {
			handler(tokenList);
		}
		++line;
	}

	odb::TrigBatch batch( trigsInThisBatch );
	toReturn->trigBatches.push_back( batch );
	batch.material = currentMaterial;

	return toReturn;
}

vector<std::string> consolidateLines(std::istream& data) {
	vector<std::string> lines;

	std::stringstream buffer;
	std::stringstream dataSource;

	dataSource << data.rdbuf();

	for ( auto c : dataSource.str() ) {
		if ( c == '\n' ) {
			lines.push_back( buffer.str() );
			buffer.str(std::string());
		} else {
			buffer << c;
		}
	}

	return lines;
}

std::string extractIdFrom( std::string line ) {
	return line.substr( 2 );
}

std::string extractMaterialFileFrom( std::string line ) {
	return line.substr( 7 );
}

void populateSceneWith( std::istream& meshData, std::shared_ptr<odb::Scene> scene, std::shared_ptr<Knights::IFileLoaderDelegate> fileLoader ) {
	vector<std::string> tokenList = consolidateLines( meshData );

	auto it = tokenList.begin();
	auto end = tokenList.end();

	while ( it != end ) {

		std::string stringLine = *it;
		if ( stringLine[ 0 ] == 'm' ) {
			std::string materialFile = extractMaterialFileFrom( stringLine );
			std::istringstream materialFileContents( fileLoader->loadFileFromPath( materialFile));
			scene->materialList = readMaterialsFrom(materialFileContents);
			++it;
		} else if ( stringLine[ 0 ] ==  'o' || stringLine[ 0 ] ==  'g' ) {
			std::string id = extractIdFrom( stringLine );
			++it;
			auto object = readObjectFrom( it, end, scene->materialList );
			scene->meshObjects[ id ] = object;
		} else {
			++it;
		}
	}
}



std::shared_ptr<odb::Scene> readScene(std::istream& objFileContents, std::istream& materialFileContents) {

	std::shared_ptr<odb::Scene> scene = std::make_shared<odb::Scene>();
	scene->materialList = readMaterialsFrom(materialFileContents);
	populateSceneWith( objFileContents, scene, nullptr );
	return scene;
}

std::shared_ptr<odb::Scene> readScene(std::istream& objFileContents, std::shared_ptr<Knights::IFileLoaderDelegate> fileLoader) {
	std::shared_ptr<odb::Scene> scene = std::make_shared<odb::Scene>();
	populateSceneWith( objFileContents, scene, fileLoader );
	return scene;
}
