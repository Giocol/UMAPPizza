//
// Created by luca on 4/7/19.
//

#include "headers.h"
#define PRAGMA_SUB(x) _Pragma (#x)
#define DO_PRAGMA(x) PRAGMA_SUB(x)

void nn_descent(
		t_data data_in[N_POINTS*N_FEATURES],
        //K_NEIGHBORS
        int rng_state_in[N_STATES],
        //MAX_CANDIDATES
        //N_ITERS
        //DELTA
        //RHO
        //rp_tree_init considered True
#if RP_TREE_INIT
		t_index leaf_array_in[N_LEAF * LEAF_SIZE],
#endif
        t_index result_out_index[N_POINTS*K_NEIGHBORS],
        t_distance result_out_dist[N_POINTS*K_NEIGHBORS]
){
DO_PRAGMA(HLS INTERFACE m_axi depth=N_POINTS*N_FEATURES port=data_in offset=slave bundle=hostmem)
DO_PRAGMA(HLS INTERFACE m_axi depth=N_STATES port=rng_state_in offset=slave bundle=hostmem)
DO_PRAGMA(HLS INTERFACE m_axi depth=N_POINTS*K_NEIGHBORS port=result_out_index offset=slave bundle=hostmem)
DO_PRAGMA(HLS INTERFACE m_axi depth=N_POINTS*K_NEIGHBORS port=result_out_dist offset=slave bundle=hostmem)
#if RP_TREE_INIT
DO_PRAGMA(HLS INTERFACE m_axi depth=N_LEAF*LEAF_SIZE port=leaf_array_in offset=slave bundle=hostmem)
#pragma HLS INTERFACE s_axilite port=leaf_array_in bundle=control
#endif
#pragma HLS INTERFACE s_axilite port=data_in bundle=control
#pragma HLS INTERFACE s_axilite port=rng_state_in bundle=control
#pragma HLS INTERFACE s_axilite port=result_out_index bundle=control
#pragma HLS INTERFACE s_axilite port=result_out_dist bundle=control

#pragma HLS INTERFACE s_axilite port=return bundle=control

    //utility variables/array used to statically save intermediate results
    t_distance d;
    static t_index candidate_neighbors_index[N_POINTS*MAX_CANDIDATES];
    static t_distance candidate_neighbors_dist[N_POINTS*MAX_CANDIDATES];
    boolean candidate_neighbors_flag[N_POINTS*MAX_CANDIDATES];

    int c;
    t_index p;
    t_index q;
    static t_index current_graph_index[N_POINTS * K_NEIGHBORS];
    static t_distance current_graph_dist[N_POINTS * K_NEIGHBORS];
    boolean current_graph_flag[N_POINTS * K_NEIGHBORS];

    //burst memory access to data
    static t_data data[N_POINTS * N_FEATURES];
    static t_index result_index[N_POINTS*K_NEIGHBORS];
    static t_distance result_dist[N_POINTS*K_NEIGHBORS];
    int rng_state[N_STATES];
    memcpy(data,(const t_data*)data_in, N_POINTS*N_FEATURES*sizeof(t_data));
    memcpy(rng_state, (const int*)rng_state_in, N_STATES*sizeof(int));
#if RP_TREE_INIT
    static t_index leaf_array[N_LEAF*LEAF_SIZE];
    memcpy(leaf_array, (const t_index*)leaf_array_in, N_LEAF*LEAF_SIZE*sizeof(t_index));
#endif
    //in substitution of make_heap we initialize directly our heap
    for(int n_points=0; n_points < N_POINTS; n_points++){
        for(int i_neighbor = 0; i_neighbor< K_NEIGHBORS; i_neighbor++){
                current_graph_index[n_points * K_NEIGHBORS + i_neighbor] = -1;
                current_graph_dist[n_points * K_NEIGHBORS + i_neighbor] = MAX_DIST;
                current_graph_flag[n_points * K_NEIGHBORS + i_neighbor] = 0;
        }
    } 

    //push of random points in each point's heap (~random initialization)
    static t_index indices[K_NEIGHBORS];

    if(VERBOSE) printf("Random Inizialization Start\n");

    //cycle on each data point
    for(int i=0; i< N_POINTS; i++){
        //create an array of random indices used to select randomly some element from data to initialize the heaps
        rejection_sample(K_NEIGHBORS, N_POINTS, rng_state, indices); //what happens if i is in indices?

        for(int j=0; j<K_NEIGHBORS; j++){
            //compute the distance for the current pair of point and push them in each other's heap
            d = euclidean_distance( &data[i*N_FEATURES], &data[indices[j]*N_FEATURES]);
            heap_push(current_graph_index, current_graph_dist, current_graph_flag, i, d, indices[j], 1);
            heap_push(current_graph_index, current_graph_dist, current_graph_flag, indices[j], d, i, 1);
        }
    }

    if(VERBOSE) printf("Random Inizialization End\n");
    if(VERBOSE) printf("RPTree Inizialization Start\n");

    //push in each point's heap the approximate neighbor derived from RPTree (~ heuristic initialization)
    // How do RPTree work? https://www.youtube.com/watch?v=UFE2XtAaCck Minute 1:48
#if RP_TREE_INIT
    //cycle on each leaf array
    for(int m=0; m<N_LEAF; m++){

        //cycle on each element of each leaf
        for(int i=0; i< LEAF_SIZE; i++){
            //are elements not initialized of the leaf initialized to -1?
            //they are not considered
            if (leaf_array[m*LEAF_SIZE + i] < 0){
                break;
            }
            //cycle on all successor elements in the leaf
            for(int j=i+1; j<LEAF_SIZE; j++){
                if(leaf_array[m*LEAF_SIZE + j] < 0){
                    break;
                }
                //compute distances between each pair (i,j) of points
                d= euclidean_distance(&data[leaf_array[m*LEAF_SIZE + i] * N_FEATURES], &data[leaf_array[m*LEAF_SIZE +j] * N_FEATURES]);

                heap_push(current_graph_index, current_graph_dist, current_graph_flag, leaf_array[m*LEAF_SIZE + i], d, leaf_array[m*LEAF_SIZE + j], 1);
                heap_push(current_graph_index, current_graph_dist, current_graph_flag, leaf_array[m*LEAF_SIZE + j], d, leaf_array[m*LEAF_SIZE + i], 1);
            }
        }
    }

#endif

    if(VERBOSE) printf("RPTree Inizialization End\n");
    if(VERBOSE) printf("Main Block Start\n");

    //main block of NNDescent
    //in each iteration we scan the neighbors' neighbors and (if not already compared before) we try to push them in the current point heap
    //OSS: we consider each point singularly and then confront all its neighbors between them tryin to push each one in the other
    //(a point neighbors are neighbor's neighbor each one of the other)

    //for each iteration of the algorithm
    for(int n=0; n<N_ITERS; n++){

        if(VERBOSE) printf("Iteration n° %d\n", n);
        //initialize the array of potential candidate neighbor for each array
        
        for(int n_point=0; n_point < N_POINTS; n_point++){
            for(int i_neighbor = 0; i_neighbor< MAX_CANDIDATES; i_neighbor++){
                    candidate_neighbors_index[n_point*MAX_CANDIDATES + i_neighbor] = -1;
                    candidate_neighbors_dist[n_point*MAX_CANDIDATES + i_neighbor] = MAX_DIST;
                    candidate_neighbors_flag[n_point*MAX_CANDIDATES + i_neighbor] = 0;
            }
        }

        build_candidates(current_graph_index, current_graph_flag,
                         rng_state, candidate_neighbors_index, candidate_neighbors_dist, candidate_neighbors_flag);

        if(VERBOSE) printf("Candidates Built\n");

        c=0;

        //cycle on each data point ~ i is the index of the point we are analyzing
        for(int i=0; i<N_POINTS; i++){
//#pragma HLS PIPELINE II=1

            //cycle on each candidate neighbor of the i-th point
            for(int j=0; j<MAX_CANDIDATES; j++){

                //p is the index of the j-th candidate neighbor data point 
                p= candidate_neighbors_index[i*MAX_CANDIDATES + j];

                //are elements not initialized of the leaf initialized to -1?
                //ignore element with negative index
                //randomly ignore the 50% of candidate neighbor ~ Dropout to get better performances
                if(p<0 || tau_rand(rng_state) < RHO){
                    continue;
                }

                //cycle on each candidate neighbor of the i-th point
                for(int k=0; k<MAX_CANDIDATES;k++){

                    //q is the index of the k-th candidate neighbor data point 
                    q = candidate_neighbors_index[i*MAX_CANDIDATES + k];

                    //ignore element with negative index
                    //ignore the pair in which the two are both flagged not new ('0')
                    if(q<0 || (!candidate_neighbors_flag[i*MAX_CANDIDATES + j] && !candidate_neighbors_flag[i*MAX_CANDIDATES + k])){
                        continue;
                    }
                    d= euclidean_distance(&data[p*N_FEATURES],&data[q*N_FEATURES]);

                    //c represents the number of updates executed during the iteration
                    // try to push p, q in each other's graph
                    c+= heap_push(current_graph_index, current_graph_dist, current_graph_flag, p, d, q, 1);
                    c+= heap_push(current_graph_index, current_graph_dist, current_graph_flag, q, d, p, 1);
                    //heap_push_opt(current_graph_index, current_graph_dist, current_graph_flag, q, d, p, 1, &c);
                    //heap_push_opt(current_graph_index, current_graph_dist, current_graph_flag, q, d, p, 1, &c);
                }
            }

        }
        if(VERBOSE) printf("N° of updates: %d\n\n",c);
        //if too few updates happened, the algorithm stops
        if(c<= DELTA * K_NEIGHBORS * N_POINTS){
            break;
        }
    }
    //remove the heap structure ~linearize the array of our final result
    deheap_sort(current_graph_index, current_graph_dist, current_graph_flag, result_index, result_dist);

    //burst memory access to result_out
    memcpy(result_out_index,(const t_index*)result_index, N_POINTS*K_NEIGHBORS*sizeof(t_index));
    memcpy(result_out_dist,(const t_distance*)result_dist, N_POINTS*K_NEIGHBORS*sizeof(t_distance));
}
