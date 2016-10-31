//
//  point.hpp
//  Avalgo-tsp
//
//  Created by Edvard Ahlsén on 28/10/16.
//  Copyright © 2016 Edvard Ahlsén. All rights reserved.
//

#ifndef point_hpp
#define point_hpp

#include <stdio.h>



namespace tsp {

    struct point {
        double x,y;
        point(double x = 0, double y = 0): x(x), y(y) {};
        
    };
}

#endif /* point_hpp */