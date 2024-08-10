#include <stdio.h>
#include <math.h>
#include <stdlib.h>
// #include <omp.h>

#define GRIDSIZE 10
static float h = 0.1;
static float residual = 1e-5;


void initialise(float *u, float *rho) {

// Create a start configuration with the heat energy u=10 at the x=0 boundary
#pragma omp parallel for default(none) shared(u,rho)
   for(int i=0; i < GRIDSIZE+2; i++) {
      u[i] = 0.0;
      rho[i] = 0.0;
   }

   u[0] = 10.0;

}

/* Apply a single time step */
double poisson_step( 
     float *u, float *unew, float *rho
   ){
   double unorm=0.0;

   // Calculate one timestep
#pragma omp parallel default(none) shared(h,u,unew,rho) reduction(+:unorm)
{
#pragma omp for
   for( int i=1; i < GRIDSIZE+1; i++){
      float difference = u[i-1] + u[i+1];
      unew[i] = 0.5*( difference - h*h*rho[i] );
      float diff = unew[i]-u[i];
      unorm +=diff*diff;
   }
 
   // Overwrite u with the new value
#pragma omp for
   for( int i = 1;i < GRIDSIZE+1;i++){
      u[i] = unew[i];
   }
}

   return unorm;
}



int main(int argc, char** argv) {

   // auto start_time=std::chrono::system_clock::now();

   // The heat energy in each block
   float u[GRIDSIZE+2], unew[GRIDSIZE+2], rho[GRIDSIZE+2];
   int i;
   double unorm;

 
   // Initialise solution and source
   initialise(u,rho);


   // Run a single iteration first
   unorm = poisson_step( u, unew, rho);
   // for (i=0;i<GRIDSIZE;i++) {
   //     printf("u[%d]=%lf\n",i,u[i]);
   // }

  if( unorm == 25 ){
    printf("TEST SUCCEEDED after 1 iteration\n");
  } else {
    printf("TEST FAILED after 1 iteration\n");
    printf("Norm %g\n", unorm);
  }

  // Run 9 more iterations for a total of 10
  for( i=1; i<100; i++) 
    unorm = poisson_step( u, unew, rho);
    

  if( fabs(unorm - 0.000200706) < 1e-6 ){
    printf("TEST SUCCEEDED after 100 iteration\n");
  } else {
    printf("TEST FAILED after 100 iteration\n");
    printf("Norm %g\n", unorm);
  }
 
   // // Run iterations until the field reaches an equilibrium
   // // and no longer changes
   // for( i=0; i<10000; i++ ) {
   //   unorm = poisson_step( u, unew, rho );
   //   printf("Iteration %d: Residue %g\n", i, unorm);
     
   //   if( sqrt(unorm) < sqrt(residual) ){
   //     break;
   //   }
   // }
 
   printf("Run completed with residue %g\n", unorm);

   // auto final_time=std::chrono::system_clock::now();

   return 0;

}