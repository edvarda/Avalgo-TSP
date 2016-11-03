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
    
    /**
     Implementation of Christofides algorithm as described in the report

     @param map : The problem instance
     @param tour : The current tour
     */
    void christofides(tsp::instance &map, std::vector<int> &tour) {
        std::vector<tsp::edge> *mst;
        mst = tsp::kruskal(map); // Get a minimum spanning tree of the graph using kruskals algorithm
        matchOdds(mst, map);
        getHamiltonTour(mst,map,tour);
        delete mst;
    }
 
    /**
     Constructs a set of all vertices with odd degree and does a greedy min-weight matching of them.
     The resulting edges are added to MST. (Technically making it into something other than a MST, but the name is kept for continuity)

     @param mst : The minimum spanning tree
     @param map : The problem instance
     */
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
    
    /**
     Performs Fleurys algorithm, and then computes a Hamiltonian cycle from the result

     @param mst : The minimum-spanning-tree augmentet with the odd-degree matching
     @param map : The problem instance
     @param tour : The tour-vector to fill
     */
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
    
    /**
     Helper function for fleurys algorithm. Count the number of nodes reachable from u

     @param edgeList : List of edges in the subgraph
     @param visited : List of vertices visited
     @param u : The node from which to start
     @return : Integer representing the number of nodes reachable from @param u
     */
    int dfsCount(std::vector<std::vector<int> > &edgeList, std::vector<bool> &visited, int u) {
        int c = 1;
        visited[u] = true;
        
        // Recursive call for all vertices adjacent to this vertex
        for (auto it = edgeList[u].begin(); it != edgeList[u].end(); ++it)
            if (*it != -1 && !visited[*it])
                c += dfsCount(edgeList, visited, *it);
        return c;
    }
    
    
    /**
     Helper function for Fleurys. Removes edge (u,v) from edgeList

     @param edgeList : List of edges in the subgraph
     @param u : first vertex
     @param v : second vertex
     */
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
    
    /**
     Helper function for Fleurys. Adds an edge (u,v) to the subgraph.

     @param edgeList : The edges in the subgraph
     @param u : first vertex
     @param v : second vertex
     */
    void addEdge(std::vector<std::vector<int> > &edgeList, int u, int v) {
        edgeList[v].push_back(u);
        edgeList[u].push_back(v);
    }
    
    /**
     Helper function for Fleurys. Determines wether an edge is a valid next edge for the Euler-tour

     @param edgeList : Edges in the subgraph
     @param u : First vertex of (u,v)
     @param v : Second vertex of (u,v)
     @return : Bool saying if edge (u,v) is a valid next edge
     */
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
    
    /**
     Flaurys algorithm for finding an Euler-tour in a graph. The graph is known to only have vertexes of even degree. The function uses recursive calls to search the graph for valid edges.

     @param edgeList : Edges in the graph
     @param eulerTour : The vector holding the tour under construction
     @param u : The next vertex being examined
     */
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

