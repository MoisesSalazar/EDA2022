#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <iostream>
#include <vector>
#include <array>
#include <algorithm>
#include <bitset>
#include <string>
#include <map>
#include <cstdlib>

#define N 10
using namespace std;
int gen_random(int n) {
    return rand() % n + 1;
}

struct Point
{
	array<int, N> point;
	int dim = N;
	Point() {
		dim = N;
		for (int i = 0; i < dim; i++)
		{
			point[i] = 0;
		}
	}
	Point(int dim) {
		this->dim = dim;
		for (int i = 0; i < dim; i++)
		{
			point[i] = gen_random(10);
		}
	}
	void Copy(Point p) {
		this->point = p.point;
		this->dim = p.dim;
	}
	void Show() {
		cout << "(";
		for (int i = 0; i < dim; i++)
		{
			cout << point[i];
			if (i < dim - 1) {
				cout << ",";
			}
			
		}
		cout << ") ";

	}
};
void print(vector<Point> vect)
{
	for (Point i : vect) {
		i.Show();
		cout << endl;
	}
}
void Copy(vector<array<int, N>> &vect, vector<Point> array_point) {
	for (int i = 0; i < array_point.size(); i++)
	{
		vect.push_back(array_point[i].point);
	}
}
void RCopy(vector<array<int, N>> vect, vector<Point> &array_point) {
	array_point.clear();

	for (int i = 0; i < vect.size(); i++)
	{
		Point p1;
		p1.point = vect[i];
		array_point.push_back(p1);
	}
}

unsigned long long PointToBinary(Point p) {
	vector<string> Binary;
	
	for (int i = 0; i < p.dim; i++)
	{
		bitset<4> binary(p.point[i]);
		Binary.push_back(binary.to_string());
		//cout << binary.to_string() << endl;
	}
	string concat = "";

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < Binary.size(); j++)
		{
			concat += Binary[j][i];
		}
	}
	bitset<40> binary(concat);
	
	return binary.to_ullong() ;

}

vector<Point> escogerPuntosAleatorios(vector<Point> p, int n) {
	vector<Point> pp1;
	for (int i = 0; i < n; i++)
	{
		int num = gen_random(p.size() - 1);
		pp1.push_back(p[num]);
	}
	return pp1;
}

void ordenarV(vector<Point>& array_point) {
	vector<array<int, N>> vect;
	Copy(vect, array_point);
	sort(vect.begin(), vect.end());
	RCopy(vect, array_point);
}

void ZOrden(vector<unsigned long long> &array_point_1, vector<Point> array_point_10) {
	for (size_t i = 0; i < array_point_10.size(); i++)
	{
		array_point_1.push_back(PointToBinary(array_point_10[i]));
	}
}
void print_dim_1(vector<unsigned long long> array_point_1) {
	for (int i = 0; i < array_point_1.size(); i++)
	{
		cout << array_point_1[i] << endl;
	}
	
}

float distance(Point p,Point p1) {
	float ans = 0;
	for (int i = 0; i < p.dim; i++)
	{
		ans += (p.point[i] - p1.point[i]) * (p.point[i] - p1.point[i]);
	}
	return sqrt(ans);
}

vector<Point> KNN_10(Point p1, vector<Point> a_p_10, int n, int visualizar) {
	map<float, array<int, N>> knn;

	float dis = 0;
	for (int i = 0; i < a_p_10.size(); i++)
	{
		dis = distance(p1, a_p_10[i]);
		knn.insert(make_pair(dis, a_p_10[i].point));
	}
	if (visualizar)
	{
		cout << endl << "KNN" << endl;
	}
	auto iter = knn.begin();
	vector<Point> result_knn;
	Point pp1;
	int j = 0;

	while (iter != knn.end()) {
		pp1.point = iter->second;
		if (visualizar and j<=n)
		{
			cout << "Point: ";
			for (int i = 0; i < iter->second.size(); i++)
			{
				cout << iter->second[i] << " ";
				
			}
			cout << " ------- ";

			cout << "Dis: " << iter->first << endl;

		}
		result_knn.push_back(pp1);
		if (j==0 and visualizar)
		{
			cout << "Sus vecinos son :" << endl;
			cout << "--------------------" << endl;
		}
		j++;
		++iter;

	}
	result_knn.resize(n+1);
	return result_knn;
}

vector<unsigned long long> KNN_1(unsigned long long p, vector<unsigned long long> pt, int n, int visualizar) {
	map<unsigned long long, unsigned long long> knn;
	unsigned long long dis = 0;
	if (visualizar)
	{
		cout << endl << "KNN" << endl;
	}
	for (int i = 0; i < pt.size(); i++)
	{
		dis = abs(static_cast<long>(p - pt[i]));
		knn.insert(make_pair(dis,pt[i]));
	}

	auto iter = knn.begin();
	vector<unsigned long long> knn_result;
	int j = 0;
	while (iter != knn.end()) {
		if (visualizar and  j<=n)
		{
			cout << "Point: " << iter->second << "  ";
			cout << "  ---> Dis: " << iter->first << endl;
		}
		if (j == 0 and visualizar)
		{
			cout << "Vecinos mas cercanos:" << endl;
		}
		knn_result.push_back(iter->second);
		++iter;
		j++;
	}
	knn_result.resize(n + 1);
	return knn_result;
}

