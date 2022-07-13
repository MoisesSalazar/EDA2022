#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#define D 5    // Dimension de los Puntos
#define M 7    // Numero de Celulas del NodeDirectory
#define NND 15 // Numero de Elementos de un Nodo Data
#define MAX_OVERLAF 10000
//============================================================== ESTRUCTURA PUNTO ===============================================================

int int_random(int N, int M1)
{
    int numero = rand() % M1 + N;
    return numero;
}

typedef struct PUNTO
{
    int num_elem;
    int total;
    int *arreglo;
} PUNTO;

PUNTO *INICIALIZAR_PUNTO()
{
    PUNTO *new_punto = malloc(sizeof(PUNTO));
    new_punto->num_elem = 0;
    new_punto->total = D;

    int *new_arreglo = malloc(sizeof(int) * new_punto->num_elem);
    for (int i = 0; i < new_punto->total; i++)
    {
        (new_arreglo)[i] = 0;
    }

    new_punto->arreglo = new_arreglo;
    return new_punto;
}

void ACTUALIZAR_PUNTO(PUNTO *P, int *arr)
{
    P->num_elem = D;
    for (size_t i = 0; i < P->total; i++)
    {
        P->arreglo[i] = arr[i];
    }
}

PUNTO *COPIAR_PUNTO(PUNTO *P)
{
    PUNTO *copy = malloc(sizeof(PUNTO));
    copy->total = P->total;
    copy->num_elem = P->num_elem;
    copy->arreglo = malloc(sizeof(int) * P->total);
    for (int i = 0; i < P->total; i++)
    {
        copy->arreglo[i] = P->arreglo[i];
    }
    return copy;
}

void MOSTRAR_PUNTO(PUNTO *P)
{

    int *ptr = P->arreglo;
    for (int i = 0; i < D; i++)
    {
        printf("%d ", ptr[i]);
    }
}

//============================================================= ESTRUCTURA NODO DATA =============================================================
typedef struct NODO_DATA
{
    int total;
    int num_elem;
    PUNTO **vector;
} NODO_DATA;

NODO_DATA *INICIALIZAR_NODO_DATA()
{
    NODO_DATA *new_nodo_data = malloc(sizeof(NODO_DATA));
    PUNTO **new_vector_point = malloc(sizeof(PUNTO) * NND);
    new_nodo_data->total = NND;
    new_nodo_data->num_elem = 0;
    new_nodo_data->vector = new_vector_point;

    PUNTO **iter = new_nodo_data->vector;

    for (int i = 0; i < new_nodo_data->total; i++)
    {
        (iter)[i] = INICIALIZAR_PUNTO();
    }

    return new_nodo_data;
}

NODO_DATA *COPIAR_NODO_DATA(NODO_DATA *ND)
{
    NODO_DATA *copy = INICIALIZAR_NODO_DATA();
    copy->num_elem = ND->num_elem;
    copy->total = ND->total;
    for (int i = 0; i < copy->num_elem; i++)
    {
        (copy->vector)[i] = COPIAR_PUNTO((ND->vector)[i]);
    }
    return copy;
}

int INSERTAR_NODO_DATA(NODO_DATA *ND, PUNTO *P)
{
    if (ND->num_elem < ND->total)
    {

        ACTUALIZAR_PUNTO((ND->vector)[ND->num_elem], (*P).arreglo);
        ND->num_elem += 1;
        // printf("Insertado PUNTO NODO DATA:%d\n", ND->num_elem);
        // MOSTRAR_NODO_DATA(ND);
        return 1;
    }
    else
    {
        printf("NO Insertado\n");
        return 0;
    }
}

void MOSTRAR_NODO_DATA(NODO_DATA *ND)
{

    for (int i = 0; i < ND->num_elem; i++)
    {
        MOSTRAR_PUNTO((*ND).vector[i]);
        printf("\n");
    }
}

