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
#include "tests.hpp"
#include "k_opt.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <sys/time.h>
#include <cassert>


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

void printTourWeightToFile(std::vector<int> &tour, tsp::instance map, std::string fileName) {
    size_t weight = 0;
    for (int i = 0; i < tour.size()-1; i++) {
        weight += map.distances[tour[i]][tour[i+1]];
    }
    std::ofstream myfile;
	myfile.open ("file.txt");
	myfile << "weight of tour: " << weight;
	myfile.close();
}



int getCurrTime() {
    struct timeval tp;
    gettimeofday(&tp, NULL);
    return (int) (tp.tv_sec * 1000 + tp.tv_usec / 1000);
}



// to compile in terminal: g++ -g -O2 -static -std=gnu++11 *.cpp -o theprogram. Fuck READMEs

int main() {
    long int startTime = getCurrTime();
    const bool debug = false;
    const bool fileIn = false;
    std::ifstream in;
    if (fileIn) {
        in = std::ifstream("test1000.in");
        std::cin.rdbuf(in.rdbuf());
    }
    
    
    size_t n;
    std::cin >> n; // Läs in storlek på problemet
    tsp::instance map(n); // Skapar instans-objekt
    std::vector<int> tour;
    tour = std::vector<int>(n);
    
    /*
    long int startTime = getCurrTime();
    sa(tour, map, startTime);
    */
    
    map.readCities(std::cin);
    map.computeDistances();
    if (debug) {
        if (!validateEdges(map)) {
            std::cerr << "incorrect distances" << std::endl;
            exit(1);
        };
    }

    /*
    map.nneighbour(tour);
    std::cout << "nneighbour:" << std::endl;
    printTour(tour);
    printTourWeight(tour, map);
    */
    
    /*
    std::cout << "mst-walk:" << std::endl;
    std::vector<tsp::edge> *mst;
    mst = tsp::kruskal(map);
    tsp::makePreorderWalk(tour, mst);
    printTourWeight(tour, map);
    */
    if (map.size == 1) {
        std::cout << "0" << std::endl;
        exit(0);
    }
    
    
    if (debug) {std::cout << "christofides:" << std::endl;}
    tsp::christofides(map,tour);
    //tsp::validateTour(tour, map);
    if (tour.size() != map.size) {
        tour = std::vector<int>(n);
        map.nneighbour(tour);
        tsp::two_opt(map,tour);
        printTour(tour);
        exit(0);
    }
    
    if (debug) {printTourWeight(tour, map);}
    
    tsp::two_opt(map,tour);
    printTour(tour);
    if (debug) {printTourWeight(tour, map);}
    if (debug) {
        
    }
}