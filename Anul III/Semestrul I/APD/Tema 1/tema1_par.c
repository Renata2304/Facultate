// Author: APD team, except where source was noted

#include "helpers.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define CONTOUR_CONFIG_COUNT    16
#define FILENAME_MAX_SIZE       50
#define STEP                    8
#define SIGMA                   200
#define RESCALE_X               2048
#define RESCALE_Y               2048

#define CLAMP(v, min, max) if(v < min) { v = min; } else if(v > max) { v = max; }

typedef struct{
    int id;
    int num_threads;
    ppm_image* image;
    ppm_image* scaled_image;
} thread;

// int min(double x, double y) {
//     return x < y ? x : y;
// }

 #define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

// Creates a map between the binary configuration (e.g. 0110_2) and the corresponding pixels
// that need to be set on the output image. An array is used for this map since the keys are
// binary numbers in 0-15. Contour images are located in the './contours' directory.
ppm_image** init_contour_map() {
    ppm_image** map = (ppm_image**)malloc(CONTOUR_CONFIG_COUNT * sizeof(ppm_image*));
    if (!map) {
        fprintf(stderr, "Unable to allocate memory\n");
        exit(1);
    }

    for (int i = 0; i < CONTOUR_CONFIG_COUNT; i++) {
        char filename[FILENAME_MAX_SIZE];
        sprintf(filename, "./contours/%d.ppm", i);
        map[i] = read_ppm(filename);
    }

    return map;
}

// Updates a particular section of an image with the corresponding contour pixels.
// Used to create the complete contour image.
void update_image(ppm_image* image, ppm_image* contour, int x, int y) {
    for (int i = 0; i < contour->x; i++) {
        for (int j = 0; j < contour->y; j++) {
            int contour_pixel_index = contour->x * i + j;
            int image_pixel_index = (x + i) * image->y + y + j;

            image->data[image_pixel_index].red = contour->data[contour_pixel_index].red;
            image->data[image_pixel_index].green = contour->data[contour_pixel_index].green;
            image->data[image_pixel_index].blue = contour->data[contour_pixel_index].blue;
        }
    }
}

// Corresponds to step 1 of the marching squares algorithm, which focuses on sampling the image.
// Builds a p x q grid of points with values which can be either 0 or 1, depending on how the
// pixel values compare to the `sigma` reference value. The points are taken at equal distances
// in the original image, based on the `step_x` and `step_y` arguments.
unsigned char** sample_grid(ppm_image* image, int step_x, int step_y, unsigned char sigma) {
    int p = image->x / step_x;
    int q = image->y / step_y;

    unsigned char** grid = (unsigned char**)malloc((p + 1) * sizeof(unsigned char*));
    if (!grid) {
        fprintf(stderr, "Unable to allocate memory\n");
        exit(1);
    }

    for (int i = 0; i <= p; i++) {
        grid[i] = (unsigned char*)malloc((q + 1) * sizeof(unsigned char));
        if (!grid[i]) {
            fprintf(stderr, "Unable to allocate memory\n");
            exit(1);
        }
    }

    for (int i = 0; i < p; i++) {
        for (int j = 0; j < q; j++) {
            ppm_pixel curr_pixel = image->data[i * step_x * image->y + j * step_y];

            unsigned char curr_color = (curr_pixel.red + curr_pixel.green + curr_pixel.blue) / 3;

            if (curr_color > sigma) {
                grid[i][j] = 0;
            }
            else {
                grid[i][j] = 1;
            }
        }
    }
    grid[p][q] = 0;

    // last sample points have no neighbors below / to the right, so we use pixels on the
    // last row / column of the input image for them
    for (int i = 0; i < p; i++) {
        ppm_pixel curr_pixel = image->data[i * step_x * image->y + image->x - 1];

        unsigned char curr_color = (curr_pixel.red + curr_pixel.green + curr_pixel.blue) / 3;

        if (curr_color > sigma) {
            grid[i][q] = 0;
        }
        else {
            grid[i][q] = 1;
        }
    }
    for (int j = 0; j < q; j++) {
        ppm_pixel curr_pixel = image->data[(image->x - 1) * image->y + j * step_y];

        unsigned char curr_color = (curr_pixel.red + curr_pixel.green + curr_pixel.blue) / 3;

        if (curr_color > sigma) {
            grid[p][j] = 0;
        }
        else {
            grid[p][j] = 1;
        }
    }

    return grid;
}

