//
//  vertex.h
//  MeshViewer
//
//  Created by Jiang Jie on 10/6/23.
//

#ifndef vertex_h
#define vertex_h

#include "point.h"
#include "half_edge.h"
#include "face.h"
#include <vector>
using namespace std;

template<typename T, unsigned int D>
class HalfEdge;

template<typename T, unsigned int D>
class Face;

template<typename T, unsigned int D>
class Vertex : public Point<T, D> {
public:
    typedef HalfEdge<T, D>      HalfEdgeType;
    typedef Face<T, D>          FaceType;
    
    Vertex() :
        Point<T, D>(),
        edge_(nullptr)
    {
        
    }
    
    Vertex(T x, T y, T z) :
        Point<T, D>(x, y, z),
        edge_(nullptr)
    {
        
    }
    
    void setEdge(HalfEdgeType* e) { edge_ = e; }
    HalfEdgeType* edge() { return edge_; }
    
    /**
     Return all incident faces.
     */
    vector<FaceType*> faces() {
        vector<FaceType*> f;
        if (edge_ == nullptr)
            return f;
        bool finish = false;
        HalfEdgeType* search = edge_;
        while(search) {
            f.push_back(search->face());
            if (search->twin())
                search = search->twin()->next();
            else
                break;
            if (search == edge_) {
                finish = true;
                break;
            }
        }
        
        if (finish)
            return f;
        
        // search the other direction
        search = edge_->prev()->twin();
        while(search) {
            f.push_back(search->face());
            if (search->prev() != nullptr)
                search = search->prev()->twin();
            else
                break;
        }
        
        return f;
    }
    
    
protected:
    
    
private:
    HalfEdgeType*       edge_;
    
};


#endif /* vertex_h */
