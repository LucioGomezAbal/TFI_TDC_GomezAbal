#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
typedef struct {
    double Kp; // Constante proporcional
    double Ki; // Constante integral
    double Kd; // Constante derivativa
    double setpoint; // Temperatura deseada
    double integral; // Acumulador integral
    double previous_error; // Error previo
} PIDController;

// Inicialización del controlador PID
void PID_Init(PIDController* pid, double Kp, double Ki, double Kd, double setpoint) {
    pid->Kp = Kp;
    pid->Ki = Ki;
    pid->Kd = Kd;
    pid->setpoint = setpoint;
    pid->integral = 0.0;
    pid->previous_error = 0.0;
}

// Función de cálculo del PID
double PID_Compute(PIDController* pid, double current_temp, double dt) {
    double error = pid->setpoint - current_temp;
    pid->integral += error * dt;
    double derivative = (error - pid->previous_error) / dt;
    double output = pid->Kp * error + pid->Ki * pid->integral + pid->Kd * derivative;
    pid->previous_error = error;
    return output;
}

// Simulación de la dinámica del sistema de calefacción
double simulate_heating_system(double current_temp, double heater_output, double dt) {
    // Modelo simplificado de la dinámica térmica
    double ambient_temp = 20.0; // Temperatura ambiente constante
    double thermal_mass = 10.0; // Masa térmica (afecta la inercia térmica)
    double heat_transfer_coeff = 0.1; // Coeficiente de transferencia de calor

    // Actualización de la temperatura
    double temp_change = (heater_output - heat_transfer_coeff * (current_temp - ambient_temp)) * dt / thermal_mass;
    return current_temp + temp_change;
}
void aumentarLuz(int *luz, int cantidad) {
    *luz += cantidad;
}

void disminuirLuz(int *luz, int cantidad) {
    *luz -= cantidad;
}

bool controlarHoraDia(int *horaDia, int *luz) {
    if (*horaDia == 7) {
        printf("Ha Amanecido, la luz ambiente aumenta en 400\n");
        aumentarLuz(luz, 400);
    }
    if (*horaDia == 19) {
        printf("Ha anochecido, la luz ambiente disminuye en 400\n");
        disminuirLuz(luz, 400);
    }
    if (*horaDia == 24) {
        printf("Ha Finalizado el Dia, presiona cualquier boton para salir\n");
        getchar(); 
         getchar();
        return true;  // Indica que el programa debe finalizar
    }
    return false;  // Indica que el programa debe continuar
}

