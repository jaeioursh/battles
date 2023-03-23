#include <cstddef>


template <typename T> class node{
    public:
        node<T>* prev;
        node<T>* next;
        T* elem;
};

template <typename T> class lil{
    public:
        node<T>* head;        
        int len;


        lil();
        void add(node<T>*);
        void remove(node<T>*);
        
        
};


template <typename T> lil<T>::lil(){
    head=NULL;
    len=0;

}


template <typename T> void lil<T>::add(node<T>* n){
    n->next=head;
    n->prev=NULL;
    if (n->next != NULL)
        n->next->prev=n;
    head=n;
    len++;
}


template <typename T> void lil<T>::remove(node<T>* n){
    if (n->next != NULL)
        n->next->prev=n->prev;
    
    if (n->prev != NULL)
        n->prev->next=n->next;
    n->next=NULL;
    n->prev=NULL;
    len--;

}


