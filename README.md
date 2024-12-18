# Projeto IoT - Monitoramento de Temperatura e Presença com ESP32

Este projeto é focado em **economia de luz**, utilizando a tecnologia IoT para automatizar o controle de dispositivos baseados em temperatura e presença de pessoas no ambiente. A ideia principal é garantir que os dispositivos, como o ar-condicionado, sejam desligados automaticamente quando não houver presença no ambiente ou quando a temperatura estiver fora de uma faixa ideal. O sistema também monitora a umidade e a temperatura do ambiente, acionando alertas caso as condições sejam extremas.

---

## Integrantes

- **Victor Fanfoni** - RM: 99173
- **Helena Paixão** - RM: 550929
- **Gustavo Costa** - RM: 99102
- **Julia Nery** - RM: 552292
- **Giulia Pina** - RM: 97694

---

## Funcionalidades

- **Monitoramento de Temperatura e Umidade**: O sensor **DHT22** coleta dados de temperatura e umidade do ambiente.
- **Detecção de Presença**: O sensor **PIR** detecta a presença de pessoas, ativando ou desativando LEDs e enviando dados para a plataforma **Thinger.io**.
- **Controle de LEDs**:
  - **LED Azul**: Indica a presença de uma pessoa.
  - **LED Amarelo**: Indica que o dispositivo está ligado.
  - **LED Vermelho**: Indica se o ar-condicionado está ligado.
  - **LED Laranja**: Indica que a temperatura está muito alta (acima de 38°C).
  - **LED Ciano**: Indica que a temperatura está muito baixa (abaixo de 16°C).

---

## Componentes Necessários

- **ESP32** (ou qualquer outra placa compatível com o Thinger.io)
- **Sensor DHT22** (para medir temperatura e umidade)
- **Sensor PIR** (para detectar presença)
- **LEDs**:
  - Azul, Amarelo, Vermelho, Laranja, Ciano
- **Resistores** (para os LEDs)
- **Fios de conexão**

---

## Dependências

- **ThingerESP32**: Biblioteca para integração com o Thinger.io.
- **DHT**: Biblioteca para leitura do sensor de temperatura e umidade **DHT22**.
- **Wire**: Biblioteca para comunicação I2C (se necessário).
- **Arduino IDE**: Para programar o ESP32.

---

## Configuração

### 1. Configuração do Thinger.io

1. Crie uma conta no [Thinger.io](https://thinger.io/).
2. Crie um **Device** no painel de controle do Thinger.io.
3. Anote o **USERNAME**, **DEVICE_ID**, e **DEVICE_CREDENTIAL** do seu dispositivo.

### 2. Conectar o ESP32 à Rede Wi-Fi

No código, você deve configurar sua rede Wi-Fi:

```cpp
#define SSID "SeuSSID"           // Nome da sua rede Wi-Fi
#define SSID_PASSWORD "SuaSenha" // Senha da sua rede Wi-Fi
```

### 3. Conectar os Sensores

- **Sensor DHT22**: Conecte o pino de dados do DHT22 ao pino 15 do ESP32.
- **Sensor PIR**: Conecte o sensor PIR ao pino 14 do ESP32.
- **LEDs**: Conecte os LEDs aos pinos definidos no código (azul, amarelo, vermelho, laranja e ciano).

### 4. Carregar o Código

1. Abra o **Arduino IDE**.
2. Selecione a placa **ESP32** e a porta correta.
3. Carregue o código no ESP32.

### 5. Monitoramento Remoto

Após carregar o código e conectar o dispositivo à sua rede Wi-Fi, o ESP32 estará enviando dados para o **Thinger.io**, onde você pode monitorar:

- Temperatura
- Umidade
- Presença detectada
- Alertas de temperatura (quente/frio)

Você pode controlar o dispositivo, ligar/desligar o ar-condicionado e visualizar os dados em tempo real.

---

## Estrutura do Código

O código é dividido em várias funções principais:

- **setup()**: Configura o dispositivo, os sensores, a rede Wi-Fi e a plataforma Thinger.io.
- **loop()**: Lê os sensores e controla os LEDs com base nos dados recebidos.
- **readTemperatureHumidity()**: Lê os dados de temperatura e umidade do sensor DHT22.
- **controlPresenceLED()**: Controla os LEDs de presença e de acendimento com base no sensor PIR.
- **controlLEDs()**: Controla os LEDs de acordo com a temperatura ambiente.
- **setLEDState()**: Define o estado de cada LED (ligado ou desligado).

---

## Thinger.io API

O projeto utiliza a API do Thinger.io para enviar e receber dados:

- **temperature**: Envia a temperatura lida do sensor DHT22.
- **humidity**: Envia a umidade lida do sensor DHT22.
- **presence**: Envia o status de presença detectada.
- **device_on**: Controle do estado do dispositivo (ligado/desligado).
- **ac_on**: Controle do estado do ar-condicionado.
- **alert**: Envia mensagens de alerta sobre a temperatura.

---

## Exemplo de Dados

Você pode monitorar e controlar os seguintes parâmetros no Thinger.io:

- **Temperatura**: Valor da temperatura atual (°C).
- **Umidade**: Valor da umidade atual (%).
- **Presença**: Status de presença detectada (true ou false).
- **Dispositivo Ligado**: Estado do dispositivo (true ou false).
- **Ar-Condicionado**: Estado do ar-condicionado (true ou false).
- **Alertas**: Mensagens de alerta sobre a temperatura (muito quente, muito frio ou temperatura normal).

---

## Conclusão

Este projeto permite monitorar a temperatura, umidade e presença de forma simples, utilizando o **ESP32** e a plataforma **Thinger.io**. Com LEDs indicadores e controle remoto, ele é uma base sólida para sistemas de automação residencial e IoT, visando **economia de luz** ao controlar o uso de dispositivos com base na presença e na temperatura ambiente.
