#include "lector.h"
#include <stdio.h>

int main(){
    int correccio[PREGUNTES][RESPOSTES];
    int respostes[2*PREGUNTES][RESPOSTES];
    
    leerPlantilla("plantilla.txt", correccio);
    interpretarHojaUPF("alumno.txt", respostes);
    
    float NF = comparar(correccio, respostes);
    if(PREGUNTES == 10){
        printf("\nNF = %.2f\n", NF);
    }
    else{
        printf("\nNF = %.2f / %d\n", NF, PREGUNTES);
        printf("\nNF NORMALITZADA = %.2f\n\n", NF * 10/(float)PREGUNTES);
    }
}