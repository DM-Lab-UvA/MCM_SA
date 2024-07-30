#include "header.h"
#include <time.h>
#include <ctime> 
#include <ratio>
#include <chrono>

void simulated_annealing(Partition &p_struct, 
	unsigned int max_iterations, 
	unsigned int max_no_improve) {

	cout << "\n- running simulated annealing algorithm\n" << "\n"; 

	unsigned int f;
    unsigned int iterations = 0;
    unsigned int steps_since_improve = 0;

    // annealing parameters
    double T0 = 100.0; // initial annealing temperature
    p_struct.T = T0;
    unsigned int update_schedule = 100; // number of iterations at same annealing temperature

	// performance
    auto start = chrono::system_clock::now();

    for (unsigned int i = 0; i < max_iterations; i++){

    	iterations++;

    	// check for limit cases 
    	if (p_struct.nc == p_struct.n){
    		f = 0; // always merge if all independent communities
    	} else if (p_struct.nc == 1){
    		f = 1; // always split if one big community
    	} else {
    		f = rand()/(RAND_MAX/3);
    	}

    	// choose proposal function
		switch(f){
		case 0: 
			merge_partition(p_struct);
			break;
		case 1:
			split_partition(p_struct);
			break;
		case 2:
			switch_partition(p_struct);
			break;
		}

		// update annealing temperature
		if (i % update_schedule == 0){
			p_struct.T = T0 / (1 + log(1 + i));
		}

		// compare and update best log-evidence
		if ((p_struct.current_log_evidence > p_struct.best_log_evidence) && !(DoubleSame(p_struct.current_log_evidence, p_struct.best_log_evidence))){

			p_struct.best_log_evidence = p_struct.current_log_evidence;
			p_struct.best_partition = p_struct.current_partition;

			cout << "best log-evidence: " << p_struct.best_log_evidence << "\t@T = " << p_struct.T << "\ti = " << i << "\n";
			
			steps_since_improve = 0;

		} else {
			steps_since_improve++;
		}

		// stop if no improvement 
		if (steps_since_improve > max_no_improve){
			cout << "\n- maximum iterations without improvement reached" << "\n";
			break;
		}
    }

    // performance 
    auto end = chrono::system_clock::now();
	chrono::duration<double> elapsed = end - start;
	double its_per_second = static_cast <double> (iterations) / elapsed.count();
	cout << "- iterations per second: " << its_per_second << "\n" << "\n";

	// at completion, make sure current partition is equal to best partition
	p_struct.current_log_evidence = p_struct.best_log_evidence;
	p_struct.current_partition = p_struct.best_partition;

}