from pathlib import Path
from time import time

import numpy as np
import umap
from sklearn.datasets import load_digits, fetch_mldata, fetch_openml
from umap.nndescent import (
    make_initialisations,
    make_initialized_nnd_search,
    initialise_search,
)
from umap.utils import deheap_sort
from umap.umap_ import (
    INT32_MAX,
    INT32_MIN,
    make_forest,
    rptree_leaf_array,
    nearest_neighbors,
    smooth_knn_dist,
    fuzzy_simplicial_set,
    UMAP,
)

import math

SET_VERBOSE=0

#random generated data boundaries
MIN_N = 0
MAX_N = 10000

#project-wide defines
MAX_CANDIDATES=30 #default = 60
RHO=0.5 #default
#N_ITERS generated automatically
DELTA=0.001 #default = 0.001
N_STATES=3 #default = 3
N_POINTS=2500
N_FEATURES=5
K_NEIGHBORS=50
L2_K_NEIGHBORS= math.ceil(math.log2(K_NEIGHBORS)) #l2(K_NEIGHBORS+1), rounded up
# N_LEAF=N_POINTS generated automatically
#LEAF_SIZE generated automatically
RP_TREE_INIT=1

# create a Random Generated data Array
data = np.random.randint(MIN_N, MAX_N, size=(N_POINTS, N_FEATURES))
rng_state = np.random.randint(INT32_MIN, INT32_MAX, 3).astype(np.int64)

start = time()
(knn_indices, knn_dists, leaf_array, N_ITERS) = nearest_neighbors(data, K_NEIGHBORS, "euclidean", {}, False, rng_state)
end = time()

LEAF_SIZE = leaf_array.shape[1]
N_LEAF = leaf_array.shape[0]

if SET_VERBOSE:
    for i in range(N_POINTS):
            print ("Considering the point: idx: " + str(i), end="")
            for k in range(N_FEATURES):
                feature = chr(ord('A') + k)
                print (feature + ":" + str(data[i][k]) + " ", end="")
            print ("\n", end="")
            for j in range(K_NEIGHBORS):
                print ("idx:" + str(knn_indices[i][j]) + "(", end="")
                for k in range(N_FEATURES):
                    feature = chr(ord('A') + k)
                    print (feature + ":" + str(data[j][k]) + " ", end="")
                print (")at distance " + str(knn_dists[i][j]) + "\n", end="")
            print ("\n", end="")

# save the current defines in a file
defines_text = """#define MAX_DIST INT_MAX
#define MAX_CANDIDATES {}
#define RHO {}
#define N_ITERS {}
#define DELTA {}
#define N_STATES {}

#define N_POINTS {}
#define N_FEATURES {}
#define K_NEIGHBORS {}
#define L2_K_NEIGHBORS {} // l2(K_NEIGHBORS+1), rounded up

#define N_LEAF {}
#define LEAF_SIZE {}

#define RP_TREE_INIT {}""".format(MAX_CANDIDATES, RHO, N_ITERS, DELTA, N_STATES, N_POINTS,N_FEATURES, K_NEIGHBORS, L2_K_NEIGHBORS, N_LEAF, LEAF_SIZE, RP_TREE_INIT)


def array_to_text(type, name, data):
    text_data = "{} {}[] =".format(type,name) + " {\n"
    for i in range(data.shape[0]):
        for j in range(data.shape[1]):
            text_data += str(data[i][j]) + ", "
        text_data += "\n"
    text_data += "};"
    return text_data

data_test= array_to_text("int","data_test", data)
knn_indices_test= array_to_text("int","knn_indices_test", knn_indices)
knn_dists_test= array_to_text("double", "knn_dists_test", knn_dists)
leaf_array_test= array_to_text("int", "leaf_array_test", leaf_array)

if SET_VERBOSE:
    print("DATA:")
    print(data_test)
    print("TEXT:")
    print(defines_text)
    print("KNN_INDICES")
    print(knn_indices_test)
    print("KNN_DISTS:")
    print(knn_dists_test)
    print("LEAF_ARRAY:")
    print(leaf_array_test)

file = open("./signals.h", "w")
file.write("#ifndef NNDESCENT_SIGNALS_H\n#define NNDESCENT_SIGNALS_H\n")
for array in [data_test, knn_indices_test, knn_dists_test, leaf_array_test]:
    file.write(array)
    file.write("\n\n")
file.write("double time_python={};\n".format(end-start))
file.write("#endif //NNDESCENT_SIGNALS_H\n")

defines = open("./defines.h", "w")
defines.write(defines_text)

print("Python_time: " + str(end-start))
