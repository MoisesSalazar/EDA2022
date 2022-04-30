// FInal-KDTree.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//

#include <time.h>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include "KDTree.h"


//Function for print nodes
void print_nodes(const KDTree::KdNodeVector& nodes) {
    size_t i, j;
    for (i = 0; i < nodes.size(); ++i) {
        if (i > 0)
            std::cout << " ";
        std::cout << "(";
        for (j = 0; j < nodes[i].point.size(); j++) {
            if (j > 0)
                std::cout << ",";
            std::cout << nodes[i].point[j];
        }
        std::cout << ")";
    }
    std::cout << std::endl;
}


int main(int argc, char** argv) {


    std::cout << "Prueba" << std::endl;


    //Instanciamos el arbol o lo construimos con los siguientes puntos
    KDTree::KdNodeVector nodes;
    int points[10][2] = { {40, 45}, {15, 70}, {70, 10}, {69, 50}, {66, 85}, {85, 90} };
    //Agregamos los puntos
    for (int i = 0; i < 6; ++i) {
        std::vector<double> point(2);
        point[0] = points[i][0];
        point[1] = points[i][1];
        nodes.push_back(KDTree::NodeKDTree(point));
    }

    KDTree::KDTree tree(&nodes);
    std::cout << "Puntos en el kd-tree:\n  ";
    print_nodes(tree.allnodes);

    // Los KNN mas cercanos
    KDTree::KdNodeVector result;
    //Punto de referencia
    std::vector<double> point_referencia = {70,10};
    
    tree.k_nearest_neighbors(point_referencia, 3, &result);
    std::cout << "3 vecinos mas cercanos" << std::endl;
    print_nodes(result);

    std::cout << "range Query" << std::endl;
    tree.range_nearest_neighbors(point_referencia, 80.0, &result);
    std::cout << "Vecinos en el rango de 80" << std::endl;
    print_nodes(result);


    return 0;
}

// Ejecutar programa: Ctrl + F5 o menú Depurar > Iniciar sin depurar
// Depurar programa: F5 o menú Depurar > Iniciar depuración

// Sugerencias para primeros pasos: 1. Use la ventana del Explorador de soluciones para agregar y administrar archivos
//   2. Use la ventana de Team Explorer para conectar con el control de código fuente
//   3. Use la ventana de salida para ver la salida de compilación y otros mensajes
//   4. Use la ventana Lista de errores para ver los errores
//   5. Vaya a Proyecto > Agregar nuevo elemento para crear nuevos archivos de código, o a Proyecto > Agregar elemento existente para agregar archivos de código existentes al proyecto
//   6. En el futuro, para volver a abrir este proyecto, vaya a Archivo > Abrir > Proyecto y seleccione el archivo .sln
