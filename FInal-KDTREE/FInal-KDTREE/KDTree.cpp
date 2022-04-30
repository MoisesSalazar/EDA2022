#include "KDTree.h"
#include <math.h>
#include <algorithm>
#include <limits>
#include <stdexcept>

namespace KDTree {


    class compare_dim {
    public:
        compare_dim(size_t dim) { d = dim; }
        bool operator()(const NodeKDTree& p, const NodeKDTree& q) {
            return (p.point[d] < q.point[d]);
        }
        size_t d;
    };

    class kdtree_node {
    public:
        kdtree_node() {
            dataindex = cutdim = 0;
            loson = hison = (kdtree_node*)NULL;
        }
        ~kdtree_node() {
            if (loson) delete loson;
            if (hison) delete hison;
        }
        size_t dataindex;
        size_t cutdim;
        CoordPoint point;
        kdtree_node* loson, * hison;
        CoordPoint lobound, upbound;
    };

    class DistanceMeasure {
    public:
        DistanceMeasure() {}
        virtual ~DistanceMeasure() {}
        virtual double distance(const CoordPoint& p, const CoordPoint& q) = 0;
        virtual double coordinate_distance(double x, double y, size_t dim) = 0;
    };

    class DistanceL0 : virtual public DistanceMeasure {
        DoubleVector* w;

    public:
        DistanceL0(const DoubleVector* weights = NULL) {
            if (weights)
                w = new DoubleVector(*weights);
            else
                w = (DoubleVector*)NULL;
        }
        ~DistanceL0() {
            if (w) delete w;
        }
        double distance(const CoordPoint& p, const CoordPoint& q) {
            size_t i;
            double dist, test;
            if (w) {
                dist = (*w)[0] * fabs(p[0] - q[0]);
                for (i = 1; i < p.size(); i++) {
                    test = (*w)[i] * fabs(p[i] - q[i]);
                    if (test > dist) dist = test;
                }
            }
            else {
                dist = fabs(p[0] - q[0]);
                for (i = 1; i < p.size(); i++) {
                    test = fabs(p[i] - q[i]);
                    if (test > dist) dist = test;
                }
            }
            return dist;
        }
        double coordinate_distance(double x, double y, size_t dim) {
            if (w)
                return (*w)[dim] * fabs(x - y);
            else
                return fabs(x - y);
        }
    };

    class DistanceL1 : virtual public DistanceMeasure {
        DoubleVector* w;

    public:
        DistanceL1(const DoubleVector* weights = NULL) {
            if (weights)
                w = new DoubleVector(*weights);
            else
                w = (DoubleVector*)NULL;
        }
        ~DistanceL1() {
            if (w) delete w;
        }
        double distance(const CoordPoint& p, const CoordPoint& q) {
            size_t i;
            double dist = 0.0;
            if (w) {
                for (i = 0; i < p.size(); i++) dist += (*w)[i] * fabs(p[i] - q[i]);
            }
            else {
                for (i = 0; i < p.size(); i++) dist += fabs(p[i] - q[i]);
            }
            return dist;
        }
        double coordinate_distance(double x, double y, size_t dim) {
            if (w)
                return (*w)[dim] * fabs(x - y);
            else
                return fabs(x - y);
        }
    };

    class DistanceL2 : virtual public DistanceMeasure {
        DoubleVector* w;

    public:
        DistanceL2(const DoubleVector* weights = NULL) {
            if (weights)
                w = new DoubleVector(*weights);
            else
                w = (DoubleVector*)NULL;
        }
        ~DistanceL2() {
            if (w) delete w;
        }
        double distance(const CoordPoint& p, const CoordPoint& q) {
            size_t i;
            double dist = 0.0;
            if (w) {
                for (i = 0; i < p.size(); i++)
                    dist += (*w)[i] * (p[i] - q[i]) * (p[i] - q[i]);
            }
            else {
                for (i = 0; i < p.size(); i++) dist += (p[i] - q[i]) * (p[i] - q[i]);
            }
            return dist;
        }
        double coordinate_distance(double x, double y, size_t dim) {
            if (w)
                return (*w)[dim] * (x - y) * (x - y);
            else
                return (x - y) * (x - y);
        }
    };
    KDTree::~KDTree() {
        if (root) delete root;
        delete distance;
    }

