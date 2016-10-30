//
//  kruskal.cpp
//  Avalgo-tsp
//
//  Created by Edvard Ahlsén on 29/10/16.
//  Copyright © 2016 Edvard Ahlsén. All rights reserved.
//

#include <algorithm>
#include "kruskal.hpp"
namespace tsp {
    
    int find(std::vector<subset> subsets, int u) {
        if (subsets[u].parent != u)
            subsets[u].parent = find(subsets, subsets[u].parent);
        return subsets[u].parent;
    }
    
    void doUnion(std::vector<subset> subsets, int u, int v) { // TODO
        int root_u = find(subsets, u);
        int root_v = find(subsets, v);
        
        // Attatch the smaller of the trees under the larger
        if (subsets[root_u].rank < subsets[root_v].rank) {
            subsets[root_u].parent = root_v;
        } else if (subsets[root_u].rank > subsets[root_v].rank) {
            subsets[root_v].parent = root_u;
        } else { // If they are the same rank, just increase one's rank and use that
            subsets[root_v].parent = root_u;
            subsets[root_u].rank++;
        }
    }
    
    //Comparison function to pass to qsort for sorting edges accoring to weight
    int edgeComp(const edge a, const edge b) { // TODO
        return (a.w > b.w);
    }
    
    // Bygg upp mst genom att välja minsta edgen, kolla om den kan läggas till utan att skapa en cykel.
    void kruskal(std::vector<std::vector<int>> distances) {
        size_t V = distances.size();
        std::vector<edge> *mst = new std::vector<edge>();
        std::vector<subset> subsets = std::vector<subset>(V);
        
        
        std::vector<edge> edges = std::vector<edge>(V*V); // GÖRA EDGES från dist
        for (int i = 0; i<V;i++) {
            subsets[i] = subset(i,0);
            for (int j = 0 ; j < V; j++) {
                edges[i*j+j] = edge(i,j,distances[i][j]);
            }
        }
        sort(edges.begin(), edges.end()); //Sortera edges
        
        int i = 0;
        while (mst->size() < V-1) { // While the MST is smaller than a MST needs to be.
            edge next = edges[i];
            int u = find(subsets,next.u);
            int v = find(subsets,next.v);
            if (u != v) { // TODO write oprator= for edge
                mst->push_back(next);
                doUnion(subsets, u ,v);
            }
            i++;
        }
    }
    
}
