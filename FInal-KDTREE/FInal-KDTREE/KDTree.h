#pragma once
#ifndef __kdtree_HPP
#define __kdtree_HPP

#include <cstdlib>
#include <queue>
#include <vector>

namespace KDTree {

    typedef std::vector<double> CoordPoint;
    typedef std::vector<double> DoubleVector;

    // for passing points to the constructor of KDTree
    struct NodeKDTree {
        CoordPoint point;
        void* data;
        NodeKDTree(const CoordPoint& p, void* d = NULL) {
            point = p;
            data = d;
        }
        NodeKDTree() { data = NULL; }
    };

    typedef std::vector<NodeKDTree> KdNodeVector;

    struct KdNodePredicate {
        virtual ~KdNodePredicate() {}
        virtual bool operator()(const NodeKDTree&) const { return true; }
    };

    class kdtree_node;
    class DistanceMeasure;
    class nn4heap {
    public:
        size_t dataindex;  
        double distance;   
        nn4heap(size_t i, double d) {
            dataindex = i;
            distance = d;
        }
    };
    class compare_nn4heap {
    public:
        bool operator()(const nn4heap& n, const nn4heap& m) {
            return (n.distance < m.distance);
        }
    };

    class KDTree {
    private:

        kdtree_node* build_tree(size_t depth, size_t a, size_t b);
        CoordPoint lobound, upbound;
        std::priority_queue<nn4heap, std::vector<nn4heap>, compare_nn4heap>* neighborheap;
        std::vector<size_t> range_result;
        int distance_type;
        bool neighbor_search(const CoordPoint& point, kdtree_node* node, size_t k);
        void range_search(const CoordPoint& point, kdtree_node* node, double r);
        bool bounds_overlap_ball(const CoordPoint& point, double dist, kdtree_node* node);
        bool ball_within_bounds(const CoordPoint& point, double dist, kdtree_node* node);
        DistanceMeasure* distance;
        KdNodePredicate* searchpredicate;

    public:
        KdNodeVector allnodes;
        size_t dimension;
        kdtree_node* root;
        KDTree(const KdNodeVector* nodes, int distance_type = 2);
        ~KDTree();
        void set_distance(int distance_type, const DoubleVector* weights = NULL);
        void k_nearest_neighbors(const CoordPoint& point, size_t k, KdNodeVector* result, KdNodePredicate* pred = NULL);
        void range_nearest_neighbors(const CoordPoint& point, double r, KdNodeVector* result);
    };

}
#endif