#include<stdlib.h>
#include<stdio.h>
#include"vectors.h"


int main(int argc, char **argv){
    struct vector_ a_,b_,c_;
    Vec a = &a_, b = &b_, c = &c_;
    set_values(a, 1,2,3);
    set_values(b, -1,-2,-3);
    
    printf("Testing a = (1,2,3)\nb=(-1,-2,-3)\n");

    printf("Testing Vector addition of a + b\nExpecting c = (0,0,0)\n"
    "Got: ");
    vec_add(a,b,c);
    print_vector(c);

    printf("Testing Vector subtraction of a - b\nExpecting c = (2,4,6)"
    "\nGot: ");
    vec_subtract(a,b,c);
    print_vector(c);

    printf("Testing Vector multiplication of a * 5\nExpecting c = (5,10,15)"
    "\nGot: ");
    scalar_mult(a, 5);
    print_vector(a);
    set_values(a, 1,2,3);

    printf("Testing Vector division of a / 3\nExpecting c = (1/3, 2/3, 1)"
    "\nGot: ");
    scalar_divide(a, 5);
    print_vector(a);
    set_values(a, 1,2,3);

    printf("Testing Vector norm of a and b/ 3\nExpecting ~3.74165"
    "\nGot: %f and %f\n", norm(a), norm(b));

    printf("Testing Vector dot product a and b/ 3\nExpecting -14"
    "\nGot: %f\n", dot_product(a,b));
    print_vector(b);


    
    make_unit(a, b);
    printf("Testing Unit Vector conversion with input a and output b"
    "\nGot: %f and %f \n", norm(a), norm(b));
    print_vector(a);
    print_vector(b);


    set_values(b, -1,-2,-3);
    make_unit(b, a);
    printf("Testing Unit Vector conversion with input b and output a"
    "\nGot: %f and %f \n", norm(b), norm(a));
    print_vector(b);
    print_vector(a);


    return 0;

}