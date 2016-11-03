//
//  k_opt.cpp
//  Avalgo-tsp
//
//  Created by Edvard Ahlsén on 01/11/16.
//  Copyright © 2016 Edvard Ahlsén. All rights reserved.
//

#include "k_opt.hpp"
#include <cmath>
#include <sys/time.h>
#include <algorithm>


namespace tsp {
    
    /**
     2-optimisation for a tour, as described in the report

     @param map : problem instance
     @param tour : current tour
     */
    void fast_two_opt(tsp::instance &map, std::vector<int> &tour) {
        int iter = 0;
        updateMaxLink(map,tour);
        updateWhichSlot(map, tour);
        
        // To hold edges (u,v) (w,z)
        int u,v,w,z;
        int currentLink, candidateMove;
        int c2;
        bool improved;
        improved = true;
        int tourLength;
        while (improved) {
            improved = false;
            tourLength = getWeight(map, tour);
            
            for (v = 0; v < map.size; ++v) {// Loop over tour
                ((v-1) < 0) ? u = (int) map.size-1 : u = v-1;
                
                for (int j2 = 0; j2 < map.size/10; ++j2) {
                    iter++;
                    c2 = map.nbhd[tour[v]][j2]; // store the j2:th closest city to u.
                    z = map.c[c2].i;
                    ((z-1) < 0) ? w = (int) map.size-1 : w = z-1;
                
                    if (v == w || z == u) {
                        continue;
                    }
                
                    if (map.D[tour[v]][tour[z]]+map.min_link >
                        map.D[tour[u]][tour[v]]+map.max_link) {
                        break; // out of j2 loop, to go next u
                    }
                    currentLink = map.D[tour[u]][tour[v]]+map.D[tour[w]][tour[z]];
                    candidateMove = map.D[tour[u]][tour[w]]+map.D[tour[v]][tour[z]];
                
                    if (currentLink > candidateMove) { // If the move is better, execute it!
                        swap_two(map,tour,v,w);
                        improved = true;
                        if (map.max_link < map.D[tour[u]][tour[w]]) {
                            map.max_link = map.D[tour[u]][tour[w]];
                        }
                        if (map.max_link < map.D[tour[v]][tour[z]]) {
                            map.max_link = map.D[tour[v]][tour[z]];
                        }
                        
                    }
                }
            }
        }
    }
    
