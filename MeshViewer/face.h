//
//  Face.h
//  FirstOpenGL
//
//  Created by Jiang Jie on 5/6/23.
//

#ifndef face_h
#define face_h

#include "point.h"
#include "half_edge.h"
#include <vector>

using namespace std;

template<typename T, unsigned int D>
class HalfEdge;

template<typename T, unsigned int D> // D is the degree. If D = 3, the face is a triangle
class Face {
public:
    typedef Point<T, D> PointType;
    typedef HalfEdge<T, D> HalfEdgeType;
    typedef PointType VecType;
    
    Face();
    
    void setEdge(HalfEdgeType* edge);
    HalfEdgeType* edge();
    vector<HalfEdgeType*> allEdges();
    vector<PointType> contour(bool include_start = false);
    
    void init();
    T area();
    VecType normal();
    
private:
    void compArea();
    void compNormal();
    
    HalfEdgeType* edge_;
    VecType normal_;
    T area_;
};


#endif /* Face_h */
