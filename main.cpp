//
//  main.cpp
//  Avalgo-tsp
//
//  Created by Edvard Ahlsén on 05/10/16.
//  Copyright © 2016 Edvard Ahlsén. All rights reserved.
//

#include "instance.hpp"

#include <iostream>
#include <fstream>

void printTour(std::vector<int> &tour) { // TODO just for testing early
    for (int i = 0; i < tour.size(); i++) {
        std::cout << tour[i] << std::endl;
    }
}

int main() {
    //std::ifstream in("test1000.in");
    //std::cin.rdbuf(in.rdbuf());
    
    size_t n;
    std::cin >> n; // Läs in storlek på problemet
    tsp::instance map(n); // Skapar instans-objekt
    std::vector<int> tour;
    tour = std::vector<int>(n);
    
    map.readCities(std::cin);
    map.computeDistances();
    map.nneighbour(tour);
    printTour(tour);

}