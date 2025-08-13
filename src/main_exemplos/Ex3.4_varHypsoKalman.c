/* Neste exemplo utilizei a equação hypsométrica com correção por temperatura em tempo real, mantendo a lógica de captura da 
pressão de referência ao ligar a placa além de usar um filtro de Kalman para suavizar as leituras de altitude.
O filtro de Kalman é uma técnica poderosa para estimar estados de sistemas dinâmicos, mesmo com ruído nas medições.
 */


#include <stdio.h>
#include <math.h>
#include "pico/stdlib.h"
#include "src/ms5637_02ba03/ms5637.h"

#define GAS_CONSTANT_AIR 287.05f    // J/(kg·K)
#define GRAVITY_ACCEL    9.80665f   // m/s²

// Parâmetros do filtro
#define ALT_MEAS_VAR   0.018f   // R: variância de medição (OSR=8192)
#define ALT_PROC_VAR   0.05f    // Q: variância de processo

// Estrutura do Kalman Filter
typedef struct {
    float estimate;      // última estimativa de altitude
    float error_est;     // variância da estimativa
    float error_meas;    // variância de medição (R)
    float process_noise; // variância de processo (Q)
    float gain;          // ganho de Kalman
} KalmanFilter;

// Inicializa o filtro
void kalman_init(KalmanFilter *kf,
                 float init_estimate,
                 float init_error_est,
                 float meas_var,
                 float proc_var)
{
    kf->estimate      = init_estimate;
    kf->error_est     = init_error_est;
    kf->error_meas    = meas_var;
    kf->process_noise = proc_var;
    kf->gain          = 0.0f;
}

// Atualiza o filtro com uma nova medição
float kalman_update(KalmanFilter *kf, float measurement) {
    // 1) Previsão
    kf->error_est += kf->process_noise;

    // 2) Cálculo do ganho de Kalman
    kf->gain = kf->error_est / (kf->error_est + kf->error_meas);

    // 3) Correção da estimativa
    kf->estimate += kf->gain * (measurement - kf->estimate);

    // 4) Atualiza variância da estimativa
    kf->error_est *= (1.0f - kf->gain);

    return kf->estimate;
}


/**
 * Calcula a altitude corrigida pela equação hypsométrica com a correção pela temperatura da coluna de ar.
 * @param pressure_hpa    Pressão atual medida (hPa ou mbar)
 * @param baseline_hpa    Pressão de referência ao ligar o dispositivo
 * @param temperature_c   Temperatura atual medida (°C)
 * @return                Altura relativa estimada (m)
 */
static float calculate_altitude_hypsometric(float pressure_hpa,
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

    // Mesma lógica de captura da pressão de referência (baseline) ao ligar
    // Isso permite calcular a variação de altura em relação a essa pressão atmosférica
    float baseline_pressure = 0.0f;
    float temperature, pressure;
    while (baseline_pressure == 0.0f) {
        if (ms5637_read_temperature_pressure(&temperature, &pressure) == MS5637_STATUS_OK) {
            baseline_pressure = pressure;
            printf("Pressão ref: %.2f mbar\n", baseline_pressure);
        }
        sleep_ms(200);
    }

    // Inicializa Kalman
    KalmanFilter alt_filter;
    kalman_init(&alt_filter,
                0.0f,          // estimativa inicial
                1.0f,          // incerteza inicial alta
                ALT_MEAS_VAR,  // R
                ALT_PROC_VAR); // Q

    // Loop principal
    while (true) {
        if (ms5637_read_temperature_pressure(&temperature, &pressure) == MS5637_STATUS_OK) {
            // Altitude bruta
            float alt_raw = calculate_altitude_hypsometric(
                                pressure, baseline_pressure, temperature);
            // Altitude filtrada
            float alt_filt = kalman_update(&alt_filter, alt_raw);

            printf("Alt fil: %.2f m | Alt bruta: %.2f m | P: %.2f mbar | T: %.2f°C\n",
                   alt_filt, alt_raw, pressure, temperature);
        }
        sleep_ms(500);
    }
}