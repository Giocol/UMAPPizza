#include "headers.h"
#include "signals.h"
//#include <math.h>
#include <time.h>

int main(int argc, char *argv[]){

	static t_data *data = data_test;
    static t_index result_index[N_POINTS*K_NEIGHBORS]={-1};
    static t_distance result_dist[N_POINTS*K_NEIGHBORS]={-1};
#if RP_TREE_INIT
    static t_index *leaf_array = leaf_array_test;
#endif
    int rng_state[]= {7,456,99};
    int i;
    int j;


    clock_t begin = clock();
    nn_descent(data, 
    rng_state,
#if RP_TREE_INIT
    		leaf_array,
#endif
			result_index,
            result_dist);
    clock_t end = clock();
		
	int N_PRINT=N_POINTS;
    for(i=N_POINTS-N_PRINT; i<N_POINTS; i++){
        printf("Considering the point: idx:%d ", i);
        for(int k=0; k<N_FEATURES; k++){
            printf("%c:%d ", 'A'+k, data[i*N_FEATURES + k]);
        }
        printf("\n");
        for(j=0; j<K_NEIGHBORS; j++){
            printf("idx:%d ( ",result_index[i*K_NEIGHBORS + j]);
            //for(int k=0; k<N_FEATURES; k++){
            //    printf("%c:%d ", 'A'+k, data[result_index[i*K_NEIGHBORS + j]* N_FEATURES +k]);
            //}
            printf(")at distance %lld", result_dist[i*K_NEIGHBORS + j]);
            //printf(")at distance %g", sqrt(result_dist[i*K_NEIGHBORS + j]));
            //printf(")at distance %lld", result_dist[i*K_NEIGHBORS + j]);
            printf(" (from Python idx: %d, dist: %f)", knn_indices_test[i*K_NEIGHBORS + j], knn_dists_test[i*K_NEIGHBORS + j]*knn_dists_test[i*K_NEIGHBORS + j]);
            printf("\n");
        }
        printf("\n");
    }

    printf("Time C: %f, Time Python: %f", ((double) end-begin) /CLOCKS_PER_SEC, time_python);

}
