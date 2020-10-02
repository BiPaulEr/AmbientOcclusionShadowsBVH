#pragma once

#include <cmath>
#include <iostream>

/// Vector in 4 dimensions, with basics operators overloaded.
template <typename T>
class Vec4 {
public:
  inline Vec4 ()  { m_p[0] = m_p[1] = m_p[2] = m_p[3] = 0.0; }

  inline Vec4 (T p0, T p1, T p2, T p3) {
    m_p[0] = p0;
    m_p[1] = p1;
    m_p[2] = p2;
    m_p[3] = p3;
  };

  inline Vec4 (const Vec4 & v) {
    init (v[0], v[1], v[2], v[3]);
  }

  ~Vec4() {}

  inline T& operator[] (int Index) {
    return (m_p[Index]);
  };

  inline const T& operator[] (int Index) const {
    return (m_p[Index]);
  };

  inline Vec4& operator= (const Vec4 & p) {
    m_p[0] = p[0];
    m_p[1] = p[1];
    m_p[2] = p[2];
    m_p[3] = p[3];
    return (*this);
  };

  inline Vec4& operator+= (const Vec4 & p) {
    m_p[0] += p[0];
    m_p[1] += p[1];
    m_p[2] += p[2];
    m_p[3] += p[3];
    return (*this);
  };

  inline Vec4& operator-= (const Vec4 & p) {
    m_p[0] -= p[0];
    m_p[1] -= p[1];
    m_p[2] -= p[2];
    m_p[3] -= p[3];
    return (*this);
  };

  inline Vec4& operator*= (const Vec4 & p) {
    m_p[0] *= p[0];
    m_p[1] *= p[1];
    m_p[2] *= p[2];
    m_p[3] *= p[3];
    return (*this);
  };

  inline Vec4& operator*= (T s) {
    m_p[0] *= s;
    m_p[1] *= s;
    m_p[2] *= s;
    m_p[3] *= s;
    return (*this);
  };

  inline Vec4& operator/= (const Vec4 & p) {
    m_p[0] /= p[0];
    m_p[1] /= p[1];
    m_p[2] /= p[2];
    m_p[3] /= p[3];
    return (*this);
  };

  inline Vec4& operator/= (T s) {
    m_p[0] /= s;
    m_p[1] /= s;
    m_p[2] /= s;
    m_p[3] /= s;
    return (*this);
  };

  inline Vec4 operator+ (const Vec4 & p) const {
    Vec4 res;
    res[0] = m_p[0] + p[0];
    res[1] = m_p[1] + p[1];
    res[2] = m_p[2] + p[2];
    res[3] = m_p[3] + p[3];
    return (res);
  };

  inline Vec4 operator- (const Vec4 & p) const {
    Vec4 res;
    res[0] = m_p[0] - p[0];
    res[1] = m_p[1] - p[1];
    res[2] = m_p[2] - p[2];
    res[3] = m_p[3] - p[3];
    return (res);
  };

  inline Vec4 operator- () const {
    Vec4 res;
    res[0] = -m_p[0];
    res[1] = -m_p[1];
    res[2] = -m_p[2];
    res[3] = -m_p[3];
    return (res);
  };

  inline Vec4 operator* (const Vec4 & p) const {
    Vec4 res;
    res[0] = m_p[0] * p[0];
    res[1] = m_p[1] * p[1];
    res[2] = m_p[2] * p[2];
    res[3] = m_p[3] * p[3];
    return (res);
  };

  inline Vec4 operator* (T s) const {
    Vec4 res;
    res[0] = m_p[0] * s;
    res[1] = m_p[1] * s;
    res[2] = m_p[2] * s;
    res[3] = m_p[3] * s;
    return (res);
  };

  inline Vec4 operator/ (const Vec4 & p) const {
    Vec4 res;
    res[0] = m_p[0] / p[0];
    res[1] = m_p[1] / p[1];
    res[2] = m_p[2] / p[2];
    res[3] = m_p[3] / p[3];
    return (res);
  };

  inline Vec4 operator/ (T s) const {
    Vec4 res;
    res[0] = m_p[0] / s;
    res[1] = m_p[1] / s;
    res[2] = m_p[2] / s;
    res[3] = m_p[3] / s;
    return (res);
  };

  inline bool operator == (const Vec4 & a) const {
    return(m_p[0] == a[0] && m_p[1] == a[1] && m_p[2] == a[2] && m_p[3] == a[3]);
  };

  inline bool operator != (const Vec4 & a) const {
    return(m_p[0] != a[0] || m_p[1] != a[1] || m_p[2] != a[2] || m_p[3] != a[3]);
  };

  inline bool operator < (const Vec4 & a) const {
    return(m_p[0] < a[0] && m_p[1] < a[1] && m_p[2] < a[2] && m_p[3] < a[3]);
  };

  inline bool operator >= (const Vec4 &a) const {
    return(m_p[0] >= a[0] && m_p[1] >= a[1] && m_p[2] >= a[2] && m_p[3] >= a[3]);
  };

  inline Vec4 & init (T x, T y, T z, T w) {
    m_p[0] = x;
    m_p[1] = y;
    m_p[2] = z;
    m_p[3] = w;
    return (*this);
  };

  inline T squaredLength() const {
    return (dot (*this, *this));
  };

  inline T length() const {
    return (T)sqrt (squaredLength());
  };

  /// Return length after normalization
  inline T normalize () {
    T l = length ();
    if (l == 0.0f)
      return 0;
    T invL = 1.0f / l;
    m_p[0] *= invL;
    m_p[1] *= invL;
    m_p[2] *= invL;
    m_p[3] *= invL;
    return l;
  };


  inline Vec4 projectOn (const Vec4 & N, const Vec4 & P) const {
    T w = dot (((*this) - P), N);
    return (*this) - (N * w);
  }

protected:
  T m_p[4];
};

template <class T>
inline T length (const Vec4<T> & a) {
  return a.length ();
}

  template <class T>
inline T dist (const Vec4<T> & a, const Vec4<T> & b) {
  return (a-b).length ();
}

template <class T>
inline T dot (const Vec4<T> & a, const Vec4<T> & b) {
  return (a[0] * b[0] + a[1] * b[1] + a[2] * b[2] + a[3] * b[3]);
}

template <class T>
inline Vec4<T> cross (const Vec4<T> & a, const Vec4<T> & b) {
  Vec4<T> r;
  r[0] = a[1] * b[2] - a[2] * b[1];
  r[1] = a[2] * b[0] - a[0] * b[2];
  r[2] = a[0] * b[1] - a[1] * b[0];
  r[3] = a[3];  //Not sure
  return r;
}

template <class T>
inline Vec4<T> normalize (const Vec4<T> & x) {
  Vec4<T> n (x);
  n.normalize ();
  return n;
}

template <class T>
inline Vec4<T> mix (const Vec4<T> & u, const Vec4<T> & v, float alpha) {
  return (u * (1.0 - alpha) + v * alpha);
}

template <class T>
inline Vec4<T> operator * (const T &s, const Vec4<T> &P) {
  return (P * s);
}

template <class T>
std::ostream & operator<< (std::ostream & output, const Vec4<T> & v) {
  output << v[0] << " " << v[1] << " " << v[2] << " " << v[3];
  return output;
}

template <class T>
std::istream & operator>> (std::istream & input, Vec4<T> & v) {
  input >> v[0] >> v[1] >> v[2] >> v[3];
  return input;
}

typedef Vec4<float> Vec4f;
typedef Vec4<double> Vec4d;
typedef Vec4<int> Vec4i;
