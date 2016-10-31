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

    	long int currTime = getCurrTime();//Fetches current time.
		int temperature = 14000;
		int city1;
		int city2;

    	srand(time(NULL));//Necessary for rng.


		for (int i = 0; i < tour.size(); i++) {
			std::cout << "Before: " << tour[i] << std::endl; //Just a print
		}
    	//while(currTime - startTime < 1950){
    		//1. Pick two new points and reverse them.


    		city1 = rand() % tour.size();
    		city2 = rand() % tour.size();
    		std::cout << "city1: " << city1 << std::endl; //Just a print
    		std::cout << "city2: " << city2 << std::endl; //Just a print
    		//tour.reverse(city1, city2);
    		reverse(tour, city1, city2);

    		//2. Jump to point with some probability
    		//3. Decrease temperature.
    		currTime = getCurrTime();
    	//}

        for (int i = 0; i < tour.size(); i++) {
			std::cout << "After: " << tour[i]  << std::endl; //Just a print
		}

    }

    int getCurrTime() {
    	struct timeval tp;
    	gettimeofday(&tp, NULL);
    	return (tp.tv_sec * 1000 + tp.tv_usec / 1000);
    }



    //Reverses the tour between city1 and city2.
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

    	std::cout << "turns: " << ((ending-beginning)/2)+1  << std::endl; //Just a print
    	for(int i = 0 ; i < ((ending-beginning)/2)+1 ; i++){
    		tmp1 = tour[beginning+i];
    		tmp2 = tour[ending-i];
    		tour[beginning+i] = tmp2;
    		tour[ending-i] = tmp1;
    	}
    }


}
