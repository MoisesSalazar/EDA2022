#include <iostream>
#include <utility>
#include <vector>
using namespace std;


struct Point
{
    pair<double, double> coor;

    Point()
    {
        coor.first = 0;
        coor.second = 0;
    }

    Point(const Point& p)
    {
        this->coor = p.coor;
    }

    Point(double x, double y)
    {
        this->coor.first = x;
        this->coor.second = y;
    }

    Point(pair<double, double> coord)
    {
        this->coor.first = coord.first;
        this->coor.second = coord.second;
    }

    void copy(Point p)
    {
        this->coor = p.coor;
    }

    bool equal(Point p)
    {
        return (p.coor.first == this->coor.first and p.coor.second == this->coor.second);
    }

    void showP()
    {
        cout << "Punto: " << this->coor.first << ":" << this->coor.second << endl;
    }
};

struct NodeQuad
{
    Point* mmr[2];
    vector<Point> points;
    NodeQuad* branch[4];

    NodeQuad()
    {
        mmr[0] = new Point();
        mmr[1] = new Point();

        for (int i = 0; i < 4; ++i)
            branch[i] = nullptr;
    }

    NodeQuad(const NodeQuad& qn)
    {
        this->mmr[0] = new Point(*qn.mmr[0]);
        this->mmr[1] = new Point(*qn.mmr[1]);
        this->points = qn.points;

        for (int i = 0; i < 4; ++i)
            branch[i] = nullptr;
    }

    NodeQuad(Point up, Point below)
    {
        this->mmr[0] = new Point(up);
        this->mmr[1] = new Point(below);

        for (int i = 0; i < 4; ++i)
            branch[i] = nullptr;
    }

    NodeQuad(Point up, Point below, vector<Point> ps)
    {
        this->mmr[0] = new Point(up);
        this->mmr[1] = new Point(below);
        this->points = ps;

        for (int i = 0; i < 4; ++i)
            branch[i] = nullptr;
    }

    void insert(Point p)
    {
        this->points.push_back(p);
    }

    void copy(NodeQuad qn)
    {
        this->mmr[0] = new Point(*qn.mmr[0]);
        this->mmr[1] = new Point(*qn.mmr[1]);
        this->points = qn.points;
    }

    void printQN()
    {
        cout << "Cuadrante Coordenadas: " << endl;
        this->mmr[0]->showP();
        this->mmr[1]->showP();
        cout << "Puntos en el cuadrante: ";
        int tam = 0;
        for (int i = 0, s = points.size(); i < s; ++i)
        {
             points[i].showP();
             tam++;
        }
        if (tam == 0)
        {
            cout << "Ningun punto existe en este cuadrante" << endl;
        }
    }
};

struct quad_QT
{
    int md;
    NodeQuad* root;

    quad_QT()
    {
        this->md = 0;
        this->root = new NodeQuad(Point(), Point());
    }

    quad_QT(int d, Point up, Point below)
    {
        this->md = d;
        this->root = new NodeQuad(up, below);

        NodeQuad** gen = &root;
        generate(gen, md);
    }

    void showQP(NodeQuad* aux)
    {
        if (aux)
        {
            showQP(aux->branch[0]);
            showQP(aux->branch[1]);
            aux->printQN();
            showQP(aux->branch[2]);
            showQP(aux->branch[3]);
        }
    }

    void generate(NodeQuad** gen, int d)
    {
        if (d > 1)
        {
            double div_x = (*gen)->mmr[1]->coor.first - ((*gen)->mmr[1]->coor.first - (*gen)->mmr[0]->coor.first) / 2;
            double div_y = (*gen)->mmr[0]->coor.second - ((*gen)->mmr[0]->coor.second - (*gen)->mmr[1]->coor.second) / 2;
            (*gen)->branch[0] = new NodeQuad(Point(pair<double, double>{ (*gen)->mmr[0]->coor.first, div_y }), Point(pair<double, double>{ div_x, (*gen)->mmr[1]->coor.second }));
            (*gen)->branch[1] = new NodeQuad(Point({ (*gen)->mmr[0]->coor.first, (*gen)->mmr[0]->coor.second }), Point({ div_x, div_y }));
            (*gen)->branch[2] = new NodeQuad(Point({ div_x,  (*gen)->mmr[0]->coor.second }), Point({ (*gen)->mmr[1]->coor.first, div_y }));
            (*gen)->branch[3] = new NodeQuad(Point({ div_x, div_y }), Point({ (*gen)->mmr[1]->coor.first, (*gen)->mmr[1]->coor.second }));

            for (int i = 0; i < 4; ++i)
            {
                generate(&(*gen)->branch[i], d - 1);
            }
        }
    }

    bool inside(Point p, NodeQuad qn)
    {
        if (p.coor.first >= qn.mmr[0]->coor.first and p.coor.second <= qn.mmr[0]->coor.second and p.coor.first <= qn.mmr[1]->coor.first and p.coor.second >= qn.mmr[1]->coor.second)
            return true;
        return false;
    }

    void ins(Point p, NodeQuad** ptr)
    {
        for (int i = 0; i < 4; ++i)
        {
            if ((*ptr)->branch[i] and inside(p, *(*ptr)->branch[i]))
            {
                (*ptr)->branch[i]->points.push_back(p);
                ins(p, &(*ptr)->branch[i]);
            }
        }
    }

    void insert(Point p)
    {
        NodeQuad** finder = &root;
        (*finder)->points.push_back(p);
        ins(p, finder);
    }
};



int main()
{
    Point lim_superior(0, 100), lim_inferior(100, 0);
    /*Insertamos los dos puntos para el mapeo*/
    quad_QT QT(2, lim_superior, lim_inferior);
    cout << "Se imprime los rectangulos, con sus delimitadores" << endl;
    cout << "Como se ve no hay ningun punto porque no insertamos ninguno aun" << endl;
    QT.showQP(QT.root);
    system("pause");
    system("cls");

    cout << "Se imprime los rectangulos, con sus delimitadores" << endl;
    QT.insert(Point(25, 25));
    QT.showQP(QT.root);
    system("pause");
    system("cls");

    cout << "Se imprime los rectangulos, con sus delimitadores" << endl;
    QT.insert(Point(25, 75));
    QT.showQP(QT.root);
    system("pause");
    system("cls");

    cout << "Se imprime los rectangulos, con sus delimitadores" << endl;
    QT.insert(Point(75, 25));
    QT.showQP(QT.root);
    system("pause");
    system("cls");

    cout << "Se imprime los rectangulos, con sus delimitadores" << endl;
    QT.insert(Point(75, 75));
    QT.showQP(QT.root);
    system("pause");
    system("cls");


}