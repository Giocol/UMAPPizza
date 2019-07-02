//
// Created by Aaron (with a lot of Luca's work) on 9/7/19.
//

#include "headers.h"

//TODO
//Push a new element onto the heap.
// returns the number of new elements successfully pushed into the heap.
int heap_push(
    t_index current_graph_index[N_POINTS*K_NEIGHBORS], 
    t_distance current_graph_dist[N_POINTS*K_NEIGHBORS], 
    boolean current_graph_flag[N_POINTS*K_NEIGHBORS],
    int row,                                      //determines which data point we are addressing
    t_distance weight,                                 //determines the distance (for heap sorting)
    t_index index,                                    //is the element to add
    boolean flag                                     //determines whether this is to be considered a new addition
)
{
    //break if the new element is more distant than the actual max
    if (weight >= current_graph_dist[row*K_NEIGHBORS +0])
    {
        return 0;
    }

    int idx;
    //break if we already have this element
    for (idx = 0; idx < K_NEIGHBORS; idx++)
    {
        if (index == current_graph_index[row*K_NEIGHBORS +idx])
        {
            return 0;
        }
    }

    // insert val at position zero
    current_graph_dist[row*K_NEIGHBORS +0] = weight;
    current_graph_index[row*K_NEIGHBORS +0] = index;
    current_graph_flag[row*K_NEIGHBORS +0] = flag;

    // descend the current_graph, swapping values until the max heap criterion is met
    int i = 0;
    int i_swap; //temp index to swap with the i-th
    int ic1; //position 1
    int ic2; //position 2

    while (1)
    {
        ic1 = 2 * i + 1;
        ic2 = ic1 + 1;

        //determine whether the i-th element has to be swapped with one of his son
        if (ic1 >= K_NEIGHBORS)
            break;
        else if (ic2 >= K_NEIGHBORS)
        {    //if the distance in position ic1 is more than the distance passed to the function
            if (current_graph_dist[row*K_NEIGHBORS +ic1] > weight)
                i_swap = ic1; //temp equals to the position ic1
            else
                break;
        }
        //if the distance in position ic1 is >= of the distance in position ic2
        else if (current_graph_dist[row*K_NEIGHBORS +ic1] >= current_graph_dist[row*K_NEIGHBORS +ic2]) 
        {     //if the distance in position ic1 is more than the distance passed to the function
            if (weight < current_graph_dist[row*K_NEIGHBORS +ic1])
            {
                i_swap = ic1; //temp equals to the position ic1
            }
            else
            {
                break;
            }
        }
        else
        {     //if the distance in position ic2 is more than the distance passed to the function
            if (weight < current_graph_dist[row*K_NEIGHBORS +ic2])
                i_swap = ic2; //temp equals to the position ic2
            else
                break;
        }

        //do the swap
        current_graph_dist[row*K_NEIGHBORS +i] = current_graph_dist[row*K_NEIGHBORS +i_swap];
        current_graph_index[row*K_NEIGHBORS +i] = current_graph_index[row*K_NEIGHBORS +i_swap];
        current_graph_flag[row*K_NEIGHBORS +i] = current_graph_flag[row*K_NEIGHBORS +i_swap];

        i = i_swap;
    }
     //"i" has the last i_swap's value of the cycle 'while'
     //in that 'i' position the programm insert the values passed to the function
    current_graph_dist[row*K_NEIGHBORS +i] = weight;
    current_graph_index[row*K_NEIGHBORS +i] = index;
    current_graph_flag[row*K_NEIGHBORS +i] = flag;
    
    return 1; //The number of new elements successfully pushed into the heap.
}

