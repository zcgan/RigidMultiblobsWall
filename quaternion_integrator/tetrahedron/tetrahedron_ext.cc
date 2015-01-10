// Functions for tetrahedron simulation written
// in C++ for improved speed.

#include <boost/python.hpp>
#include <math.h>
#include <vector>
#include <iostream>


void PrintTest(void) {
	std::cout << "printing stuff" << std::endl;
}

void TestList(boost::python::list list_to_print) {
    // Test using the python list implemented in boost.
    int n  = boost::python::len(list_to_print);
    for (int k = 0; k < n; ++k) {
        std::cout << "x at " << k << " is "
                  << boost::python::extract<double>(list_to_print[k])
                  << std::endl;
    }
}

void SingleWallFluidMobility(boost::python::list r_vectors, 
														 double eta,
														 double a, int num_particles,
														 boost::python::list mobility) {
	// TODO: Replace this with accurate version.
	double pi = 3.141592653;
	// Create the mobility of particles in a fluid with a single wall at z = 0.
	double* R;
	R = new double[3];
	double h = 0.0;
	for (int j = 0; j < num_particles; ++j) {
		for (int k = 0; k < num_particles; ++k) {
			// Here notation is based on appendix C of the Swan and Brady paper:
			//  'Simulation of hydrodynamically interacting particles near a no-slip
			//   boundary.'
			h = r_vectors[k][2];
			for (int l = 0; l < 2; ++l) {
				R[l] = (r_vectors[j][l] - (r_vectors[k][l]))/a;
			}
			R[2] = (r_vectors[j][2] - (r_vectors[k][2]) - 2.0*h)/a;

			double R_norm = 0.0;
			for (int l = 0; l < 3; ++l) {
				R_norm += pow(R[j], 2);
			}
			R_norm = sqrt(R_norm);
			double* e = new double[3];
			for (int l = 0; l < 3; ++l) {
				e[l] = R[l]/R_norm;
			}
			double* e_3 = new double[3];
			e_3[0] = 0.0;
			e_3[1] = 0.0;
			e_3[2] = e[2];
			double h_hat = h/(a*R[2]);
			
			// Taken from Appendix C expression for M_UF.
			for (int l = 0; l < 3; ++l) {
				for (int m = 0; m < 3; ++m) {
					mobility[j*3 + l][k*3 + m] += (1./(6.*pi*eta*a))*
						(-0.25*(3.*(1. - 6.*h_hat*(1. - h_hat)*pow(e[2],2))/R_norm
										- 6.*(1. - 5.*pow(e[2],2))/(pow(R_norm,3))
										+ 10.*(1. - 7.*pow(e[2],2))/(pow(R_norm,5)))*(e[l]*e[m])
						 - (0.25*(3.*(1. + 2.*h_hat*(1. - h_hat)*pow(e[2],2))/R_norm
											+ 2.*(1. - 3.*pow(e[2],2))/(pow(R_norm,3))
											- 2.*(2. - 5.*pow(e[2],2))/(pow(R_norm,5))))*(l == m ? 1.0 : 0.0)
						 + 0.5*(3.*h_hat*(1. - 6.*(1. - h_hat)*pow(e[2],2))/R_norm
										- 6.*(1. - 5.*pow(e[2],2))/(pow(R_norm,3))
										+ 10.*(2. - 7.*pow(e[2],2))/(pow(R_norm,5)))*(e[l], e_3[m])
						 + 0.5*(3.*h_hat/R_norm - 10./(pow(R_norm,5)))*(e_3[l], e[m])
						 - (3.*(pow(h_hat,2))*(pow(e[2],2))/R_norm 
								+ 3.*(pow(e[2],2))/(pow(R_norm,3))
								+ (2. - 15.*pow(e[2],2))/(pow(R_norm,5)))*
						 (e_3[l], e_3[m])/(pow(e[2],2)));
				}
			}

  //     # Taken from Appendix C expression for M_UF
  //     mobility[(j*3):(j*3 + 3), (k*3):(k*3 + 3)] += (1./(6.*np.pi*eta*a))*(
  //       -0.25*(3.*(1. - 6.*h_hat*(1. - h_hat)*e[2]**2)/R_norm
  //              - 6.*(1. - 5.*e[2]**2)/(R_norm**3)
  //              + 10.*(1. - 7.*e[2]**2)/(R_norm**5))*np.outer(e, e)
  //        - (0.25*(3.*(1. + 2.*h_hat*(1. - h_hat)*e[2]**2)/R_norm
  //                 + 2.*(1. - 3.*e[2]**2)/(R_norm**3)
  //                 - 2.*(2. - 5.*e[2]**2)/(R_norm**5)))*np.identity(3)
  //        + 0.5*(3.*h_hat*(1. - 6.*(1. - h_hat)*e[2]**2)/R_norm
  //               - 6.*(1. - 5.*e[2]**2)/(R_norm**3)
  //               + 10.*(2. - 7.*e[2]**2)/(R_norm**5))*np.outer(e, e_3)
  //        + 0.5*(3.*h_hat/R_norm - 10./(R_norm**5))*np.outer(e_3, e)
  //        - (3.*(h_hat**2)*(e[2]**2)/R_norm 
  //           + 3.*(e[2]**2)/(R_norm**3)
  //           + (2. - 15.*e[2]**2)/(R_norm**5))*np.outer(e_3, e_3)/(e[2]**2))
			

	// Unclear about this right now.
  //     mobility[(k*3):(k*3 + 3), (j*3):(j*3 + 3)] = (
  //       mobility[(j*3):(j*3 + 3), (k*3):(k*3 + 3)].T)

  // for j in range(len(r_vectors)):
  //   # Diagonal blocks, self mobility.
  //   h = r_vectors[j][2]/a
  //   for l in range(3):
  //     for m in range(3):
  //       mobility[j*3 + l][j*3 + m] += (1./(6.*np.pi*eta*a))*(
  //         (l == m)*(l != 2)*(-1./16.)*(9./h - 2./(h**3) + 1./(h**5))
  //         + (l == m)*(l == 2)*(-1./8.)*(9./h - 4./(h**3) + 1./(h**5)))
  // return mobility
		}
	}
}


BOOST_PYTHON_MODULE(tetrahedron_ext)
{
    using namespace boost::python;
    def("print_test", PrintTest);
    def("test_list", TestList);
    def("single_wall_fluid_mobility", SingleWallFluidMobility);
}