NODO_DATA *ORDENAR_NODO_DATA(NODO_DATA *NA1)
{
    NODO_DATA *NA = COPIAR_NODO_DATA(NA1);
    // MOSTRAR_NODO_DATA(NA);
    PUNTO *temp = INICIALIZAR_PUNTO();
    int x, indiceActual;
    for (x = 0; x < NA->num_elem; x++)
    {
        for (indiceActual = 0; indiceActual < NA->num_elem - 1; indiceActual++)
        {
            int indiceSiguienteElemento = indiceActual + 1;
            int sum1 = 0;
            int sum2 = 0;
            for (int i = 0; i < (NA->vector)[0]->total; i++)
            {
                sum1 += (NA->vector)[indiceActual]->arreglo[i];
                sum2 += (NA->vector)[indiceSiguienteElemento]->arreglo[i];
                if (sum1 > sum2)
                {
                    temp = COPIAR_PUNTO((NA->vector)[indiceActual]);
                    (NA->vector)[indiceActual] = COPIAR_PUNTO((NA->vector)[indiceSiguienteElemento]);
                    (NA->vector)[indiceSiguienteElemento] = COPIAR_PUNTO(temp);
                }
            }
        }
    }
    // printf("Ordenado\n");
    // MOSTRAR_NODO_DATA(NA);
    return NA;
}

//============================================================= ESTRUCTURA RECTANGULO =============================================================

typedef struct RECTANGULO
{
    PUNTO *DS;
    PUNTO *II;
} RECTANGULO;

RECTANGULO *INICIALIZAR_RECTANGULO()
{
    RECTANGULO *new_rect = malloc(sizeof(RECTANGULO));
    new_rect->DS = INICIALIZAR_PUNTO();
    new_rect->II = INICIALIZAR_PUNTO();
    return new_rect;
}

RECTANGULO *COPIAR_RECTANGULO(RECTANGULO *R)
{
    RECTANGULO *copy = malloc(sizeof(RECTANGULO));
    copy->DS = COPIAR_PUNTO(R->DS);
    copy->II = COPIAR_PUNTO(R->II);
    return copy;
}

RECTANGULO *ACTUALIZAR_RECTANGULO(PUNTO *DS, PUNTO *II)
{
    RECTANGULO *update = malloc(sizeof(RECTANGULO));
    update->DS = COPIAR_PUNTO(DS);
    update->II = COPIAR_PUNTO(II);
    return update;
}

void MOSTRAR_RECTANGULO(RECTANGULO *R)
{
    printf("Lado Derecho Superior: ");
    MOSTRAR_PUNTO(R->DS);
    printf("\nLado Izquierdo Inferior: ");
    MOSTRAR_PUNTO(R->II);
    printf("\n");
}

int AREA_RECTANGULO(RECTANGULO *R)
{
    int area = 1;
    for (int i = 0; i < R->DS->total; i++)
    {
        area *= abs((R->II->arreglo)[i] - R->DS->arreglo[i]);
    }
    return area;
}

void AJUSTAR_RECTANGULO(RECTANGULO *R, PUNTO *P)
{
    for (int i = 0; i < P->total; i++)
    {
        if (!((R->II->arreglo)[i] <= (P->arreglo)[i] && (R->DS->arreglo)[i] >= (P->arreglo)[i]))
        {
            if ((R->II->arreglo)[i] > (P->arreglo)[i])
            {
                (R->II->arreglo)[i] = (P->arreglo)[i];
            }
            else
            {
                (R->DS->arreglo)[i] = (P->arreglo)[i];
            }
        }
    }
}

//============================================================= ESTRUCTURA CELULA =============================================================

typedef struct NODO_DIRECTORIO NODO_DIRECTORIO; // PORQUE AUN NO CREAMOS NODO_DIRECTORIO y LO CREAMOS EN CELULA

typedef struct CELULA
{
    RECTANGULO *R;
    NODO_DIRECTORIO *N_Di;
    NODO_DATA *N_Da;
} CELULA;

CELULA *INICIALIZAR_CELULA()
{
    CELULA *C = malloc(sizeof(CELULA));
    C->R = INICIALIZAR_RECTANGULO();
    C->N_Da = NULL;
    C->N_Di = NULL;
}

CELULA *COPIAR_CELULA(CELULA *C)
{
    CELULA *copy = INICIALIZAR_CELULA();
    if (C->N_Da != NULL)
    {
        copy->N_Da = COPIAR_NODO_DATA(C->N_Da);
        copy->R = COPIAR_RECTANGULO(C->R);
    }
    return copy;
}

void MOSTRAR_CELULA(CELULA *C)
{
    printf("Datos de Celula:\n");
    MOSTRAR_RECTANGULO(C->R);
    if (C->N_Da == NULL)
    {
        printf("No tiene Nodo de Almacenamiento:\n");
    }
    else
    {
        printf("Puntos:\n");
        MOSTRAR_NODO_DATA(C->N_Da);
    }
    if (C->N_Di == NULL)
    {
        printf("No tiene nodo Directorio:\n");
    }
    else
    {
        printf("Si tiene nodo Directorio:\n");
    }
}

