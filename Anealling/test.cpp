#include <bits/stdc++.h>
using namespace std;

double calculateTemperature3(int i, int n, double t0, double tn){
    double a = (double)log(t0 - tn) / log(n);
    double t = t0 - pow((double)i, a);
    cout << t << endl;
    return t;
}

int main(){

    double t;
    t = calculateTemperature3(125000, 250000, 1.0, 0.0);
    cout << t << endl;

}
