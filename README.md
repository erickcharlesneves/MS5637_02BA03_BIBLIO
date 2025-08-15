
# Biblioteca, exemplos e rotinas de testes p/ sensor de pressão MS5637 - 02BA03 | Plataforma de SE: PCEIoT-Board V1.1.0 / Raspberry Pi Pico W

## 📖 Introdução
Este é o repositório de desenvolvimento da biblioteca/driver em C para configurar e acessar de forma fácil e modularizada o sensor de pressão barométrica e temperatura [**MS5637 - 02BA03**](https://github.com/anderson-pereira/PCEIoT-Board/blob/main/Datasheets/MS563702BA03.pdf)  via comunicação **I²C** com a plataforma de desenvolvimento [**PCEIoT-Board V1.1.0**](https://github.com/anderson-pereira/PCEIoT-Board/tree/main) baseada no microcontrolador RP2040 - [**Raspberry Pi Pico W**](https://github.com/anderson-pereira/PCEIoT-Board/blob/main/Datasheets/SC0918-RPI%20pico%20W.pdf)  com SDK do Pico.

O Sensor MS5637 - 02BA03 da TE Connectivity é barômetro digital de alta resolução, com ADC ΔΣ de 24 bits, de ultra baixo consumo e coeficientes de calibração gravados de fábrica, muito sensível, capaz de detectar uma mudança de  altitude ao nível do mar de 13 cm de ar! 

---

## 🔌 Conexões com o Raspberry Pi Pico W

| Pico W Pin  | MS5637 Pin | Função   |
|-------------|-----------|----------|
| GP4         | SDA       | I²C SDA  |
| GP5         | SCL       | I²C SCL  |
| 3V3         | VDD       | Alimentação |
| GND         | GND       | Terra    |

**OBS: Outras características e padrões mais detalhados de fabrica do sensor podem ser consultados no [datasheet](https://github.com/anderson-pereira/PCEIoT-Board/blob/main/Datasheets/MS563702BA03.pdf).


**I²C** configurado para **400 kHz (Fast Mode)**:

```c
#define MS5637_I2C i2c0
#define MS5637_I2C_SDA 4
#define MS5637_I2C_SCL 5
#define MS5637_I2C_FREQ 400000
```

## 📚Recursos da biblioteca

 - Inicialização do MS5637 e leitura robusta de PROM (com verificação
   CRC-4).

 - Leitura de temperatura (°C) e pressão (mbar **(milibar)** / hPa **(hectopascal)**) 1 hPa = 1 mbar.
 
 - Suporte a várias resoluções (OSR) — padrão: OSR_8192 (maior precisão).

 - Exemplos de uso com cálculo de altitude relativa e outros conforme pasta main_exemplos.

## 🔬Recursos Avançados:
### Resolução do MS5637

O sensor MS5637 suporta diferentes resoluções OSR:

-   `MS5637_OSR_8192`: Máxima resolução (17ms)
-   `MS5637_OSR_4096`: Alta resolução (9ms)
-   `MS5637_OSR_2048`: Resolução média (5ms)
-   `MS5637_OSR_1024`: Resolução básica (3ms)

## 📦 Estrutura da biblioteca

```
  📁 src/
		📁 ms5637_02ba03/		# (obrigatório) pasta da biblioteca em si. 
		├── ms5637.c        	# implementação da biblioteca.
		└── ms5637.h			# interface pública da biblioteca.
		
		📁 main_exemplos/			# (sugestão) pasta com exemplos de uso.
		├── Ex1_leitura.c   		# implementação simples de leitura contínua de temperatura e pressão atmosférica em mbar na saída serial.
		├── Ex2_barometro.c			# Cálculo barométrico com a formula barométrica.
		├── Ex3_varBarometrica.c 	# Calc. variação de altura baseada na pressão atm capturada ao ligar a placa como referencial.
		├── Ex3.1_varBaroMediaMov.c # Calc. idem Ex3, com Média móvel p/ reduzir flutuações e ruídos nas leituras.
		├── Ex3.2_varBaroKalman.c 	# Calc. idem Ex3, com filtro de Kalman p/ reduzir flutuações e ruídos nas leituras
		├── Ex3.3_varHypsometrica.c # Calc. idem Ex3, com correção da altitude pela equação hypsométrica pela temperatura da coluna de ar.
		├── Ex3.4_varHypsoKalman 	# Calc. idem Ex3,com correção pela eq. hypsométrica e filtro de Kalman.
		└── Ex4_ajustepressao.c 	# Calc. inverso da eq. barométrica para ajuste da pressão ao nível do mar baseado em altitude conhecida em metros (CG).

		📁 rotina_test/			# (sugestão) pasta com rotina de testes de validação do sensor.
		└── test_ms5637.c       # implementação da rotina de testes para o sensor.
		
     └── main.c 	# (sugestão) para verificação inicial do funcionameto.
     
 └── CMakeLists.txt  # Importante para correto acessos da biblioteca e seus exemplos.
 └── README.md 
```

## 🧭Instalação e Compilação


### ✅Pré-requisitos

[](https://github.com/erickcharlesneves/ProjetoIntegrado_PCEIoT_Board?tab=readme-ov-file#pr%C3%A9-requisitos)

-   **Pico SDK 2.2.0**  ou superior
-   **CMake 3.13**  ou superior
-   **GCC ARM**  toolchain (versão 14_2_Rel1 recomendada)
-   **Git**  para clonagem do repositório (opcionalmente recomendado) 

### Passos de Instalação


1.  **Clone o repositório com Git**:

git clone https://github.com/erickcharlesneves/ProjetoIntegrado_PCEIoT_Board.git
(Alternativamente - baixe a pasta que contém apenas o sensor e adicione a pasta do mesmo, ao seu projeto)

cd ProjetoIntegrado_PCEIOT_Board

2.  **Configure o ambiente Pico SDK**:

export PICO_SDK_PATH=/path/to/pico-sdk

3.  **Crie o diretório de build**:

mkdir build

4.  **Configure e compile**:

cmake -B build -G Ninja
ninja -C build

5.  **Gravação no Pico W**:
    
    Para isso utilizando a extensão  _Raspberry Pi Pico_:

	1.  Antes ative o modo  _**BOOTSEL**_  da  _Raspberry Pi Pico_  conectada ao PC digitando no terminal o comando:  `picotool reboot -f -u` (Ou Mantenha o botão BOOTSEL pressionado e conecte a Pico W ao seu PC.)
	2. Compile o projeto
	3. Rode o projeto(USB)
	
	Caso não consiga  rodar há outra forma, porém não tão recomendada :
    -   Copie o arquivo  `ProjetoIntegrado_PCEIoT_Board.uf2`  para o dispositivo
   
## :shipit: Requisitos
**Necessários:** 
 - [ ] Raspberry Pi Pico / Pico W (ou compatível com Pico SDK)
 - [ ] Pico SDK (`PICO_SDK_PATH` definida) configurado e funcionando (CMake)
 - [ ] Cabo USB / Fonte 3.3V
 - [ ] MS5637  (definido na lib com SDA	no GPIO4 (MS5637_I2C_SDA) SCL	no GPIO5 (MS5637_I2C_SCL) além da alimentação VCC	3.3V, GND.
 
**Outros utilizados:**
- [ ]   Placa:  **PCEIoT-Board-V1.1.0**
- [ ]   VSCode com extensão Raspberry Pi Pico

## 🔍 **Funções Públicas**

```c
void ms5637_init(void)
```

Inicializa a interface I2C na Raspberry Pi Pico, configurando os pinos SDA (GPIO 4) e SCL (GPIO 5) com frequência de 400 kHz. Envia um comando de reset ao sensor para garantir um estado conhecido e carrega os coeficientes de calibração da PROM. E realiza automaticamente a verificação de CRC dos dados lidos.

----------

```c
ms5637_status_t ms5637_reset(void)
```

Envia um comando de reset (0x1E) ao sensor via I2C, reinicializando seu estado interno e preparando-o para novas operações.  

_Retorno:_

-   `MS5637_STATUS_OK`  se o reset for bem-sucedido
    
-   `MS5637_STATUS_ERROR`  em falha de comunicação I2C
    

----------

```c
ms5637_status_t ms5637_read_temperature_pressure(float  _temperature, float  _pressure)
```
Realiza medições simultâneas de temperatura e pressão com a resolução configurada (padrão: OSR 8192).  

_Parâmetros:_

-   `temperature`: Ponteiro para armazenar temperatura em °C
    
-   `pressure`: Ponteiro para armazenar pressão em mbar
    

_Processo:_

1.  Inicia conversão de temperatura (D2)
    
2.  Aguarda tempo de conversão conforme OSR
    
3.  Lê valor ADC de temperatura
    
4.  Repete processo para pressão (D1)
    
5.  Calcula valores usando coeficientes da PROM
    

_Retorno:_

-   `MS5637_STATUS_OK`  em sucesso
    
-   `MS5637_STATUS_ERROR`  em falha de comunicação
    
-   `MS5637_STATUS_CRC_ERROR`  se coeficientes inválidos
    

----------

## 🔩Características Principais

-   Suporte a múltiplas resoluções (OSR 256 a 8192)
    
-   Cálculo automático de compensação térmica
    
-   Verificação de integridade dos dados (CRC4)
    
-   Interface I2C otimizada para Raspberry Pi Pico


## 🧮 Cálculo de altitude relativa
A biblioteca fornece pressão em mbar (hPa). Para calcular a variação de altitude relativa a partir de uma pressão de referência (baseline) ao ligar a placa, use a fórmula padrão do barômetro:

```c
float calc_altitude(float pressure_hpa, float baseline_hpa) {
    return 44330.0f * (1.0f - powf(pressure_hpa / baseline_hpa, 1.0f / 5.255f));
}
```
## 🧪 Rotina de testes 

Arquivo `test_ms5637.c`:

Para testar a rotina de testes e sensor além dos exemplos:

Remova o comentário `#` correspondente ao teste no arquivo CMakeLists.txt em:

```c
add_executable(MS5637_02BA03_BIBLIO

`#` src/ms5637_02ba03/ms5637.c
src/rotina_test/test_ms5637.c  <–(removido o #) descomentando a linha correspondente
```

**O objetivo é verificar:**

- Comunicação I2C correta com o sensor

- Leitura válida dos coeficientes da PROM

- Validação do CRC

- Leitura confiável de pressão e temperatura

- Detecção de erro de conexão


Essa rotina pode ser usada tanto em desenvolvimento quanto em validação de hardware e firmware.

___________


**Demais Exemplos de uso:**  Documentarei  descritivo futuramente.
____________
## 🔧 Principais comandos de registrador utilizados na biblioteca 

| Função  | Comando base | Descrição   |
|-------------|-----------|----------|
| Reset         | 0x1E      | Reinicia o sensor  |
| Conversão pressão (D1)         | 0x40–0x4A      | Depende do OSR  |
| Conversão temperatura (D2)         | 0x50–0x5A      | Depende do OSR |
| Leitura ADC        | 0x00       | Retorna resultado da última conversão    |
| Leitura PROM        | 0xA0–0xAE      | 7 endereços para coeficientes   |

## 📊 **Tabela de características resumidas do sensor em si**


| **Parâmetro** | **Valor**                            |**Unidade** | **Observações** |
|---------------------------------------|----------------------------------------------|------------------|-----------------|
| **Faixa de pressão operacional**      | 300 a 1200                                   | mbar             | Faixa típica de operação |
| **Faixa de pressão estendida**        | 10 a 2000                                    | mbar             | Sem garantia total de especificação |
| **Faixa de temperatura**              | -40 a +85                                    | °C               | Totalmente funcional |
| **Resolução máxima (pressão)**        | 0,016                                        | mbar             | OSR = 8192 |
| **Resolução máxima (temperatura)**    | 0,002                                        | °C               | OSR = 8192 |
| **Consumo em standby**                | ≤ 0,1                                        | µA               | Modo idle |
| **Consumo em medição**                 | 0,63 a 20                                    | µA               | Dependente do OSR |
| **Tensão de alimentação (VDD)**       | 1,5 a 3,6                                    | V                | Recom. 3,3 V |
| **Tempo conversão mínima**            | 0,54                                         | ms               | OSR = 256 |
| **Tempo conversão máxima**            | 16,44                                        | ms               | OSR = 8192 |
| **Interface de comunicação**          | I²C                                          | —                | Endereço 0x76 |
| **Coeficientes de calibração**        | 6                                            | —                | Armazenados na PROM interna |
| **Precisão absoluta**                  | ±2                                           | mbar             | De 700 a 1100 mbar, 0–+60 °C |
| **Dimensões do encapsulamento**       | 3 × 3 × 0,9                                  | mm³              | QFN |
| **Resistência ESD**                   | 2                                            | kV               | HBM |

---

## 🎯 **Conclusão**  


Esta biblioteca representa uma implementação robusta e otimizada para o sensor barométrico  **MS5637-02BA03**  na plataforma  **Raspberry Pi Pico W**, oferecendo:

1.  **Precisão e Performance:**
    
    -   Suporte completo às 6 resoluções OSR (256 a 8192)
        
    -   Implementação fiel dos algoritmos de compensação térmica do fabricante TE Connectivity.
        
    -   Leituras estáveis com resolução de até 0.016 mbar (pressão) e 0.002°C (temperatura)
        
2.  **Confiabilidade Operacional:**
    
    -   Verificação CRC-4 dos coeficientes de calibração da PROM.
        
    -   Tratamento robusto de erros de comunicação I²C.
        
    -   Tempos de conversão rigorosamente respeitados conforme datasheet.
        
3.  **Modularidade e Extensibilidade:**
    
    -   Arquitetura desacoplada permitindo fácil portabilidade.
        
    -   Exemplos práticos cobrindo desde leitura básica até técnicas avançadas (Kalman, Hypsometric).
        
    -   Interface clara com funções bem documentadas para rápida integração.
        
4.  **Validação Rigorosa:**
    
    -   Rotina de testes abrangente (comunicação, CRC, leituras).
        
    -   Implementação de referência para a PCEIoT-Board V1.1.0.
        
    -   Compatibilidade comprovada com SDK Pico 2.2.0+
        
5.  **Otimizações Específicas:**
    
    -   Comunicação I²C a 400 kHz (Fast Mode).
        
    -   Cálculos inteiros de 64 bits para máxima precisão.
        
    -   Algoritmos de compensação térmica implementados conforme AN520.
        

**Impacto Prático:**  A biblioteca habilita aplicações profissionais de meteorologia, sistemas de navegação por pressão, estações ambientais compactas e dispositivos IoT de monitoramento atmosférico, superando as limitações de soluções genéricas através de:

-   Cálculo de altitude relativa com múltiplas técnicas (barométrica, hipsométrica).
    
-   Implementações otimizadas de filtragem (média móvel, Kalman).
    
-   Correções avançadas para variações térmicas.
    




## 📚 Referência

-   **Datasheet MS5637-02BA03 – TE Connectivity**  
    https://www.te.com/commerce/DocumentDelivery/DDEController?Action=srchrtrv&DocNm=MS5637-02BA03


## Contribuições:

Caso tenha sugestões ou melhorias, sinta-se à vontade para contribuir! 🚀✨ 
Sinta-se à vontade para levantar novas questões, e abrir pull requests. Considere dar uma estrela e bifurcar este repositório!

Se você tiver alguma dúvida sobre, não hesite em entrar em contato comigo no Gmail: [erick.cassiano@ee.ufcg.edu.br](mailto:erick.cassiano@ee.ufcg.edu.br) ou abrir um problema no GitHub.

## Contributing:

Contributions are always welcomed. Feel free to raise new issues, file new PRs. Consider giving it a star and fork this repo!

If you have any question about this opinionated list, do not hesitate to contact me on Gmail: [erick.cassiano@ee.ufcg.edu.br](mailto:erick.cassiano@ee.ufcg.edu.br) or open an issue on GitHub.

[![Linkedin](https://img.shields.io/badge/linkedin-%230077B5.svg?&style=for-the-badge&logo=linkedin&logoColor=white)](https://www.linkedin.com/in/erick-charles-neves/)


