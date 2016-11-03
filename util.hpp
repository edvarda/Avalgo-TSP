//
//  util.cpp
//  Avalgo-tsp
//
//  Created by Edvard Ahlsén on 03/11/16.
//  Copyright © 2016 Edvard Ahlsén. All rights reserved.
//
#ifndef util_h
#define util_h


#include <stdio.h>
#include "instance.hpp"
namespace tsp {
    
    // Print a tour so that Kattis can understand it
    void printTour(std::vector<int> &tour) {
        for (int i = 0; i < tour.size(); i++) {
            std::cout << tour[i] << std::endl;
        }
    }
    
    // Print the total weight of a tour
    void printTourWeight(std::vector<int> &tour, tsp::instance map) {
        std::cout << "weight of tour: " << getWeight(map,tour) << std::endl;
    }
    
    // Get the weight of a tour
    int getWeight(tsp::instance &map, std::vector<int> &tour) {
        int w = 0;
        int j = 0;
        for (int i = 0; i < tour.size(); i++) {
            j = (i+1) % tour.size();
            w += map.D[tour[i]][tour[j]];
        }
        return w;
    }
}

#endif /* util_h */
