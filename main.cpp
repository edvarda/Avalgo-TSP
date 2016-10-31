//
//  main.cpp
//  Avalgo-tsp
//
//  Created by Edvard Ahlsén on 05/10/16.
//  Copyright © 2016 Edvard Ahlsén. All rights reserved.
//

#include "instance.hpp"
#include "kruskal.hpp"
#include "simann.hpp"
#include "christofides.hpp"

#include <iostream>
#include <fstream>
#include <sys/time.h>

void printTour(std::vector<int> &tour) { // TODO just for testing early
    for (int i = 0; i < tour.size(); i++) {
        std::cout << tour[i] << std::endl;
    }
}

void printTourWeight(std::vector<int> &tour, tsp::instance map) {
    size_t weight = 0;
    for (int i = 0; i < tour.size()-1; i++) {
        weight += map.distances[tour[i]][tour[i+1]];
    }
    std::cout << "weight of tour: " << weight << std::endl;
}

int getCurrTime() {
    struct timeval tp;
    gettimeofday(&tp, NULL);
    return (int) (tp.tv_sec * 1000 + tp.tv_usec / 1000);
}



// to compile in terminal: g++ -g -O2 -static -std=gnu++11 *.cpp -o theprogram. Fuck READMEs

int main() {

    long int startTime = getCurrTime();
    std::ifstream in("test.in");
    std::cin.rdbuf(in.rdbuf());
    
    size_t n;
    std::cin >> n; // Läs in storlek på problemet
    tsp::instance map(n); // Skapar instans-objekt
    std::vector<int> tour;
    //std::vector<tsp::edge> *mst; // testar kruskal
    tour = std::vector<int>(n);
    //long int startTime = getCurrTime();
    //sa(tour, map, startTime);
    map.readCities(std::cin);
    map.computeDistances();
    map.nneighbour(tour);
    
    sa(tour, map, startTime); //Run SA



    std::cout << "nneighbour:" << std::endl;
    printTour(tour);
    //std::cout << "nneighbour:" << std::endl;
    //printTour(tour);
    //printTourWeight(tour, map);
    
    //std::cout << "mst-walk:" << std::endl;
    //mst = tsp::kruskal(map);
    //tsp::makePreorderWalk(tour, mst);
    //printTour(tour);
    //printTourWeight(tour, map);
    
    //std::cout << "christofides:" << std::endl;
    std::vector<int> christTour = std::vector<int>(map.size);
    tsp::christofides(map,christTour);
    printTour(christTour);
    printTourWeight(christTour, map);
}