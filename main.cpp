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
#include "util.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <sys/time.h>
#include <chrono>


int main() {
    
    // Initialization
    //-----------------------------------------------------------------------
    
    // Create a deadline and record a starttime
    std::chrono::time_point<std::chrono::high_resolution_clock> deadline =
        std::chrono::high_resolution_clock::now() + std::chrono::milliseconds(1900);
    std::chrono::time_point<std::chrono::high_resolution_clock> deadlineSA =
        std::chrono::high_resolution_clock::now() + std::chrono::milliseconds(1000);
    std::chrono::time_point<std::chrono::high_resolution_clock> start_time =
    std::chrono::high_resolution_clock::now();
    
    const bool debug = true; // DEBUG FLAG
    const bool fileIn = true; // REDIRECT FILE TO STDIN
    
    std::ifstream in;
    if (fileIn) {
        in = std::ifstream("test1000.in");
        std::cin.rdbuf(in.rdbuf());
    }
    
    size_t n;
    std::cin >> n; // Read instance size
    tsp::instance map(n); // Create instance object
    std::vector<int> tour; // Create and initialize tour vector
    tour = std::vector<int>(n);
    
    // Construct instance
    map.readCities(std::cin);
    map.computeDistances();
    
    // Some trivial cases
    //-----------------------------------------------------------------------
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
    
    // Approximation
    //-----------------------------------------------------------------------
    //map.nneighbour(tour);
    if (debug) {std::cout << "running christofides:" << std::endl;}
    //tsp::christofides(map,tour);
    
    if (debug) {std::cerr << "running fast-2-opt" << std::endl;}
    //tsp::fast_two_opt(map,tour);
    
    if (debug) {
        //tsp::validateTour(tour, map);
        //printTourWeight(tour, map);
    }
    
    if (debug) {std::cerr << "running fast-3-opt" << std::endl;}
    //tsp::fast_three_opt(map,tour, deadline);
    
    // Print result & debug information
    //-----------------------------------------------------------------------
    
    if (debug) {
        tsp::validateTour(tour, map);
        printTourWeight(tour, map);
        std::chrono::milliseconds totalTime =
        std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start_time);
        std::cerr << "Total time: " << totalTime.count() << "ms" << std::endl;
    }

    tsp::christofides(map,tour);
    //sa(tour, map, deadlineSA);
    //tsp::fast_two_opt(map,tour);
    tsp::fast_three_opt(map, tour, deadline);
    if (!debug) {tsp::printTour(tour);}

}
