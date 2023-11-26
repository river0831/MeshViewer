//
//  Model.cpp
//  FirstOpenGL
//
//  Created by Jiang Jie on 5/6/23.
//

#include "model.h"
#include <limits>

template<typename T, unsigned int D>
Model<T,D>::Model() :
    bx_min_(PointType(numeric_limits<T>::max())),
    bx_max_(PointType(numeric_limits<T>::min()))
{
    
}

template<typename T, unsigned int D>
Model<T,D>::Model(
      const vector<VertexType*>& points,
      const vector<vector<unsigned int>>& elements,
      const vector<FaceType*>& faces
) {
    vertices_ = points;
    elements_ = elements;
    faces_ = faces;
}

template<typename T, unsigned int D>
vector<T> Model<T, D>::getPointArray() {
    vector<T> array(vertices_.size() * D);
    for (int i = 0; i < vertices_.size(); ++i) {
        for (int j = 0; j < D; ++j)
            array[i * D + j] = vertices_[i]->value(j);
    }
    return array;
}

template<typename T, unsigned int D>
vector<T> Model<T, D>::getVertexArray(vector<ModelAttrType> attrs) {
    int num_ver = vertices_.size();
    vector<T> result;
    int num_values = 0;
    for (int i = 0; i < attrs.size(); ++i)
        num_values += attrs[i].size();
    num_values *= num_ver;
    result.reserve(num_values);
    for (int i = 0; i < num_ver; ++i) {
        for (int j = 0; j < attrs.size(); ++j) {
            switch(attrs[j].type()) {
                case AttrType::POINT:
                    for (int k = 0; k < D; ++k)
                        result.push_back((*vertices_[i])[k]);
                    break;
                case AttrType::NORMAL:
                    for (int k = 0; k < D; ++k)
                        result.push_back(normals_[i][k]);
                    break;
                default:
                    break;
            }
        }
    }
    return result;
}

template<typename T, unsigned int D>
vector<unsigned int> Model<T, D>::getElementArray() {
    vector<unsigned int> array;
    if (elements_.empty())
        return array;
    array.reserve(elements_.size() * elements_[0].size());
    for (int i = 0; i < elements_.size(); ++i) {
        array.insert(array.end(), elements_[i].begin(), elements_[i].end());
    }
    return array;
}

template<typename T, unsigned int D>
vector<T> Model<T, D>::getEdgeArray() {
    vector<T> result;
    unsigned int num_lines = 0;
    for (int i = 0; i < elements_.size(); ++i)
        num_lines += elements_[i].size();
    result.reserve(num_lines * 2 * D);
    for (int i = 0; i < elements_.size(); ++i) {
        vector<unsigned int> indices = elements_[i];
        for (int j = 0; j < indices.size(); ++j) {
            unsigned int start = indices[j];
            unsigned int end = j == indices.size() - 1? indices[0] : indices[j + 1];
            for (int k = 0; k < D; ++k)
                result.push_back((*vertices_[start])[k]);
            for (int k = 0; k < D; ++k)
                result.push_back((*vertices_[end])[k]);
        }
    }
    return result;
}

/**
 typename Model<T, D>::PointType Model<T, D>::getCenter() returns the center of the model.
 It is also the center of the bounding box.
 */
template<typename T, unsigned int D>
typename Model<T, D>::PointType Model<T, D>::getCenter() {
    return (bx_min_ + bx_max_) / 2;
}

template<typename T, unsigned int D>
typename Model<T, D>::VecType Model<T, D>::getSideLengths() {
    VecType lengths;
    for (int i = 0; i < D; ++i)
        lengths[i] = bx_max_[i] - bx_min_[i];
    return lengths;
}

/**
 void Model<T, D>::init() computes basic information about a model like normal, face area etc.
 */
template<typename T, unsigned int D>
void Model<T, D>::init() {
    for (int i = 0; i < faces_.size(); ++i)
        faces_[i]->init();
    
    // Compute vertex normals
    normals_.reserve(vertices_.size());
    for (int i = 0; i < vertices_.size(); ++i) {
        // Get the incident faces
        vector<FaceType*> incident_faces = vertices_[i]->faces();
        double sum = 0;
        for (int j = 0; j < incident_faces.size(); ++j)
            sum += incident_faces[j]->area();
        
        // Compute weighted normal
        VecType norm(0);
        for (int j = 0; j < incident_faces.size(); ++j) {
            double factor = incident_faces[j]->area() / sum;
            norm += factor * incident_faces[j]->normal();
        }
        
        normals_.push_back(norm);
    }
    compBoundingBox();
}

/**
 void Model<T, D>::compBoundingBox() computes the bounding box of the model
    and saves in bx_min_ and bx_max_
 */
template<typename T, unsigned int D>
void Model<T, D>::compBoundingBox() {
    bx_min_ = PointType(numeric_limits<T>::max());
    bx_max_ = PointType(numeric_limits<T>::min());
    for (int i = 0; i < vertices_.size(); ++i) {
        for (int j = 0; j < D; ++j) {
            if ((*vertices_[i])[j] < bx_min_[j])
                bx_min_[j] = (*vertices_[i])[j];
            if ((*vertices_[i])[j] > bx_max_[j])
                bx_max_[j] = (*vertices_[i])[j];
        }
    }
}

template class Model<float, 3>;
template class Model<double, 3>;
