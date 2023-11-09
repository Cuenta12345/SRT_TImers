#include "mbed.h"
#include <cstdio>
#include "Thread.h"

#define TOTALPROCESOS 3
DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);


// Estructura de procesos
struct PROCESS {
    int prioridad;
    int tiempoRestante;
    int periodo;
} procesos[TOTALPROCESOS];

// Timers
Timer timerPlanificadorGeneral;

 int instante = 0;

int comprobarProcesoPrioritario() {
    int indice = -1;
    int maxPrioridad = 99;

    for (int i = 0; i < TOTALPROCESOS; i++) {
        if (procesos[i].tiempoRestante != 0 && procesos[i].prioridad < maxPrioridad) {
            indice = i;
            maxPrioridad = procesos[i].prioridad;
        }
    }
    return indice;
}

void comprobarReinicios(){
    for (int i = 0; i < TOTALPROCESOS; i++) {
        if (( instante % procesos[i].periodo) == 0) {
            procesos[i].tiempoRestante = procesos[i].prioridad;
        }
    }

}

void ejecutarProceso(int indice) {
    printf("El proceso número %d se está ejecutando\n", indice + 1);
    procesos[indice].tiempoRestante--;
    
    if (indice == 0){
        led1 = !led1;
    }
    if (indice == 1){
        led2 = !led2;
    }
    if (indice == 2){
        led3 = !led3;
    }
}

void planificador() {
    int indice = -1;
    comprobarReinicios();
    indice = comprobarProcesoPrioritario();
    led1=0; led2=0; led3=0;
    if (indice != -1) {
        ejecutarProceso(indice);
    }else{
        printf("Ninguno se ejecuta\n");
    }
    instante++;        
}

void inicializaciones(){
    procesos[0].prioridad = 2;
    procesos[0].tiempoRestante = procesos[0].prioridad;
    procesos[0].periodo = 8;
    
    procesos[1].prioridad = 3;
    procesos[1].tiempoRestante = procesos[1].prioridad;
    procesos[1].periodo = 12;
    
    procesos[2].prioridad = 4;
    procesos[2].tiempoRestante = procesos[2].prioridad;
    procesos[2].periodo = 16;
}

int main() {
    
    // Inicializa los procesos con prioridades y tiempos restantes
    inicializaciones();

    timerPlanificadorGeneral.start();
    printf("______________________________________________________________________________________________________________________\n");
    while (true) {
        if (timerPlanificadorGeneral.read_ms() > 1000){
            planificador();
            timerPlanificadorGeneral.reset();
        }
    }
}
