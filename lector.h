#include "macros.h"
#include <string.h>
#include <stdio.h>

void leerPlantilla(char* archivo, int corr[PREGUNTES][RESPOSTES]){
    FILE *f = fopen(archivo, "r");
    if(f == NULL){
        printf("Error: no se pudo abrir %s\n", archivo);
        return;
    }
    
    char linea[100];
    int pregunta = 0;
    
    while(fgets(linea, sizeof(linea), f) && pregunta < PREGUNTES){
        // limpiar array
        for(int j = 0; j < RESPOSTES; j++){
            corr[pregunta][j] = 0;
        }
        
        // buscar [X] en la línea
        for(int i = 0; i < strlen(linea) - 2; i++){
            if(linea[i] == '[' && linea[i+1] == 'X' && linea[i+2] == ']'){
                // determinar qué opción es basándose en la posición
                int opcion = 0;
                int count_brackets = 0;
                for(int j = 0; j <= i; j++){
                    if(linea[j] == '['){
                        if(j == i) break;
                        count_brackets++;
                    }
                }
                if(count_brackets < RESPOSTES){
                    corr[pregunta][count_brackets] = 1;
                }
            }
        }
        pregunta++;
    }
    fclose(f);
}

void interpretarHojaUPF(char* archivo, int resp[2*PREGUNTES][RESPOSTES]){
    FILE *f = fopen(archivo, "r");
    if(f == NULL){
        printf("Error: no se pudo abrir %s\n", archivo);
        return;
    }
    
    char linea[100];
    int pregunta = 0;
    int fila = 0; // 0 = superior, 1 = inferior
    
    while(fgets(linea, sizeof(linea), f) && pregunta < PREGUNTES){
        // saltar líneas vacías
        if(strlen(linea) <= 1) continue;
        
        // determinar si es línea de pregunta o de cancelación
        if(linea[0] >= '1' && linea[0] <= '9'){
            fila = 0; // fila superior
        } else if(linea[0] == ' '){
            fila = 1; // fila inferior (cancelación)
        } else {
            continue;
        }
        
        // limpiar arrays para esta pregunta si es fila superior
        if(fila == 0){
            for(int j = 0; j < RESPOSTES; j++){
                resp[2*pregunta][j] = 0;
                resp[2*pregunta+1][j] = 0;
            }
        }
        
        // buscar [X] en la línea y marcarlos temporalmente
        int temp_marcas[RESPOSTES] = {0};
        for(int i = 0; i < strlen(linea) - 2; i++){
            if(linea[i] == '[' && linea[i+1] == 'X' && linea[i+2] == ']'){
                int count_brackets = 0;
                for(int j = 0; j <= i; j++){
                    if(linea[j] == '['){
                        if(j == i) break;
                        count_brackets++;
                    }
                }
                if(count_brackets < RESPOSTES){
                    temp_marcas[count_brackets] = 1;
                }
            }
        }
        
        // aplicar las marcas
        for(int j = 0; j < RESPOSTES; j++){
            if(temp_marcas[j] == 1){
                resp[2*pregunta + fila][j] = 1;
            }
        }
        
        // si acabamos de procesar la fila inferior, aplicar lógica de cancelación
        if(fila == 1){
            for(int j = 0; j < RESPOSTES; j++){
                // si está marcado arriba Y abajo, cancelar (quitar de arriba)
                if(resp[2*pregunta][j] == 1 && resp[2*pregunta+1][j] == 1){
                    resp[2*pregunta][j] = 0;
                    resp[2*pregunta+1][j] = 0; // limpiar también abajo
                }
            }
            pregunta++; // pasar a siguiente pregunta
        }
    }
    fclose(f);
}

float comparar(int corr[PREGUNTES][RESPOSTES], int resp[2*PREGUNTES][RESPOSTES]){

    int correctes = 0, incorrectes = 0, no_contestades = 0, anulades = 0, contestades;
    float puntuacioPregunta, NF = 0;
    char resposta;

    for(int i = 0; i < 2*PREGUNTES; i+=2){
        printf("pregunta %d\n", (i/2)+1);
        contestades = 0;
        puntuacioPregunta = 0;
        resposta = '-';
        for(int j = 0; j < RESPOSTES; j++){
            if(resp[i][j] == 1 && resp[i+1][j] == 0){
                contestades++;
                if(contestades > 1){
                    puntuacioPregunta = 0;
                    printf("    ANULADA        punts: 0.00\n");
                    anulades++;
                    goto pregunta_anulada;
                }
                
                switch (j)
                {
                case 0:
                    resposta = 'a';
                    break;
                case 1:
                    resposta = 'b';
                    break;
                case 2:
                    resposta = 'c';
                    break;
                case 3:
                    resposta = 'd';
                    break;
                case 4:
                    resposta = 'e';
                    break;
                case 5:
                    resposta = 'f';
                    break;
                }

                if(resp[i][j] == corr[i/2][j]){ // correcte
                    puntuacioPregunta += 1;
                }

                else{ // incorrecte
                    puntuacioPregunta -= 1/(float)PENALITZACIO;
                }
            }
        }
        if(contestades == 0){no_contestades++;}
        else if(puntuacioPregunta > 0){correctes++;}
        else if(puntuacioPregunta < 0){incorrectes++;}
        NF += puntuacioPregunta;
        printf("    resposta: %c", resposta);
        printf("    punts: %.2f\n", puntuacioPregunta);

        
    pregunta_anulada:
        continue;
    }

    if(NF < 0){NF = 0;}
    printf("\nRESUM\n    Correctes: %d\n    Incorrectes: %d\n    No contestades: %d\n    Anulades: %d\n",
        correctes, incorrectes, no_contestades, anulades);

    return NF;
}