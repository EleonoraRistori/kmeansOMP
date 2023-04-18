#include <iostream>
#include <string>
#include <random>
#include <cstdlib>
#include <fstream>
#include <queue>
#include <chrono>
#include "s_kmeans.h"
#include "p_kmeans.h"
#include "utils.h"



int main() {
    const int num_points=50000000;  // number of experiments
    const int data_point_dim = 2;
    int k = 5;
    const float sigma = 5;
    float *points;
    int* cluster_assignment;
    float *centroids;
    std::vector<std::chrono::milliseconds> sequentialDuration;
    std::vector<std::chrono::milliseconds> parallelDuration;


    //Test #threads
    points = new float[num_points*data_point_dim];
    cluster_assignment = new int[num_points];
    centroids = new float[k*data_point_dim];
    generateCluster(num_points, data_point_dim, k, sigma, points, centroids);
    //saveArray(num_points, data_point_dim, points, "points.csv");
    for(int num_threads = 1; num_threads <= 24; num_threads++){
        auto begin = std::chrono::high_resolution_clock::now();
        kmeans(points, centroids, cluster_assignment, num_points, data_point_dim, k, 20, sigma, num_threads);
        auto end = std::chrono::high_resolution_clock::now();
        parallelDuration.push_back(std::chrono::duration_cast<std::chrono::milliseconds>(end - begin));
        std::cout << num_threads;

    }
    //saveArray(num_points, data_point_dim, cluster_assignment, "cluster_assignment.csv"); //TODO use this to save cluster_assignment for graphic

    delete[] points;
    delete[] cluster_assignment;
    delete[] centroids;
    saveTime(23, parallelDuration, "p_time_n_threads.csv");
    parallelDuration.clear();

    //Test #points
    int num_threads = 12;
    centroids = new float[k*data_point_dim];

    int num_it = 0;
    for(int i = 5000; i <= 50000000; i = i*5){
        num_it++;
        points = new float[i*data_point_dim];
        cluster_assignment = new int[i];
        std::cout << i;
        generateCluster(i, data_point_dim, k, sigma, points, centroids);
        auto begin = std::chrono::high_resolution_clock::now();
        kmeans_s(points, centroids, cluster_assignment, i, data_point_dim, k, 20, sigma);
        auto end = std::chrono::high_resolution_clock::now();
        sequentialDuration.push_back(std::chrono::duration_cast<std::chrono::milliseconds>(end - begin));
        begin = std::chrono::high_resolution_clock::now();
        kmeans(points, centroids, cluster_assignment, i, data_point_dim, k, 20, sigma, num_threads);
        end = std::chrono::high_resolution_clock::now();
        parallelDuration.push_back(std::chrono::duration_cast<std::chrono::milliseconds>(end - begin));
        delete[] points;
        delete[] cluster_assignment;

    }
    delete[] centroids;
    saveTime(num_it, sequentialDuration, "time_n_points.csv");
    saveTime(num_it, parallelDuration, "p_time_n_points.csv");

    sequentialDuration.clear();
    parallelDuration.clear();



    //Test #cluster
    for(k = 2; k <= 20; k++){
        num_it++;
        points = new float[num_points*data_point_dim];
        cluster_assignment = new int[num_points];
        centroids = new float[k*data_point_dim];
        std::cout << k;
        generateCluster(num_points, data_point_dim, k, sigma, points, centroids);
        auto begin = std::chrono::high_resolution_clock::now();
        kmeans_s(points, centroids, cluster_assignment, num_points, data_point_dim, k, 20, sigma);
        auto end = std::chrono::high_resolution_clock::now();
        sequentialDuration.push_back(std::chrono::duration_cast<std::chrono::milliseconds>(end - begin));
        begin = std::chrono::high_resolution_clock::now();
        kmeans(points, centroids, cluster_assignment, num_points, data_point_dim, k, 20, sigma, num_threads);
        end = std::chrono::high_resolution_clock::now();
        parallelDuration.push_back(std::chrono::duration_cast<std::chrono::milliseconds>(end - begin));
        std::cout << k;
        delete[] points;
        delete[] cluster_assignment;
        delete[] centroids;

    }
    saveTime(19, sequentialDuration, "time_n_centroids.csv");
    saveTime(19, parallelDuration, "p_time_n_centroids.csv");

    return 0;
}