import numpy as np
import pandas as pd
import math
from typing import List, Tuple
from statistics import mean
from itertools import product

###### class and function definition

class Cluster():
    def __init__(self, subClusters:List=None, singleVal:float=float("NaN")):
        self._sub_clusters = subClusters # a list of cluster objects
        # singleVal: index of the data point
        self._single_value = singleVal # only assign a value when it's a one-value cluster
    
    def get_all_values_indices(self): # return a list with all indices
        if self._sub_clusters == None:
            return [self._single_value]
        else:
            list_to_return = []
            for cluster in self._sub_clusters:
                list_to_return += cluster.get_all_values_indices()
            return list_to_return
        
    def get_all_subclusters(self):
        if self._sub_clusters == None:
            return [self._single_value]
        else:
            return [ c.get_all_subclusters() for c in self._sub_clusters ]

def euclidean_distance(p1,p2): # p1, p2: 2 different data points with same shape
    if p1.shape[0] != p2.shape[0]: raise Exception("Input data points have different shapes.")
    dist_sq_sum = 0
    for i in range(p1.shape[0]):
        dist_sq_sum += np.square(p1[i] - p2[i])
    return np.sqrt(dist_sq_sum)

# the average linkage distance between 2 clusters
def avgLinkage_distance(c1:Cluster, c2:Cluster, data):
    data_indices1 = c1.get_all_values_indices()
    data_indices2 = c2.get_all_values_indices()
    distance_vals = []
    for i1 in data_indices1:
        for i2 in data_indices2:
            distance_vals.append( euclidean_distance( data[i1,], data[i2,] ) )
    return mean(distance_vals)

def get_idx_of_min(distance_matrix):
    current_non0_min = np.inf
    min_idx = (0,0) # return (0,0) if only data are in one cluster
    for i, j in product(range(distance_matrix.shape[0]), range(distance_matrix.shape[1])):
        if distance_matrix[i,j] > 0 and distance_matrix[i,j] < current_non0_min:
            current_non0_min = distance_matrix[i,j]
            min_idx = (i, j)
    return min_idx

# need to merge clusters first
def update_dist_matrix(min_idx_tuple: Tuple[int], dist_mtrx,
                       idx_key_list, cluster_dict, data):
    if len(min_idx_tuple) != 2: raise Exception("Size of the index tuple is not 2.")
    
    min_ij = min(min_idx_tuple)
    max_ij = max(min_idx_tuple)
    
    dist_mtrx = np.delete(dist_mtrx, obj=max_ij, axis=0)
    dist_mtrx = np.delete(dist_mtrx, obj=max_ij, axis=1)
    
    for v in range(len(dist_mtrx[min_ij,:])):
        if v != min_ij:
            clus_1 = cluster_dict[idx_key_list[min_ij]]
            clus_2 = cluster_dict[idx_key_list[v]]
            update_dist = avgLinkage_distance(clus_1, clus_2, data)
            dist_mtrx[min_ij, v] = update_dist
            dist_mtrx[v, min_ij] = update_dist
    return dist_mtrx
        
        
def merge_clusters(min_idx_tuple: Tuple[int],
                       idx_key_list, cluster_dict, data):
    if len(min_idx_tuple) != 2: raise Exception("Size of the index tuple is not 2.")
    idx_i = min(min_idx_tuple)
    idx_j = max(min_idx_tuple)
    
    key_i = idx_key_list[idx_i]
    key_j = idx_key_list[idx_j]
    key_newCluster = key_i + '_' + key_j
    cluster_dict[ key_newCluster ] = Cluster([cluster_dict[key_i], cluster_dict[key_j]])
    del cluster_dict[key_i], cluster_dict[key_j]
    
    idx_key_list[idx_i] = key_newCluster
    del idx_key_list[idx_j]
    
    return idx_key_list, cluster_dict
###### class and function definition


# generating data
x1 = np.random.normal(loc=np.random.uniform(size=(5,))*10-5,size=(10,5))
x2 = np.random.normal(loc=np.random.uniform(size=(5,))*10-5,size=(10,5))
x3 = np.random.normal(loc=np.random.uniform(size=(5,))*10-5,size=(10,5))
x = np.vstack((x1,x2,x3)) # 1500 data points and 5 variables
shuffle = np.random.permutation(x.shape[0])
x = x[shuffle,:]

# construct the distance matrix
dist_matrix = np.full((x.shape[0], x.shape[0]), -1.0)
for i, j in product(range(dist_matrix.shape[0]), range(dist_matrix.shape[1])):
    if i != j:
        dist_matrix[i,j] = euclidean_distance(x[i,], x[j,])
    else:
        dist_matrix[i,j] = 0

# key: indices of data points in this cluster
# value: the cluster object
cluster_dict = {}
for i in range(x.shape[0]):
    cluster_dict[str(i)] = Cluster(None, i)  

# list of keys of cluster in another dictionary
# the indices of this list correspond to the indices of distance matrix
idx_to_clusterKey = list(cluster_dict.keys())

# when there are more than one cluster
while len(cluster_dict) > 1:
    min_index = get_idx_of_min(dist_matrix)
    idx_to_clusterKey, cluster_dict = merge_clusters(min_index, idx_to_clusterKey,
                                                     cluster_dict, x)
    dist_matrix = update_dist_matrix(min_index, dist_matrix, idx_to_clusterKey, 
                                     cluster_dict, x)
    # check how clusters merge
    print(cluster_dict.keys(),'\n')

# print the final cluster in layers of list
print(cluster_dict[list(cluster_dict.keys())[0]].get_all_subclusters())