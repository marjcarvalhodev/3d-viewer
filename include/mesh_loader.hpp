
#ifndef MESH_LOADER_HPP
#define MESH_LOADER_HPP

#include "tiny_obj_loader.h"
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>

struct MeshData
{
    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<float> texCoords;
};

namespace MeshLoader
{

    inline void test()
    {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn, err;

        // Test LoadObj
        bool success = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, "assets/models/ball/ball.obj");
        if (!success)
        {
            std::cerr << "Failed to load OBJ.\n";
        }
        else
        {
            std::cout << "Loaded OBJ.\n";
        }
    }

    inline MeshData loadModel(const std::string &objPath)
    {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn, err;

        bool success = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, objPath.c_str());
        if (!success)
        {
            throw std::runtime_error("Failed to load OBJ file: " + objPath + "\n" + err);
        }

        if (!warn.empty())
        {
            std::cerr << "TinyObjLoader Warning: " << warn << std::endl;
        }

        MeshData meshData;

        for (const auto &shape : shapes)
        {
            for (const auto &index : shape.mesh.indices)
            {
                // Vertex positions
                meshData.vertices.push_back(attrib.vertices[3 * index.vertex_index + 0]); // x
                meshData.vertices.push_back(attrib.vertices[3 * index.vertex_index + 1]); // y
                meshData.vertices.push_back(attrib.vertices[3 * index.vertex_index + 2]); // z

                // Normals
                if (!attrib.normals.empty() && index.normal_index >= 0)
                {
                    meshData.normals.push_back(attrib.normals[3 * index.normal_index + 0]); // nx
                    meshData.normals.push_back(attrib.normals[3 * index.normal_index + 1]); // ny
                    meshData.normals.push_back(attrib.normals[3 * index.normal_index + 2]); // nz
                }

                // Texture coordinates
                if (!attrib.texcoords.empty() && index.texcoord_index >= 0)
                {
                    meshData.texCoords.push_back(attrib.texcoords[2 * index.texcoord_index + 0]); // u
                    meshData.texCoords.push_back(attrib.texcoords[2 * index.texcoord_index + 1]); // v
                }
            }
        }

        return meshData;
    }

} // namespace MeshLoader

#endif
