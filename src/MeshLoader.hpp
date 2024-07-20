//
// Created by Chase Nagle on 7/18/24.
//

#include <string>
#include <vector>
#include <fstream>

#ifndef OPENGL_ENGINE_MESHLOADER_HPP
#define OPENGL_ENGINE_MESHLOADER_HPP



struct Vertex {
    float position[3];
    float normal[3]; //TODO: Will need to include texture coordinates
};

std::vector<Vertex> LoadOBJMesh(std::string filename){
    std::fstream meshData;
    meshData.open(filename);
    if (!meshData.is_open()) {
        throw std::runtime_error("Failed to open file");
    }

    while(meshData){
        
    }


}


#endif //OPENGL_ENGINE_MESHLOADER_HPP
