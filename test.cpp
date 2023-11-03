#include"lil.h"
#include"utils.h"
//#include"bullet.h"
#include<iostream>

using namespace std;

void test_csv(){
    vector<vector<int>> a;
    a=read_csv("teams/e1.csv");
    
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            cout<<a[i][j]<<' ';
        }
        cout<<endl;
    }

}

void test_lil(){
    cout<<"beggining lil test"<<endl;
    lil<int> lst;


    int A=5;
    int B=6;
    int C=7;


    
    lst.add(&A);
    lst.add(&B);
    lst.add(&C);
    

    lst.clear();

    lst.add(&A);
    lst.add(&B);
    lst.add(&C);
    


    for(int i=0;i<3;i++){
        cout<<*lst.get()<<" "<< lst.len << endl;
        lst.inc();
    }
    lst.reset();
    lst.add(&A);
    for(int i=0;i<3;i++){
        cout<<*lst.get()<<" "<< lst.len << endl;
        lst.inc();
    }
    return;
}

void test_game(){


}

int main(){
    //test_lil();
    //test_csv();

    return 0; 
}

