//
// Created by wojkly on 3/12/21.
//

#include <stdio.h>
#include <gsl/gsl_ieee_utils.h>

int main (void)
{
    float f = 1.0/3.0;

    int i = 0;
    gsl_ieee_printf_float(&f);
    while( i < 149){
        f /= 2.0;
        gsl_ieee_printf_float(&f);
        printf("\n");
        i+= 1;
    }

    return 0;
}