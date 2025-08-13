// Exemplo de uso do sensor MS5637-02BA03 para calcular a altitude baseada na pressão atmosférica
#include <stdio.h>
#include <math.h>
#include "pico/stdlib.h"
#include "src/ms5637_02ba03/ms5637.h"


// Função para calcular a altitude baseada na pressão atmosférica ao nível do mar P0(1013.25 mbar) considerando a média global
// Fórmula barométrica: h = 44330 * (1 - (P / P0)^(1/5.255))
// onde P0 é a pressão ao nível do mar (nesse exemplo pode ser calibrada para o meu local manualmente)    
float calc_altitude(float pressure, float sea_level_pressure) {
    return 44330.0f * (1.0f - powf(pressure / sea_level_pressure, 1.0f / 5.255f));
}

int main() {
    stdio_init_all();
    ms5637_init();

    const float sea_level_pressure = 1013.25f; // mbar média global (pode ser calibrado para o local 1016.0f média local para Campina Grande)

    while (true) {
        float temperature, pressure;
        if (ms5637_read_temperature_pressure(&temperature, &pressure) == MS5637_STATUS_OK) {
            float altitude = calc_altitude(pressure, sea_level_pressure);
            printf("Altura estimada: %.2f m | Pressão: %.2f mbar | Temp: %.2f °C\n",
                   altitude, pressure, temperature);
        } else {
            printf("Erro de leitura do sensor\n");
        }
        sleep_ms(1000);
    }
}
