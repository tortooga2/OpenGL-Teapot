//
// Created by Chase Nagle on 7/18/24.
//

#include <string>
#include <vector>
#include <fstream>

#include "Vectors.hpp"

#ifndef OPENGL_ENGINE_MESHLOADER_HPP
#define OPENGL_ENGINE_MESHLOADER_HPP



using namespace std;



vector<vector<string>> convertFileToVector(string filename){
    fstream file (filename);

    string f;
    vector<vector<string>> digestedFile;

    while(getline(file, f)){
        vector<string> tempVector;
        string temp;
        for(int i = 0; i < f.length(); i++){
            if(f[i] != ' '){
                temp += f[i];
            }
            else{

                tempVector.push_back(temp);
                temp = "";
            }
        }
        tempVector.push_back(temp);
        digestedFile.push_back(tempVector);
    };

    return digestedFile;

}

std::vector<std::string> split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(str);

    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }

    return tokens;
}


vector<Vertex> unpackOBJ(string filename){

    vector<vector<string>> obj = convertFileToVector(filename);



    vector<Vector3> vertices;
    vector<Vector3> normal;

    vector<Vertex> mesh;

    for(int i = 0; i < obj.size(); i++) {
        if (obj[i][0] == "v") {
            float v1 = stof(obj[i][1]);
            float v2 = stof(obj[i][2]);
            float v3 = stof(obj[i][3]);
            vertices.push_back({v1, v2, v3});
        }
        if (obj[i][0] == "vn") {
            float v1 = stof(obj[i][1]);
            float v2 = stof(obj[i][2]);
            float v3 = stof(obj[i][3]);
            normal.push_back({v1, v2, v3});
        }
        if (obj[i][0] == "f"){
//
            for(int j = 1; j < obj[i].size(); j++){
                vector<string> face = split(obj[i][j], '/');

                int vIndex = stoi(face[0]) - 1;
                int nIndex = stoi(face[2]) - 1;

                Vertex v = {
                        {vertices[vIndex].Position[0], vertices[vIndex].Position[1], vertices[vIndex].Position[2]},
                        {normal[nIndex].Position[0], normal[nIndex].Position[1], normal[nIndex].Position[2]}
                };


                mesh.push_back( v );

            }

        }



    }



    return mesh;
}


#endif //OPENGL_ENGINE_MESHLOADER_HPP
