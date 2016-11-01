//
//  simann.cpp
//  Avalgo-tsp
//
//  Created by Edvard Ahlsén on 29/10/16.
//  Copyright © 2016 Edvard Ahlsén. All rights reserved.
//


#include "twoopt.hpp"

namespace tsp {

    void twoopter(std::vector<int> &tour, instance map, long startTime) {
    	 /*
    	 repeat until no improvement is made {
    	        start_again:
    	        best_distance = calculateTotalDistance(existing_route)
    	        for (i = 0; i < number of nodes eligible to be swapped - 1; i++) {
    	            for (k = i + 1; k < number of nodes eligible to be swapped; k++) {
    	                new_route = 2optSwap(existing_route, i, k)
    	                new_distance = calculateTotalDistance(new_route)
    	                if (new_distance < best_distance) {
    	                    existing_route = new_route
    	                    goto start_again
    	                }
    	            }
    	        }
    	    }
		*/
		int city1 = 0;
		int city2 = tour.size()-1; //Is this correct?
		int k2;
		int lowestTotalWeightSoFar;
		int newLowestTotalWeight;
		std::vector<int> newTour;
		for(int i = 0 ; i < tour.size() ; i++){
			for(int k = 1 ; k < 100 ; k++){
				k2 = (i+k);
				newTour = twooptswap(tour, i, k2);
				lowestTotalWeightSoFar = evaluateTwoOpt(tour, map, i, k2);
				newLowestTotalWeight = evaluateTwoOpt(newTour, map, i, k2);
				if(newLowestTotalWeight < lowestTotalWeightSoFar){
					//std::cout << "found better tour by : " << (newLowestTotalWeight-lowestTotalWeightSoFar) << std::endl;
					tour = newTour;

				}
				else{
					//std::cout << "NOT BETTER!" << std::endl;
				}
			}
		}
		/*
		size_t weight = 0;
		for (int i = 0; i < tour.size()-1; i++) {
		 weight += map.distances[tour[i]][tour[i+1]];
		}
		std::cout << "weight of tour: " << weight << std::endl;
		*/
    }

    std::vector<int> twooptswap(std::vector<int> &tour, int &i, int &k){
    	/*
    	1. take route[1] to route[i-1] and add them in order to new_route
		2. take route[i] to route[k] and add them in reverse order to new_route
		3. take route[k+1] to end and add them in order to new_route
		return new_route;
		*/
    	std::vector<int> newTour = std::vector<int>(tour.size());
    	if(k>=tour.size()){
    		return tour;
    	}
    	//TODO: Make certain of correctness.
    	for(int index = 0 ; index < i ; index++){
    		newTour[index] = tour[index];
    	}
    	int counter = 0;
    	for(int index = i ; index <= k ; index++){
			newTour[index] = tour[k-counter];
			counter++;
		}
    	for(int index = k+1 ; index < tour.size() ; index++){
			newTour[index] = tour[index];
		}
    	return newTour;
    }


    int evaluateTwoOpt(std::vector<int> &tour, tsp::instance &map , int &city1, int &city2) {
        int totalLength = 0;
        int beginning = std::min(city1, city2);
        int ending = std::max(city1, city2);
        if(beginning-2 >= 1 && ending+2 < tour.size()){
			for (int i = beginning-2; i < ending+2; i++) {
				totalLength += map.distances[tour[i-1]][tour[i]];
			}
        }
        else{
        	for (int i = 1; i < tour.size(); i++) {
				totalLength += map.distances[tour[i-1]][tour[i]];
			}
        }
        return totalLength;
    }

}
