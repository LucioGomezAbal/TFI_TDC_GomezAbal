#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

double contador = 0;
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
    printf("\nSon las 2 de la tarde de un dia soleado, te encontras en una habitacion utilizada como oficina de una vivienda familiar\n");
    printf("\nValor de la LUZ = 850 lux\n");
    printf("\nMenu de opciones de perturbaciones:\n");
    printf("1. Abrir cortinas (Aumenta luz en 400)\n");
    printf("2. El sol da directo en la ventana (Aumenta luz en 300)\n");
    printf("3. Cerrar cortinas (Disminuye luz en 400)\n");
    printf("4. Se Nublo (Disminuye luz en 300)\n");
    printf("5. Simular 5 perturbaciones internas\n");
    //printf("6. Simular salida de Controlador PID de un Aire Acondicionado, con diversas perturbaciones aleatorias\n");
    printf("6: Graficar la relacion de la perturbacion con la salida del sistema \n");
    printf("7: Graficar la relacion de entrada con la salida del controlador \n");
    printf("8: Graficar la relacion de la perturbacion con la salida del controlador \n");
    printf("0. Salir\n");
    printf("Ingrese su opcion: ");
}
void controladorAire(){
    PIDController pid;
    PID_Init(&pid, 2.0, 0.1, 0.5, 24.0); // Inicialización del PID con Kp, Ki, Kd y setpoint

    double current_temp = 24.0; // Temperatura inicial
    double dt = 1.0; // Intervalo de tiempo (1 segundo)
    double simulation_time = 15.0; // Tiempo total de simulación (600 segundos)
    double time_elapsed = 0.0;
    double PIDideal = PID_Compute(&pid, 24.0, dt);
      FILE *file = fopen("temperature_data.txt", "w");
    if (file == NULL) {
        printf("Error al abrir el archivo.\n");
    }
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
        fprintf(file, "%.2f %.2f %.1f\n", current_temp, heater_output, time_elapsed);
        time_elapsed += dt;
    }
     fclose(file);

    // Generar gráfico usando GNUplot
    FILE *gnuplot = popen("gnuplot -persistent", "w");
    fprintf(gnuplot, "set title 'Simulación del Sistema de Control de Temperatura de un Aire Acondicionado'\n");
    fprintf(gnuplot, "set xlabel 'Temperatura (°C)'\n");
    fprintf(gnuplot, "set ylabel 'Controlador'\n");
    fprintf(gnuplot, "plot 'temperature_data.txt' using 1:2 with points title 'Temperatura'\n");
    fclose(gnuplot);
}
double controladorTemp(double current_lux){
    PIDController pid;
    PID_Init(&pid, 2.0, 0.12, 0.48, 850); // Inicialización del PID con Kp, Ki, Kd y setpoint
    double PIDideal = PID_Compute(&pid, 850, contador);
    double lux_output = PID_Compute(&pid, current_lux, contador);
    return lux_output;
}
void perturbacionesAleatorias(int cantidad, FILE* file, FILE* file1, FILE* file2){
   srand(time(NULL));
    while(cantidad > 0){
        int random_value = (rand() % 1201) - 600;
        contador++;
        int valorDeLuz = 850 + random_value; 
        double controller_output = controladorTemp(valorDeLuz);
        printf("Hubo una perturbacion interna que llevo a una variacion de %d LUX\n. La intensidad luminica actual es de %d\n. El controlador debera trabajar para llevarlo a su valor referencial de 850 \n", random_value , valorDeLuz);
        fprintf(file, "%d %d \n", random_value, valorDeLuz);
        fprintf(file1, "%d %d \n", 850, 0);
        fprintf(file1, "%d %.2f \n", valorDeLuz, controller_output);
        fprintf(file2, "%d %.2f \n", random_value, controller_output);
        cantidad--;
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
      FILE *file = fopen("pert_out.txt", "w");
    if (file == NULL) {
        printf("Error al abrir el archivo.\n");
        return 1;
    }
      FILE *file1 = fopen("in_controller.txt", "w");
    if (file1 == NULL) {
        printf("Error al abrir el archivo.\n");
        return 1;
    }
      FILE *file2 = fopen("pert_controller.txt", "w");
    if (file2 == NULL) {
        printf("Error al abrir el archivo.\n");
        return 1;
    }
    while (1) {
        mostrarMenu();
        scanf("%d", &opcion);

        switch (opcion) {
            case 1: aumentarLuz(&luz, 400);
                    printf("La salida del sistema de de 1250\n");
                    printf("El sistema detecto mayor intensidad luminica de la deseada, se apagaran las luces del techo para volver al valor referencial\n");
                    contador ++;
                    double controller_output = controladorTemp(1250);
                    fprintf(file, "%d %d \n", 400, 1250);
                    fprintf(file1, "%d %d \n", 850, 0);
                    fprintf(file1, "%d %.2f \n", 1250, controller_output);
                    fprintf(file2, "%d %.2f \n", 400, controller_output);
                    break;
            case 2: aumentarLuz(&luz, 300);
                    printf("La salida del sistema de de 1150\n");
                    printf("El sistema detecto mayor intensidad luminica de la deseada, se apagaran las luces del escritorio para volver al valor referencial\n");
                    contador ++;
                    double controller_output2 = controladorTemp(1150);
                    fprintf(file, "%d %d \n", 300, 1150);
                    fprintf(file1, "%d %d \n", 850, 0);
                    fprintf(file1, "%d %.2f \n", 1150, controller_output2);
                    fprintf(file2, "%d %.2f \n", 300, controller_output2);
                    break;
            case 3: disminuirLuz(&luz, 400);
                    printf("La salida del sistema de de 450\n");
                    printf("El sistema detecto menor intensidad luminica de la deseada, se prenderan las luces del techo para volver al valor referencial\n");
                    contador ++;
                    double controller_output3 = controladorTemp(450);
                    fprintf(file, "%d %d \n", -400, 450);
                    fprintf(file1, "%d %d \n", 850, 0);
                    fprintf(file1, "%d %.2f \n", 450, controller_output3);
                    fprintf(file2, "%d %.2f \n", -400, controller_output3);
                    break;
            case 4: disminuirLuz(&luz, 300);
                    printf("La salida del sistema de de 550\n");
                    printf("El sistema detecto menor intensidad luminica de la deseada, se prenderan las luces del escritorio para volver al valor referencial\n"); 
                    contador ++;
                    double controller_output4 = controladorTemp(550);
                    fprintf(file, "%d %d \n", -300, 550);
                    fprintf(file1, "%d %d \n", 850, 0);
                    fprintf(file1, "%d %.2f \n", 550, controller_output4);
                    fprintf(file2, "%d %.2f \n", -300, controller_output4);
                    break;
            /*case 5: 
                    perturbacionesAleatorias(5, file,file1,file2);
                    break;*/
            case 5: controladorAire(); 
                    break;
            case 6: fclose(file);
                    FILE *gnuplot = popen("gnuplot -persistent", "w");
                    fprintf(gnuplot, "set title 'Relacion de la perturbacion con la salida del sistema'\n");
                    fprintf(gnuplot, "set xlabel 'Perturbacion (LUX)'\n");
                    fprintf(gnuplot, "set ylabel 'Salida del Sistema (LUX)'\n");
                    fprintf(gnuplot, "plot 'pert_out.txt' using 1:2 with lines title 'Salida del Sistema'\n");
                    fclose(gnuplot);
                    break;
            case 7: fclose(file1);
                    FILE *gnuplot1 = popen("gnuplot -persistent", "w");
                    fprintf(gnuplot1, "set title 'Relacion de entrada con la salida del controlador'\n");
                    fprintf(gnuplot1, "set xlabel 'Entrada (LUX)'\n");
                    fprintf(gnuplot1, "set ylabel 'Controlador'\n");
                    fprintf(gnuplot1, "plot 'in_controller.txt' using 1:2 with points title 'Controlador'\n");
                    fclose(gnuplot1);
                    break;
            case 8: fclose(file2);
                    FILE *gnuplot2 = popen("gnuplot -persistent", "w");
                    fprintf(gnuplot2, "set title 'Relacion de la perturbacion con la salida del controlador'\n");
                    fprintf(gnuplot2, "set xlabel 'Perturbacion (LUX)'\n");
                    fprintf(gnuplot2, "set ylabel 'Controlador'\n");
                    fprintf(gnuplot2, "plot 'pert_controller.txt' using 1:2 with points title 'Controlador'\n");
                    fclose(gnuplot2);
                    break;
            case 0: printf("Saliendo...\n"); return 0;
            default: printf("Opcion no válida\n"); continue;
        }
    }

    return 0;
}

