//
//  Point.h
//  FirstOpenGL
//
//  Created by Jiang Jie on 5/6/23.
//

#ifndef point_h
#define point_h

#include <cmath>

// T specifies the data type of coordinates, e.g, float, double
// D is the degree of a point, e.g., 1 for 1D point, 2 for 2D point, 3 for 3D point.
template <typename T, unsigned int D>
class Point{
public:
    Point(T v = 0) {
        for (int i = 0; i < D; ++i)
            coordinate[i] = v;
    }
    
    Point(T x, T y) {
        if (D == 0)
            return;
        coordinate[0] = x;
        if (D > 1)
            coordinate[1] = y;
    }
    
    Point(T x, T y, T z) {
        if (D == 0)
            return;
        coordinate[0] = x;
        if (D > 1)
            coordinate[1] = y;
        if (D > 2)
            coordinate[2] = z;
    }
    
    T x() {
        return D >= 1 ? coordinate[0] : 0;
    }
    
    T y() {
        return D >= 2 ? coordinate[1] : 0;
    }
    
    T z() {
        return D >= 3 ? coordinate[2] : 0;
    }
    
    void setX(T x_) {
        coordinate[0] = x_;
    }
    
    void setY(T y_) {
        if (D > 1)
            coordinate[1] = y_;
    }
    
    void setZ(T z_) {
        if (D > 2)
            coordinate[2] = z_;
    }
    
    T value(int pos) {
        return pos < D ? coordinate[pos] : 0;
    }
    
    void setValue(int pos, T v) {
        if (D > pos)
            coordinate[pos] = v;
    }
    
    inline T dot(Point<T, D> p) {
        T sum = 0;
        for (int i = 0; i < D; ++i)
            sum += coordinate[i] * p[i];
        return sum;
    }
    
    inline Point<T, D> cross(Point<T, D> p) {
        return Point<T, D>(y() * p.z() - p.y() * z(),
                           p.x() * z() - x() * p.z(),
                           x() * p.y() - p.x() * y());
    }
    
    inline Point<T, D> operator+ (Point<T, D> p) {
        Point<T, D> new_p;
        for (int i = 0; i < D; ++i)
            new_p[i] = coordinate[i] + p[i];
        return new_p;
    }
    
    inline Point<T, D> operator += (Point<T, D> p) {
        for (int i = 0; i < D; ++i)
            coordinate[i] += p[i];
        return *this;
    }
    
    inline Point<T, D> operator- (Point<T, D> p) {
        Point<T, D> new_p;
        for (int i = 0; i < D; ++i)
            new_p[i] = coordinate[i] - p[i];
        return new_p;
    }
    
    inline Point<T, D> operator -= (Point<T, D> p) {
        for (int i = 0; i < D; ++i)
            coordinate[i] -= p[i];
        return *this;
    }
    
    inline Point<T, D> operator / (T v) {
        Point<T, D> p;
        for (int i = 0; i < D; ++i)
            p[i] = coordinate[i] / v;
        return p;
    }
    
    inline friend Point<T, D> operator * (T v, const Point<T, D>& obj) {
        Point<T, D> result;
        for (int i = 0; i < D; ++i)
            result[i] = v * obj[i];
        return result;
    }
    
    inline void operator = (Point<T, D> p) {
        for (int i = 0; i < D; ++i)
            coordinate[i] = p[i];
    }
    
    inline bool operator == (Point<T, D> p) {
        for (int i = 0; i < D; ++i) {
            if (coordinate[i] != p[i])
                return false;
        }
        return true;
    }
    
    inline T operator [] (int pos) const {
        return coordinate[pos];
    }
    
    inline T& operator [] (int pos) {
        return coordinate[pos];
    }
    
    T modulus() {
        double sum = 0;
        for (int i = 0; i < D; ++i)
            sum += coordinate[i] * coordinate[i];
        return sqrt(sum);
    }
    
    void normalize() {
        T mod = modulus();
        for (int i = 0; i < D; ++i)
            coordinate[i] /= mod;
    }
    
private:
    T coordinate[D];
};

typedef Point<float, 3>  Point3F;
typedef Point<double, 3> Point3D;
typedef Point3F          Vec3F;
typedef Point3D          Vec3D;

#endif /* Point_h */
