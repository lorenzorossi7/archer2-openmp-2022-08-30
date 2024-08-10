#include <iostream>
#include <omp.h>
#include <cmath>

using std::cout;

static int maxballs_per_thread=100;

int main(int argc, char **argv) {

	if (omp_get_num_threads()!=2) {
		printf("A ping pong match requires exactly two players. Setting num_threads to 2.\n");
		omp_set_num_threads(2);
	}

	int count0=0,count1=0;

#pragma omp parallel default(none) shared(maxballs_per_thread,cout,count0,count1)
{
	int threadid=omp_get_thread_num();
	while (count1<maxballs_per_thread) {
		if (threadid==1) cout << "Count for thread " << threadid << " is " << count1++ << "\n";
#pragma omp barrier
		if (threadid==0 && count0!=maxballs_per_thread-1) cout << "Count for thread 0 is " << count0++ << "\n";
#pragma omp barrier
	}
}

	cout << "thread 1 gave up: it received the ball " << count1 << " times.\n";

	return 0;
}