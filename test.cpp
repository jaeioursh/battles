#include"lil.h"
#include<iostream>

using namespace std;


void test_lil(){
    cout<<"beggining lil test"<<endl;
    lil<int> lst;
    node<int> a;
    node<int> b;
    node<int> c;
    node<int>* ptr;

    int A=5;
    int B=6;
    int C=7;

    a.elem=&A;
    b.elem=&B;
    c.elem=&C;
    
    lst.add(&a);
    lst.add(&b);
    lst.add(&c);
    lst.remove(&c);


    cout<< *a.elem<<endl;
    cout<< lst.len<<endl;
    ptr=lst.head;
    while (ptr != NULL)
    {
        cout <<*ptr->elem<<endl;
        ptr=ptr->next;
    }
    
    return;
}


int main(){
    test_lil();
    return 0;    
}

