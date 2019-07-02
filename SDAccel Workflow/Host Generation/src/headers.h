#ifndef NNDESCENT_HEADERS_H
#define NNDESCENT_HEADERS_H

#define VERBOSE 1

#include <stdio.h>
#include <string.h>
#include <limits.h> //for INT_MAX
#include "defines.h"
#include <ap_cint.h>

typedef int t_distance;
typedef int t_index;
typedef uint1 boolean;
typedef int t_data;
typedef int t_state;

void rejection_sample(
    int n_samples,
    int pool_size,
    int rng_state[], 
    t_index result[]
    );

void build_candidates(
	t_index current_graph_index[N_POINTS*K_NEIGHBORS], 
	boolean current_graph_flag[N_POINTS*K_NEIGHBORS],
    int rng_state[],
	t_index candidate_neighbors_index[N_POINTS*MAX_CANDIDATES], 
	t_distance candidate_neighbors_dist[N_POINTS*MAX_CANDIDATES], 
	boolean candidate_neighbors_flag[N_POINTS*MAX_CANDIDATES]
	);

int result[N_POINTS][K_NEIGHBORS];

void nn_descent(
		t_data data_in[N_POINTS*N_FEATURES],
        //K_NEIGHBORS
        int rng_state[N_STATES],
        //MAX_CANDIDATES
        //N_ITERS
        //DELTA
        //RHO
        //rp_tree_init considered True
#if RP_TREE_INIT
        t_index leaf_array[N_LEAF*LEAF_SIZE],
#endif
        t_index result_out_index[N_POINTS*K_NEIGHBORS],
        t_distance result_out_dist[N_POINTS*K_NEIGHBORS]
);

t_distance euclidean_distance(t_data x[N_FEATURES],t_data y[N_FEATURES]);

int heap_push(
    t_index current_graph_index[N_POINTS*K_NEIGHBORS], 
    t_distance current_graph_dist[N_POINTS*K_NEIGHBORS], 
    boolean current_graph_flag[N_POINTS*K_NEIGHBORS],
    int row,                                      //determines which data point we are addressing
    t_distance weight,                                 //determines the distance (for heap sorting)
    t_index index,                                    //is the element to add
    boolean flag                                     //determines whether this is to be considered a new addition
);

void heap_push_opt(
    t_index current_graph_index[N_POINTS*K_NEIGHBORS], 
    t_distance current_graph_dist[N_POINTS*K_NEIGHBORS], 
    boolean current_graph_flag[N_POINTS*K_NEIGHBORS],
    int row,                                      //determines which data point we are addressing
    t_distance weight,                                 //determines the distance (for heap sorting)
    t_index index,                                    //is the element to add
    boolean flag,                                     //determines whether this is to be considered a new addition
    int *c
);

int heap_push_candidate(
    t_index current_graph_index[N_POINTS*MAX_CANDIDATES], 
    t_distance current_graph_dist[N_POINTS*MAX_CANDIDATES], 
    boolean current_graph_flag[N_POINTS*MAX_CANDIDATES],
    int row,                                      //determines which data point we are addressing
    t_distance weight,                                 //determines the distance (for heap sorting)
    t_index index,                                    //is the element to add
    boolean flag                                     //determines whether this is to be considered a new addition
);

t_index tau_rand_t_index(int state[N_STATES]);
int tau_rand(int rng_state[]);

void siftdown(t_index current_graph_index[N_POINTS * K_NEIGHBORS], 
    t_distance current_graph_dist[N_POINTS * K_NEIGHBORS], 
    boolean current_graph_flag[N_POINTS * K_NEIGHBORS],
    int size_heap,
    int row,
    int elt);

void deheap_sort(
    t_index current_graph_index[N_POINTS * K_NEIGHBORS], 
    t_distance current_graph_dist[N_POINTS * K_NEIGHBORS], 
    boolean current_graph_flag[N_POINTS * K_NEIGHBORS], 
    t_index result_index[N_POINTS*K_NEIGHBORS], 
    t_distance result_dist[N_POINTS*K_NEIGHBORS]
);

void swap_boolean(boolean *a, boolean *b);
void swap_t_distance(t_distance *a, t_distance *b);
void swap_t_index(t_index *a, t_index *b);
void swap_int(int *a, int *b);
#endif //NNDESCENT_HEADERS_H
