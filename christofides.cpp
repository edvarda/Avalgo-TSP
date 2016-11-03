//
//  christofides.cpp
//  Avalgo-tsp
//
//  Created by Edvard Ahlsén on 31/10/16.
//  Copyright © 2016 Edvard Ahlsén. All rights reserved.
//

#include "christofides.hpp"
#include <unordered_map>
#include <cassert>

namespace tsp {
    
    void christofides(tsp::instance &map, std::vector<int> &tour) {
        std::vector<tsp::edge> *mst;
        mst = tsp::kruskal(map); // Get a minimum spanning tree of the graph using kruskals algorithm
        matchOdds(mst, map);
        getHamiltonTour(mst,map,tour);
        delete mst;
    }
 
    void matchOdds(std::vector<tsp::edge> *mst, tsp::instance &map) {
        std::unordered_map<int, std::vector<edge> > edgeList =
            std::unordered_map<int, std::vector<edge> >();
        std::vector<int> oddList = std::vector<int>();
        
        assert(mst->size() == map.size-1);
        for (int i = 0; i < mst->size(); i++) {
            edgeList[(*mst)[i].u].push_back((*mst)[i]);
            edgeList[(*mst)[i].v].push_back((*mst)[i]);
        }
        
        for (int i = 0; i < map.size; i++) {
            if ((edgeList[i].size() % 2)  != 0) { // If d(v)
                oddList.push_back(i); // Spara den i oddList
            }
        }
        
        // Greedy matching on the set of all vertices with odd degree
        int current, min;
        std::vector<int>::iterator closest;
        while(!oddList.empty()) {
            min = 100000000;
            current = oddList.back();
            oddList.pop_back();
            for (auto it = oddList.begin(); it != oddList.end(); it++) {
                if (map.D[current][*it] < min) {
                    closest = it;
                    min = map.D[current][*it];
                }
            }
            // Now, the variablename mst becomes misleading to say the least,
            // beacuse we're addings cycles.
            mst->push_back(edge(current, *closest, map.D[current][*closest]));
            oddList.erase(closest);
        }
        return;
    }
    
    void getHamiltonTour(std::vector<tsp::edge> *mst, tsp::instance &map, std::vector<int> &tour) {
        int current;
        std::vector<int> eulerTour = std::vector<int>();
        std::vector<std::vector<int> > edgeList = std::vector<std::vector<int> >(map.size);
        
        for (int i = 0; i < mst->size(); ++i) { // Gör om edgelisten. Ja, det är ineffektivt.
            edgeList[(*mst)[i].u].push_back((*mst)[i].v);
            edgeList[(*mst)[i].v].push_back((*mst)[i].u);
        }
        
        current = (*mst)[0].u; //start at some vertex in graph
        
        // Use fleurys algorithm to find an Euler-tour.
        fleury(edgeList, eulerTour,0);
        
        // Walk through the Euler-tour and take a shortcut
        // everytime we encounter a previously visited vertex.
        tour.clear();
        std::vector<bool> visited = std::vector<bool>(map.size, false);
        for (auto it = eulerTour.begin(); it != eulerTour.end(); it++) {
            if (!visited[*it]) {
                visited[*it] = true;
                tour.push_back(*it);
            }
        }
    }
    
    int dfsCount(std::vector<std::vector<int> > &edgeList, std::vector<bool> &visited, int u) {
        int c = 1;
        visited[u] = true;
        
        // Recursive call for all vertices adjacent to this vertex
        for (auto it = edgeList[u].begin(); it != edgeList[u].end(); ++it)
            if (*it != -1 && !visited[*it])
                c += dfsCount(edgeList, visited, *it);
        return c;
    }
    
    // Sets an edge between u and v to -1 in the edgeList
    void removeEdge(std::vector<std::vector<int> > &edgeList, int u, int v) {
        std::vector<int>::iterator it_u;
        for (it_u = edgeList[v].begin(); it_u != edgeList[v].end(); ++it_u) {
            if (*it_u == u) {
                (*it_u) = -1;
                break;
            }
        }
        std::vector<int>::iterator it_v;
        for (it_v = edgeList[u].begin(); it_v != edgeList[u].end(); ++it_v) {
            if (*it_v == v) {
                (*it_v) = -1;
                break;
            }
        }
    }
    
    void addEdge(std::vector<std::vector<int> > &edgeList, int u, int v) {
        edgeList[v].push_back(u);
        edgeList[u].push_back(v);
    }
    
    bool isValidNext(std::vector<std::vector<int> > &edgeList,int u, int v) {
        // v can be a valid next edge in two ways:
        
        // 1: v is a valid next edge if it is the only neighbour of u.
        int nbrs = 0;
        for (auto it = edgeList[u].begin(); it != edgeList[u].end(); ++it) {
            if (*it != -1) {
                nbrs++;
            }
        }
        if (nbrs == 1) {
            return true;
        }
        
        // 2: v is a valid next edge if it is not a "bridge". i.e. doesn't decrease the number of reachable vertices if followed
        std::vector<bool> visited = std::vector<bool>(edgeList.size(), false);
        int prior_count = dfsCount(edgeList,visited, u);
        
        // 2.b) Remove edge (u, v) and after removing the edge, count
        // vertices reachable from u
        removeEdge(edgeList, u, v);
        visited = std::vector<bool>(edgeList.size(), false); // New visited-list
        int post_count = dfsCount(edgeList,visited, u);
        
        // 2.c) Add the edge back to the graph
        addEdge(edgeList, u, v);
        
        // 2.d) If count1 is greater, then edge (u, v) is a bridge
        return (prior_count > post_count) ? false: true;
        
    }
    
    void fleury(std::vector<std::vector<int> > &edgeList, std::vector<int> &eulerTour, int u) {
        // Start at vertex 0
        eulerTour.push_back(u);
        // Go through the edges of u
        std::vector<int>::iterator it;
        for (int i = 0; i < edgeList[u].size(); ++i) {
            int v = edgeList[u][i];
            // If edge u-v is not removed and it's a a valid next edge
            if (v != -1 && isValidNext(edgeList, u, v)) {
                removeEdge(edgeList, u, v);
                fleury(edgeList,eulerTour, v);
            }
        }
    }
}