    KDTree::KDTree(const KdNodeVector* nodes, int distance_type /*=2*/) {
        size_t i, j;
        double val;
        // copy over input data
        if (!nodes || nodes->empty()) {
            throw std::invalid_argument("KDTree::KDTree(): argument nodes must not be empty");
        }
        dimension = nodes->begin()->point.size();
        allnodes = *nodes;
        distance = NULL;
        this->distance_type = -1;
        set_distance(distance_type);
        lobound = nodes->begin()->point;
        upbound = nodes->begin()->point;
        for (i = 1; i < nodes->size(); i++) {
            for (j = 0; j < dimension; j++) {
                val = allnodes[i].point[j];
                if (lobound[j] > val) lobound[j] = val;
                if (upbound[j] < val) upbound[j] = val;
            }
        }
        root = build_tree(0, 0, allnodes.size());
    }

    void KDTree::set_distance(int distance_type,
        const DoubleVector* weights /*=NULL*/) {
        if (distance) delete distance;
        this->distance_type = distance_type;
        if (distance_type == 0) {
            distance = (DistanceMeasure*)new DistanceL0(weights);
        }
        else if (distance_type == 1) {
            distance = (DistanceMeasure*)new DistanceL1(weights);
        }
        else {
            distance = (DistanceMeasure*)new DistanceL2(weights);
        }
    }

    kdtree_node* KDTree::build_tree(size_t depth, size_t a, size_t b) {
        size_t m;
        double temp, cutval;
        kdtree_node* node = new kdtree_node();
        node->lobound = lobound;
        node->upbound = upbound;
        node->cutdim = depth % dimension;
        if (b - a <= 1) {
            node->dataindex = a;
            node->point = allnodes[a].point;
        }
        else {
            m = (a + b) / 2;
            std::nth_element(allnodes.begin() + a, allnodes.begin() + m,
                allnodes.begin() + b, compare_dim(node->cutdim));
            node->point = allnodes[m].point;
            cutval = allnodes[m].point[node->cutdim];
            node->dataindex = m;
            if (m - a > 0) {
                temp = upbound[node->cutdim];
                upbound[node->cutdim] = cutval;
                node->loson = build_tree(depth + 1, a, m);
                upbound[node->cutdim] = temp;
            }
            if (b - m > 1) {
                temp = lobound[node->cutdim];
                lobound[node->cutdim] = cutval;
                node->hison = build_tree(depth + 1, m + 1, b);
                lobound[node->cutdim] = temp;
            }
        }
        return node;
    }

    void KDTree::k_nearest_neighbors(const CoordPoint& point, size_t k,
        KdNodeVector* result,
        KdNodePredicate* pred /*=NULL*/) {
        size_t i;
        NodeKDTree temp;
        searchpredicate = pred;

        result->clear();
        if (k < 1) return;
        if (point.size() != dimension) {
            throw std::invalid_argument(
                "KDTree::k_nearest_neighbors(): point must be of same dimension as "
                "KDTree");
        }
            

        neighborheap = new std::priority_queue<nn4heap, std::vector<nn4heap>, compare_nn4heap>();
        if (k > allnodes.size()) {
            k = allnodes.size();
            for (i = 0; i < k; i++) {
                if (!(searchpredicate && !(*searchpredicate)(allnodes[i])))
                    neighborheap->push(
                        nn4heap(i, distance->distance(allnodes[i].point, point)));
            }
        }
        else {
            neighbor_search(point, root, k);
        }

        while (!neighborheap->empty()) {
            i = neighborheap->top().dataindex;
            neighborheap->pop();
            result->push_back(allnodes[i]);
        }

        k = result->size();
        for (i = 0; i < k / 2; i++) {
            temp = (*result)[i];
            (*result)[i] = (*result)[k - 1 - i];
            (*result)[k - 1 - i] = temp;
        }
        delete neighborheap;
    }

