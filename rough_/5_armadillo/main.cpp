#include <iostream>
#include<string>
#include <armadillo>
using namespace std;

int add(int a, int b){
    return a+b;
}

int main(){
    cout<<add(2, 3)<<endl;
    arma::mat matrix = {
        {1,2,3},
        {4,5,6},
        {7,8,9}
        };
    matrix.set_size(3,3);
    matrix.print("matrix");
    return 0;
}