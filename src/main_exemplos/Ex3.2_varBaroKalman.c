/* Neste exemplo, implementei juntamente da equação barométrica um filtro de Kalman para suavizar as leituras de altitude do sensor MS5637-02BA03.
O filtro de Kalman é uma técnica poderosa para estimar estados de sistemas dinâmicos, mesmo com ruído nas medições.
 */

#include <stdio.h>
#include <math.h>
#include "pico/stdlib.h"
#include "src/ms5637_02ba03/ms5637.h"

// Parâmetros do filtro de Kalman
#define ALT_MEAS_VAR   0.018f   // R: variância de medição (OSR=8192)
#define ALT_PROC_VAR   0.001f   // Q: variância de processo (muito baixo para estático = 0.001f)

// Estrutura do Kalman Filter
typedef struct {
    float estimate;      // última estimativa (m)
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

// Atualiza o filtro com a nova medida de altitude
float kalman_update(KalmanFilter *kf, float measurement) {
    // 1) Previsão: a incerteza cresce um pouco
    kf->error_est += kf->process_noise;

    // 2) Cálculo do ganho de Kalman
    kf->gain = kf->error_est / (kf->error_est + kf->error_meas);

    // 3) Atualiza a estimativa
    kf->estimate += kf->gain * (measurement - kf->estimate);

    // 4) Atualiza a variância da estimativa
    kf->error_est *= (1.0f - kf->gain);

    return kf->estimate;
}

// A mesma função simples de cálculo de altitude relativa
// Fórmula barométrica: h = 44330 * (1 - (P / P0)^(1/5.255))
static float calc_altitude(float pressure_hpa, float baseline_hpa) {
    return 44330.0f * (1.0f - powf(pressure_hpa / baseline_hpa, 1.0f / 5.255f));
}

int main() {
    stdio_init_all();
    ms5637_init();

    // 1) Captura a pressão de referência (baseline) ao ligar
    float baseline_pressure = 0.0f;
    float temperature, pressure;
    while (baseline_pressure == 0.0f) {
        if (ms5637_read_temperature_pressure(&temperature, &pressure) == MS5637_STATUS_OK) {
            baseline_pressure = pressure;
            printf("Pressão ref capturada: %.2f mbar\n", baseline_pressure);
        }
        sleep_ms(200);
    }

    // 2) Inicializa o Kalman Filter
    KalmanFilter alt_filter;
    kalman_init(&alt_filter,
                0.0f,          // estimativa inicial = 0 m
                ALT_MEAS_VAR,  // incerteza inicial = variância de medição
                ALT_MEAS_VAR,  // R
                ALT_PROC_VAR);// Q

    // 3) Loop principal: mede, calcula altitude e filtra
    while (true) {
        if (ms5637_read_temperature_pressure(&temperature, &pressure) == MS5637_STATUS_OK) {
            // Altitude sem filtro
            float alt_raw  = calc_altitude(pressure, baseline_pressure);

            // Altitude com Kalman
            float alt_filt = kalman_update(&alt_filter, alt_raw);

            printf("Alt filt: %.2f m | Alt bruta: %.2f m | P: %.2f mbar | T: %.2f°C\n",
                   alt_filt, alt_raw, pressure, temperature);
        } else {
            printf("Erro ao ler sensor\n");
        }
        sleep_ms(500);
    }
}