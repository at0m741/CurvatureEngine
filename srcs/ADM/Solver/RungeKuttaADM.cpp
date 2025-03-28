#include <Geodesics.h>

void Grid::logger_evolve(Grid &grid_obj, double dt, int nstep)
{    
	Cell2D &cell = grid_obj.getCell(0, 0, 0);
	printf("=============================================\n");
	printf("Time step: %f, nstep: %d\n", dt, nstep);
	printf("=============================================\n");
	printf("alpha: %e\n", cell.gauge.alpha);
	printf("beta: %e %e %e\n", cell.gauge.beta[0], cell.gauge.beta[1], cell.gauge.beta[2]);
	printf("Hamiltonian: %e\n", cell.matter.hamiltonian);
	printf("Momentum: %e %e %e\n", cell.matter.momentum[0], cell.matter.momentum[1], cell.matter.momentum[2]);
	printf("dtAtilde:\n");
	for (int i = 0; i < 3; i++)
		printf("	  %e %e %e\n", cell.atilde.dt_Atilde[i][0], \
								   cell.atilde.dt_Atilde[i][1], 
								   cell.atilde.dt_Atilde[i][2]);
	/* printf("T_uv:\n"); */
	/* for (int i = 0; i < 4; i++) */
	/* 	printf("	  %e %e %e %e\n", cell.matter.T[i][0], \ */
	/* 							   cell.matter.T[i][1],  */
	/* 							   cell.matter.T[i][2], */
	/* 							   cell.matter.T[i][3]); */
	printf("Chi: %e\n", cell.dt_chi);
	compute_ADM_mass();
	printf("=============================================\n");
	printf("\n\n");
	
}

void Grid::export_1D_tilde_gamma_xx(int j_fixed, int k_fixed, double time) {
	std::ofstream out("Output/tilde_gamma_xx_t=" + std::to_string(time) + ".csv");

	for (int i = 0; i < NX; ++i) {
		double x = i * DX;
		double gxx = globalGrid[i][j_fixed][k_fixed].geom.tilde_gamma[0][0];
		out << x << "," << gxx << "\n";
	}

	out.close();
}




void Grid::evolve(Grid &grid_obj, double dtInitial, int nSteps) {
    initialize_grid();
    GridTensor gridTensor;
    double CFL = 0.5;
    double dt = dtInitial;
    double hamiltonian;
    double momentum[3];

    for (int step = 0; step < nSteps; step++) {
        dt = computeCFL_dt(CFL);
        apply_boundary_conditions(grid_obj);

#pragma omp parallel
        {
            auto forEachCell = [&](auto func) {
#pragma omp for collapse(3) schedule(runtime)
                for (int i = 1; i < NX - 1; i++) {
                    for (int j = 1; j < NY - 1; j++) {
                        for (int k = 1; k < NZ - 1; k++) {
                            func(i, j, k);
                        }
                    }
                }
            };

            forEachCell([&](int i, int j, int k) {
                copyInitialState(globalGrid[i][j][k]);
            });

            forEachCell([&](int i, int j, int k) {
                compute_time_derivatives(grid_obj, i, j, k);
                /* update_fluid_velocity(i, j, k, dt); */
                /* compute_fluid_derivatives(i, j, k); */
                /* update_energy_momentum_tensor(i, j, k); */
                /* compute_energy_momentum_evolution(i, j, k, dt); */

                double d_alpha_dt, d_beta_dt[3];
                compute_gauge_derivatives(grid_obj, i, j, k, d_alpha_dt, d_beta_dt);
                compute_constraints(grid_obj, i, j, k, hamiltonian, momentum);
                storeStage(globalGrid[i][j][k], 0, d_alpha_dt, d_beta_dt);
            });

            forEachCell([&](int i, int j, int k) {
                updateIntermediateState(globalGrid[i][j][k], 0.5 * dt, 0);
            });

            forEachCell([&](int i, int j, int k) {
                compute_time_derivatives(grid_obj, i, j, k);
                /* update_fluid_velocity(i, j, k, dt); */
                /* compute_fluid_derivatives(i, j, k); */
                /* update_energy_momentum_tensor(i, j, k); */
                /* compute_energy_momentum_evolution(i, j, k, dt); */

                double d_alpha_dt, d_beta_dt[3];
                compute_gauge_derivatives(grid_obj, i, j, k, d_alpha_dt, d_beta_dt);
                storeStage(globalGrid[i][j][k], 1, d_alpha_dt, d_beta_dt);
            });

            forEachCell([&](int i, int j, int k) {
                updateIntermediateState(globalGrid[i][j][k], 0.5 * dt, 1);
            });

            forEachCell([&](int i, int j, int k) {
                compute_time_derivatives(grid_obj, i, j, k);
                /* update_fluid_velocity(i, j, k, dt); */
                /* compute_fluid_derivatives(i, j, k); */
                /* update_energy_momentum_tensor(i, j, k); */
                /* compute_energy_momentum_evolution(i, j, k, dt); */

                double d_alpha_dt, d_beta_dt[3];
                compute_gauge_derivatives(grid_obj, i, j, k, d_alpha_dt, d_beta_dt);
                compute_constraints(grid_obj, i, j, k, hamiltonian, momentum);
                storeStage(globalGrid[i][j][k], 2, d_alpha_dt, d_beta_dt);
            });

            forEachCell([&](int i, int j, int k) {
                updateIntermediateState(globalGrid[i][j][k], dt, 2);
            });

            forEachCell([&](int i, int j, int k) {
                compute_time_derivatives(grid_obj, i, j, k);
                /* update_fluid_velocity(i, j, k, dt); */
                /* compute_fluid_derivatives(i, j, k); */
                /* update_energy_momentum_tensor(i, j, k); */
                /* compute_energy_momentum_evolution(i, j, k, dt); */

                double d_alpha_dt, d_beta_dt[3];
                compute_gauge_derivatives(grid_obj, i, j, k, d_alpha_dt, d_beta_dt);
                compute_constraints(grid_obj, i, j, k, hamiltonian, momentum);
                storeStage(globalGrid[i][j][k], 3, d_alpha_dt, d_beta_dt);
            });

            forEachCell([&](int i, int j, int k) {
                combineStages(globalGrid[i][j][k], dt);
            });
        }

#pragma omp single nowait
        {
            logger_evolve(grid_obj, dt, step);
            if (step == nSteps - 1) {
                printf("Exporting slices\n");
                export_K_slice(grid_obj, NX / 2);
                export_gauge_slice(grid_obj, NY / 2);
                gridTensor.export_christoffel_slice(grid_obj, NY / 2);
                export_alpha_slice(grid_obj, NY / 2);
                export_K_3D(grid_obj);
            }
        }

        grid_obj.time += dt;
    }
}
