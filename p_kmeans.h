//
// Created by eleri on 12/04/2023.
//

#ifndef KMEANSOMP_P_KMEANS_H
#define KMEANSOMP_P_KMEANS_H

#include <iostream>
#include <string>
#include <random>
#include <cstdlib>
#include <fstream>
#include <queue>
#include <chrono>

void initialize_centroids_p(int data_point_dim, float* centroids, float *data_points, int k, float threshold){
    for(int dim=0; dim<data_point_dim; dim++){
        centroids[dim] = data_points[dim];
    }
    int num_assigned_centroids = 1;
    int point_id = 1;
    while(num_assigned_centroids != k){

        for(int dim=0; dim<data_point_dim; dim++){
            centroids[num_assigned_centroids*data_point_dim+dim] =  data_points[point_id*data_point_dim+dim];
        }
        int centroid_id = 0;
        float sum_distances;
        do {
            sum_distances = 0;
            for(int dim=0; dim<data_point_dim; dim++) {
                float diff = centroids[centroid_id*data_point_dim+dim] - centroids[num_assigned_centroids*data_point_dim+dim];
                sum_distances += diff*diff;
            }
            centroid_id++;
        }while(centroid_id < num_assigned_centroids && sum_distances > threshold);
        if(centroid_id == num_assigned_centroids){
            num_assigned_centroids++;
            point_id++;
        }
        else {
            point_id++;
        }
    }
}

float centroid_distance(float* o_centroids, float* n_centroids, int k, int data_point_dim){
    float distance = 0;
    float diff;
    for(int id = 0 ; id < k; id++){
        for(int dim=0; dim < data_point_dim; dim++){
            diff = o_centroids[id*data_point_dim+dim] - n_centroids[id*data_point_dim+dim];
            distance += diff * diff;
        }
    }
    return distance;
}

void kmeans(float* data_points, float* centroids, int* cluster_assignment, int num_data_points, int data_point_dim, int num_centroids, int max_iterations, float tolerance, int num_threads) {
    initialize_centroids_p(data_point_dim, centroids, data_points, num_centroids, 100);
    float* old_centroids = new float[num_centroids * data_point_dim];
    int iteration = 0;
    float distance = 0;
    do {
        // Assegnazione dei punti ai centroidi
#pragma omp parallel for num_threads(num_threads) default(none) firstprivate(num_data_points, data_point_dim, num_centroids, data_points, centroids) shared(cluster_assignment)
        for(int data_point_id=0; data_point_id<num_data_points; data_point_id++){
            float min_dist = INFINITY;
            int min_centroid_id = -1;
            for (int centroid_id = 0; centroid_id < num_centroids; centroid_id++) {
                float dist = 0;
                for (int dim = 0; dim < data_point_dim; dim++) {
                    float diff = data_points[data_point_id * data_point_dim + dim] - centroids[centroid_id * data_point_dim + dim];
                    dist += diff * diff;
                }
                if(dist < min_dist){
                    min_dist = dist;
                    min_centroid_id = centroid_id;
                }

            }
            cluster_assignment[data_point_id] = min_centroid_id;
        }

        // Ricalcolo dei centroidi
#pragma omp parallel for num_threads(num_threads) default(none) firstprivate(num_data_points, data_point_dim, num_centroids, data_points, cluster_assignment) shared(centroids, old_centroids)
        for(int centroid_id=0; centroid_id<num_centroids; centroid_id++){
            for (int dim = 0; dim < data_point_dim; dim++) {
                int num_points_assigned = 0;
                float sum = 0;
                for (int data_point_id = 0; data_point_id < num_data_points; data_point_id++) {
                    if (cluster_assignment[data_point_id] == centroid_id) {
                        sum += data_points[data_point_id * data_point_dim + dim];
                        num_points_assigned++;
                    }
                }
                if (num_points_assigned > 0) {
                    old_centroids[centroid_id * data_point_dim + dim] = centroids[centroid_id * data_point_dim + dim];
                    centroids[centroid_id * data_point_dim + dim] = sum / float(num_points_assigned);
                }
            }
        }

        iteration++;

    }while (iteration < max_iterations && centroid_distance(old_centroids, centroids, num_centroids, data_point_dim) > tolerance);
    delete[] old_centroids;
}

#endif //KMEANSOMP_P_KMEANS_H
