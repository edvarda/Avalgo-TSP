//
//  instance.cpp
//  Avalgo-tsp
//
//  Created by Edvard Ahlsén on 28/10/16.
//  Copyright © 2016 Edvard Ahlsén. All rights reserved.
//

#include "instance.hpp"
#include "algorithm"
#include <cmath>

namespace tsp {
    
    void instance::readCities(std::istream& in) {
        double x,y;
        for (size_t i = 0; i < size; ++i) {
            in >> x >> y;
            cities[i] = (point(x,y));
        }
    }
    
    struct distance_to {
        int d;
        int city;
        distance_to(int distance, int to_city) : d(distance), city(to_city){}
        distance_to() : d(-1), city(-1) {};
        bool operator<(distance_to other) const {
            return d < other.d;
        }
    };
    
    
    //Comparison function
    int distComp(const distance_to a, const distance_to b) { // TODO
        return (a.d > b.d);
    }
    
    void instance::computeDistances() {
        double dx, dy;
        int min_link_candidate = 10000000;
        std::vector<std::vector<distance_to>> helper = std::vector<std::vector<distance_to>>(size);
        for (size_t i = 0; i < size; i++) {
            distances[i] = (std::vector<int>(size,0));
            helper[i] = (std::vector<distance_to>(size));
            for (size_t j = 0; j < size; j++) {
                if (i == j) {
                    distances[i][j] = -1;
                    
                    helper[i][j] = distance_to(10000000, (int)j);
                } else {
                    dx = cities[i].x - cities[j].x;
                    dy = cities[i].y - cities[j].y;
                    distances[i][j] = round(sqrt(dx*dx+dy*dy));
                    
                    helper[i][j] = distance_to(distances[i][j], (int)j);
                }
            }
        }
        
        
            
        for (size_t i = 0; i < size; i++) {
            sort(helper[i].begin(), helper[i].end());
            if (helper[i][0].d < min_link_candidate)
                min_link_candidate = helper[i][0].d;
            nbhd[i] = (std::vector<int>(size,0));
            for (size_t j = 0; j < size; j++) {
                nbhd[i][j] = helper[i][j].city;
                
            }
        }
        
        min_link = min_link_candidate;
        
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
                if (distance < min && distance != -1) {
                    min = distance;
                    nearestStop = j;
                }
            }
            currentStop = nearestStop;
        }
    }
}
