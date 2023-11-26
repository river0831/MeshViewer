//
//  model_loader.cpp
//  FirstOpenGL
//
//  Created by Jiang Jie on 5/6/23.
//

#include "model_loader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>
#include <cmath>

template<typename T, unsigned int D>
ModelLoader<T, D>::ModelLoader(string path) {
    path_ = path;
}

template<typename T, unsigned int D>
Model<T, D>* ModelLoader<T, D>::load() {
    ModelFromate type = ModelFromate::M;
    switch (type) {
        case ModelFromate::M:
            return loadMModel();
        case ModelFromate::OBJ:
            return nullptr; // Not supported
        case ModelFromate::PLY:
            return nullptr; // Not supported
        default:
            return nullptr;
    }
}

// Load a model file with extension ".m"
// .m file is formated as:
// # comment line(s)
// Vertex index x y z
// ...
// Face index index index
// ...
template<typename T, unsigned int D>
Model<T, D>* ModelLoader<T, D>::loadMModel() {
    if (path_.empty())
        return nullptr;
    
    ifstream file;
    file.open(path_);
    if (file.fail())
        return nullptr;
    
    string content;
    
    // Go through the file to count vertices and faces first
    int v_count = 0;
    int f_count = 0;
    while(getline(file, content)) {
        // Check first character. If it is "#", ignore the line
        if (content.empty())
            continue;
        switch(content[0]) {
            case '#':
                continue;
            case 'V':
                ++v_count;
                continue;
            case 'F':
                ++f_count;
                continue;
            default:
                continue;
        }
    }
    
    // Reset ifstream to beginning
    file.clear();
    file.seekg(0);
    
    content.clear();
    vector<VertexType*> vertices(v_count);
    vector<vector<unsigned int>> elements(f_count);
    vector<string> str_list;
    while(getline(file, content)) {
        // Check first character. If it is "#", ignore the line
        if (content.empty())
            continue;
        if (content[0] == '#')
            continue;
        str_list = split(content, " ");
        unsigned int idx = stoi(str_list[1]) - 1;
        switch(content[0]) {
            case 'V':
                vertices[idx] = new VertexType();
                vertices[idx]->setX(stod(str_list[2]));
                vertices[idx]->setY(stod(str_list[3]));
                vertices[idx]->setZ(stod(str_list[4]));
                continue;
            case 'F':
                elements[idx].push_back(stoi(str_list[2]) - 1);
                elements[idx].push_back(stoi(str_list[3]) - 1);
                elements[idx].push_back(stoi(str_list[4]) - 1);
                continue;
            default:
                continue;
        }
    }
    
    // Construct edges & face, and link them
    vector<vector<HalfEdgeType*>> edges(v_count);
    vector<FaceType*> faces(f_count);
    
    std::unordered_map<string, HalfEdgeType*> mp;
    
    for (int i = 0; i < elements.size(); ++i) {
        vector<HalfEdgeType*> face_edges;
        FaceType* face = new FaceType();
        faces[i] = face;
        for (int j = 0; j < elements[i].size(); ++j) {
            unsigned int start = elements[i][j];
            unsigned int end = j == elements[i].size() - 1? elements[i][0] : elements[i][j + 1];
            HalfEdgeType* edge = new HalfEdgeType();
            face_edges.push_back(edge);
            
            // Set edge'sface
            edge->setFace(face);
            // Set edge's origin
            edge->setOrigin(vertices[start]);
            // Set edge for vertex
            if (vertices[start]->edge() == nullptr)
                vertices[start]->setEdge(edge);
            
            string name = to_string(min(start, end)) + "," + to_string(max(start,end));
            auto iter = mp.find(name);
            if (iter != mp.end()) {
                // The twin edge has been created.
                iter->second->setTwin(edge);
                edge->setTwin(iter->second);
                // Remove from mp
                mp.erase(iter);
            } else {
                mp[name] = edge;
            }
        }
        
        // Link prev & next
        for (int j = 0; j < face_edges.size(); ++j) {
            if (j == 0)
                face_edges[j]->setPrev(face_edges.back());
            else
                face_edges[j]->setPrev(face_edges[j - 1]);
            
            if (j != face_edges.size() - 1)
                face_edges[j]->setNext(face_edges[j + 1]);
            else
                face_edges[j]->setNext(face_edges[0]);
        }
        
        // Set face's edge
        face->setEdge(face_edges[0]);
    }
        
    Model<T, D>* model = new Model<T, D>(vertices, elements, faces);
    model->init();
    return model;
}

template<typename T, unsigned int D>
vector<string> ModelLoader<T, D>::split(const string& str, const string& pattern) {
    char * strc = new char[strlen(str.c_str())+1];
    strcpy(strc, str.c_str());
    vector<string> res;
    if (str.empty())
        return res;
    char* temp = strtok(strc, pattern.c_str());
    while(temp != NULL)
    {
        res.push_back(string(temp));
        temp = strtok(NULL, pattern.c_str());
    }
    delete[] strc;
    return res;
}

template class ModelLoader<float, 3>;
template class ModelLoader<double, 3>;