void heap_push_opt(
    t_index current_graph_index[N_POINTS*K_NEIGHBORS], 
    t_distance current_graph_dist[N_POINTS*K_NEIGHBORS], 
    boolean current_graph_flag[N_POINTS*K_NEIGHBORS],
    int row,                                      //determines which data point we are addressing
    t_distance weight,                                 //determines the distance (for heap sorting)
    t_index index,                                    //is the element to add
    boolean flag,                                     //determines whether this is to be considered a new addition
    int *c
)
{   
    t_distance cur_elem_dist = weight;
    t_index cur_elem_index = index;
    boolean cur_elem_flag = flag;

    int current_element; //local max
    int current_son; //left son
    int current_daughter; //right son
    int idx1;
    int idx2;
    boolean found=0; //boolean 

    //do not insert first element if we already have this element
    for (idx1 = 0; idx1 < K_NEIGHBORS; idx1++)
    {
        if (index == current_graph_index[row*K_NEIGHBORS + idx1])
        {
            found= 1;
        }
    }

    //try to insert val in position 0
    if(!found && current_graph_dist[row*K_NEIGHBORS + 0] > weight){
        swap_t_index(&current_graph_index[row*K_NEIGHBORS + 0], &cur_elem_index);
        swap_t_distance(&current_graph_dist[row*K_NEIGHBORS + 0], &cur_elem_dist);
        swap_boolean(&current_graph_flag[row*K_NEIGHBORS + 0], &cur_elem_flag);
        *c = *c + 1;
    }

    //recreate the heap push (to have more regularity this is done the same number of times (ln(K_NEIGHBORS)) to have more regularity)
    current_element = 0;
    current_son = 1;
    current_daughter = 2;
    //while (current_son < K_NEIGHBORS) //not unrollable
    for(int i=0; i< L2_K_NEIGHBORS-1; i++) //unrollable //arrive till the level before the last (so current element is always < K_NEIGHBOR)
    {
        if(current_daughter<K_NEIGHBORS 
            && current_graph_dist[row*K_NEIGHBORS + current_daughter] > current_graph_dist[row*K_NEIGHBORS + current_son] 
            && current_graph_dist[row*K_NEIGHBORS + current_daughter] > current_graph_dist[row*K_NEIGHBORS + current_element]){

            idx1 = current_daughter;
            idx2 = current_element;
            current_element = current_daughter;
            current_son = current_daughter*2+1;
            current_daughter = current_son+1;
        }
        else if(current_son<K_NEIGHBORS 
            &&current_graph_dist[row*K_NEIGHBORS + current_son] > current_graph_dist[row*K_NEIGHBORS + current_element]){
                
            idx1 = current_son;
            idx2 = current_element;
            current_element = current_son;
            current_son = current_son*2+1;
            current_daughter = current_son +1;
        }
        else //fake case in which no swap is to be made (current element is a leaf or no child is more distant than current element)
        {
            idx1 = current_element;
            idx2 = current_element;
            current_element = current_son;
            current_son = current_son*2+1;
            current_daughter = current_son+1;
        }
        swap_t_index(&current_graph_index[row*K_NEIGHBORS + idx1], &current_graph_index[row*K_NEIGHBORS + idx2]);
        swap_t_distance(&current_graph_dist[row*K_NEIGHBORS + idx1], &current_graph_dist[row*K_NEIGHBORS + idx2]);
        swap_boolean(&current_graph_flag[row*K_NEIGHBORS + idx1], &current_graph_flag[row*K_NEIGHBORS + idx2]);
    }

}

