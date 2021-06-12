#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<math.h>
#include<omp.h>
#include<time.h>
#include"vectors.h"
#include"bitmap.h"

#define FILENAME "sphere_C.bmp"

#define WMAX 10
#define THE_R 4.4

/* Generates and writes bitmap of nxn sphere */
double serial_sphere(int n, Vec L, Vec C){
    struct timespec start, end;
    float W_MAX = (float) WMAX, R = (float) THE_R; 
    int b, i, j;
    float delta_x, delta_z, W_x, W_z, D, s_dot_n;
    struct vector_ N_, S_, I_, I_min_C_, L_min_I_;
    Vec N = &N_, S = &S_, I = &I_, I_min_C = &I_min_C_, L_min_I = &L_min_I_;
    delta_x = 2 * W_MAX / n;
    delta_z = delta_x;
    RgbTriple* G = malloc(n * n * sizeof(RgbTriple));
    /* start timing */
    clock_gettime(CLOCK_MONOTONIC, &start);
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            W_x = i * delta_x - W_MAX;
            W_z = j * delta_z - W_MAX;
            D = R * R - (W_x - C->x)*(W_x - C->x) - (W_z - C->z)*(W_z - C->z);

            if(D < 0){
                b = 0;
            }
            else{
                I->y = C->y - sqrt(D);
                set_values(I, W_x, I->y, W_z);
                vec_subtract(I, C, I_min_C);
                vec_subtract(L, I, L_min_I);
                make_unit(I_min_C, N);
                make_unit(L_min_I, S);
                s_dot_n = dot_product(S, N);

                if(s_dot_n > 0){
                    s_dot_n *= 255;
                    b = (int) rint(s_dot_n);
                }
                else{
                    b = 0;
                }
            }
            (&G[i * n + j])->red = b;
            (&G[i * n + j])->green = b;
            (&G[i * n + j])->blue = b;
        }
    }
    /* stop timing */
    clock_gettime(CLOCK_MONOTONIC, &end);

    save_bitmap(G, n, n, FILENAME);
    free(G);

    return (end.tv_sec - start.tv_sec)
        + 1e-9 * (end.tv_nsec - start.tv_nsec);
}


double parallel_sphere(int n, Vec L, Vec C){
    struct timespec start, end;

    RgbTriple* G = malloc(n * n * sizeof(RgbTriple));
    /* start timing */
    clock_gettime(CLOCK_MONOTONIC, &start);


    #pragma omp parallel for shared(n) proc_bind(close)
    for (int i = 0; i < n; i++)
    {
        float W_MAX = (float) WMAX, R = (float) THE_R; 
        float delta_x, delta_z;
        delta_x = 2 * W_MAX / n;
        delta_z = delta_x;
        int b;
        float W_x, W_z, D, s_dot_n;
        struct vector_ N_, S_, I_, I_min_C_, L_min_I_;
        Vec N = &N_, S = &S_, I = &I_, I_min_C = &I_min_C_, L_min_I = &L_min_I_;
        //printf("%d\n", i);
        for (int j = 0; j < n; j++)
        {
            W_x = i * delta_x - W_MAX;
            W_z = j * delta_z - W_MAX;
            D = R * R - (W_x - C->x)*(W_x - C->x) - (W_z - C->z)*(W_z - C->z);

            if(D < 0){
                b = 0;
            }
            else{
                I->y = C->y - sqrt(D);
                set_values(I, W_x, I->y, W_z);
                vec_subtract(I, C, I_min_C);
                vec_subtract(L, I, L_min_I);
                make_unit(I_min_C, N);
                make_unit(L_min_I, S);
                s_dot_n = dot_product(S, N);

                if(s_dot_n > 0){
                    s_dot_n *= 255;
                    b = (int) rint(s_dot_n);
                    //printf("float is %f and int is %d\n", s_dot_n, b);
                }
                else{
                    b = 0;
                }
            }
            (&G[i * n + j])->red = b;
            (&G[i * n + j])->green = b;
            (&G[i * n + j])->blue = b;
        }
    }

    /* stop timing */
    clock_gettime(CLOCK_MONOTONIC, &end);
    save_bitmap(G, n, n, FILENAME);
    free(G);

    return (end.tv_sec - start.tv_sec)
        + 1e-9 * (end.tv_nsec - start.tv_nsec);
}


int main(int argc, char **argv){
    int n = 1000, t_count;
    double total_time;
    char thread_count[100];
    struct vector_ L_, C_;
    Vec L = &L_, C = &C_;

    set_values(L, 4, 4, -1);
    set_values(C, 0, 12, 0);
    serial_sphere(n, L, C);

    if(argc > 1){
        n = atoi(argv[1]);
    }
    if(!getenv("OMP_NUM_THREADS")){
        total_time = serial_sphere(n, L, C);
    }
    else{
        strcpy(thread_count, getenv("OMP_NUM_THREADS"));
        t_count = atoi(thread_count);
        if (t_count <= 1){
            total_time = serial_sphere(n, L, C);
        }
        else{
            total_time = parallel_sphere(n, L, C);
        }
    }

    fprintf(stderr, "total time:%f\n", total_time);
    printf("%f\n", total_time);

    return 0;
}