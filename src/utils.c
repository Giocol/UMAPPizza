//
//	Created by Giorgio - 2019-04-08
//

#include "headers.h"
//The two tau_rand functions generate a pseudo-random number
// through some bitwise operator magic, following the umap python code

int _abs_t_index(t_index x){
	if (x>=0)
		return x;
	return -x;
}

t_index tau_rand_t_index(int state[N_STATES])	{

	// rng_state is an array that represents the internal state of the rng
	state[0] = (((state[0] & 4294967294) << 12) & 0xffffffff) ^ ((((state[0] << 13) & 0xffffffff) ^ state[0]) >> 19);
	state[1] = (((state[1] & 4294967288) << 4) & 0xffffffff) ^ ((((state[1] << 2) & 0xffffffff) ^ state[1]) >> 25);
	state[2] = (((state[2] & 4294967280) << 17) & 0xffffffff) ^ ((((state[2] << 3) & 0xffffffff) ^ state[2]) >> 11);

	return (t_index) state[0]^state[1]^state[2];
}

int tau_rand_int(int state[N_STATES])	{

	// rng_state is an array that represents the internal state of the rng
	state[0] = (((state[0] & 4294967294) << 12) & 0xffffffff) ^ ((((state[0] << 13) & 0xffffffff) ^ state[0]) >> 19);
	state[1] = (((state[1] & 4294967288) << 4) & 0xffffffff) ^ ((((state[1] << 2) & 0xffffffff) ^ state[1]) >> 25);
	state[2] = (((state[2] & 4294967280) << 17) & 0xffffffff) ^ ((((state[2] << 3) & 0xffffffff) ^ state[2]) >> 11);

	return state[0]^state[1]^state[2];
}


int tau_rand(int state[]) {
	// Similar to the previous function, but it returns a int between 0 and 1
	int rng;
	rng = tau_rand_int(state);

	if(rng > 0){
		return 1; //50% of chance of selecting it
	}
	return 0;
}

void rejection_sample(int n_samples,int pool_size, int rng_state[], t_index result[]) {
	// n_samples: number of random samples to pick from the pool of samples
	// pool_size: size of the pool of samples
	// rng_state: state of the random vector (similar to tau_rand's arg)

	int i, k;
	t_index j;
	boolean reject_sample = 0;

	for(i = 0; i < n_samples; i++)	{
		reject_sample = 1;
		while(reject_sample == 1)	{
			j = _abs_t_index(tau_rand_t_index(rng_state)) % pool_size;
			for(k = 0; k < i; k++)	{
				if(j == result[k])
					break;
			}
			if(j != result[k])
				reject_sample = 0;
		result[i] = j;
		}
	}

}

/*
void rejection_sample_new(int n_samples,int pool_size, int rng_state[], t_index result[]) {
	// n_samples: number of random samples to pick from the pool of samples
	// pool_size: size of the pool of samples
	// rng_state: state of the random vector (similar to tau_rand's arg)

	int i, k;
	t_index j;
	boolean reject_sample = 0;

	for(i = 0; i < n_samples; i++)	{
		boolean found = 0;
		reject_sample = 1;
		while(reject_sample == 1) {
			if(found = 0)	{
				j = _abs_t_index(tau_rand_t_index(rng_state)) % pool_size; // generates a random number to drop around 50% of the data
				for(k = 0; k < i; k++)	{
					if(j == result[k])
						found = 1;
					else
						found = 0;
				}
				if(j != result[k])
					reject_sample = 0;
				else
					reject_sample = 1; // Redudant, but necessary for HW optimization on FPGA
				result[i] = j;
			}
			else {		// "Fake" cycle that doesn't do anything, necessary for HW optimization
				j = j;
				for(k = 0; k < i; k++)	{
					if(j == result[k])
						found = 1;
					else
						found = 1;
				}
				if(j != result[k])
					reject_sample = 1;
				else
					reject_sample = 1;
				result[i] = result[i];
			}
		}
	}
}
*/
void build_candidates(
	t_index current_graph_index[N_POINTS*K_NEIGHBORS],
	boolean current_graph_flag[N_POINTS*K_NEIGHBORS],
    int rng_state[],
	t_index candidate_neighbors_index[N_POINTS*MAX_CANDIDATES],
	t_distance candidate_neighbors_dist[N_POINTS*MAX_CANDIDATES],
	boolean candidate_neighbors_flag[N_POINTS*MAX_CANDIDATES]
	){
	int i, j, idx, isn;
	int d;

	for(i = 0; i < N_POINTS; i++) {
		for(j = 0; j < K_NEIGHBORS; j++) {
			// if(current_graph_index[i*K_NEIGHBORS + j] < 0) 	// The function should work without this if statement
			// 	continue;
			idx = current_graph_index[i*K_NEIGHBORS + j];
			isn = current_graph_flag[i*K_NEIGHBORS + j];
			d = tau_rand(rng_state); //Remove tau_rand when changing build_candidates
			heap_push_candidate(candidate_neighbors_index,candidate_neighbors_dist,candidate_neighbors_flag, i, d, idx, isn);
			heap_push_candidate(candidate_neighbors_index,candidate_neighbors_dist,candidate_neighbors_flag, idx, d, i, isn);
			current_graph_flag[i*K_NEIGHBORS + j] = 0;
		}
	}
}
/*
void build_candidate_opt(
	t_index current_graph_index[N_POINTS*K_NEIGHBORS],
	boolean current_graph_flag[N_POINTS*K_NEIGHBORS],
    int rng_state[],
	t_index candidate_neighbors_index[N_POINTS*MAX_CANDIDATES/2],
	t_distance candidate_neighbors_dist[N_POINTS*MAX_CANDIDATES/2],
	boolean candidate_neighbors_flag[N_POINTS*MAX_CANDIDATES/2]
){
	
}
*/
t_distance euclidean_distance(t_data x[N_FEATURES],t_data y[N_FEATURES]){
	t_distance result=0;

	for(t_index i=0; i<N_FEATURES; i++){
			result += (((t_distance)x[i]-(t_distance)y[i]))*((t_distance)x[i]-(t_distance)y[i]);
	}
	return result;
}
