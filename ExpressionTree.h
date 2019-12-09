#include "evaluate.h"
typedef union{
    char optr;
    int opnd;
} data_t;

/*data_t &operator=(data_t data,const int &d){
    data.opnd=d;
    return data;
}

data_t & operator=(data_t data,const char &c){
    data.optr=d;
    return data;
}*/

typedef enum {OPTR,OPND} Type;

struct ExpressionNode{
    data_t data;
    Type type;
    //ExpressionNode *parent;
    ExpressionNode *left;
    ExpressionNode *right;
};

class ExpressionTree{
    private:
        ExpressionNode *_root;
    public:
        ExpressionTree(){}
        ExpressionTree(ExpressionNode *x){_root=x;}
        ~ExpressionTree();
        ExpressionNode *insertAsRoot(ExpressionNode *x){return _root=x;}
        Calcresult Evaluate(ExpressionNode *x);
        ExpressionNode *root() const {return _root;}
    protected:
        void removeLeaf(ExpressionNode *x);
        void remove(ExpressionNode *x);
};

ExpressionTree::~ExpressionTree(){
    remove(_root);
    _root=NULL;
}

void ExpressionTree::removeLeaf(ExpressionNode *x){//x is a leaf node
    delete x;
}

void ExpressionTree::remove(ExpressionNode *x){
    if(!x) return;
    if(x->left) remove(x->left);
    if(x->right) remove(x->right);
    removeLeaf(x);
}

Calcresult ExpressionTree::Evaluate(ExpressionNode *x) {
    if(!x) return EVALUATE_ERROR;
    if(x->type==OPND) return x->data.opnd;
    if(!(x->left)||!(x->right)) return EVALUATE_ERROR;
    Calcresult opnd1=Evaluate(x->left);
    Calcresult opnd2=Evaluate(x->right);
    if(!(opnd1.flag&&opnd2.flag)) return EVALUATE_ERROR;
    return calculate(opnd1.result,opnd2.result,x->data.optr);
}

