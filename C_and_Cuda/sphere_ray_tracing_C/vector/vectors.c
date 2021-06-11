#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"vectors.h"

/* sets vector values */
void set_values(Vec a, float x, float y, float z){
    a->x = x; a->y = y; a->z = z;
    return;
}

/* Adds a and b into c */
void vec_add(Vec a, Vec b, Vec c){
    c->x = a->x + b->x;
    c->y = a->y + b->y;
    c->z = a->z + b->z;
    return;
}

/* subtracts b from a into c */
void vec_subtract(Vec a, Vec b, Vec c){
    c->x = a->x - b->x;
    c->y = a->y - b->y;
    c->z = a->z - b->z;
    return;
}

/* returns the norm of a vector*/
float norm(Vec c){
    return sqrt(c->x*c->x + c->y*c->y + c->z*c->z);
}


/* returns dot product of vector */
float dot_product(Vec a, Vec b){
    return a->x * b->x + a->y * b->y + a->z * b->z;
}

/* multiplies a vector by a scalar */
void scalar_mult(Vec a, float scalar){
    a->x = a->x * scalar;
    a->y = a->y * scalar;
    a->z = a->z * scalar;
    return;
}

/* divides a vector by a scalar */
void scalar_divide(Vec a, float scalar){
    a->x = a->x / scalar;
    a->y = a->y / scalar;
    a->z = a->z / scalar;
    return;
}

/* makes c into the unit vector of c*/
void make_unit(Vec a, Vec c){
    set_values(c, a->x, a->y, a->z);
    scalar_divide(c, norm(a));
    return;
}

/* prints a vector */
void print_vector(Vec a){
    printf("(%f, %f, %f)\n", a->x, a->y, a->z);
    return;
}