//
//  kruskal.cpp
//  Avalgo-tsp
//
//  Created by Edvard Ahlsén on 29/10/16.
//  Copyright © 2016 Edvard Ahlsén. All rights reserved.
//

#include <algorithm>
#include "kruskal.hpp"
#include <unordered_map>
#include <queue>

namespace tsp {
    
    int find(std::vector<subset> &subsets, int u) {
        if (subsets[u].parent != u) {
            subsets[u].parent = find(subsets, subsets[u].parent);
        }
        return subsets[u].parent;
    }
    
    void doUnion(std::vector<subset> &subsets, int u, int v) { // TODO
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
    
    //Test w/ comparison function for determining which vertex is the left-uppermost.
    int pointComp(const point a, const point b) {
        return (a.x > b.x);
    }
    
    // Bygg upp mst genom att välja minsta edgen, kolla om den kan läggas till utan att skapa en cykel.
    std::vector<edge>* kruskal(instance &map) {
        std::vector<std::vector<int> > distances = map.distances;
        size_t V = distances.size();
        std::vector<edge> *mst = new std::vector<edge>();
        std::vector<subset> subsets = std::vector<subset>(V);
        
        
        std::vector<edge> edges(0); // Gör edges från dist
        for (int i = 0; i<V;i++) {
            subsets[i] = subset(i,0);
            for (int j = i+1 ; j < V; j++) {
                //edges[i*j+j] = edge(i,j,distances[i][j]);
                if (i != j) {
                    edges.push_back(edge(i,j,distances[i][j]));
                }
            }
        }
        sort(edges.rbegin(), edges.rend()); //Sortera edges
        
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
        return mst;
    }
    
    //returns weight of tour
    void makePreorderWalk(std::vector<int> &tour, std::vector<edge> *mst) {
        std::unordered_map<int, std::vector<edge>> edgeList;
        std::vector<bool> visited(tour.size(),false);
        std::queue<int> bfsQ = std::queue<int>();
        edgeList = std::unordered_map<int, std::vector<edge> >();
        
        for (int i = 0; i < mst->size(); i++) {
            edgeList[(*mst)[i].u].push_back((*mst)[i]);
            edgeList[(*mst)[i].v].push_back((*mst)[i]);
        }
            
        bfsQ.push(0);
        int stop = 0;
        while(!bfsQ.empty()) {
            visited[bfsQ.front()] = true;
            for(auto it = edgeList[bfsQ.front()].begin(); it != edgeList[bfsQ.front()].end(); it++) {
                if (bfsQ.front() == (*it).u  && !visited[(*it).v]) {
                    bfsQ.push((*it).v);
                } else if (bfsQ.front() == (*it).v && !visited[(*it).u]) {
                    bfsQ.push((*it).u);
                }
            }
            tour[stop] = bfsQ.front(); // lägg till hörnet i touren
            bfsQ.pop(); // ta bort hörnet ur kön
            stop++; // skifta till nästa stop-index i touren
        }
    }
    
}