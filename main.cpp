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
#include "twoopt.hpp"
#include "christofides.hpp"
#include "tests.hpp"
#include "k_opt.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <sys/time.h>
#include <cassert>
#include <chrono>


void printTour(std::vector<int> &tour) { // TODO just for testing early
    for (int i = 0; i < tour.size(); i++) {
        std::cout << tour[i] << std::endl;
    }
}

void printTourWeight(std::vector<int> &tour, tsp::instance map) {
//    size_t weight = 0;
//    for (int i = 0; i < tour.size()-1; i++) {
//        weight += map.D[tour[i]][tour[i+1]];
//    }
    std::cout << "weight of tour: " << getWeight(map,tour) << std::endl;
}

void printTourWeightToFile(std::vector<int> &tour, tsp::instance map, std::string fileName) {
    size_t weight = 0;
    for (int i = 0; i < tour.size()-1; i++) {
        weight += map.D[tour[i]][tour[i+1]];
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

int main() {
    std::chrono::time_point<std::chrono::high_resolution_clock> deadline =
        std::chrono::high_resolution_clock::now() + std::chrono::milliseconds(1900);
    std::chrono::time_point<std::chrono::high_resolution_clock> start_time =
    std::chrono::high_resolution_clock::now();
    
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
    
    // Construct instance
    map.readCities(std::cin);
    map.computeDistances();
    
    if (debug) {
        if (!validateEdges(map)) {
            std::cerr << "incorrect distances" << std::endl;
            exit(1);
        };
    }
    
    // Some trivial cases
    if (map.size == 1) {
        std::cout << "0" << std::endl;
        exit(0);
    } else if (map.size == 2) {
        std::cout << "0" << std::endl << "1" << std::endl;
        exit(0);
    } else if (map.size == 3) {
        std::cout << "0" << std::endl << "1" << std::endl << "2" << std::endl;
        exit(0);
    }
    
    // Algorithms
    
    if (debug) {std::cout << "christofides:" << std::endl;}
    tsp::christofides(map,tour);
    
    if (tour.size() != map.size) {
        std::cerr << "INCORRECT CHRISTO RESULT" << std::endl;
    //if (false) {
        tour = std::vector<int>(n);
        map.nneighbour(tour);
        tsp::fast_two_opt(map,tour);
        tsp::fast_three_opt(map,tour, deadline);
        printTour(tour);
        exit(0);
    }
    
    
    if (debug) {
        std::cerr << "using fast-2-opt" << std::endl;
    }
    
    tsp::fast_two_opt(map,tour);
    
    if (debug) {
        tsp::validateTour(tour, map);
        printTourWeight(tour, map);
    }
    
    if (debug) {
        std::cerr << "using fast-3-opt" << std::endl;
    }
    
    tsp::fast_three_opt(map,tour, deadline);
    
    if (debug) {
        tsp::validateTour(tour, map);
        printTourWeight(tour, map);
        std::chrono::milliseconds totalTime =
        std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start_time);
        std::cerr << "Total time: " << totalTime.count() << "ms" << std::endl;
    }
    
    
    if (!debug) {
                printTour(tour);
    }
    
    
}