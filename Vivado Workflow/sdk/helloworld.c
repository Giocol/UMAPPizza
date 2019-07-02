#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "xparameters.h"
#include "xnn_descent.h"
#include "signals.h"
#include "xuartps.h"

#include <stdlib.h>
#include <limits.h> //for INT_MAX
#include "xtime_l.h"
#include "headers.h"
#include "signals.h"


int main()
{
    init_platform();

    XTime begin;
    XTime end;

    print("Hello World\n\r");

    ///////////////////////////////////////////////

    int *data = data_test;
	int rng_state[]= {7,456,99};
	int *leaf_array = leaf_array_test;
	int i;
	int j;
	int *result_hw_index =(int*) malloc(N_POINTS * K_NEIGHBORS * sizeof(int));
	int *result_hw_dist = (int*) malloc(N_POINTS * K_NEIGHBORS * sizeof(int));

	////////////////////////////////////////////////////////////////////////////


	XNn_descent myNnDescent;

	printf("Comincia il divertimento!\n");

	XTime_GetTime((XTime *) &begin);

	//printf("1\n");

	XNn_descent_Initialize(&myNnDescent, XPAR_NN_DESCENT_0_DEVICE_ID);
	//printf("2\n");
	XNn_descent_Set_data_in(&myNnDescent,(u32) data);
	//printf("3\n");
	XNn_descent_Set_rng_state_in(&myNnDescent,(u32) rng_state);
	//printf("4\n");
	XNn_descent_Set_leaf_array_in(&myNnDescent,(u32) leaf_array);
	//printf("5"\n");
	XNn_descent_Set_result_out_index(&myNnDescent,(u32) result_hw_index);
	//printf("6\n");
	XNn_descent_Set_result_out_dist(&myNnDescent,(u32) result_hw_dist);
	//printf("Prima While!\n");


	while(!XNn_descent_IsReady(&myNnDescent)){

	}

	//printf("Dopo While!\n");

	XNn_descent_Start(&myNnDescent);

	//printf("Start Sent!\n");

	while(!XNn_descent_IsDone(&myNnDescent)){

	}



	//printf("Getting Results :)\n");

	result_hw_index = (int*) XNn_descent_Get_result_out_index(&myNnDescent);
	result_hw_dist = (int*) XNn_descent_Get_result_out_dist(&myNnDescent);

	XTime_GetTime((XTime *) &end);

	printf("HW COMPUTATION ENDED!");

	/////////////////////////////////////////////////////////////////////////////

	int N_PRINT=10;
	for(i=N_POINTS-N_PRINT; i<N_POINTS; i++){
		printf("Considering the point: idx:%d ", i);
		for(int k=0; k<N_FEATURES; k++){
			printf("%c:%d ", 'A'+k, data[i*N_FEATURES + k]);
		}
		printf("\n");
		for(j=0; j<K_NEIGHBORS; j++){
			printf("idx:%d ",result_hw_index[i*K_NEIGHBORS + j]);
			//printf("( ");
			//for(int k=0; k<N_FEATURES; k++){
			//    printf("%c:%d ", 'A'+k, data[result_hw_index[i*K_NEIGHBORS + j]* N_FEATURES +k]);
			//}
			//printf(")");
			printf("at distance %d", result_hw_dist[i*K_NEIGHBORS + j]);
			//printf(")at distance %g", sqrt(result_hw_dist[i*K_NEIGHBORS + j]));
			//printf(")at distance %lld", result_hw_dist[i*K_NEIGHBORS + j]);
			printf(" (from Python idx: %d, dist: %f)", knn_indices_test[i*K_NEIGHBORS + j], knn_dists_test[i*K_NEIGHBORS + j]*knn_dists_test[i*K_NEIGHBORS + j]);
			printf("\n");
		}
		printf("\n");
	}

	printf("Time FPGA(including memory): %f, Time Python: %f", 1.0 * (end - begin) / (COUNTS_PER_SECOND), time_python);

	printf("PRINT ENDED!");

    cleanup_platform();

    return 0;
}
