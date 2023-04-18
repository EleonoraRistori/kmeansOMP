//
// Created by eleri on 12/04/2023.
//

#ifndef KMEANSOMP_S_KMEANS_H
#define KMEANSOMP_S_KMEANS_H

void initialize_centroids(int data_point_dim, float* centroids, float *data_points, int k, float threshold){
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



void calculate_distance_s(float* data_points, float* centroids, float* distances, int num_data_points, int num_centroids, int data_point_dim) {
    for(int data_point_id=0; data_point_id<num_data_points; data_point_id++){
        for (int centroid_id = 0; centroid_id < num_centroids; centroid_id++) {
            float distance = 0;
            for (int dim = 0; dim < data_point_dim; dim++) {
                float diff = data_points[data_point_id * data_point_dim + dim] - centroids[centroid_id * data_point_dim + dim];
                distance += diff * diff;
            }
            distances[data_point_id * num_centroids + centroid_id] = distance;
        }
    }

}

void assign_cluster_s(int* cluster_assignment, float* data_points, float* centroids, int num_data_points, int num_centroids, int data_point_dim) {
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
}

void calculate_centroid_s(float* data_points, int* cluster_assignment, float* centroids, float* old_centroids, int num_data_points, int num_centroids, int data_point_dim) {
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

}


float centroid_distance_s(float* o_centroids, float* n_centroids, int k, int data_point_dim){
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

void kmeans_s(float* data_points, float* centroids, int* cluster_assignment, int num_data_points, int data_point_dim, int num_centroids, int max_iterations, float tolerance) {

    //float* distances = new float[num_data_points * num_centroids];
    float* old_centroids = new float[num_centroids * data_point_dim];
    // Initialize centroids randomly
    initialize_centroids(data_point_dim, centroids, data_points, num_centroids, 100);


    // Main loop
    int iteration = 0;
    do {

        //calculate_distance_s(data_points, centroids, distances, num_data_points, num_centroids, data_point_dim);

        // Assign each data point to the nearest centroid
        assign_cluster_s(cluster_assignment, data_points, centroids, num_data_points, num_centroids, data_point_dim);


        calculate_centroid_s(data_points, cluster_assignment, centroids, old_centroids, num_data_points, num_centroids, data_point_dim);


        iteration++;

    }while (iteration < max_iterations && centroid_distance_s(old_centroids, centroids, num_centroids, data_point_dim) > tolerance);

    //delete[] distances;
    delete[] old_centroids;
}




#endif //KMEANSOMP_S_KMEANS_H
