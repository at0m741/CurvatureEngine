#include <Geodesics.h>
#include "Spectral.h"
/*
 * Theses functions are used to compute the partial derivative of the metric tensor
 * and the Christoffel symbols, gauge.beta and gauge.alpha gauges conditions
 * All of these use a 2nd, fourth or sixth order finite difference scheme
 *
 * The functions are used in the computation of the ADM/BSSN equations
 *
 * derivatives are computed using the following formulas:
 * \partial_x f = (f_{i+1} - f_{i-1}) / (2 * dx)
 * \partial_y f = (f_{j+1} - f_{j-1}) / (2 * dy)
 * \partial_z f = (f_{k+1} - f_{k-1}) / (2 * dz)
 * \partial_{xx} f = (f_{i+1} - 2 * f_i + f_{i-1}) / dx^2
 * \partial_{yy} f = (f_{j+1} - 2 * f_j + f_{j-1}) / dy^2
 * \partial_{zz} f = (f_{k+1} - 2 * f_k + f_{k-1}) / dz^2
 * \partial_{xy} f = (f_{i+1,j+1} - f_{i+1,j-1} - f_{i-1,j+1} + f_{i-1,j-1}) / (4 * dx * dy)
 * \partial_{xz} f = (f_{i+1,k+1} - f_{i+1,k-1} - f_{i-1,k+1} + f_{i-1,k-1}) / (4 * dx * dz)
 * \partial_{yz} f = (f_{j+1,k+1} - f_{j+1,k-1} - f_{j-1,k+1} + f_{j-1,k-1}) / (4 * dy * dz)
 * \partial_{xxy} f = (f_{i+2} - 2 * f_{i+1} + 2 * f_{i-1} - f_{i-2}) / (12 * dx)
 * \partial_{yyz} f = (f_{j+2} - 2 * f_{j+1} + 2 * f_{j-1} - f_{j-2}) / (12 * dy)
 * \partial_{zzx} f = (f_{k+2} - 2 * f_{k+1} + 2 * f_{k-1} - f_{k-2}) / (12 * dz)
 * \partial_{xxyy} f = (f_{i+1,j+1} - 2 * f_{i+1,j} + f_{i+1,j-1} - f_{i-1,j+1} + 2 * f_{i-1,j} - f_{i-1,j-1}) / (4 * dx * dy)
 * \partial_{yyzz} f = (f_{j+1,k+1} - 2 * f_{j+1,k} + f_{j+1,k-1} - f_{j-1,k+1} + 2 * f_{j-1,k} - f_{j-1,k-1}) / (4 * dy * dz)
 * \partial_{zzxx} f = (f_{k+1,i+1} - 2 * f_{k+1,i} + f_{k+1,i-1} - f_{k-1,i+1} + 2 * f_{k-1,i} - f_{k-1,i-1}) / (4 * dz * dx)
* */




float partialXX_alpha(Grid &grid_obj, int i, int j, int k) {
    return (grid_obj.getCell(i+1, j, k).gauge.alpha - 2.0 * grid_obj.getCell(i, j, k).gauge.alpha + grid_obj.getCell(i-1, j, k).gauge.alpha) 
           / (DX * DX);
}

float partialYY_alpha(Grid &grid_obj, int i, int j, int k) {
    return (grid_obj.getCell(i, j+1, k).gauge.alpha - 2.0 * grid_obj.getCell(i, j, k).gauge.alpha + grid_obj.getCell(i, j-1, k).gauge.alpha) 
           / (DY * DY);
}

float partialZZ_alpha(Grid &grid_obj, int i, int j, int k) {
    return (grid_obj.getCell(i, j, k+1).gauge.alpha - 2.0 * grid_obj.getCell(i, j, k).gauge.alpha + grid_obj.getCell(i, j, k-1).gauge.alpha) 
           / (DZ * DZ);
}


float partialXY_alpha(Grid &grid_obj, int i, int j, int k) {
    return (grid_obj.getCell(i+1, j+1, k).gauge.alpha - grid_obj.getCell(i+1, j-1, k).gauge.alpha
            - grid_obj.getCell(i-1, j+1, k).gauge.alpha + grid_obj.getCell(i-1, j-1, k).gauge.alpha)
           / (4.0 * DX * DY);
}

float partialXZ_alpha(Grid &grid_obj, int i, int j, int k) {
    return (grid_obj.getCell(i+1, j, k+1).gauge.alpha - grid_obj.getCell(i+1, j, k-1).gauge.alpha
            - grid_obj.getCell(i-1, j, k+1).gauge.alpha + grid_obj.getCell(i-1, j, k-1).gauge.alpha)
           / (4.0 * DX * DZ);
}