int INSERTAR_PUNTO_CELULA(CELULA *C, PUNTO *P)
{
    if (C->N_Da->num_elem < C->N_Da->total)
    {
        AJUSTAR_RECTANGULO(C->R, P);
        return INSERTAR_NODO_DATA(C->N_Da, P);
    }

    return 0;
}

// Patentar criterio de
void DIVIDIR_CELULA(CELULA **C, CELULA **C1, PUNTO *P)
{
    NODO_DATA *NA = ORDENAR_NODO_DATA((*C)->N_Da);
    CELULA *C_aux = INICIALIZAR_CELULA();
    *C1 = INICIALIZAR_CELULA();
    int mitad = NA->total / 2;
    for (int i = 0; i < NA->total; i++)
    {
        if (i >= mitad)
        {
            if ((*C1)->N_Da == NULL)
            {
                (*C1)->N_Da = INICIALIZAR_NODO_DATA();
                (*C1)->R = ACTUALIZAR_RECTANGULO((NA)->vector[i], (NA)->vector[i]);
                INSERTAR_PUNTO_CELULA(*C1, (NA)->vector[i]);
            }
            else
            {
                INSERTAR_PUNTO_CELULA(*C1, (NA)->vector[i]);
            }
        }
        else
        {
            if (C_aux->N_Da == NULL)
            {
                C_aux->N_Da = INICIALIZAR_NODO_DATA();
                C_aux->R = ACTUALIZAR_RECTANGULO((NA)->vector[i], (NA)->vector[i]);
                INSERTAR_PUNTO_CELULA(C_aux, (NA)->vector[i]);
            }
            else
            {
                INSERTAR_PUNTO_CELULA(C_aux, (NA)->vector[i]);
            }
        }
    }
    // El punto en cual nos conviene meter en C1 o C2
    *C = C_aux;
    RECTANGULO *R = COPIAR_RECTANGULO((*C)->R);
    AJUSTAR_RECTANGULO(R, P);
    int area_c_aux = AREA_RECTANGULO(R);

    R = COPIAR_RECTANGULO((*C1)->R);
    AJUSTAR_RECTANGULO(R, P);
    int area_c1 = AREA_RECTANGULO(R);

    if (area_c_aux < area_c1)
    {
        INSERTAR_PUNTO_CELULA(*C, P);
    }
    else
    {
        INSERTAR_PUNTO_CELULA(*C1, P);
    }
}

//============================================================= ESTRUCTURA NODO DIRECTORIO =============================================================

typedef struct NODO_DIRECTORIO
{
    int total;
    int tipo;
    int num_elem;
    CELULA **vector;
    NODO_DIRECTORIO *padre;
} NODO_DIRECTORIO;

NODO_DIRECTORIO *INICIALIZAR_NODO_DIRECTORIO()
{
    NODO_DIRECTORIO *NDi = malloc(sizeof(NODO_DIRECTORIO));
    NDi->total = M * 2;
    NDi->tipo = 0;
    NDi->num_elem = 0;
    NDi->vector = malloc(sizeof(CELULA) * M);
    NDi->padre = NULL;
    for (int i = 0; i < NDi->total; i++)
    {
        (NDi->vector)[i] = INICIALIZAR_CELULA();
    }
    NDi->total = M;
    return NDi;
}

void MOSTRAR_NODO_DIRECTORIO(NODO_DIRECTORIO *NDi)
{
    printf("\nDatos Nodo Directorio\n");
    printf("Numero de Celular activas: %d\n", NDi->num_elem);
    for (int i = 0; i < NDi->total; i++)
    {
        printf("=====================%d===================\n", i + 1);
        MOSTRAR_CELULA((NDi->vector)[i]);
    }
}

void AJUSTAR_RECTANGULO_NODO_DIRECTORIO(NODO_DIRECTORIO *NDi, RECTANGULO **R)
{
    // printf("+++--------------------%d----------------+++\n", NDi->num_elem);

    *R = ACTUALIZAR_RECTANGULO((NDi->vector)[0]->N_Da->vector[0], (NDi->vector)[0]->N_Da->vector[0]);

    for (int i = 0; i < NDi->total; i++)
    {
        if ((NDi->vector)[i]->N_Da != NULL)
        {
            for (int j = 0; j < (NDi->vector)[i]->N_Da->num_elem; j++)
            {
                AJUSTAR_RECTANGULO(*R, (NDi->vector)[i]->N_Da->vector[j]);
            }
            // printf("\n");
        }
    }
    // MOSTRAR_RECTANGULO(*R);
    // MOSTRAR_NODO_DIRECTORIO(NDi);
    // printf("+++--------------------%d----------------+++\n", NDi->num_elem);
}

