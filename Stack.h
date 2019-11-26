#ifndef STACK_H
#define STACK_H

#ifndef DEFAULT_CAPACITY
#define DEFAULT_CAPACITY 1000
#endif

template<class T>
class Stack{
    private:
        T *_elem;
        int _size;
        int _capacity;
    protected:
        void expand();
        void shrink();
    public:
        Stack(){_elem=new T[_capacity=DEFAULT_CAPACITY];_size=0;}
        ~Stack(){delete [] _elem;}
        int size() const {return _size;}
        bool empty() const {return _size==0;}
        void push(const T &e);
        T pop();
        T top() const {return _elem[_size-1];}
};

template <class T>
void Stack<T>::expand() {
    T *nelem=new T[_capacity<<=1];
    for(int i=0;i<_size;i++) nelem[i]=_elem[i];
    delete [] _elem;
    _elem=nelem;
}

template <class T>
void Stack<T>::shrink() {
    T *nelem=new T[_capacity>>=1];
    for(int i=0;i<_size;i++) nelem[i]=_elem[i];
    delete [] _elem;
    _elem=nelem;
}

template <class T>
void Stack<T>::push(const T &e) {
    if(_size==_capacity) expand();
    _elem[_size++]=e;
}

template <class T>
T Stack<T>::pop() {
    T tmp=_elem[--_size];
    if(_size<_capacity>>1) shrink();
    return tmp;
}

#endif //INC_1206PASCAL_STACK_H