float partialYZ_alpha(Grid &grid_obj, int i, int j, int k) {
    return (grid_obj.getCell(i, j+1, k+1).gauge.alpha - grid_obj.getCell(i, j+1, k-1).gauge.alpha
            - grid_obj.getCell(i, j-1, k+1).gauge.alpha + grid_obj.getCell(i, j-1, k-1).gauge.alpha)
           / (4.0 * DY * DZ);
}

float second_partial_alpha(Grid &grid_obj, int i, int j, int k, int a, int b)
{
	if(a==0 && b==0) return partialXX_alpha(grid_obj, i, j, k);
	if(a==1 && b==1) return partialYY_alpha(grid_obj, i, j, k);
	if(a==2 && b==2) return partialZZ_alpha(grid_obj, i, j, k);

	if((a==0 && b==1)||(a==1 && b==0)) return partialXY_alpha(grid_obj, i, j, k);
	if((a==0 && b==2)||(a==2 && b==0)) return partialXZ_alpha(grid_obj, i, j, k);
	if((a==1 && b==2)||(a==2 && b==1)) return partialYZ_alpha(grid_obj, i, j, k);

	return 0.0;
}


float GridTensor::partialX_gamma(Grid &grid_obj, int i, int j, int k, int a, int b) {
    if (i >= 2 && i <= NX - 3) {
        return fourth_order_diff(
            grid_obj.getCell(i+2, j, k).geom.gamma[a][b],
            grid_obj.getCell(i+1, j, k).geom.gamma[a][b],
            grid_obj.getCell(i-1, j, k).geom.gamma[a][b],
            grid_obj.getCell(i-2, j, k).geom.gamma[a][b],
            DX 
        );
    } else if (i >= 1 && i <= NX - 2) {
        return second_order_diff(
            grid_obj.getCell(i+1, j, k).geom.gamma[a][b],
            grid_obj.getCell(i-1, j, k).geom.gamma[a][b],
            DX 
        );
    } else if (i == 0) {
        return (grid_obj.getCell(i+1, j, k).geom.gamma[a][b] - 
                grid_obj.getCell(i, j, k).geom.gamma[a][b]) / DX;
    } else if (i == NX - 1) {
        return (grid_obj.getCell(i, j, k).geom.gamma[a][b] - 
                grid_obj.getCell(i-1, j, k).geom.gamma[a][b]) / DX;
    }
    return 0.0;
}

float GridTensor::partialY_gamma(Grid &grid_obj, int i, int j, int k, int a, int b) {
    if (j >= 2 && j <= NY - 3) {
        return fourth_order_diff(
            grid_obj.getCell(i, j+2, k).geom.gamma[a][b],
            grid_obj.getCell(i, j+1, k).geom.gamma[a][b],
            grid_obj.getCell(i, j-1, k).geom.gamma[a][b],
            grid_obj.getCell(i, j-2, k).geom.gamma[a][b],
            DY
        );
    } else if (j >= 1 && j <= NY - 2) {
        return second_order_diff(
            grid_obj.getCell(i, j+1, k).geom.gamma[a][b],
            grid_obj.getCell(i, j-1, k).geom.gamma[a][b],
            DY
        );
    } else if (j == 0) {
        return (grid_obj.getCell(i, j+1, k).geom.gamma[a][b] - 
                grid_obj.getCell(i, j, k).geom.gamma[a][b]) / DY;
    } else if (j == NY - 1) {
        return (grid_obj.getCell(i, j, k).geom.gamma[a][b] - 
                grid_obj.getCell(i, j-1, k).geom.gamma[a][b]) / DY;
    }
    return 0.0;
}

/*
 * The partial derivative of the metric tensor with respect to z
 * it uses a 2nd, fourth or sixth order finite difference scheme
 * */

float GridTensor::partialZ_gamma(Grid &grid_obj, int i, int j, int k, int a, int b) {
    if (k >= 2 && k <= NZ - 3) {
        return fourth_order_diff(
            grid_obj.getCell(i, j, k+2).geom.gamma[a][b],
            grid_obj.getCell(i, j, k+1).geom.gamma[a][b],
            grid_obj.getCell(i, j, k-1).geom.gamma[a][b],
            grid_obj.getCell(i, j, k-2).geom.gamma[a][b],
            DZ
        );
    } else if (k >= 1 && k <= NZ - 2) {
        return second_order_diff(
            grid_obj.getCell(i, j, k+1).geom.gamma[a][b],
            grid_obj.getCell(i, j, k-1).geom.gamma[a][b],
            DZ
        );
    } else if (k == 0) {
        return (grid_obj.getCell(i, j, k+1).geom.gamma[a][b] - 
                grid_obj.getCell(i, j, k).geom.gamma[a][b]) / DZ;
    } else if (k == NZ - 1) {
        return (grid_obj.getCell(i, j, k).geom.gamma[a][b] - 
                grid_obj.getCell(i, j, k-1).geom.gamma[a][b]) / DZ;
    }
    return 0.0;
}

