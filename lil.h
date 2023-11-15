#ifndef LIL_H
#define LIL_H

#include <cstddef>




template <typename T> class lil{
    public:
    
    struct node{
        node* prev;
        node* next;
        T* elem;
    };

    public:
        node* head;        
        node* point;
        int len;


        lil();
        void add(T*);
        void remove();
        bool inc();
        T* get();
        void reset();
        void clear();
        
};



template <typename T> lil<T>::lil(){
    head=NULL;
    point=NULL;
    len=0;
}

//adds at thefront
template <typename T> void lil<T>::add(T* val){
    node* n=new node;
    n->elem=val;

    n->next=head;
    n->prev=NULL;
    if (n->next != NULL)
        n->next->prev=n;
    head=n;
    //point=n;
    len++;
}

//remove and increment
template <typename T> void lil<T>::remove(){
    node* n = point;

    if (n->next != NULL) //if not last
        n->next->prev=n->prev;
    
    if (n->prev != NULL){ //if not first
        n->prev->next=n->next;
        
    }
    else{ //if first one
        head=n->next;
    }
    point=n->next;
    delete n;
    len--;

}

//moves to next in list # DONT USE AFTER REMOVE

template <typename T> bool lil<T>::inc(){
    
    point=point->next;
    if (point==NULL)
        return 0;
    else
        return 1;
}

template <typename T> T* lil<T>::get(){
    return point->elem;
}


template <typename T> void lil<T>::reset(){
    point=head;
}

//delete all
template <typename T> void lil<T>::clear(){
    node* ptr;
    if (len>0){
        do{
            ptr=head;
            head=head->next;
            delete ptr;
        }while(head != NULL);

        head=NULL;
        point=NULL;
        len=0;
    }
}


#endif