//============================================================= ESTRUCTURA XTRE =============================================================

typedef struct XTREE
{
    NODO_DIRECTORIO *root;
} XTREE;

XTREE *INICIALIZAR_XTREE()
{
    XTREE *new = malloc(sizeof(XTREE));
    new->root = NULL;
    return new;
}

int ELEGIR_INDEX_CELULA(NODO_DIRECTORIO *NDi, PUNTO *P) // Choose Leaf
{
    int index = 0;
    int area = 0;
    RECTANGULO *R;

    R = COPIAR_RECTANGULO((NDi->vector)[0]->R);
    AJUSTAR_RECTANGULO(R, P);
    area = AREA_RECTANGULO(R);

    // printf("\nNumero de elementos: %d\n", NDi->num_elem);

    for (int i = 1; i < NDi->num_elem; i++)
    {
        R = COPIAR_RECTANGULO((NDi->vector)[i]->R);
        AJUSTAR_RECTANGULO(R, P);
        int aux_area = AREA_RECTANGULO(R);
        // printf("Area: %d -> Area: %d\n", area, aux_area);
        if (aux_area < area)
        {
            area = aux_area;
            index = i;
        }
    }

    if (area > MAX_OVERLAF)
    {
        // printf("SUPERNODO\n");

        NDi->total = M * 2;
    }

    return index;
}

NODO_DIRECTORIO *ELEGIR_NODO_DIRECTORIO(NODO_DIRECTORIO *E, PUNTO *P)
{
    if (E->vector[0]->N_Di == NULL)
    {
        return E;
    }
    else
    {
        int index = ELEGIR_INDEX_CELULA(E, P);
        // AJUSTAR_RECTANGULO_NODO_DIRECTORIO((E->vector)[index]->N_Di, &(E->vector)[index]->R);
        return ELEGIR_NODO_DIRECTORIO((E->vector)[index]->N_Di, P);
    }
}

void DIVIDIR_NODO_DIRECTORIO(NODO_DIRECTORIO *NDi, PUNTO *P, int index)
{
    // printf("++++++++++++++++++++++++++++++++++++++++\n");
    NODO_DIRECTORIO *NDi_1 = INICIALIZAR_NODO_DIRECTORIO();
    NODO_DIRECTORIO *NDi_2 = INICIALIZAR_NODO_DIRECTORIO();
    CELULA *overflow = INICIALIZAR_CELULA();
    DIVIDIR_CELULA(&(NDi->vector)[index], &overflow, P);
    // MOSTRAR_NODO_DIRECTORIO(NDi_1);
    //  MOSTRAR_NODO_DIRECTORIO(NDi_2);

    for (int i = 0; i < (NDi->total / 2) + 1; i++)
    {
        (NDi_1->vector)[i] = COPIAR_CELULA((NDi->vector)[i]);
        NDi_1->num_elem += 1;
    }

    int j = 0;
    for (int i = (NDi->total / 2) + 1; i < NDi->total; i++, j++)
    {
        (NDi_2->vector)[j] = COPIAR_CELULA((NDi->vector)[i]);
        NDi_2->num_elem += 1;
    }
    (NDi_2->vector)[j] = overflow;
    NDi_2->num_elem += 1;

    (NDi->vector)[0]->N_Da = NULL;
    (NDi->vector)[0]->N_Di = NDi_1;
    (NDi->vector)[1]->N_Da = NULL;
    (NDi->vector)[1]->N_Di = NDi_2;

    for (int i = 2; i < NDi->total; i++)
    {
        (NDi->vector)[i]->N_Da = NULL;
        (NDi->vector)[i]->N_Di = NULL;
        (NDi->vector)[i]->R = INICIALIZAR_RECTANGULO();
    }
    NDi->num_elem = 2;
    // MOSTRAR_NODO_DIRECTORIO(NDi_1);
    AJUSTAR_RECTANGULO_NODO_DIRECTORIO(NDi_1, &(NDi->vector)[0]->R);
    // MOSTRAR_NODO_DIRECTORIO(NDi_2);
    AJUSTAR_RECTANGULO_NODO_DIRECTORIO(NDi_2, &(NDi->vector)[1]->R);
    // printf("++++++++++++++++++++++++++++++++++++++++\n");
}