/*
 * these functions are used to compute the partial derivative of the extrinsic curvature tensor
 * and the Christoffel symbols, gauge.beta and gauge.alpha gauges conditions
 * All of these use a 2nd, fourth or sixth order finite difference scheme
 * it's working nicely but it's not optimized and probably not the best way to do it
 *
 * I'm currently working on a new method based on the spectral differentiation using Chebyshev polynomials
 * and the barycentric interpolation or the Lagrange interpolation (not sure yet)
 * */


float GridTensor::partialX_Kij(Grid &grid_obj, int i, int j, int k, int a, int b)
{
	/*
	 * The fourth order difference is used when the point is not near the boundary
	 * */
	if (i >= 2 && i <= NX - 3) {
		return fourth_order_diff(
			grid_obj.getCell(i+2, j, k).curv.K[a][b],
			grid_obj.getCell(i+1, j, k).curv.K[a][b],
			grid_obj.getCell(i-1, j, k).curv.K[a][b],
			grid_obj.getCell(i-2, j, k).curv.K[a][b],
			DX 
		);

	/*
	 * The second order difference is used when the point is near the boundary
	 *  This is done to avoid the out of bounds error (boundary conditions are not implemented yet 
	 *  exept for a Cell copy at the boundary, not really useful for now but it's a start)
	 * */

	} else if (i >= 1 && i <= NX - 2) {
		return second_order_diff(
			grid_obj.getCell(i+1, j, k).curv.K[a][b],
			grid_obj.getCell(i-1, j, k).curv.K[a][b],
			DX 
		);
		/*
		 * The first order difference is used when the point is at the boundary
		 * */
	} else if (i == 0) {
		return (grid_obj.getCell(i+1, j, k).curv.K[a][b] - 
				grid_obj.getCell(i, j, k).curv.K[a][b]) / DX;
	} else if (i == NX - 1) {
		return (grid_obj.getCell(i, j, k).curv.K[a][b] - 
				grid_obj.getCell(i-1, j, k).curv.K[a][b]) / DX;
	}
	return 0.0;
}

float GridTensor::partialY_Kij(Grid &grid_obj, int i, int j, int k, int a, int b)
{
	if (j >= 2 && j <= NY - 3) {
		return fourth_order_diff(
			grid_obj.getCell(i, j+2, k).curv.K[a][b],
			grid_obj.getCell(i, j+1, k).curv.K[a][b],
			grid_obj.getCell(i, j-1, k).curv.K[a][b],
			grid_obj.getCell(i, j-2, k).curv.K[a][b],
			DY
		);
	} else if (j >= 1 && j <= NY - 2) {
		return second_order_diff(
			grid_obj.getCell(i, j+1, k).curv.K[a][b],
			grid_obj.getCell(i, j-1, k).curv.K[a][b],
			DY
		);
	} else if (j == 0) {
		return (grid_obj.getCell(i, j+1, k).curv.K[a][b] - 
				grid_obj.getCell(i, j, k).curv.K[a][b]) / DY;
	} else if (j == NY - 1) {
		return (grid_obj.getCell(i, j, k).curv.K[a][b] - 
				grid_obj.getCell(i, j-1, k).curv.K[a][b]) / DY;
	}
	return 0.0;
}


float GridTensor::partialZ_Kij(Grid &grid_obj, int i, int j, int k, int a, int b)
{
	if (k >= 2 && k <= NZ - 3) {
		return fourth_order_diff(
			grid_obj.getCell(i, j, k+2).curv.K[a][b],
			grid_obj.getCell(i, j, k+1).curv.K[a][b],
			grid_obj.getCell(i, j, k-1).curv.K[a][b],
			grid_obj.getCell(i, j, k-2).curv.K[a][b],
			DZ
		);
	} else if (k >= 1 && k <= NZ - 2) {
		return second_order_diff(
			grid_obj.getCell(i, j, k+1).curv.K[a][b],
			grid_obj.getCell(i, j, k-1).curv.K[a][b],
			DZ
		);
	} else if (k == 0) {
		return (grid_obj.getCell(i, j, k+1).curv.K[a][b] - 
				grid_obj.getCell(i, j, k).curv.K[a][b]) / DZ;
	} else if (k == NZ - 1) {
		return (grid_obj.getCell(i, j, k).curv.K[a][b] - 
				grid_obj.getCell(i, j, k-1).curv.K[a][b]) / DZ;
	}
	return 0.0;
}



