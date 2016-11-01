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
#include <cassert>
#include <algorithm>


namespace tsp {
    
    int getCurrTimeK() {
        struct timeval tp;
        gettimeofday(&tp, NULL);
        return (int) (tp.tv_sec * 1000 + tp.tv_usec / 1000);
    }

    int getWeight(tsp::instance &map, std::vector<int> &tour) {
        int w = 0;
        int j = 0;
        for (int i = 0; i < tour.size(); i++) {
            j = i+1 % tour.size();
            w += map.distances[tour[i]][tour[j]];
        }
        return w;
    }

    void printTour(std::vector<int> &tour) { // TODO just for testing early
        for (int i = 0; i < tour.size(); i++) {
            std::cout << tour[i] << std::endl;
        }
    }
    
    void updateMinLink(tsp::instance &map, std::vector<int> &tour) {
        int t;
        int min_found = 10000000; // candidate for min_link
        for (t = 0; t < map.size; ++t) {
            int tn = (t+1 % map.size);
            if (map.distances[tour[t]][tour[tn]] < min_found) {
                min_found = map.distances[tour[t]][tour[tn]];
            }
        }
        map.min_link = min_found;
    }
    
    void updateWhichSlot(tsp::instance &map, std::vector<int> &tour) {
        for (int t = 0; t < map.size; ++t) {
            map.cities[tour[t]].which_slot = t;
        }
    }
    
    void updateWhichSlot(tsp::instance &map, std::vector<int> &tour, int from, int to) {
        for (int t = from; t <= to; ++t) {
            map.cities[tour[t]].which_slot = t;
        }
    }

    std::vector<int> swap_two(std::vector<int> &tour, int i, int j) {
        std::vector<int> newTour = std::vector<int>(tour.size());
        int s = 0;
        for (auto it = tour.begin(); it < tour.begin()+i; it ++) {
            newTour[s] = *it;
            s++;
        }
        for (auto it = tour.begin()+j; it >= tour.begin()+i; it --) {
            newTour[s] = *it;
            s++;
        }
        for (auto it = tour.begin()+j+1; it <= tour.end(); it ++) {
            newTour[s] = *it;
            s++;
        }
        s--;
        return newTour;
    }
    
    void reverse_swap(std::vector<int> &tour, int i, int j) {
        std::reverse(tour.begin()+i,tour.begin()+j+1);
    }
    
    void reverse_swap2(std::vector<int> &tour, int v1, int u2) {
        if (v1 > u2) {
            v1 -= 1;
            u2 += 1;
            std::reverse(tour.begin()+u2,tour.begin()+v1+1);
        } else {
            u2 = (u2 + 1) % tour.size();
            std::reverse(tour.begin()+v1,tour.begin()+u2);
        }
    }


    void two_opt(tsp::instance &map, std::vector<int> &tour) {
        int iter = 0;
        int impro = 0;
        size_t size = map.size;
        int shortest, candidate, original, before, after;
        bool improved = true;
        long int startTime = getCurrTimeK();
        
        while (improved) {
            improved = false;
            //shortest = getWeight(map,tour);
            for ( int i = 0; i < size - 1; i++ ) {
                for ( int k = i + 1; k < size; k++) {
                    iter++;
                    before = (i-1);
                    if (before < 0) {before += size;}
                    after = (k+1);
                    if (after >= size) {after -= size;}
                    
                    if (i != k && before != after && before != k && after != i) {
                        original = map.distances[tour[before]][tour[i]]+map.distances[tour[k]][tour[after]];
                        candidate = map.distances[tour[before]][tour[k]]+map.distances[tour[i]][tour[after]];
                        assert(candidate > 0);
                        if ( candidate < original ) {
                            reverse_swap(tour, i, k);
                            improved = true;
                            impro += (original - candidate);
                        }
                    }  
                }
            }
        }
        std::cerr << "old-opt-iter: " << iter << std::endl;
        std::cerr << "old-opt-impro: " << impro << std::endl;
    }
    
    void fast_two_opt(tsp::instance &map, std::vector<int> &tour) {
        int iter = 0;
        int impro = 0;
        int impro2 = 0;
        updateMinLink(map,tour);
        updateWhichSlot(map, tour);
        
        int u1,u2,v1,v2;
        int currentLink, candidateMove;
        int c2;
        bool improved;
        long int startTime = getCurrTimeK();
        improved = true;
        int tourLength;
        while (improved) {
            improved = false;
            tourLength = getWeight(map, tour);
            for (v1 = 0; v1 < map.size; ++v1) {// Loop over tour
                u1 = (v1 - 1 + map.size) % map.size;
                for (int j2 = 0; j2 < map.size; ++j2) {
                        iter++;
                        c2 = map.nbhd[tour[v1]][j2]; // store the j2:th closest city to u.
                        v2 = map.cities[c2].which_slot;
                        u2 = (v2 - 1 + map.size) % map.size;
                        if (map.distances[tour[v1]][tour[v2]]+map.min_link >
                            map.distances[tour[u1]][tour[v1]]+map.max_link) {
                            break; // out of j2 loop, to go next u
                        }
                        currentLink = map.distances[tour[u1]][tour[v1]]+map.distances[tour[u2]][tour[v2]];
                        candidateMove = map.distances[tour[u1]][tour[u2]]+map.distances[tour[v1]][tour[v2]];
                    
                        if (currentLink > candidateMove) { // If the move is better, execute it!
                            assert((currentLink - candidateMove) > 0);
                            if (map.min_link > map.distances[tour[u1]][tour[u2]])
                                map.min_link = map.distances[tour[u1]][tour[u2]];
                            if (map.min_link > map.distances[tour[v1]][tour[v2]])
                                map.min_link = map.distances[tour[v1]][tour[v2]];
                            int current = getWeight(map, tour); // DEBUG
                            reverse_swap2(tour,v1,u2);
                            int newW = getWeight(map, tour); // DEBUG
                            updateWhichSlot(map,tour);
                            improved = true;
                            impro += (currentLink-candidateMove);
                            impro2 += (current-newW);
                            if ((currentLink-candidateMove) != (current-newW)) {
                                std::cerr << "old: " <<  currentLink << std::endl;
                                std::cerr << "new: " <<  candidateMove << std::endl;
                                std::cerr << "currentW: " <<  current << std::endl;
                                std::cerr << "newW: " <<  newW << std::endl;
                                std::cerr << "1: " << u1 << " " << v1 << " 2: " << u2 << " " << v2 << std::endl;
                                std::cerr << "----------------" << std::endl;
                            }
                        }
                }
            }
        }
        std::cerr << "new-opt-iter: " << iter << std::endl;
        std::cerr << "new-opt-impro: " << impro << std::endl;
        std::cerr << "new-opt-impro2: " << impro2 << std::endl;
    }
}