#include <iostream>
using namespace std;

int fib2_0(int n, int pretprosli = 0, int prosli = 1){
    if(n == 0) return pretprosli;
    if(n == 1) return prosli;
    int rezultat = pretprosli + prosli;
    return fib2_0(n - 1, prosli, rezultat);
}

int nzd(int x, int y){
    if(y == 0) return x;
    return nzd(y, x % y);
}

int main(){
    return 0;
}