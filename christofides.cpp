//
//  christofides.cpp
//  Avalgo-tsp
//
//  Created by Edvard Ahlsén on 31/10/16.
//  Copyright © 2016 Edvard Ahlsén. All rights reserved.
//

#include "christofides.hpp"
#include <unordered_map>
#include <stack>
#include <cassert>

namespace tsp {
    
    int testEulerWalk(std::vector<std::vector<bool> > &edges, int size) {
        std::vector< std::vector<bool> >::iterator row;
        std::vector<bool>::iterator col;
        std::vector<bool> v_exists = std::vector<bool>(size, false);
        for (row = edges.begin(); row != edges.end(); ++row) {
            for (col = row->begin(); col != row->end(); ++col) {
                if (*col == true)
                    v_exists[row - edges.begin()] = true;
            }
        }
        
        for (auto it = v_exists.begin(); it != v_exists.end(); ++it) {
            if (*it == false)
                return -1;
        }
        return 1;
    }
    
    int testEulerTour(std::vector<int> &etour,int size) {
        std::vector<int>::iterator it;
        std::vector<bool> v_exists = std::vector<bool>(size, false);
        for (it = etour.begin(); it != etour.end(); ++it) {
            v_exists[*it] = true;
        }
        
        for (auto it = v_exists.begin(); it != v_exists.end(); ++it) {
            if (*it == false)
                return -1;
        }
        return 1;
    }


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
            if (edgeList[i].size() % 2  != 0) { // Om hörn i har ett udda antal grannar i MST
                oddList.push_back(i); // Spara den i oddList
            }
        }
        int oddDegs = (int) oddList.size();
        
        // en greedy matchning av alla odd vertices?
        int current, min;
        std::vector<int>::iterator closest;
        while(!oddList.empty()) {
            min = 100000000;
            current = oddList.back();
            oddList.pop_back();
            for (auto it = oddList.begin(); it != oddList.end(); it++) {
                if (map.distances[current][*it] < min) {
                    closest = it;
                    min = map.distances[current][*it];
                }
            }
            // Now, the variablename mst becomes misleading to say the least, beacuse we're addings cycles.
            mst->push_back(edge(current, *closest, map.distances[current][*closest]));
            oddList.erase(closest);
        }
        // Nu innehåller mst kanterna som motsvarar en perfekt-ish matchning av alla hörn med udda grad
        assert(mst->size() == map.size-1+(oddDegs/2));
        return;
    }
    
    void getHamiltonTour(std::vector<tsp::edge> *mst, tsp::instance &map, std::vector<int> &tour) {
        int current;
        std::stack<int> eulerStack = std::stack<int>();
        std::vector<int> eulerTour = std::vector<int>();
        std::vector<std::vector<bool> > edges = std::vector<std::vector<bool> >(map.size);
        
        for (auto it = edges.begin(); it != edges.end(); ++it) {(*it) = std::vector<bool>(map.size,false);}
        int foundEdges = 0;
        for (int i = 0; i < mst->size(); i++) { // Gör om edgelisten. Ja, det är ineffektivt.
            edges[(*mst)[i].u][(*mst)[i].v] = true;
            edges[(*mst)[i].v][(*mst)[i].u] = true;
            foundEdges++;
        }
        assert(foundEdges == mst->size());
        current = (*mst)[0].u; //start at some vertex in graph
        int neighbour;
        
        if (testEulerWalk(edges, map.size) == -1) {
            assert(false);
        } // Edges täcker alla hörn
        
        // Nu ska vi bygga en euler-vandring
        do {
            if (getNeighbour(edges, current) != -1) {
                eulerStack.push(current); // we push our node on a stack.
                neighbour = getNeighbour(edges, current); // choose a neighbour
                assert(neighbour >= 0);
                edges[current][neighbour] = false; // erase the edge between them.
                edges[neighbour][current] = false; // erase the edge between them.
                current = neighbour; // and set it as the current node
            } else {
                assert(eulerStack.size() > 0);
                eulerTour.push_back(current);
                current = eulerStack.top();
                eulerStack.pop();
            }
            
        } while (!eulerStack.empty());
        
        
        if (testEulerTour(eulerTour, map.size) == -1) {
            assert(false);
        }
        
        
        std::vector< std::vector<bool> >::iterator row;
        std::vector<bool>::iterator col;
        for (row = edges.begin(); row != edges.end(); ++row) {
            for (col = row->begin(); col != row->end(); ++col) {
                assert(*col == false); // alla edges är borttagna
            }
        }
        
        int numbervisited = 0;
        //assert(eulerTour.size() >= map.size);
        // Sen ska vi plocka ut en hamiltoncykel
        
        tour.clear();
        assert(tour.size() == 0);
        std::vector<bool> visited = std::vector<bool>(map.size, false);
        for (auto it = eulerTour.begin(); it != eulerTour.end(); it++) {
            if (!visited[*it]) {
                visited[*it] = true;
                numbervisited++;
                tour.push_back(*it);
            }
        }
        //assert(numbervisited == map.size);
        assert(tour.size() <= map.size);
    }
    
    int getNeighbour(std::vector<std::vector<bool>> &edges, int v) {
        for (auto it = edges[v].begin(); it != edges[v].end(); ++it) {
                if (*it == true) {
                    assert(it - edges[v].begin() >= 0);
                    return (int) (it - edges[v].begin()); // Return the idx of a neighbour
                }
        }
        return -1; // No neighbours for v left
    }
    
}

