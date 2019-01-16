//
// Created by sagy on 1/6/19.
//

#ifndef SERVER_CACHEMANAGER_H
#define SERVER_CACHEMANAGER_H

#include <string>

template<class Problem, class Solution>
class CacheManager {
    virtual bool is_solution_exists(Problem prob) = 0;
    virtual Solution getSolution(Problem prob) = 0;
    virtual void saveMapToFile() = 0;
};


#endif //SERVER_CACHEMANAGER_H
