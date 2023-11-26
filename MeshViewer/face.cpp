//
//  face.cpp
//  MeshViewer
//
//  Created by Jiang Jie on 8/6/23.
//

#include "face.h"

template<typename T, unsigned int D>
Face<T, D>::Face() :
    edge_(nullptr)
{
        
}

template<typename T, unsigned int D>
void Face<T, D>::setEdge(HalfEdgeType* edge) {
    edge_ = edge;
}

template<typename T, unsigned int D>
typename Face<T,D>::HalfEdgeType* Face<T, D>::edge() {
    return edge_;
}

template<typename T, unsigned int D>
vector<typename Face<T,D>::HalfEdgeType*> Face<T, D>::allEdges() {
    vector<HalfEdgeType*> edges;
    if (edge_ == nullptr)
        return edges;
    edges.push_back(edge_);
    HalfEdgeType* search = edge_->next();
    while(search && search != edge_) {
        edges.push_back(search);
        search = search->next();
    }
    return edges;
}

/**
 
 */
template<typename T, unsigned int D>
vector<typename Face<T, D>::PointType> Face<T, D>::contour(bool include_start) {
    vector<HalfEdgeType*> edges = allEdges();
    vector<PointType> points;
    points.reserve(edges.size());
    for (int i = 0; i < edges.size(); ++i)
        points.push_back(*dynamic_cast<PointType*>(edges[i]->origin()));
    if (include_start)
        points.push_back(points[0]);
    return points;
}

/**
 void Face<T, D>::init()
 computes the basic information of face, such as area, normal etc.
 */
template<typename T, unsigned int D>
void Face<T, D>::init() {
    compArea();
    compNormal();
}

/**
 T Face<T, D>::area() returns the area of the face.
 */
template<typename T, unsigned int D>
T Face<T, D>::area() {
    return area_;
}

/**
 void Face<T, D>::compArea()
 computes the area of the face.
 */
template<typename T, unsigned int D>
void Face<T, D>::compArea() {
    vector<PointType> points = contour(true);
    PointType start = points[0];
    double sum = 0;
    for (int i = 1; i < points.size(); ++i) {
        VecType v1 = points[i] - start;
        VecType v2 = points[i - 1] - start;
        VecType v = v1.cross(v2);
        sum += v.z();
    }
    area_ = abs(sum);
}

/**
 typename Face<T, D>::VecType Face<T, D>::normal()
 returns the normal of the face.
 */
template<typename T, unsigned int D>
typename Face<T, D>::VecType Face<T, D>::normal() {
    return normal_;
}

/**
 void Face<T, D>::compNormal()
 computes the normal of the face.
 */
template<typename T, unsigned int D>
void Face<T, D>::compNormal() {
    PointType* p = edge_->origin();
    PointType* p_next = edge_->next()->origin();
    PointType* p_prev = edge_->prev()->origin();
    VecType v1 = *p - *p_prev;
    VecType v2 = *p_next - *p;
    normal_ = v1.cross(v2);
    normal_.normalize();
}

template class Face<float, 3>;
template class Face<double, 3>;