double match_orden(vector<Point> KNN_point_10, vector<unsigned long long> KNN_point_1,int visualizar) {
	if (visualizar)
	{
		cout << "Tablas de coincidencia: " << endl;
		cout << "Point     Binary Original    Binary KNN    Coincidencia" << endl;
	}
	int cont = 0;
	for (int i = 1; i < KNN_point_10.size(); i++)
	{
		if (visualizar)
		{
			KNN_point_10[i].Show();
			cout << "        " << PointToBinary(KNN_point_10[i]);
			cout << "   " << KNN_point_1[i];
			cout << "  " << (PointToBinary(KNN_point_10[i]) == KNN_point_1[i]) << endl;

		}
		if ((PointToBinary(KNN_point_10[i]) == KNN_point_1[i]))
		{
			cont++;
		}
	}
	//cout << cont << endl;
	int tam = (KNN_point_10.size() - 1);
	//cout << tam << endl;
	double total = double(cont) / double (tam);
	return total*100;
}

double match_desorden(vector<Point> KNN_point_10, vector<unsigned long long> KNN_point_1, int visualizar) {
	int cont = 0;
	for (int i = 1; i < KNN_point_10.size(); i++)
	{
		for (int j = 1; j < KNN_point_1.size(); j++)
		{
			if (PointToBinary(KNN_point_10[i]) == KNN_point_1[j])
			{
				cont++;
			}
		}
	}
	int tam = (KNN_point_10.size() - 1);
	double total = double(cont) / double(tam);
	return total*100;
}
void test_estadistico(int visualizar,int aleatorios ,int numeros_generados, int knn_iteraciones, int rango_iteraciones) {
	cout << "--------------------------- Iniciando Programa Z Orden ---------------------------" << endl;
	cout << "------------------------- Moises Salazar Machaca - UCSP --------------------------" << endl;
	cout << endl << endl << endl << endl;

	//Generando vector de Puntos
	vector<Point> puntos_dim_10;
	
	//Generamos N puntos
	cout << "Generando " << numeros_generados << " puntos de dimencion " << N << endl << endl;
	for (int i = 0; i < numeros_generados; i++) { Point p(N); puntos_dim_10.push_back(p); }
	
	if (visualizar)
	{
		cout << "Puntos Generados:" << endl;
		print(puntos_dim_10);
	}

	// Escogiendo Aleatorios
	cout << endl << "Escogiendo " << aleatorios << " numeros aleatorios" << endl << endl;
	vector<Point> puntos_ale_10 = escogerPuntosAleatorios(puntos_dim_10, aleatorios);
	if (visualizar)
	{
		cout << "Aleatorios Seleccionados:" << endl;
		print(puntos_ale_10);
	}
	
	//Ordenando Puntos
	cout << endl << "Ordenando puntos Generados " << endl << endl;
	ordenarV(puntos_dim_10);
	if (visualizar)
	{
		cout << "Puntos Generados Ordenado:" << endl;
		print(puntos_dim_10);
	}

	//Generando Z Orden
	cout << endl << "Aplicando Z Orden " << endl << endl;
	vector<unsigned long long> point_dim_1;
	ZOrden(point_dim_1, puntos_dim_10);
	if (visualizar)
	{
		for (int i = 0; i < numeros_generados; i++)
		{
			puntos_dim_10[i].Show();
			cout << " --> " << point_dim_1[i] << endl;
		}
	}
	//Aleatorios en 1 dimencion
	vector<unsigned long long> point_ale_1;
	ZOrden(point_ale_1, puntos_ale_10);
	if (visualizar){

		cout << endl << "Aleatorios " << endl;
		for (int i = 0; i < aleatorios; i++)
		{
			puntos_ale_10[i].Show();
			cout << " --> " << point_ale_1[i] << endl;
		}
	}

	int knn = rango_iteraciones;

	//Estadistica Total
	vector<pair<int, vector<pair<float, float>>>> s;
	for (int j = 0; j < aleatorios; j++)
	{
		cout << "Aleatorio: " << j + 1 << endl;
		vector<pair<float, float>> s1;
		for (int i = 0; i < knn_iteraciones; i++)
		{

			cout << "Para los " << knn << " Mas Cercanos" << endl;
			vector<Point> KNN_point_10 = KNN_10(puntos_ale_10[j], puntos_dim_10, knn, visualizar);
			vector<unsigned long long> KNN_point_1 = KNN_1(point_ale_1[j], point_dim_1, knn, visualizar);
			int mo = match_orden(KNN_point_10, KNN_point_1, visualizar);
			int md = match_desorden(KNN_point_10, KNN_point_1, visualizar);
			cout << mo << "%" << endl;
			cout << md << "%" << endl;
			knn += rango_iteraciones;
			s1.push_back({mo,md});
		}
		s.push_back({ j,s1 });
		knn = rango_iteraciones;
	}

	cout << "Estadistica General: " << endl;
	knn = rango_iteraciones;
	for (int i = 0; i < s.size(); i++)
	{
		cout << "Punto Aleatorio: " << i + 1 << endl;
		for (int j = 0; j < s[i].second.size(); j++)
		{
			cout << "Para los " << knn << endl;
			cout << "Macth Orden: " << s[i].second[j].first << "% -- Macth ZOrden: " << s[i].second[j].second << "%" << endl;
			knn += rango_iteraciones;
		}
		knn = rango_iteraciones;
		
	}
}
int main()
{
	srand(time(NULL));
	//	Visualizar cout - Numeros aleatorios generados - Numero generados - Knn iteraciones - rango de itereaciones
	test_estadistico(0,10,500,5,25);
	

}

