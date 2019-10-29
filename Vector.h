//
// Created by pc on 2019/10/24.
//

#ifndef VECTOR_H
#define VECTOR_H

#ifndef DEFAULT_CAPACITY
#define DEFAULT_CAPACITY 1000
#endif

#ifndef Rank
#define Rank int
#endif

template <class T>
class Vector{
    private:
        T *_elem;
        int _size;
        int _capacity;
    protected:
        void expand();
        void shrink();
    public:
        Vector():_size(0),_capacity(DEFAULT_CAPACITY){_elem=new T[_capacity];}
        Vector(T *elem,int size);
        Vector(const Vector<T> &v);
        ~Vector(){delete [] _elem;}
        bool empty() const {return _size==0;}
        T& operator[](Rank r){return _elem[r];}
        T& operator[](Rank r) const {return _elem[r];}
        int size() const {return _size;}
        void push_back(const T &e);
        T pop_back();
        //void push_front(const T &e);
        //T pop_front();
        //void insert(Rank r,const T &e);
        //void remove(Rank r);
        //void remove(Rank start,Rank end);
        Rank search(const T &e) const;
        Rank binarysearch(const T &e,Rank lo,Rank hi) const;
        Rank binarysearch(const T &e) const {return binarysearch(e,0,_size);}
};

template <class T>
Vector<T>::Vector(T *elem,int size) {
    _elem=new T[_capacity=_size=size];
    for(Rank i=0;i<size;i++) _elem[i]=elem[i];
}

template <class T>
Vector<T>::Vector(const Vector<T> &v) {
    _elem=new T[_capacity=v._capacity];
    _size=v.size();
    for(Rank i=0;i<v.size();i++) _elem[i]=v[i];
}

template <class T>
void Vector<T>::expand() {
    T *n_elem=new T[_capacity<<=1];
    for(Rank i=0;i<_size;i++) n_elem[i]=_elem[i];
    delete [] _elem;
    _elem=n_elem;
}

template <class T>
void Vector<T>::shrink() {
    if((_capacity>>1)>=DEFAULT_CAPACITY) {
        T *n_elem = new T[_capacity >>= 1];
        for(Rank i = 0; i < _size; i++) n_elem[i] = _elem[i];
        delete[] _elem;
        _elem = n_elem;
    }
}

template <class T>
void Vector<T>::push_back(const T &e) {
    if(_size==_capacity) expand();
    _elem[_size++]=e;
}

template <class T>
T Vector<T>::pop_back() {
    T tmp=_elem[--_size];
    if(_size<_capacity>>1) shrink();
    return tmp;
}

template<class T>
Rank Vector<T>::search(const T &e) const {
    for(Rank i=0;i<_size;i++)
        if(_elem[i]==e) return i;
    return _size;
}

template <class T>
Rank Vector<T>::binarysearch(const T &e, Rank lo, Rank hi) const {
    while(lo<hi){
        Rank mi=(lo+hi)>>1;
        (e<_elem[mi])?hi=mi:lo=mi+1;
    }
    return --lo;
}
#endif //VECTOR_H
