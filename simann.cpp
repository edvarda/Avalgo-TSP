//
//  simann.cpp
//  Avalgo-tsp
//
//  Created by Edvard Ahlsén on 29/10/16.
//  Copyright © 2016 Edvard Ahlsén. All rights reserved.
//


#include "simann.hpp"

namespace tsp {

    void sa(std::vector<int> &tour, instance map,
                        const std::chrono::time_point<std::chrono::high_resolution_clock>& deadline) {
        int iterations;

        //Depending on how large the problem instance is we allow it to run for a maximum number of iterations.
        if(tour.size() > 800){
            iterations = 4500000;
        } else if(tour.size() > 400){
            iterations = 5000000;
        } else if(tour.size() > 200){
            iterations = 6000000;
        } else {
            iterations = 7000000;
        }

        double temperature = 6000; //This is the initial temperature
		double tempDecrPerTurn = temperature/iterations; //This is how much the initial temperature decreases per iteration.

		int city1;
		int city2;
        int beginning;
        int ending;
        int original;
        int candidate;
        int timeCheckCounter=1000; //Every thousand iterations. We should check if we are running out of the time designated for SA.
    	srand(time(NULL));//Necessary for rng.
		while(iterations > 1){
            timeCheckCounter--;
            if(timeCheckCounter == 0){
                timeCheckCounter=1000;
                if (std::chrono::high_resolution_clock::now() > deadline) {
                    return;
                }
            }
    		//1. Pick two new points and reverse them.
    		city1 = rand() % tour.size();
            city2 = rand() % tour.size();
    		
    		//2. Evaluate solution. If it is better change it.
    		//Else, still change it but with come probability.
            beginning = std::min(city1, city2);
            ending = std::max(city1, city2);
    		original = map.D[tour[beginning-1]][tour[beginning]] + map.D[tour[ending]][tour[ending+1]];
    		candidate = map.D[tour[beginning-1]][tour[ending]] + map.D[tour[beginning]][tour[ending+1]];


    		if(original > candidate){
    			reverse(tour, city1, city2);
    		} else {
    			//Reset the tour
    			double delta = original - candidate;
    			double exponent = delta/temperature;
    			double probability = exp(exponent);
    			double diceRoll = ((double) rand() / (RAND_MAX));
    			if(diceRoll<=probability){
    				reverse(tour, city1, city2);
    			}
    		}
    		//3. Decrease temperature.
    		iterations-=1;
    		temperature -= tempDecrPerTurn;
    	}


    }


    //Returns a reversed version of the tour between city1 and city2.
    //For example, if we current tour (1, 2, 3, 4, 5, 6, 7, 8),
    //and we choose cities 4 and 6,
    //the new tour is (1, 2, 3, 6, 5, 4, 7, 8)
    void reverse(std::vector<int> &tour, int city1, int city2) {
        	int tmp;
        	int beginning = std::min(city1, city2);
        	int ending = std::max(city1, city2);
        
        	for(int i = 0 ; i < ((ending-beginning)/2)+1 ; i++){
        		tmp = tour[beginning+i];
        		tour[beginning+i] = tour[ending-i];
        		tour[ending-i] = tmp;
        	}

        }

    //Returns the length of the two edges e={beginning, beginning-1} and e={ending, ending+1}, where
    //beginning is the first of the two cities city1 and city2 and ending the second.
    int evaluate(std::vector<int> &tour, tsp::instance &map , int &city1, int &city2) {
        int totalLength = 0;
        int beginning = std::min(city1, city2);
        int ending = std::max(city1, city2);
        totalLength = map.D[tour[beginning-1]][tour[beginning]] + map.D[tour[ending]][tour[ending+1]];
        return totalLength;
    }

}
