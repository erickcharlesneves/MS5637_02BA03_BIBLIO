# Biblioteca e exemplos de uso para o sensor de pressão MS5637 - 02BA03 - Raspberry Pi Pico - Placa PCEIoT-Board

### *README EM EDIÇÃO....*

### Biblioteca ms5637-02ba03 para Raspberry Pi Pico (C / Pico SDK)

Descrição: Biblioteca em C para leitura do sensor barométrico MS5637-02BA03 usando Raspberry Pi Pico (Pico SDK) para a Placa PCEIoT-Board. 
Fornece inicialização, leitura de temperatura e pressão e utilitários para cálculo de altitude relativa. 

Projetada para ser simples de integrar em projetos que usem I²C.

## Recursos
Inicialização do MS5637 e leitura robusta de PROM (com verificação CRC4).

Leitura de temperatura (°C) e pressão (mbar / hPa).

Suporte a várias resoluções (OSR) — padrão: OSR_8192 (maior precisão).

Exemplos de uso com cálculo de altitude relativa.


## Conteúdo do repositório
src/ms5637_02ba03/ms5637.c — implementação.

src/ms5637_02ba03/ms5637.h — interface pública.

examples/ — (sugestão) exemplos de main.c 

rotina_test/ - Rotina de testes

## Requisitos
Raspberry Pi Pico / Pico W (ou compatível com Pico SDK)

Pico SDK configurado e funcionando (CMake)

Cabo USB / Fonte 3.3V

MS5637 Pin	Pico (definido na lib)
SDA	GPIO4 (MS5637_I2C_SDA)
SCL	GPIO5 (MS5637_I2C_SCL)
VCC	3.3V
GND	GND

Endereço I²C definido na biblioteca: 0x76 (MS5637_ADDR).


API / Uso
Protótipos (em ms5637.h)

  
## Cálculo de altitude relativa
A biblioteca fornece pressão em mbar (hPa). Para calcular a variação de altitude relativa a partir de uma pressão de referência (baseline), use a fórmula padrão do barômetro:

```c
float calc_altitude(float pressure_hpa, float baseline_hpa) {
    return 44330.0f * (1.0f - powf(pressure_hpa / baseline_hpa, 1.0f / 5.255f));
}
```
README EM EDIÇÂO....