//Push a new element onto the heap.
// returns the number of new elements successfully pushed into the heap.
int heap_push_candidate(
    t_index current_graph_index[N_POINTS*MAX_CANDIDATES], 
    t_distance current_graph_dist[N_POINTS*MAX_CANDIDATES], 
    boolean current_graph_flag[N_POINTS*MAX_CANDIDATES],
    int row,                                      //determines which data point we are addressing
    t_distance weight,                                 //determines the distance (for heap sorting)
    t_index index,                                    //is the element to add
    boolean flag                                     //determines whether this is to be considered a new addition
)
{
    //break if the new element is more distant than the actual max
    if (weight >= current_graph_dist[row*MAX_CANDIDATES +0])
    {
        return 0;
    }

    int idx;
    //break if we already have this element
    for (idx = 0; idx < MAX_CANDIDATES; idx++)
    {
        if (index == current_graph_index[row*MAX_CANDIDATES +idx])
        {
            return 0;
        }
    }

    // insert val at position zero
    current_graph_dist[row*MAX_CANDIDATES +0] = weight;
    current_graph_index[row*MAX_CANDIDATES +0] = index;
    current_graph_flag[row*MAX_CANDIDATES +0] = flag;

    // descend the current_graph, swapping values until the max heap criterion is met
    int i = 0;
    int i_swap; //temp index to swap with the i-th
    int ic1; //position 1
    int ic2; //position 2

    while (1)
    {
        ic1 = 2 * i + 1;
        ic2 = ic1 + 1;

        //determine whether the i-th element has to be swapped with one of his son
        if (ic1 >= MAX_CANDIDATES)
            break;
        else if (ic2 >= MAX_CANDIDATES)
        {    //if the distance in position ic1 is more than the distance passed to the function
            if (current_graph_dist[row*MAX_CANDIDATES +ic1] > weight)
                i_swap = ic1; //temp equals to the position ic1
            else
                break;
        }
        //if the distance in position ic1 is >= of the distance in position ic2
        else if (current_graph_dist[row*MAX_CANDIDATES +ic1] >= current_graph_dist[row*MAX_CANDIDATES +ic2]) 
        {     //if the distance in position ic1 is more than the distance passed to the function
            if (weight < current_graph_dist[row*MAX_CANDIDATES +ic1])
            {
                i_swap = ic1; //temp equals to the position ic1
            }
            else
            {
                break;
            }
        }
        else
        {     //if the distance in position ic2 is more than the distance passed to the function
            if (weight < current_graph_dist[row*MAX_CANDIDATES +ic2])
                i_swap = ic2; //temp equals to the position ic2
            else
                break;
        }

        //do the swap
        current_graph_dist[row*MAX_CANDIDATES +i] = current_graph_dist[row*MAX_CANDIDATES +i_swap];
        current_graph_index[row*MAX_CANDIDATES +i] = current_graph_index[row*MAX_CANDIDATES +i_swap];
        current_graph_flag[row*MAX_CANDIDATES +i] = current_graph_flag[row*MAX_CANDIDATES +i_swap];

        i = i_swap;
    }
     //"i" has the last i_swap's value of the cycle 'while'
     //in that 'i' position the programm insert the values passed to the function
    current_graph_dist[row*MAX_CANDIDATES +i] = weight;
    current_graph_index[row*MAX_CANDIDATES +i] = index;
    current_graph_flag[row*MAX_CANDIDATES +i] = flag;
    
    return 1; //The number of new elements successfully pushed into the heap.
}
// return the number of new elements successfully pushed into the heap.


/*Given an array of t-heap-neighbor (current_heap), unpack the heap
    out to give and array of sorted lists of indices and weights by increasing
    weight. This is effectively just the second half of heap sort (the first
    half not being required since we already have the data in a heap).*/