void mostrarMenu() {
    printf("\nMenu de opciones de perturbaciones:\n");
    printf("1. Abrir cortinas (Aumenta luz en 400)\n");
    printf("2. Encender lampara (Aumenta luz en 300)\n");
    printf("3. Encender lampara de escritorio (Aumenta luz en 200)\n");
    printf("4. Encender luz de techo (Aumenta luz en 200)\n");
    printf("5. Salio el Sol (Aumenta luz en 300)\n");
    printf("6. Cerrar cortinas (Disminuye luz en 400)\n");
    printf("7. Apagar lampara (Disminuye luz en 300)\n");
    printf("8. Apagar lampara de escritorio (Disminuye luz en 200)\n");
    printf("9. Apagar luz de techo (Disminuye luz en 200)\n");
    printf("10. Se Nublo (Disminuye luz en 300)\n");
    printf("11. Paso una hora (No hay cambios en el ambiente)\n");
    printf("12. Simular salida de Controlador PID de un Aire Acondicionado, con diversas perturbaciones aleatorias\n");
    printf("0. Salir\n");
    printf("Ingrese su opcion: ");
}
void controlador(){
    PIDController pid;
    PID_Init(&pid, 2.0, 0.1, 0.5, 24.0); // Inicialización del PID con Kp, Ki, Kd y setpoint

    double current_temp = 24.0; // Temperatura inicial
    double dt = 1.0; // Intervalo de tiempo (1 segundo)
    double simulation_time = 15.0; // Tiempo total de simulación (600 segundos)
    double time_elapsed = 0.0;
    double PIDideal = PID_Compute(&pid, 24.0, dt);
    srand(time(NULL)); // Inicialización del generador de números aleatorios
     printf("Temp Ideal %.1f, PID %4.f \n",24.0,PIDideal);
    while (time_elapsed < simulation_time) {
        double heater_output = PID_Compute(&pid, current_temp, dt);
        current_temp = simulate_heating_system(current_temp, heater_output, dt);
        

        // Introducción de una perturbación aleatoria para simular cambios en la temperatura ambiente
        double perturbation = ((rand() % 100) / 100.0 - 0.5) * 0.1;
        current_temp += perturbation;
        double PIDideal = PID_Compute(&pid, 24.0, dt);
        // Impresión de la temperatura actual y la salida del PID
        printf("Tiempo: %.1f s, Temperatura: %.2f °C, Salida del PID: %.2f\n", time_elapsed, current_temp, heater_output);
       
        time_elapsed += dt;
    }
}
void mostrarEstadoLuz(int luz, bool luz_prendida) {
    printf("\nEstado actual de la luz Ambiente: %d\n", luz);
    if (luz_prendida) {
        printf("La luz esta encendida.\n");
    } else {
        printf("La luz esta apagada.\n");
    }
}
void aumentarHoraDia(int *horaDia){
    (*horaDia)++;
    printf("\nSon las %dhs\n",*horaDia);
}
int main() {
   int luz = 50; // Valor inicial de la luz
    int horaDia = 0;
    bool luz_prendida = false;
    int opcion;

    while (1) {
        mostrarMenu();
        scanf("%d", &opcion);

        switch (opcion) {
            case 1: aumentarLuz(&luz, 400);
                    aumentarHoraDia(&horaDia);
                    break;
            case 2: aumentarLuz(&luz, 300); 
                    aumentarHoraDia(&horaDia);
                    break;
            case 3: aumentarLuz(&luz, 200); 
                    aumentarHoraDia(&horaDia);
                    break;
            case 4: aumentarLuz(&luz, 200); 
                    aumentarHoraDia(&horaDia);
                    break;
            case 5: aumentarLuz(&luz, 300); 
                    aumentarHoraDia(&horaDia);
                    break;
            case 6: disminuirLuz(&luz, 400); 
                    aumentarHoraDia(&horaDia);
                    break;
            case 7: disminuirLuz(&luz, 300); 
                    aumentarHoraDia(&horaDia);
                    break;
            case 8: disminuirLuz(&luz, 200); 
                    aumentarHoraDia(&horaDia);
                    break;
            case 9: disminuirLuz(&luz, 200); 
                    aumentarHoraDia(&horaDia);
                    break;
            case 10: disminuirLuz(&luz, 300); 
                    aumentarHoraDia(&horaDia);
                    break;
            case 11 :
                aumentarHoraDia(&horaDia);
                break;
            case 12 :
                controlador();
                break;
            case 0: printf("Saliendo...\n"); return 0;
            default: printf("Opcion no válida\n"); continue;
        }

        if (controlarHoraDia(&horaDia, &luz)) { 
            break;  // Finaliza el programa si controlarHoraDia retorna true
        }
         if (luz < 0) {
        luz = 0;
    }

     if (luz < 300) { // Umbral para encender la luz
    luz_prendida = true;
    printf("¡La luz se ha encendido debido a la baja intensidad!\n");
} else if (luz > 800) { // Umbral para apagar la luz
    luz_prendida = false;
    printf("¡Hay mucha luz en el ambiente, conviene apagarla!\n");
} else {
    // Si la intensidad de luz está entre 300 y 800, la luz no cambia de estado
    luz_prendida = luz_prendida; // Mantiene el estado actual
}
     mostrarEstadoLuz(luz, luz_prendida);
    }

    return 0;
}

