/* Exemplo com a equação hypsométrica com correção por temperatura em tempo real, mantendo a lógica de 
captura da pressão de referência ao ligar a placa. */

#include <stdio.h>
#include <math.h>
#include "pico/stdlib.h"
#include "src/ms5637_02ba03/ms5637.h"

// Constantes físicas
#define GAS_CONSTANT_AIR 287.05f    // J/(kg·K)
#define GRAVITY_ACCEL    9.80665f   // m/s²

/**
 * Calcula a altitude corrigida pela equação hypsométrica com a correção pela temperatura da coluna de ar.
 * @param pressure_hpa    Pressão atual medida (hPa ou mbar)
 * @param baseline_hpa    Pressão de referência ao ligar o dispositivo
 * @param temperature_c   Temperatura atual medida (°C)
 * @return                Altura relativa estimada (m)
 */
float calculate_altitude_hypsometric(float pressure_hpa,
                                     float baseline_hpa,
                                     float temperature_c)
{
    float temp_k = temperature_c + 273.15f;
    float factor = (GAS_CONSTANT_AIR * temp_k) / GRAVITY_ACCEL;
    return factor * logf(baseline_hpa / pressure_hpa);
}

int main() {
    stdio_init_all();
    ms5637_init();

    float baseline_pressure = 0;
    float temperature, pressure;

    // Captura pressão de referência ao ligar
    while (baseline_pressure == 0) {
        if (ms5637_read_temperature_pressure(&temperature, &pressure) == MS5637_STATUS_OK) {
            baseline_pressure = pressure;
            printf("Pressão de referência capturada: %.2f mbar\n", baseline_pressure);
        } else {
            printf("Erro de leitura do sensor\n");
        }
        sleep_ms(500);
    }

    while (true) {
        if (ms5637_read_temperature_pressure(&temperature, &pressure) == MS5637_STATUS_OK) {
            float altitude = calculate_altitude_hypsometric(pressure, baseline_pressure, temperature);
            printf("Altura relativa (corrigida): %.2f m | Pressão: %.2f mbar | Temp: %.2f °C\n",
                   altitude, pressure, temperature);
        } else {
            printf("Erro ao ler sensor\n");
        }
        sleep_ms(500);
    }
}