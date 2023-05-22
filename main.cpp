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
    const int num_points=5000000;  // number of experiments
    const int data_point_dim = 2;
    int num_threads;
    int k = 50;
    const float sigma = 5;
    float *points;
    int* cluster_assignment;
    float *centroids;
    std::vector<std::chrono::milliseconds> sequentialDuration;
    std::vector<std::chrono::milliseconds> parallelDuration;

    int num_it = 0;

//    //Test #threads
//    k = 50;
//    points = new float[num_points*data_point_dim];
//    cluster_assignment = new int[num_points];
//    centroids = new float[k*data_point_dim];
//    generateCluster(num_points, data_point_dim, k, sigma, points, centroids);
//    //saveArray(num_points, data_point_dim, points, "points.csv");
//    for(num_threads = 1; num_threads <= 24; num_threads++){
//        auto begin = std::chrono::high_resolution_clock::now();
//        kmeans_s(points, centroids, cluster_assignment, num_points, data_point_dim, k, 20, sigma, sigma);
//        auto end = std::chrono::high_resolution_clock::now();
//        parallelDuration.push_back(std::chrono::duration_cast<std::chrono::milliseconds>(end - begin));
//        std::cout << num_threads << "\n";
//        std::cout << parallelDuration[num_threads-1].count() << "\n";
//    }
//    //saveArray(num_points, data_point_dim, cluster_assignment, "cluster_assignment.csv");
//    delete[] points;
//    delete[] cluster_assignment;
//    delete[] centroids;
//    //saveTime(24, parallelDuration, "p_time_n_threads_v2.csv");
//    parallelDuration.clear();

    //Test #points
//    num_threads = 12;
//    centroids = new float[k*data_point_dim];
//    for(int i = 5; i <= 200; i=i+5){
//        num_it++;
//        int num_data_points = i*1000000;
//        points = new float[num_data_points*data_point_dim];
//        cluster_assignment = new int[num_data_points];
//        std::cout << i << "\n";
//        generateCluster(num_data_points, data_point_dim, k, sigma, points, centroids);
//        auto begin = std::chrono::high_resolution_clock::now();
//        kmeans_s(points, centroids, cluster_assignment, num_data_points, data_point_dim, k, 20, sigma, sigma);
//        auto end = std::chrono::high_resolution_clock::now();
//        sequentialDuration.push_back(std::chrono::duration_cast<std::chrono::milliseconds>(end - begin));
//        begin = std::chrono::high_resolution_clock::now();
//        kmeans_p2(points, centroids, cluster_assignment, num_data_points, data_point_dim, k, 20, sigma, num_threads);
//        end = std::chrono::high_resolution_clock::now();
//        parallelDuration.push_back(std::chrono::duration_cast<std::chrono::milliseconds>(end - begin));
//        std::cout << sequentialDuration[num_it-1].count() << "\n";
//        std::cout << parallelDuration[num_it-1].count() << "\n";
//        delete[] points;
//        delete[] cluster_assignment;
//
//    }
//    delete[] centroids;
//    saveTime(num_it, sequentialDuration, "time_n_points_s2.csv");
//    saveTime(num_it, parallelDuration, "time_n_points_p2.csv");
//
//    sequentialDuration.clear();
//    parallelDuration.clear();
//
//
//
    //Test #cluster
    num_threads = 12;
    for(k = 10; k <= 20010; k=k+2000){
        num_it++;
        points = new float[num_points*data_point_dim];
        cluster_assignment = new int[num_points];
        centroids = new float[k*data_point_dim];
        std::cout << k << "\n";
        generateCluster(num_points, data_point_dim, k, sigma, points, centroids);
        auto begin = std::chrono::high_resolution_clock::now();
        kmeans_s(points, centroids, cluster_assignment, num_points, data_point_dim, k, 20, sigma, sigma);
        auto end = std::chrono::high_resolution_clock::now();
        sequentialDuration.push_back(std::chrono::duration_cast<std::chrono::milliseconds>(end - begin));
        begin = std::chrono::high_resolution_clock::now();
        kmeans_p2(points, centroids, cluster_assignment, num_points, data_point_dim, k, 20, sigma, num_threads);
        end = std::chrono::high_resolution_clock::now();
        parallelDuration.push_back(std::chrono::duration_cast<std::chrono::milliseconds>(end - begin));
        std::cout << sequentialDuration[num_it-1].count() << "\n";
        std::cout << parallelDuration[num_it-1].count() << "\n";
        delete[] points;
        delete[] cluster_assignment;
        delete[] centroids;

    }
    saveTime(num_it, sequentialDuration, "time_n_centroids_s.csv");
    saveTime(num_it, parallelDuration, "time_n_centroids_p2.csv");

