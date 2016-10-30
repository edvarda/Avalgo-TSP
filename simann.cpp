//
//  simann.cpp
//  Avalgo-tsp
//
//  Created by Edvard Ahlsén on 29/10/16.
//  Copyright © 2016 Edvard Ahlsén. All rights reserved.
//


#include "simann.hpp"

namespace tsp {

    void sa(std::vector<int> tour, instance map, long startTime) {
    	//Fetches current time.
		long int currTime = getCurrTime();

    	while(currTime - startTime < 1950){
    		//1. New point
    		//2. Jump to point with some probability
    		//3. Decrease temperature.
    		currTime = getCurrTime();
    	}



    }

    int getCurrTime() {
    	struct timeval tp;
    	gettimeofday(&tp, NULL);
    	return (tp.tv_sec * 1000 + tp.tv_usec / 1000);
    }



}
