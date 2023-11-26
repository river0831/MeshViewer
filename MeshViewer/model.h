//
//  model.h
//  FirstOpenGL
//
//  Created by Jiang Jie on 5/6/23.
//

#ifndef model_h
#define model_h

#include "point.h"
#include "half_edge.h"
#include "face.h"
#include "vertex.h"
#include <vector>
using namespace std;

enum AttrType{
    POINT,  // Point coordinates
    NORMAL, // Point's normal
    //COLOR   // Point's color
    NONE
};

template<unsigned int D>
class ModelAttr {
public:
    ModelAttr() {
        type_ = AttrType::NONE;
        size_ = 0;
    }
    
    ModelAttr(AttrType type) {
        type_ = type;
        switch(type) {
            case AttrType::POINT:
                size_ = D;
                break;
            case AttrType::NORMAL:
                size_ = D;
                break;
            //case AttrType::COLOR:
            //    size_ = 4;
            default:
                size_ = 0;
        }
    }
    
    // Return the type of attribute, e.g., it is a point or color or mornal or others.
    AttrType type() { return type_; }
    
    // Return the number of attributes (e.g., a 3D point coordinate has 3 attributes)
    unsigned int size() { return size_; }
    
private:
    AttrType type_;
    unsigned int size_;
};

// The definition of a 3D model
template<typename T, unsigned int D>
class Model {
public:
    typedef Point<T, D>     PointType;
    typedef Point<T, D>     VecType;
    typedef Vertex<T, D>    VertexType;
    typedef Face<T, D>      FaceType;
    typedef HalfEdge<T, D>  HalfEdgeType;
    typedef ModelAttr<D>    ModelAttrType;
    
    Model();
    
    Model(
          const vector<VertexType*>& points,
          const vector<vector<unsigned int>>& elements,
          const vector<FaceType*>& faces
    );
    
    vector<T> getPointArray();
    vector<T> getVertexArray(vector<ModelAttrType> attrs);
    vector<unsigned int> getElementArray();
    vector<T> getEdgeArray();
    
    PointType getCenter();
    VecType   getSideLengths();
    
    // Initialize each point's normal (maybe other information needed)
    void init();

private:
    void compBoundingBox();
    
    
    vector<VertexType*>             vertices_;
    vector<vector<unsigned int>>    elements_; // Record the indices of each face
    vector<FaceType*>               faces_;
    vector<VecType>                 normals_;
    
    // Bounding box
    PointType bx_min_;
    PointType bx_max_;
};

typedef Model<float, 3>  Model3F;
typedef Model<double, 3> Model3D;

#endif /* model_h */
