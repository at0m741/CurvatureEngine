#include <Geodesics.h>

void Grid::allocateGlobalGrid() {
    printf("Allocating optimized grid\n");

    globalGrid.resize(NX);
    #pragma omp parallel for
    for (int i = 0; i < NX; i++) {
        globalGrid[i].resize(NY);
        for (int j = 0; j < NY; j++) {
            globalGrid[i][j].assign(NZ, Cell2D());
        }
    }

    const size_t total_cells = NX * NY * NZ;
    for (int a = 0; a < 3; a++) {
        for (int b = 0; b < 3; b++) {
            dgammaX[a][b].clear();
            dgammaX[a][b].reserve(total_cells);  
            dgammaX[a][b].resize(total_cells, 0.0);

            dgammaY[a][b].clear();
            dgammaY[a][b].resize(total_cells, 0.0);
            
            dgammaZ[a][b].clear();
            dgammaZ[a][b].resize(total_cells, 0.0);
        }
    }
}


/* void Grid::initializeData() { */
/*     float L = 4.0;  */
/*     float x_min = -L, x_max = L; */
/*     float y_min = -L, y_max = L; */
/*     float z_min = -L, z_max = L; */
/*     float dx = (x_max - x_min) / (NX - 1); */
/*     float dy = (y_max - y_min) / (NY - 1); */
/*     float dz = (z_max - z_min) / (NZ - 1); */
/*     Matrix matrix; */
/*  */
/*     globalGrid.resize(NX); */
/*     for (int i = 0; i < NX; i++) { */
/*         globalGrid[i].resize(NY); */
/*         for (int j = 0; j < NY; j++) { */
/*             globalGrid[i][j].resize(NZ); */
/*         } */
/*     } */
/*  */
/*     #pragma omp parallel for collapse(3) */
/*     for (int i = 0; i < NX; i++) { */
/*         for (int j = 0; j < NY; j++) { */
/*             for (int k = 0; k < NZ; k++) { */
/*                 float x = x_min + i * dx; */
/*                 float y = y_min + j * dy; */
/*                 float z = z_min + k * dz; */
/*                 float r = sqrt(x * x + y * y + z * z); */
/*  */
/*                 Cell2D cell; */
/*                 float Phi = 1.0 + 0.5 * M / r; */
/*                 cell.gauge.alpha = (1.0 - M / (2 * r)) / (1.0 + M / (2 * r)); */
/*                  */
/*                 for (int a = 0; a < 3; a++) { */
/*                     for (int b = 0; b < 3; b++) { */
/*                         cell.geom.gamma[a][b] = (a == b) ? pow(Phi, 4) : 0.0; */
/*                     } */
/*                 } */
/*  */
/*                 matrix.inverse_3x3(cell.geom.gamma, cell.geom.gamma_inv); */
/*                  */
/*                 for (int a = 0; a < 3; a++) { */
/*                     for (int b = 0; b < 3; b++) { */
/*                         cell.curv.K[a][b] = 0.0; */
/*                     } */
/*                 } */
/*  */
/*                 */
/*                 globalGrid[i][j][k] = cell; */
/*  */
/*                 if (j == NY / 2 && k == NZ / 2) {  */
/*                     printf("gamma[0][0] at (i=%d, j=%d, k=%d) = %e\n", i, j, k, globalGrid[i][j][k].gamma[0][0]); */
/*                 } */
/*             } */
/*         } */
/*     } */
/* 	float test_radii[] = {0.5, 1.0, 2.0, 5.0, 10.0, 20.0, 128.0}; */
/* 	for (float test_r : test_radii) { */
/* 		float Phi_test = 1.0 + 0.5 * M / test_r; */
/* 		float alpha_test = (1.0 - M / (2 * test_r)) / (1.0 + M / (2 * test_r)); */
/* 		printf("r = %f : Phi^4 = %e, alpha = %e\n", test_r, pow(Phi_test, 4), alpha_test); */
/* 	} */
/*  */
/* 	for (int iCell = 0; iCell < 3; iCell++) { */
/* 		for (int jCell = 0; jCell < 3; jCell++) { */
/* 			for (int kCell = 0; kCell < 3; kCell++) { */
/* 				Cell2D &cell = globalGrid[iCell][jCell][kCell]; */
/* 				for (int a = 0; a < 3; a++) { */
/* 					for (int b = 0; b < 3; b++) { */
/* 						if (a != b && fabs(cell.geom.gamma[a][b]) > 1e-12) { */
/* 							printf("⚠️ γ[%d][%d] non diagonale en (%d,%d,%d) : %e\n",  */
/* 									a, b, iCell, jCell, kCell, cell.geom.gamma[a][b]); */
/* 						} */
/* 					} */
/* 				} */
/* 			} */
/* 		} */
/* 	} */
/* 	int i_far = NX - 1;   */
/* 	int j_center = NY / 2; */
/* 	int k_center = NZ / 2; */
/* 	Cell2D &cell_far = globalGrid[i_far][j_center][k_center]; */
/*  */
/*  */
/* 	float rho_horizon = M / 2.0;   */
/* 	int i_horizon = static_cast<int>((rho_horizon - x_min) / dx); */
/* 	Cell2D &cell_horizon = globalGrid[i_horizon][j_center][k_center]; */
/*  */
/* 	std::cout << "\nTest 2 (r = M/2):\n"; */
/* 	std::cout << "Position x = " << (x_min + i_horizon*dx) << "\n"; */
/* } */
/*  */
/*  */
/*  */
/*  */
