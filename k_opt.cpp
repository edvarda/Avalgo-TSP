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
        for (int i = 0; i < tour.size()-1; i++) {
            w += map.distances[tour[i]][tour[i+1]];
        }
        w += map.distances[tour[tour.size()-1]][tour[0]];
        return w;
    }

    void printTour(std::vector<int> &tour) { // TODO just for testing early
        for (int i = 0; i < tour.size(); i++) {
            std::cout << tour[i] << std::endl;
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

    void two_opt(tsp::instance &map, std::vector<int> &tour) {
        
        size_t size = map.size;
        int shortest, candidate, original, before, after;
        bool improved = true;
        long int startTime = getCurrTimeK();
        
        while (improved) {
            improved = false;
            //shortest = getWeight(map,tour);
            for ( int i = 0; i < size - 1; i++ ) {
                for ( int k = i + 1; k < size; k++) {
                    before = (i-1);
                    if (before < 0) {before += size;}
                    after = (k+1);
                    if (after >= size) {after -= size;}
                    
                    if (i != k && before != after && before != k && after != i) {
                        original = map.distances[tour[before]][tour[i]]+map.distances[tour[k]][tour[after]];
                        candidate = map.distances[tour[before]][tour[k]]+map.distances[tour[i]][tour[after]];
                        assert(candidate > 0);
                        if ( candidate < original ) {
                            //tour = swap_two(tour, tour[i], tour[k]);
                            //int beforeSwap = getWeight(map, tour);
                            reverse_swap(tour, i, k);
                            //int afterSwap = getWeight(map, tour);
                            //std::cerr << beforeSwap << " " << afterSwap << std::endl;
                            improved = true;
                            //tour.swap(new_tour);
                            //shortest = getWeight(map,tour);
                            //assert(beforeSwap > afterSwap);
                        }
                    }  
                }
            }
            if (getCurrTimeK() > startTime+1500) { break; }
        }
    }
}