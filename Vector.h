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
        void sshrink();
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
        void insert(const T &e,Rank i);//insert sth in front of _elem[i]
        T pop_back();
        void clear();
        T& last() {return _elem[_size-1];}
        Rank search(const T &e) const;//attention:if not found,return -1
        Rank binarysearch(const T &e,Rank lo,Rank hi) const;//attention:may return _size-1 or -1
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
void Vector<T>::sshrink() {
    T *nelem=new T[_size=_capacity=DEFAULT_CAPACITY];
    for(int i=0;i<_capacity;i++) nelem[i]=_elem[i];
    delete [] _elem;
    _elem=nelem;
}

template<class T>
void Vector<T>::clear() {
    if(_capacity>DEFAULT_CAPACITY) sshrink();
    _size=0;
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
    for(Rank i=_size-1;i>=0;i--)
        if(_elem[i]==e) return i;
    return -1;
}

template <class T>
Rank Vector<T>::binarysearch(const T &e, Rank lo, Rank hi) const {
    while(lo<hi){
        Rank mi=(lo+hi)>>1;
        (e<_elem[mi])?hi=mi:lo=mi+1;
    }
    return --lo;
}

template <class T>
void Vector<T>::insert(const T &e,Rank r) {
    if(_size==_capacity) expand();
    for(Rank i=_size;i>r;i--)
        _elem[i]=_elem[i-1];
    _size++;
    _elem[r]=e;
}

#endif //VECTOR_H