void INSERTAR_PUNTO_XTREE(XTREE *xtree, PUNTO *P)
{
    if (xtree->root == NULL)
    {
        printf("Xtree Vacio\n");
        xtree->root = INICIALIZAR_NODO_DIRECTORIO();
        // MOSTRAR_NODO_DIRECTORIO(xtree->root);

        (xtree->root->vector)[0]->N_Da = INICIALIZAR_NODO_DATA();
        (xtree->root->vector)[0]->R = ACTUALIZAR_RECTANGULO(P, P);

        printf("INSERTADO CORRECTAMENTE: %d\n", INSERTAR_PUNTO_CELULA((xtree->root->vector)[0], P));
        // MOSTRAR_PUNTO(P);
    }
    else
    {
        NODO_DIRECTORIO *NDi = ELEGIR_NODO_DIRECTORIO(xtree->root, P);
        // printf("LLegamos\n");
        int index = ELEGIR_INDEX_CELULA(NDi, P);

        int index_destino = 0;

        for (int i = 0; i < NDi->total; i++)
        {
            if ((NDi->vector)[i]->N_Da == NULL)
            {
                index_destino = i;
                break;
            }
        }
        // printf("\nIndex: %d\n", index);
        if ((NDi->vector)[index]->N_Da->num_elem < (NDi->vector)[index]->N_Da->total)
        {
            printf("INSERTADO CORRECTAMENTE: %d\n", INSERTAR_PUNTO_CELULA((NDi->vector)[index], P));
            // MOSTRAR_PUNTO(P);
        }
        else if (index_destino != 0)
        {

            // printf("==========================%d====================\n", index_destino);
            // printf("Parece que se lleno: %d\n", (NDi->vector)[index]->N_Da->num_elem);
            // printf("Tam Nodo Directorio Antes: %d\n", NDi->num_elem);
            DIVIDIR_CELULA(&(NDi->vector)[index], &(NDi->vector)[index_destino], P);
            NDi->num_elem += 1;
            // printf("Tam Nodo Directorio Despues: %d\n", NDi->num_elem);
        }
        else
        {
            printf("Split a Nivel de Nodo\n");
            DIVIDIR_NODO_DIRECTORIO(NDi, P, index);
        }
    }
}

void KNN(XTREE *xtree, int knn, PUNTO *P)
{
    printf("PUNTO A BUSCAR: ");
    MOSTRAR_PUNTO(P);
    printf("\n");
    NODO_DIRECTORIO *NDi = ELEGIR_NODO_DIRECTORIO(xtree->root, P);
    int knn_aux = 0;
    for (int i = 0; i < NDi->num_elem; i++)
    {
        for (int i = 0; i < (NDi->vector)[i]->N_Da->num_elem; i++)
        {
            MOSTRAR_PUNTO((NDi->vector)[i]->N_Da->vector[i]);
            printf("\n");
            if (knn_aux > knn)
            {
                break;
            }
            knn_aux++;
        }
        if (knn_aux > knn)
        {
            break;
        }
    }
}

float distancia(PUNTO *P1, PUNTO *P2)
{
    float distancia = 0;
    for (int i = 0; i < D; i++)
    {
        distancia += pow(abs(P1->arreglo[i] - P2->arreglo[i]), 2);
    }
    return sqrt(distancia);
}

void RANGE_KNN(XTREE *xtree, int range, PUNTO *P)
{
    printf("Range: ");
    printf("\n");
    NODO_DIRECTORIO *NDi = ELEGIR_NODO_DIRECTORIO(xtree->root, P);
    int knn_aux = 0;
    for (int i = 0; i < NDi->num_elem; i++)
    {
        for (int j = 0; j < (NDi->vector)[i]->N_Da->num_elem; j++)
        {
            float dis = distancia((NDi->vector)[i]->N_Da->vector[j], P);
            if (range > dis)
            {
                printf("Distancia: %f -> ", dis);
                MOSTRAR_PUNTO((NDi->vector)[i]->N_Da->vector[j]);
            }
            printf("\n");
        }
    }
}

