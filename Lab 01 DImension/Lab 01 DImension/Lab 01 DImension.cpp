#include <iostream>
#include <vector>
#include <random> 
#include <map>

using namespace std;

int GenRamdon() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(1, 1000);
    return distrib(gen);
}

struct Point
{
    vector<int> punto;
    Point(int tam) {
        for (int i = 0; i < tam; i++)
        {
            punto.push_back(GenRamdon());
        }
    }
    void print() {
        for (int i = 0; i < punto.size(); i++)
        {
            cout << punto[i] << " ";
        }
        cout << endl;
    }
};

int dist_Point(Point a, Point b) {
    int dim = a.punto.size();
    int dis = 0;
    for (int i = 0; i < dim; i++)
    {
        dis += pow((a.punto[i] - b.punto[i]), 2);
    }

    return sqrt(dis);
}
void labDimension(int dim,int tam) {

    //Punto de referencia
    Point ref_point(dim);
    vector<int> vector_dist;
    map<int, int> map_dist;
    map<int, int>::iterator iterador;
    int valor = 0;
    for (int i = 0; i < tam-1; i++)
    {
        int dis_aux = dist_Point(ref_point, Point(dim));
        vector_dist.push_back(dis_aux);

        iterador = map_dist.find(dis_aux);
        
        if (iterador == map_dist.end())
        {
            map_dist.insert(pair<int, int>(dis_aux, 1));
        }
        else {
            valor = iterador->second;
            iterador->second = valor + 1;
        }
    }

    int total = 0;
    for (iterador = map_dist.begin(); iterador != map_dist.end(); iterador++)
    {
        // "first" tiene la clave. "second" el valor
        int clave = iterador->first;
        int valor = iterador->second;
        cout << clave << " " << valor << endl;
        total += valor;
    }
    /*cout << total;*/
}
int main()
{
   /* 
   vector<int> vec_dim = { 1,2,3,4,5,6,7,8,9,10,20,30,40,50,60,70,80,90,100 };
    for (int i = 0; i < vec_dim.size(); i++)
    {
        labDimension(vec_dim[i], 20000);
        cout << endl;
    }
    */
    labDimension(100, 20000);
}
