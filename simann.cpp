//
//  simann.cpp
//  Avalgo-tsp
//
//  Created by Edvard Ahlsén on 29/10/16.
//  Copyright © 2016 Edvard Ahlsén. All rights reserved.
//


#include "simann.hpp"

namespace tsp {

    void sa(std::vector<int> &tour, instance map, long startTime) {

    	//long int currTime = getCurrTime();//Fetches current time.

		int konstant = 1;
        int iterations;
        if(tour.size() > 800){
            iterations = 4500000;
        } else if(tour.size() > 400){
            iterations = 5000000;
        } else if(tour.size() > 200){
            iterations = 6000000;
        } else {
            iterations = 7000000;
        }

        double temperature = 6000;
		double tempDecrPerTurn = temperature/iterations;

		int city1;
		int city2;
		int cityDifference;
        int beginning = std::min(city1, city2);
        int ending;
        int original;
        int candidate;
    	srand(time(NULL));//Necessary for rng.


    	//while(currTime - startTime < 1950){
		while(iterations > 1){

            /*
            size_t weight = 0;
            for (int i = 0; i < tour.size()-1; i++) {
                weight += map.D[tour[i]][tour[i+1]];
            }
            std::cout << "weight of tour: " << weight << std::endl;
            */
    		//1. Pick two new points and reverse them.
    		city1 = rand() % tour.size();
            city2 = rand() % tour.size();
    		


    		//reverse(tour, city1, city2);

    		//2. Evaluate solution. If it is better change it.
    		//Else, still change it but with come probability.
            beginning = std::min(city1, city2);
            ending = std::max(city1, city2);
    		original = map.distances[tour[beginning-1]][tour[beginning]] + map.distances[tour[ending]][tour[ending+1]];
    		candidate = map.distances[tour[beginning-1]][tour[ending]] + map.distances[tour[beginning]][tour[ending+1]];


    		if(original > candidate){
    			reverseImproved(tour, city1, city2);
    		} else {
    			//Reset the tour
    			double delta = original - candidate;
    			double exponent = delta/temperature;
    			double probability = exp(exponent);
    			double diceRoll = ((double) rand() / (RAND_MAX));
    			//std::cout << "delta: " << delta << std::endl;
    			//std::cout << "temperature: " << temperature << std::endl;
    			//std::cout << "exponent: " << exponent << std::endl;
    			//std::cout << "diceRoll: " << diceRoll << "  probability:" << probability << std::endl;
    			if(diceRoll<=probability){
    				reverseImproved(tour, city1, city2);
    			}
    		}
    		//3. Decrease temperature.

    		iterations-=1;
    		temperature -= tempDecrPerTurn;
    	}


    }

    int getCurrTime() {
    	struct timeval tp;
    	gettimeofday(&tp, NULL);
    	return (int) (tp.tv_sec * 1000 + tp.tv_usec / 1000);
    }



    //Returns a reversed version of the tour between city1 and city2.
    //For example, if we current tour (1, 2, 3, 4, 5, 6, 7, 8),
    //and we choose cities 4 and 6,
    //the new tour is (1, 2, 3, 6, 5, 4, 7, 8)
    //TODO: check validity for odd number of elements, prolly wrong.
    //TODO: Speed up perhaps
    void reverse(std::vector<int> &tour, int city1, int city2) {
    	int tmp1;
    	int tmp2;
    	int beginning = std::min(city1, city2);
    	int ending = std::max(city1, city2);
    	for(int i = 0 ; i < ((ending-beginning)/2)+1 ; i++){
    		tmp1 = tour[beginning+i];
    		tmp2 = tour[ending-i];
    		tour[beginning+i] = tmp2;
    		tour[ending-i] = tmp1;
    	}

    }
    void reverseImproved(std::vector<int> &tour, int city1, int city2) {
        	int tmp;
        	int beginning = std::min(city1, city2);
        	int ending = std::max(city1, city2);
        
        	for(int i = 0 ; i < ((ending-beginning)/2)+1 ; i++){
        		tmp = tour[beginning+i];
        		tour[beginning+i] = tour[ending-i];
        		tour[ending-i] = tmp;
        	}

        }

    int evaluate(std::vector<int> &tour, tsp::instance &map , int &city1, int &city2) {
        int totalLength = 0;
        int beginning = std::min(city1, city2);
        int ending = std::max(city1, city2);

        totalLength = map.D[tour[beginning-1]][tour[beginning]] + map.D[tour[ending]][tour[ending+1]];

       
        return totalLength;
    }

}