// Corresponds to step 2 of the marching squares algorithm, which focuses on identifying the
// type of contour which corresponds to each subgrid. It determines the binary value of each
// sample fragment of the original image and replaces the pixels in the original image with
// the pixels of the corresponding contour image accordingly.
void march(ppm_image* image, unsigned char** grid, ppm_image** contour_map, int step_x, int step_y) {
    int p = image->x / step_x;
    int q = image->y / step_y;

    for (int i = 0; i < p; i++) {
        for (int j = 0; j < q; j++) {
            unsigned char k = 8 * grid[i][j] + 4 * grid[i][j + 1] + 2 * grid[i + 1][j + 1] + 1 * grid[i + 1][j];
            update_image(image, contour_map[k], i * step_x, j * step_y);
        }
    }
}

// Calls `free` method on the utilized resources.
void free_resources(ppm_image* image, ppm_image** contour_map, unsigned char** grid, int step_x) {
    for (int i = 0; i < CONTOUR_CONFIG_COUNT; i++) {
        free(contour_map[i]->data);
        free(contour_map[i]);
    }
    free(contour_map);

    for (int i = 0; i <= image->x / step_x; i++) {
        free(grid[i]);
    }
    free(grid);

    free(image->data);
    free(image);
}

void* rescale_image_par(void* arg) {
    uint8_t sample[3];
    
    // getting the arguments from the thread
    int num_threads = ((thread*)arg)->num_threads;
    ppm_image* image = ((thread*)arg)->image;
    ppm_image* scaled_image = ((thread*)arg)->scaled_image;

    // calculating the start and the end (ab1)
    long id = ((thread*)arg)->id;
    int start = id * (double)scaled_image->x / num_threads;
    int end = min((id + 1) * (double)(scaled_image->x) / num_threads, 
                  scaled_image->x);

    // use bicubic interpolation for scaling
    for (int i = start; i < end; i++) {
        for (int j = 0; j < scaled_image->y; j++) {
            float u = (float)i / (float)(scaled_image->x - 1);
            float v = (float)j / (float)(scaled_image->y - 1);
            sample_bicubic(image, u, v, sample);

            scaled_image->data[i * scaled_image->y + j].red = sample[0];
            scaled_image->data[i * scaled_image->y + j].green = sample[1];
            scaled_image->data[i * scaled_image->y + j].blue = sample[2];
        }
    }

    return NULL;
}


int main(int argc, char* argv[]) {
    if (argc < 4) {
        fprintf(stderr, "Usage: ./tema1 <in_file> <out_file> <P>\n");
        return 1;
    }

    int step_x = STEP;
    int step_y = STEP;
    ppm_image *image = read_ppm(argv[1]);
    int num_threads =  atoi(argv[3]);
    thread thr[num_threads];
    pthread_t threads[num_threads];
    int r = 0;

    // 0. Initialize contour map
    ppm_image** contour_map = init_contour_map();
    
    // 1. Rescale the image
    ppm_image *scaled_image;

    // we only rescale downwards
    if (image->x <= RESCALE_X && image->y <= RESCALE_Y) {
        scaled_image = image;
    } else {
        // alloc space for the new rescaled image
        ppm_image* new_image = (ppm_image*)malloc(sizeof(ppm_image));
        if (!new_image) {
            fprintf(stderr, "Unable to allocate memory\n");
            exit(1);
        }

        new_image->x = RESCALE_X;
        new_image->y = RESCALE_Y;

        new_image->data = (ppm_pixel*)malloc(new_image->x * new_image->y * sizeof(ppm_pixel));
        if (!new_image) {
            fprintf(stderr, "Unable to allocate memory\n");
            exit(1);
        }

        // lab 1
       for (long id = 0; id < num_threads; id++) {
            // initialization of each thread
            thr[id].num_threads = num_threads;
            thr[id].id = id;
            thr[id].image = image;
            thr[id].scaled_image = new_image;

            r = pthread_create(&threads[id], NULL, rescale_image_par, &thr[id]);

            if (r) {
                printf("Eroare la crearea thread-ului %ld\n", id);
                exit(-1);
            }
        }

        for (long id = 0; id < num_threads; id++) {
            r = pthread_join(threads[id], NULL);

            if (r) {
                printf("Eroare la crearea thread-ului %ld\n", id);
                exit(-1);
            }
        }

        // free the memory that we don't use anymore
        free(image->data);
        free(image);

        scaled_image = new_image;
    }

    // 2. Sample the grid
    unsigned char** grid = sample_grid(scaled_image, step_x, step_y, SIGMA);

    // 3. March the squares
    march(scaled_image, grid, contour_map, step_x, step_y);

    // 4. Write output
    write_ppm(scaled_image, argv[2]);

    free_resources(scaled_image, contour_map, grid, step_x);

    return 0;
}
