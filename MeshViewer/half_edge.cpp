#include "half_edge.h"

template<typename T, unsigned int D>
HalfEdge<T, D>::HalfEdge() :
    face_(nullptr), next_(nullptr), prev_(nullptr), twin_(nullptr), origin_(nullptr)
{
    
}

template<typename T, unsigned int D>
void HalfEdge<T, D>::setNext(HalfEdgeType* next) {
    next_ = next;
}

template<typename T, unsigned int D>
void HalfEdge<T, D>::setPrev(HalfEdgeType* prev) {
    prev_ = prev;
}

template<typename T, unsigned int D>
void HalfEdge<T, D>::setTwin(HalfEdgeType* twin) {
    twin_ = twin;
}

template<typename T, unsigned int D>
void HalfEdge<T, D>::setFace(FaceType* face) {
    face_ = face;
}

template<typename T, unsigned D>
void HalfEdge<T,D>::setOrigin(VertexType* origin) {
    origin_ = origin;
}

template class HalfEdge<float, 3>;
template class HalfEdge<double, 3>;