void deheap_sort(
    t_index current_graph_index[N_POINTS * K_NEIGHBORS], 
    t_distance current_graph_dist[N_POINTS * K_NEIGHBORS], 
    boolean current_graph_flag[N_POINTS * K_NEIGHBORS], 
    t_index result_index[N_POINTS*K_NEIGHBORS], 
    t_distance result_dist[N_POINTS*K_NEIGHBORS]
){    

    for(int n_point=0; n_point< N_POINTS; n_point++){

        for(int i_neighbor=0; i_neighbor < K_NEIGHBORS; i_neighbor++){  //TODO: per i_neighbor = K_NEIGHBORS - 1 (ultima iterazione), questo fa schifo
            //swap the maximum with the last element and rebuild the heap
            //so the next iteration the current maximum will be ignored (already at its place)
            swap_t_index(&current_graph_index[n_point*K_NEIGHBORS + 0], &current_graph_index[n_point*K_NEIGHBORS + K_NEIGHBORS-1-i_neighbor]);
            swap_t_distance(&current_graph_dist[n_point*K_NEIGHBORS + 0], &current_graph_dist[n_point*K_NEIGHBORS + K_NEIGHBORS-1-i_neighbor]);
            swap_boolean(&current_graph_flag[n_point*K_NEIGHBORS + 0], &current_graph_flag[n_point*K_NEIGHBORS + K_NEIGHBORS-1-i_neighbor]);

            //Restore the heap property for a heap with an out of place element in position 0
            siftdown(current_graph_index,current_graph_dist,current_graph_flag, K_NEIGHBORS-1-i_neighbor , n_point, 0);
            
            //put the values of the current_heap inside the result in the same position
            result_dist[n_point*K_NEIGHBORS + K_NEIGHBORS-1-i_neighbor] = current_graph_dist[n_point*K_NEIGHBORS + K_NEIGHBORS-1-i_neighbor];
            result_index[n_point*K_NEIGHBORS + K_NEIGHBORS-1-i_neighbor] = current_graph_index[n_point*K_NEIGHBORS + K_NEIGHBORS-1-i_neighbor];
        }        
    }
}
//Restore the heap property for a heap with an out of place element in position elt
void siftdown(t_index current_graph_index[N_POINTS * K_NEIGHBORS], 
    t_distance current_graph_dist[N_POINTS * K_NEIGHBORS], 
    boolean current_graph_flag[N_POINTS * K_NEIGHBORS],
    int size_heap,
    int row,
    int elt){

    int left_child;
    int right_child;
    int swap; //temp for the swapping

    while(elt*2+1 < size_heap){
        left_child = elt *2+1;
        right_child= left_child+1;
        swap = elt; //temp equals to the position of the out of place element
        /*if the dist value of the current_heap in position 'swap' is less than the value in position 'left_child'
         ==> swap equals to the 'left_child'*/
        if(current_graph_dist[row*K_NEIGHBORS + swap] < current_graph_dist[row*K_NEIGHBORS + left_child]){
            swap = left_child;
        }
        /*if right_child is less than size_heap && 
        if the dist value of the current_heap in position 'swap' is less than the value in position 'right_child'
         ==> swap equals to the 'right_child'*/
        if (right_child < size_heap && current_graph_dist[row*K_NEIGHBORS + swap] < current_graph_dist[row*K_NEIGHBORS + right_child]){
            swap = right_child;
        }
        //if the swap value is equal to the 'elt' value passed to the function ==> break
        if(swap == elt){
            break;
        }
        else{
            //call a function by address for the swapping 
            swap_t_index(&current_graph_index[row*K_NEIGHBORS + elt], &current_graph_index[row*K_NEIGHBORS + swap]);
            swap_t_distance(&current_graph_dist[row*K_NEIGHBORS + elt], &current_graph_dist[row*K_NEIGHBORS + swap]);
            swap_boolean(&current_graph_flag[row*K_NEIGHBORS + elt], &current_graph_flag[row*K_NEIGHBORS + swap]);
            
            elt = swap;
        }
    }
}

void swap_t_index(t_index *a, t_index *b){
    t_index temp;

    temp = *a;
    *a = *b;
    *b = temp;
}

void swap_t_distance(t_distance *a, t_distance *b){
    t_distance temp;

    temp = *a;
    *a = *b;
    *b = temp;
}

void swap_boolean(boolean *a, boolean *b){
    boolean temp;

    temp = *a;
    *a = *b;
    *b = temp;
}

void swap_int(int *a, int *b){
    int temp;

    temp = *a;
    *a = *b;
    *b = temp;
}
