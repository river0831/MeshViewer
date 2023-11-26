#ifndef half_edge_h
#define half_edge_h

#include "face.h"
#include "vertex.h"

template<typename T, unsigned int D>
class Face;

template<typename T, unsigned int D>
class HalfEdge {
public:
    typedef Point<T, D>     PointType;
    typedef Face<T, D>      FaceType;
    typedef HalfEdge<T, D>  HalfEdgeType;
    typedef Vertex<T, D>    VertexType;
    
    HalfEdge();
    
    HalfEdgeType* next()    { return next_; }
    HalfEdgeType* prev()    { return prev_; }
    HalfEdgeType* twin()    { return twin_; }
    FaceType*     face()    { return face_; }
    VertexType*   origin()  { return origin_; }
    
    void setNext(HalfEdgeType* next);
    void setPrev(HalfEdgeType* prev);
    void setTwin(HalfEdgeType* twin);
    void setFace(FaceType* face);
    void setOrigin(VertexType* origin);
    
private:
    FaceType*       face_;
    HalfEdgeType*   next_;
    HalfEdgeType*   prev_;
    HalfEdgeType*   twin_;
    VertexType*     origin_; // this is the starting point of an edge
};

typedef HalfEdge<float, 3> HalfEdge3F;
typedef HalfEdge<double, 3> HalfEdge3D;

#endif /* HalfEdge_h */
