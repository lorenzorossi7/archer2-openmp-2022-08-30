#include <iostream>
#include <omp.h>

using std::cout;

static const int numbers=10;

int main(int argc, char** argv) {

#pragma omp parallel for default(none) shared(numbers,cout) 
 for(int i=0; i<numbers; i++ ) {
     #pragma omp critical
     cout << "I'm thread "<< omp_get_thread_num() << " and I'm printing the number " << i << ".\n";
 }

 return 0;
}