    /**
     3-optimises the tour, as described in the report

     @param map : The problem instance
     @param tour : The current tour
     @param deadline : Deadline for returning a result
     */
    void fast_three_opt(tsp::instance &map, std::vector<int> &tour,
                        const std::chrono::time_point<std::chrono::high_resolution_clock>& deadline) {
        size_t N = map.size;
        int iter = 0;
        int impro = 0;
        int tourLength;
        updateMaxLink(map,tour);
        updateWhichSlot(map, tour);
        
        // To hold the three edges (u,v) (w,z) (x,y)
        int u,v,w,z,x,y;
        // To hold the ordered edges (k,l) (m,n) (o,p)
        int k,l,m,n,o,p;
        
        int currentLink, candidateMove;
        int move_1, move_2, move_3, move_4;
        int c2,c3;
        bool improved = true;
        
        while (improved) {
            improved = false;
            tourLength = getWeight(map, tour);
            for (v = 0; v < N; ++v) {// Loop over tour
                ((v-1) < 0) ? u = (int) N-1 : u = v-1;
                
                if (std::chrono::high_resolution_clock::now() > deadline) {
                    return;
                }
                
                for (int j2 = 0; j2 < N/4; ++j2) {
                    iter++;
                    c2 = map.nbhd[tour[u]][j2]; // store the j2:th closest city to u.
                    
                    z = map.c[c2].i;
                    ((z-1) < 0) ? w = (int) N-1 : w = z-1;
                    
                    if (map.D[tour[u]][tour[z]]+2*map.min_link >
                        map.D[tour[u]][tour[v]]+2*map.max_link) {
                        break; // out of j2 loop, to go next u
                    }
                    if (map.D[tour[u]][tour[z]]+2*map.min_link >
                        map.D[tour[u]][tour[v]]+map.D[tour[w]][tour[z]]+map.max_link) {
                        continue;
                    }
                    for (int j3 = 0; j3 < N/4; ++j3) {
                        c3 = map.nbhd[tour[v]][j3];
                        y = map.c[c3].i;
                        ((y-1) < 0) ? x = (int) N-1 : x = y-1;
                        
                        if (u == w || u == x || w == x) // If two edges are the same
                            continue;
                        if (u == y || u == z) // follows
                            continue;
                        if (v == w || v == x) // precedes
                            continue;
                        
                        if (map.D[tour[u]][tour[z]]+map.D[tour[v]][tour[y]]+map.min_link >
                            map.D[tour[u]][tour[v]]+map.D[tour[w]][tour[z]]+map.max_link) {
                            break; // out of j2 loop, to go next u
                        }
                        // Now we know that there's a point to swapping some edges
                        
                        // Set {(k,l) (m,n) (o,p)} to {sorted((u,v), (w,z), (x,y))}
                        tourOrderEdges(map,tour,u,v,w,z,x,y,k,l,m,n,o,p);
                        
                        currentLink = map.D[tour[k]][tour[l]]
                                        +map.D[tour[m]][tour[n]]
                                        +map.D[tour[o]][tour[p]];
                        // Lets examine our candidate moves. We'll try just the one for starters
                        candidateMove = map.D[tour[o]][tour[k]]
                                        +map.D[tour[p]][tour[m]]
                                        +map.D[tour[l]][tour[n]];
                        
                        // This move does swap(p,k) -> swap (l,m)
                        move_1 = map.D[tour[o]][tour[k]]
                                +map.D[tour[p]][tour[m]]
                                +map.D[tour[l]][tour[n]];
                        
                        // This move does swap(l,m) -> swap (n,o)
                        move_2 = map.D[tour[k]][tour[m]]
                                +map.D[tour[l]][tour[o]]
                                +map.D[tour[n]][tour[p]];
                        
                        // This move does swap(n,o) -> swap (p,k)
                        move_3 = map.D[tour[m]][tour[o]]
                                +map.D[tour[n]][tour[k]]
                                +map.D[tour[p]][tour[l]];
                        
                        // This move does swap(n,o) -> swap (p,k) -> swap(l,m)
                        move_4 = map.D[tour[n]][tour[k]]
                                +map.D[tour[p]][tour[m]]
                                +map.D[tour[l]][tour[o]];
                        
                        if (currentLink > move_1) {
                            impro++;
                            swap_two(map, tour, p, k);
                            swap_two(map, tour, l, m);
                            updateMaxLink(map, tour);
                            improved = true;
                        } else if (currentLink > move_2) {
                            impro++;
                            swap_two(map, tour, l, m);
                            swap_two(map, tour, n, o);
                            updateMaxLink(map, tour);
                            improved = true;
                        } else if (currentLink > move_3) {
                            impro++;
                            swap_two(map, tour, n, o);
                            swap_two(map, tour, p, k);
                            updateMaxLink(map, tour);
                            improved = true;
                        } else if (currentLink > move_4) {
                            impro++;
                            swap_two(map, tour, n, o);
                            swap_two(map, tour, p, k);
                            swap_two(map, tour, l, m);
                            updateMaxLink(map, tour);
                            improved = true;
                        }
                    }
                }
            }
        }
    }
    
    /**
     Reverses a portion of the tour

     @param map : The problem instance
     @param tour : The current tour
     @param begin : Element to start reversing at (inclusive)
     @param end Element to end reversing at (inclusive)
     */
    void swap_two(tsp::instance &map, std::vector<int> &tour, int begin, int end) {
        int swapOperations;
        if (begin <= end) {
            swapOperations = ((end - begin)+1)/2;
        } else {
            swapOperations = (int)(((end + tour.size()) - begin)+1)/2;
        }
        int i = begin;
        int j = end;
        
        for (int n = 0; n < swapOperations; ++n) {
            std::swap(tour[i], tour[j]);
            map.c[tour[i]].i = i;
            map.c[tour[j]].i = j;
            i = (i + 1) % tour.size();
            j = (int)((j + tour.size()) - 1) % tour.size();
        }
    }
    
    /**
     Updates the value for the max-weight edge in the current tour

     @param map : The problem instance
     @param tour : The current tour
     */
    void updateMaxLink(tsp::instance &map, std::vector<int> &tour) {
        int t;
        size_t N = map.size;
        int max_found = 0; // candidate for max_link
        for (t = 0; t < N; ++t) {
            int tn = ((t+1) % N);
            if (map.D[tour[t]][tour[tn]] > max_found) {
                max_found = map.D[tour[t]][tour[tn]];
            }
        }
        map.max_link = max_found;
    }
    
    /**
     Updates the datastructure for keeping track of the citites current positions in the tour

     @param map : The problem instance
     @param tour : The current tour
     */
    void updateWhichSlot(tsp::instance &map, std::vector<int> &tour) {
        for (int t = 0; t < map.size; ++t) {
            map.c[tour[t]].i = t;
        }
    }
    
    void tourOrderEdges(tsp::instance &map, std::vector<int> &tour,
                        int& u, int& v, int& w, int& z, int& x, int& y,
                        int& k, int& l, int& m, int& n, int& o, int& p) {
        // Let (x,y) == (o,p)
        o = x;
        p = y;
        
        if ((u < w && w < x) ||
            (w < x && x < u) ||
            (x < u && u < w)) {
            k = u;
            l = v;
            m = w;
            n = z;
        } else {
            k = w;
            l = z;
            m = u;
            n = v;
        }
    }
}
