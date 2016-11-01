//
//  kruskal.hpp
//  Avalgo-tsp
//
//  Created by Edvard Ahlsén on 29/10/16.
//  Copyright © 2016 Edvard Ahlsén. All rights reserved.
//

#ifndef kruskal_hpp
#define kruskal_hpp

#include <stdio.h>
#include <vector>
#include <algorithm>
#include "instance.hpp"


namespace tsp {
    struct edge {
        int u,v,w;
        edge(int u_, int v_, int w_) : u(u_), v(v_), w(w_){};
        edge() : u(-1), v(-1), w(-1){};
        bool operator<(edge other) const {
            return w > other.w;
        }
    };

    struct subset {
        int parent,rank;
        subset(int parent_, int rank_) : parent(parent_), rank(rank_) {}
        subset() : parent(-1), rank(-1) {}
    };

    std::vector<edge>* kruskal(tsp::instance &map);
    void makePreorderWalk(std::vector<int> &tour, std::vector<edge> *mst);
    int find(std::vector<subset> &subsets, int u);
    void doUnion(std::vector<subset> &subsets, int u, int v);
    int edgeCompare(const void* a, const void* b);
}

#endif /* kruskal_hpp */