//
//  model_loader.hpp
//  FirstOpenGL
//
//  Created by Jiang Jie on 5/6/23.
//

#ifndef model_loader_h
#define model_loader_h

#include "model.h"
#include <string>

// The types of models
enum ModelFromate {
    M,
    OBJ,
    PLY
};

// T is the value type, e.g., float, double
// D is the degree, e.g., 3 for 3D model
template<typename T, unsigned int D>
class ModelLoader {
public:
    typedef Point<T,D>      PointType;
    typedef Face<T,D>       FaceType;
    typedef HalfEdge<T,D>   HalfEdgeType;
    typedef Model<T,D>      ModelType;
    typedef Vertex<T, D>    VertexType;
        
    ModelLoader(string path);
    
    // Load the file (path_) and build a model.
    Model<T, D>* load();
    
private:
    Model<T, D>* loadMModel();
    
    vector<string> split(const string& str, const string& pattern);
    
private:
    string path_;
};

typedef ModelLoader<float, 3> ModelLoader3F;
typedef ModelLoader<double, 3> ModelLoader3D;

#endif /* model_loader_h */
