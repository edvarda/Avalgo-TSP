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
		int temperature = 10000/tour.size();
		int konstant = 1;
		int iterations = 60000;
		int temperatureLowerCounterMax = iterations/temperature;
		int temperatureLowerCounter = temperatureLowerCounterMax;
		int city1;
		int city2;

    	srand(time(NULL));//Necessary for rng.


    	//while(currTime - startTime < 1950){
		while(iterations > 1){
			size_t weight = 0;
			for (int i = 0; i < tour.size()-1; i++) {
				weight += map.distances[tour[i]][tour[i+1]];
			}
			//std::cout << "weight of tour: " << weight << std::endl;
    		//1. Pick two new points and reverse them.
    		city1 = rand() % tour.size();

    		int cityDifference = (rand() % 75) + 1;
    		if(city1+cityDifference < tour.size()){
    			city2 = city1+cityDifference;
    		} else if (city1-cityDifference >= 0) {
    			city2 = city1-cityDifference;
    		} else if (city1-1 >=0){
    			city2 = city1-1;
    		} else {
    			city2 = city1+1;
    		}

    		//city2 = rand() % tour.size();
    		std::vector<int> alternativeTour = reverse(tour, city1, city2);

    		//2. Evaluate solution. If it is better change it.
    		//Else, still change it but with come probability.
    		int tourLength = evaluate(tour, map);
    		int alternativeTourLength = evaluate(alternativeTour, map);


    		if(tourLength > alternativeTourLength){
    			//std::cout << "better-------------------------------" << std::endl;
    			tour = alternativeTour;
    			//std::cout << "updated" << std::endl;
    		} else {
    			//std::cout << "worse--------------------------------" << std::endl;
    			double delta = tourLength - alternativeTourLength;
    			double t = temperature/konstant;
    			double exponent = delta/t;
    			double probability = exp(exponent);
    			double diceRoll = ((double) rand() / (RAND_MAX));

    			//std::cout << "delta: " << delta << std::endl;
    			//std::cout << "temperature: " << temperature << std::endl;
    			//std::cout << "exponent: " << exponent << std::endl;
    			//std::cout << "diceRoll: " << diceRoll << "  probability:" << probability << std::endl;
    			if(diceRoll<=probability){
    				//std::cout << "updated" << std::endl;
    				tour = alternativeTour;
    			}
    		}
    		//3. Decrease temperature.

    		iterations-=1;
    		if(temperatureLowerCounter <= 0){
    			temperature -= 1;
    			temperatureLowerCounter = temperatureLowerCounterMax;
    		} else {
    			temperatureLowerCounter -= 1;
    		}
    		//currTime = getCurrTime();



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
    std::vector<int> reverse(std::vector<int> tour, int city1, int city2) {
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


    	return tour;
    }

    int evaluate(std::vector<int> &tour, tsp::instance &map) {
        int totalLength = 0;
        for (int i = 1; i < tour.size(); i++) {
        	totalLength += map.distances[tour[i-1]][tour[i]];
        }
        return totalLength;
    }

}