int comparePoint(PUNTO *P1, PUNTO *P2)
{
    int bandera = 1;
    for (int i = 0; i < D; i++)
    {
        if (P1->arreglo[i] != P2->arreglo[i])
        {
            bandera = 0;
        }
    }
    return bandera;
}
void delete_xtree(XTREE *xtree, PUNTO *P)
{
    NODO_DIRECTORIO *NDi = ELEGIR_NODO_DIRECTORIO(xtree->root, P);
    for (int i = 0; i < NDi->num_elem; i++)
    {
        for (int i = 0; i < (NDi->vector)[i]->N_Da->num_elem; i++)
        {
            if (comparePoint(P,(NDi->vector)[i]->N_Da->vector[i]))
            {
                (NDi->vector)[i]->N_Da->vector[i] = INICIALIZAR_PUNTO();
                (NDi->vector)[i]->N_Da->vector[i] = NULL;
            }
            
        }
    }
}
void prueba_elementos_xtree()
{

    printf("PRUEBA NODO DATA\n");

    int p[] = {1, 1, 1};
    PUNTO *P = INICIALIZAR_PUNTO();
    PUNTO *P1 = INICIALIZAR_PUNTO();
    PUNTO *P2 = INICIALIZAR_PUNTO();
    PUNTO *P3 = INICIALIZAR_PUNTO();
    PUNTO *P4 = INICIALIZAR_PUNTO();
    PUNTO *P5 = INICIALIZAR_PUNTO();
    p[0] = 1;
    p[1] = 2;
    p[2] = 1;
    ACTUALIZAR_PUNTO(P, p);
    MOSTRAR_PUNTO(P);
    p[0] = 2;
    p[1] = 1;
    p[2] = 1;
    ACTUALIZAR_PUNTO(P1, p);
    MOSTRAR_PUNTO(P1);
    p[0] = 1;
    p[1] = 6;
    p[2] = 1;
    ACTUALIZAR_PUNTO(P2, p);
    MOSTRAR_PUNTO(P2);
    p[0] = 3;
    p[1] = 5;
    p[2] = 1;
    ACTUALIZAR_PUNTO(P3, p);
    MOSTRAR_PUNTO(P3);
    p[0] = 1;
    p[1] = 9;
    p[2] = 1;
    ACTUALIZAR_PUNTO(P4, p);
    MOSTRAR_PUNTO(P4);
    p[0] = 2;
    p[1] = 2;

    ACTUALIZAR_PUNTO(P5, p);
    MOSTRAR_PUNTO(P5);

    printf("\nPRUEBA NODO DATA\n");

    NODO_DATA *ND = INICIALIZAR_NODO_DATA();
    printf("%d\n", INSERTAR_NODO_DATA(ND, P1));
    printf("%d\n", INSERTAR_NODO_DATA(ND, P4));
    printf("%d\n", INSERTAR_NODO_DATA(ND, P2));

    MOSTRAR_NODO_DATA(ND);

    printf("\nPRUEBA RECTANGULO\n");

    RECTANGULO *R1 = ACTUALIZAR_RECTANGULO(P1, P2);
    MOSTRAR_RECTANGULO(R1);

    RECTANGULO *R2 = COPIAR_RECTANGULO(R1);
    R1 = ACTUALIZAR_RECTANGULO(P1, P1);
    MOSTRAR_RECTANGULO(R1);
    MOSTRAR_RECTANGULO(R2);

    printf("\nPRUEBA CELULA\n");
    CELULA *C = INICIALIZAR_CELULA();
    C->N_Da = COPIAR_NODO_DATA(ND);
    MOSTRAR_CELULA(C);

    printf("\nPRUEBA CELULA\n");
    NODO_DIRECTORIO *NDi = INICIALIZAR_NODO_DIRECTORIO();
    (NDi->vector)[2]->N_Da = COPIAR_NODO_DATA(ND);
    MOSTRAR_NODO_DIRECTORIO(NDi);
}

void prueba_xtree()
{
    XTREE *xtree = INICIALIZAR_XTREE();

    PUNTO *P = INICIALIZAR_PUNTO();
    int p[D];
    for (int i = 0; i < 1000; i++)
    {
        for (int j = 0; j < D; j++)
        {
            p[j] = int_random(5, 10);
        }
        ACTUALIZAR_PUNTO(P, p);
        MOSTRAR_PUNTO(P);
        printf("\n");
        INSERTAR_PUNTO_XTREE(xtree, P);
    }

    int knn = 20;
    printf("\nKNN %d\n", knn);
    KNN(xtree, knn, P);

    int range = 10;
    printf("\nRangeQuery %d\n", range);
    RANGE_KNN(xtree, range, P);
}

int main(void)
{
    prueba_xtree();
}