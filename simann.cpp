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
			iterations = 1300000;
		} else if(tour.size() > 400){
			iterations = 1800000;
		} else if(tour.size() > 200){
			iterations = 2500000;
		} else {
			iterations = 2500000;
		}

		double temperature = 160;
		double tempDecrPerTurn = temperature/iterations;

		int city1;
		int city2;
		int cityDifference;
    	srand(time(NULL));//Necessary for rng.


    	//while(currTime - startTime < 1950){
		while(iterations > 1){


    		//1. Pick two new points and reverse them.
    		city1 = rand() % tour.size();

    		cityDifference = (rand() % 75) + 1;
    		if(city1+cityDifference < tour.size()){
    			city2 = city1+cityDifference;
    		} else if (city1-cityDifference >= 0) {
    			city2 = city1-cityDifference;
    		} else if (city1-1 >=0){
    			city2 = city1-1;
    		} else {
    			city2 = city1+1;
    		}


    		//reverse(tour, city1, city2);

    		//2. Evaluate solution. If it is better change it.
    		//Else, still change it but with come probability.
    		int tourLength = evaluate(tour, map, city1, city2);
    		reverse(tour, city1, city2);
    		int alternativeTourLength = evaluate(tour, map, city1, city2);


    		if(tourLength > alternativeTourLength){
    			//Simply do nothing. The tour is already set
    		} else {
    			//Reset the tour
    			double delta = tourLength - alternativeTourLength;
    			double exponent = delta/temperature;
    			double probability = exp(exponent);
    			double diceRoll = ((double) rand() / (RAND_MAX));
    			//std::cout << "delta: " << delta << std::endl;
    			//std::cout << "temperature: " << temperature << std::endl;
    			//std::cout << "exponent: " << exponent << std::endl;
    			//std::cout << "diceRoll: " << diceRoll << "  probability:" << probability << std::endl;
    			if(diceRoll<=probability){
    				//Simply do nothing. The tour is already set
    			}
    			else{
    				reverse(tour, city1, city2);
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

    int evaluate(std::vector<int> &tour, tsp::instance &map , int &city1, int &city2) {
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