    void KDTree::range_nearest_neighbors(const CoordPoint& point, double r,
        KdNodeVector* result) {
        NodeKDTree temp;

        result->clear();
        if (point.size() != dimension) {
            throw std::invalid_argument(
                "KDTree::k_nearest_neighbors(): point must be of same dimension as "
                "KDTree");
        }
            
        if (this->distance_type == 2) {
            r *= r;
        }

        range_search(point, root, r);

        for (std::vector<size_t>::iterator i = range_result.begin();
            i != range_result.end(); ++i) {
            result->push_back(allnodes[*i]);
        }

        range_result.clear();
    }


    bool KDTree::neighbor_search(const CoordPoint& point, kdtree_node* node,
        size_t k) {
        double curdist, dist;

        curdist = distance->distance(point, node->point);
        if (!(searchpredicate && !(*searchpredicate)(allnodes[node->dataindex]))) {
            if (neighborheap->size() < k) {
                neighborheap->push(nn4heap(node->dataindex, curdist));
            }
            else if (curdist < neighborheap->top().distance) {
                neighborheap->pop();
                neighborheap->push(nn4heap(node->dataindex, curdist));
            }
        }

        if (point[node->cutdim] < node->point[node->cutdim]) {
            if (node->loson)
                if (neighbor_search(point, node->loson, k)) return true;
        }
        else {
            if (node->hison)
                if (neighbor_search(point, node->hison, k)) return true;
        }

        if (neighborheap->size() < k) {
            dist = std::numeric_limits<double>::max();
        }
        else {
            dist = neighborheap->top().distance;
        }
        if (point[node->cutdim] < node->point[node->cutdim]) {
            if (node->hison && bounds_overlap_ball(point, dist, node->hison))
                if (neighbor_search(point, node->hison, k)) return true;
        }
        else {
            if (node->loson && bounds_overlap_ball(point, dist, node->loson))
                if (neighbor_search(point, node->loson, k)) return true;
        }

        if (neighborheap->size() == k) dist = neighborheap->top().distance;
        return ball_within_bounds(point, dist, node);
    }

    void KDTree::range_search(const CoordPoint& point, kdtree_node* node,
        double r) {
        double curdist = distance->distance(point, node->point);
        if (curdist <= r) {
            range_result.push_back(node->dataindex);
        }
        if (node->loson != NULL && this->bounds_overlap_ball(point, r, node->loson)) {
            range_search(point, node->loson, r);
        }
        if (node->hison != NULL && this->bounds_overlap_ball(point, r, node->hison)) {
            range_search(point, node->hison, r);
        }
    }

    bool KDTree::bounds_overlap_ball(const CoordPoint& point, double dist,
        kdtree_node* node) {
        double distsum = 0.0;
        size_t i;
        for (i = 0; i < dimension; i++) {
            if (point[i] < node->lobound[i]) {  
                distsum += distance->coordinate_distance(point[i], node->lobound[i], i);
                if (distsum > dist) return false;
            }
            else if (point[i] > node->upbound[i]) { 
                distsum += distance->coordinate_distance(point[i], node->upbound[i], i);
                if (distsum > dist) return false;
            }
        }
        return true;
    }


    bool KDTree::ball_within_bounds(const CoordPoint& point, double dist,
        kdtree_node* node) {
        size_t i;
        for (i = 0; i < dimension; i++)
            if (distance->coordinate_distance(point[i], node->lobound[i], i) <= dist ||
                distance->coordinate_distance(point[i], node->upbound[i], i) <= dist)
                return false;
        return true;
    }

} 