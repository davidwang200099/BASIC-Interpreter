#ifndef QUEUE_H_INCLUDED
#define QUEUE_H_INCLUDED

#ifndef NULL
#define NULL 0
#endif

#ifndef Rank
#define Rank int
#endif
//Node
template <class T>
class Node{
public:
    Node<T> *prev;
    T data;
    Node<T> *next;

    Node(){}
    Node(const T &data,Node<T> *prevptr=NULL,Node<T> *nextptr=NULL):data(data),prev(prevptr),next(nextptr){}
    Node(const Node &node):data(node.data),prev(NULL),next(NULL){}
    Node<T>* insertAsPrev(const T &e);
    Node<T>* insertAsNext(const T &e);
};

template <class T>//
Node<T>* Node<T>::insertAsPrev(const T &e){
    Node<T>* pt=new Node<T>(e,prev,this);
    prev->next=pt;
    prev=pt;
    return pt;
}

template <class T>
Node<T>* Node<T>::insertAsNext(const T &e){
    Node<T>* pt=new Node<T>(e,this,next);
    next->prev=pt;
    next=pt;
    return pt;
}
//Node

//List
template <class T>
struct List{
//private:
    int _size;
    Node<T>* head;
    Node<T>* rear;
    //protected:
//public:
    void Init();
    void Clear();
    //void copyNodes(Node<T>* p,int n);
    void Merge(Node<T>* p1,Node<T>* p2);
    void MergeSort(Node<T> nd,int n);
    Node<T>* SelectMax(Node<T>* p,int n);
    void SelectionSort(Node<T>* p,int n);
    void InsertionSort(Node<T>* p,int n);

    //constructor
    List(){Init();}
    List(const List<T> &L);
    //Linkedlist(Linkedlist<T> const &L,Rank r,int n);
    //Linkedlist(Linkedlist<T> const &L,int n);
    ~List(){Clear();delete head;delete rear;}
    //constructor

    Rank Getsize(){return _size;}
    T Remove(Node<T>* p);
    int Deduplicate();
    int Uniquify();
    Node<T>* Find(const T &e,int n,Node<T>* p);//unsorted list,n nodes before p
    Node<T>* Search(const T &e,int n,Node<T>* p);//sorted list,n nodes before p
    Node<T>* InsertAsLast(const T &e){_size++;return rear->insertAsPrev(e);}
    Node<T>* InsertAsFirst(const T &e){_size++;return head->insertAsNext(e);}
    Node<T>* InsertAfter(Node<T>* p,const T &e){_size++;return p->insertAsNext(e);}
    Node<T>* InsertBefore(Node<T>* p,const T &e){_size++;return p->insertAsPrev(e);}

    Node<T>* First(){return head->next;}
    Node<T>* Last(){return rear->prev;}
    Node<T>* Head(){return head;}
    Node<T>* Rear(){return rear;}
};

template <class T>
void List<T>::Init(){
    head=new Node<T>;rear=new Node<T>;
    head->next=rear;head->prev=NULL;
    rear->prev=head;rear->next=NULL;
    _size=0;
}

template <class T>
void List<T>::Clear(){
    Node<T>* p;
    while((p=head->next)!=rear){
        head->next=p->next;
        p->next->prev=head;
        delete p;
    }
}

template <class T>
List<T>::List(const List<T> &L){
    Init();
    Node<T>* q;
    for(q=L.First();q->next!=NULL;q=q->next){
        InsertAsLast(q->data);
        _size++;
    }
}

template <class T>
Node<T>* List<T>::SelectMax(Node<T>* p,int n){
    Node<T>* maxnode=p;
    for(int i=1;i<n && (p=p->next)!=rear;i++) if(p->data>=maxnode->data) maxnode=p;
    return maxnode;
}


template <class T>
void List<T>::SelectionSort(Node<T>* p,int n){
    Node<T>* header=p;Node<T>* tail=p;
    for(int i=1;i<=n;i++) tail=tail->next;
    while(1<n){
        InsertBefore(tail,Remove(SelectMax(header,n)));
        tail=tail->prev;n--;
    }
}

template <class T>
void List<T>::InsertionSort(Node<T>* p,int n){
    for(int r=0;r<n;r++){
        InsertAfter(Search(p->data,r,p),p->data);
        p=p->next;
        Remove(p->prev);
    }
}

template <class T>
Node<T>* List<T>::Find(const T &e,int n,Node<T>* p){
    Node<T>* pt=p;
    for(int i=1;i<=n &&(pt=pt->prev)!=head;i++)
        if(pt->data==e) return pt;
    return NULL;
}

template <class T>
Node<T>* List<T>::Search(const T &e,int n,Node<T>* p){
    for(int i=n;i>0 && (p=p->prev)!=head;i--) if(p->data<=e) return p;
    return p;
}

template <class T>
T List<T>::Remove(Node<T>* p){
    T e=p->data;
    p->prev->next=p->next;
    p->next->prev=p->prev;
    delete p;
    _size--;
    return e;
}

template <class T>
int List<T>::Deduplicate(){
    if(_size<2) return 0;
    int oldsize=_size;
    Rank r=0;
    Node<T>* pt=head;
    while((pt=pt->next)!=rear){
        Node<T>* q=Find(pt->data,r,pt);
        if(q){q->prev->next=q->next;q->next->prev=q->prev;delete q;_size--;}
        else r++;
    }
    return oldsize-_size;

}

template <class T>
int List<T>::Uniquify(){
    if(_size<=1) return 0;
    int oldsize=_size;
    Node<T>* p=head->next;
    Node<T>* q=p->next;
    while(q!=rear){
        if(q->data!=p->data) {q=q->next;p=p->next;}
        else{Node<T>*q0=q->next;q->prev->next=q->next;q->next->prev=q->prev;delete q;_size--;q=q0;}
    }
    return oldsize-_size;
}

template <class T>
class Queue:public List<T>{
    public:
        void Enqueue(const T& e){ this->InsertAsLast(e); }
        T Dequeue(){ return this->Remove(this->head->next);}
        bool Isempty() const {return this->head->next==this->rear;}
};


#endif //QUEUE_H_INCLUDED
