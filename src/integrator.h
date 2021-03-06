//
// Created by pierfied on 11/24/18.
//

#ifndef STUPID_INTEGRATOR_H
#define STUPID_INTEGRATOR_H


#include "grid.h"
#include <string>

class integrator {
    long write_num = 0;

    const std::string file_prefix;

public:
    grid *g;

    integrator(grid &g, std::string file_prefix) : g(&g) , file_prefix(file_prefix){}

    void write_positions(double a);

    void write_momentum(double a);

    void write_pos_and_mom(double a);

    virtual void run_sim() = 0;
};


#endif //STUPID_INTEGRATOR_H
