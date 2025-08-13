/* Neste exemplo, implementei juntamente do calculo barométrico uma média móvel para suavizar as leituras de altitude do sensor MS5637-02BA03.
A média móvel é uma técnica simples que ajuda a reduzir flutuações e ruídos nas leituras, proporcionando uma estimativa mais estável da altitude:
- buffer circular altitude_window[] de tamanho ALTITUDE_WINDOW_SIZE (usei 10).
- A cada leitura, insere o valor bruto de altitude (alt_raw) no buffer.
- Recalcula a média das últimas 10 amostras e exibe alt_smooth no lugar do valor bruto, reduzindo ruídos e oscilações.
- Pode ajustar ALTITUDE_WINDOW_SIZE para reforçar a suavização (janela maior) ou reduzir a latência (janela menor).
 */

#include <stdio.h>
#include <math.h>
#include "pico/stdlib.h"
#include "src/ms5637_02ba03/ms5637.h"

// Função barometrica para calcular a variação de altura baseada na pressão atmosférica capturada inicialmente ao ligar a placa
// Fórmula barométrica: h = 44330 * (1 - (P / P0)^(1/5.255))
// onde P0 é a pressão ao nível do mar (ou ponto conhecido),
// P  é a pressão medida e h é a variação de altura em metros       
static float calc_altitude(float pressure, float baseline) {
    return 44330.0f * (1.0f - powf(pressure / baseline, 1.0f / 5.255f));
}

// Parâmetros da média móvel
#define ALTITUDE_WINDOW_SIZE 10
static float altitude_window[ALTITUDE_WINDOW_SIZE] = {0};
static int   altitude_index = 0;

int main() {
    stdio_init_all();
    ms5637_init();

    // 1) Captura pressão de referência (baseline) ao ligar
    float baseline_pressure = 0.0f;
    float temperature, pressure;
    while (baseline_pressure == 0.0f) {
        if (ms5637_read_temperature_pressure(&temperature, &pressure) == MS5637_STATUS_OK) {
            baseline_pressure = pressure;
            printf("Pressão de referência capturada: %.2f mbar\n", baseline_pressure);
        } else {
            printf("Erro de leitura do sensor\n");
        }
        sleep_ms(500);
    }

    // 2) Loop principal: mede, filtra via média móvel e exibe
    while (true) {
        if (ms5637_read_temperature_pressure(&temperature, &pressure) == MS5637_STATUS_OK) {
            // Calcula altitude bruta
            float alt_raw = calc_altitude(pressure, baseline_pressure);

            // Insere no buffer circular
            altitude_window[altitude_index] = alt_raw;
            altitude_index = (altitude_index + 1) % ALTITUDE_WINDOW_SIZE;

            // Calcula média das últimas N leituras
            float sum = 0.0f;
            for (int i = 0; i < ALTITUDE_WINDOW_SIZE; ++i) {
                sum += altitude_window[i];
            }
            float alt_smooth = sum / ALTITUDE_WINDOW_SIZE;

            // Exibe valores
            printf("Altura suavizada: %.2f m | Alt bruta: %.2f m | P: %.2f mbar | T: %.2f °C\n",
                   alt_smooth, alt_raw, pressure, temperature);
        } else {
            printf("Erro ao ler sensor\n");
        }
        sleep_ms(500);
    }

    return 0;
}