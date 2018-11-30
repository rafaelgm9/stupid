#include <iostream>

#include "stupid.h"
#include "multidim_array.h"
#include "integrator.h"
#include "leapfrog_integrator.h"
#include "cic_grid.h"
#include "tsc_grid.h"

#define NUM_DIMS 3

void hello() {
    std::cout << "Hello, World!" << std::endl;
}

void run_sim(stupid_args args) {
    array_2d<double> x(args.num_particles, NUM_DIMS);
    array_2d<double> p(args.num_particles, NUM_DIMS);

    for (int i = 0; i < args.num_particles * NUM_DIMS; ++i) {
        x.data[i] = args.x[i];
        p.data[i] = args.p[i];
    }

    particle_list plist(x, p);

    cosmology cosmo(args.Omega_m0, args.Omega_k0, args.Omega_l0);

    grid *g;

    switch (args.interp_scheme) {
        case 1:
            g = new cic_grid(args.num_cells, plist, cosmo);
        default:
            g = new tsc_grid(args.num_cells, plist, cosmo);
    }

    integrator *i;

    std::string file_prefix(args.file_prefix);
    switch (args.integrator) {
        default:
            i = new leapfrog_integrator(args.a0, args.af, args.delta_a, *g, file_prefix, args.write_nth_step);
    }

    i->run_sim();

    delete g;
    delete i;
}
