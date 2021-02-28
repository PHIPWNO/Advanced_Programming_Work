#ifndef VECTORS_H
#define VECTORS_H

typedef struct vector_{
    float x,y,z; /* x,y,z components */
    float reserved; /* reserved for some reason? */
} *Vec;

/* sets vector values */
void set_values(Vec a, float x, float y, float z);

/* Adds a and b into c */
void vec_add(Vec a, Vec b, Vec c);


/* subtracts b from a into c */
void vec_subtract(Vec a, Vec b, Vec c);

/* returns the norm of a vector*/
float norm(Vec x);


/* returns dot product of vector */
float dot_product(Vec a, Vec b);

/* multiplies a vector by a scalar */
void scalar_mult(Vec a, float scalar);

/* divides a vector by a scalar */
void scalar_divide(Vec a, float scalar);

/* makes c into the unit vector of c*/
void make_unit(Vec a, Vec c);

/* prints a vector */
void print_vector(Vec a);


#endif // VECTORS_H