//    //Test 2 versione parallela
//    centroids = new float[k*data_point_dim];
//    k = 100;
//    num_threads = 12;
//    for(int i = 5; i <= 200; i=i+5){
//        num_it++;
//        int num_data_points = i*100000; //tolto uno 0
//        points = new float[num_data_points*data_point_dim];
//        cluster_assignment = new int[num_data_points];
//        std::cout << i << "\n";
//        generateCluster(num_data_points, data_point_dim, k, sigma, points, centroids);
//        auto begin = std::chrono::high_resolution_clock::now();
//        kmeans_s(points, centroids, cluster_assignment, num_data_points, data_point_dim, k, 20, sigma, sigma);
//        auto end = std::chrono::high_resolution_clock::now();
//        sequentialDuration.push_back(std::chrono::duration_cast<std::chrono::milliseconds>(end - begin));
//        begin = std::chrono::high_resolution_clock::now();
//        kmeans_p2(points, centroids, cluster_assignment, num_data_points, data_point_dim, k, 20, sigma, num_threads);
//        end = std::chrono::high_resolution_clock::now();
//        parallelDuration.push_back(std::chrono::duration_cast<std::chrono::milliseconds>(end - begin));
//        std::cout << sequentialDuration[num_it-1].count() << "\n";
//        std::cout << parallelDuration[num_it-1].count() << "\n";
//        delete[] points;
//        delete[] cluster_assignment;
//    }
//    saveTime(num_it, sequentialDuration, "time_kmeans_s.csv");
//    saveTime(num_it, parallelDuration, "time_kmeans_p2.csv");

//    //Test #cluster 2 versioni parallele
//    num_threads = 12;
//    for(k = 2; k <= 100; k=k+2){
//        num_it++;
//        points = new float[num_points*data_point_dim];
//        cluster_assignment = new int[num_points];
//        centroids = new float[k*data_point_dim];
//        std::cout << k << "\n";
//        generateCluster(num_points, data_point_dim, k, sigma, points, centroids);
//        auto begin = std::chrono::high_resolution_clock::now();
//        kmeans(points, centroids, cluster_assignment, num_points, data_point_dim, k, 20, sigma, num_threads);
//        auto end = std::chrono::high_resolution_clock::now();
//        sequentialDuration.push_back(std::chrono::duration_cast<std::chrono::milliseconds>(end - begin));
//        begin = std::chrono::high_resolution_clock::now();
//        kmeans_p2(points, centroids, cluster_assignment, num_points, data_point_dim, k, 20, sigma, num_threads);
//        end = std::chrono::high_resolution_clock::now();
//        parallelDuration.push_back(std::chrono::duration_cast<std::chrono::milliseconds>(end - begin));
//        std::cout << sequentialDuration[num_it-1].count() << "\n";
//        std::cout << parallelDuration[num_it-1].count() << "\n";
//        delete[] points;
//        delete[] cluster_assignment;
//        delete[] centroids;
//
//    }
//    saveTime(num_it, sequentialDuration, "time_kmeans_p_cluster.csv");
//    saveTime(num_it, parallelDuration, "time_kmeans_p2_cluster.csv");
    return 0;
}