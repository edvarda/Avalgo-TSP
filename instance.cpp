//
//  instance.cpp
//  Avalgo-tsp
//
//  Created by Edvard Ahlsén on 28/10/16.
//  Copyright © 2016 Edvard Ahlsén. All rights reserved.
//

#include "instance.hpp"
#include <cmath>

namespace tsp {
    
    void instance::readCities(std::istream& in) {
        double x,y;
        for (size_t i = 0; i < size; ++i) {
            in >> x >> y;
            cities[i] = (point(x,y));
        }
    }
    
    void instance::computeDistances() {
        double dx, dy;
        for (size_t i = 0; i < size; i++) {
            distances[i] = (std::vector<int>(size,0));
            for (size_t j = 0; j < size; j++) {
                if (i == j) {
                    distances[i][j] = 0;
                } else {
                    dx = cities[i].x - cities[j].x;
                    dy = cities[i].y - cities[j].y;
                    distances[i][j] = round(sqrt(dx*dx+dy*dy));
                }
            }
        }
    }
    
    void instance::nneighbour(std::vector<int> &tour) {
        int min, currentStop, nearestStop, distance;
        std::vector<bool> visited(size,false);
        currentStop = 0;
        
        
        for (int i = 0 ; i < size ; i++) {
            min = 10000000; // 10^7 should be strictly larger than the distance between any 2 points.
            visited[currentStop] = true;
            tour[i] = (currentStop);
            nearestStop = -1;
            for (int j = 0 ; j < size ; j++) {
                if (visited[j]) {continue;}
                distance = distances[i][j];
                if (distance < min && distance != 0) {
                    min = distance;
                    nearestStop = j;
                }
            }
            currentStop = nearestStop;
        }
    }
}
