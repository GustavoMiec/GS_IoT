#define THINGER_SERIAL_DEBUG
#include <ThingerESP32.h>
#include <Wire.h>
#include <DHT.h>

#define USERNAME "GustavoMiec"  // Substitua pelo seu username do Thinger.io
#define DEVICE_ID "GsOit"       // Substitua pelo seu Device ID no Thinger.io
#define DEVICE_CREDENTIAL "qMZZFK2K%TxNY?mz"  // Substitua pela sua credential do Thinger.io

#define SSID "Wokwi-GUEST"  // Rede WiFi
#define SSID_PASSWORD ""    // Senha da rede WiFi

#define DHTPIN 15        // Pino conectado ao sensor DHT
#define DHTTYPE DHT22    // Tipo do sensor DHT

#define PIR_PIN 14       // Pino do sensor PIR (presença)
#define BLUE_LED_PIN 2   // LED Azul - Presença
#define YELLOW_LED_PIN 4 // LED Amarelo - Dispositivo ligado
#define RED_LED_PIN 16   // LED Vermelho - AC ligado
#define ORANGE_LED_PIN 13 // LED Laranja - Muito quente
#define CYAN_LED_PIN 12  // LED Ciano - Muito frio

DHT dht(DHTPIN, DHTTYPE);   // Instancia o sensor DHT
ThingerESP32 thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);

// Variáveis para controle
bool device_on = false;         // Dispositivo ligado/desligado
bool ac_on = false;             // Estado do ar-condicionado
bool presence_detected = false; // Presença detectada
String alert_message = "Sem alertas"; // Mensagem de alerta

// Protótipos
void readTemperatureHumidity();
void controlPresenceLED();
void controlLEDs(float temperature);
void setLEDState(int pin, bool state);

void setup() {
    Serial.begin(115200);
    dht.begin();

    // Conexão WiFi e Thinger.io
    thing.add_wifi(SSID, SSID_PASSWORD);

    // Configuração dos pinos
    pinMode(PIR_PIN, INPUT);
    pinMode(BLUE_LED_PIN, OUTPUT);
    pinMode(YELLOW_LED_PIN, OUTPUT);
    pinMode(RED_LED_PIN, OUTPUT);
    pinMode(ORANGE_LED_PIN, OUTPUT);
    pinMode(CYAN_LED_PIN, OUTPUT);

    // Recurso: temperatura no Thinger.io
    thing["temperature"] >> [](pson &out) {
        out = dht.readTemperature();
    };

    // Recurso: umidade no Thinger.io
    thing["humidity"] >> [](pson &out) {
        out = dht.readHumidity();
    };

    // Recurso: presença detectada no Thinger.io
    thing["presence"] >> [](pson &out) {
        out = presence_detected;
    };

    // Recurso: ligar/desligar dispositivo no Thinger.io
    thing["device_on"] << [](pson &in) {
        if (in.is_empty()) in = device_on;
        else device_on = in;
    };

    // Recurso: ligar/desligar AC no Thinger.io
    thing["ac_on"] << [](pson &in) {
        if (in.is_empty()) in = ac_on;
        else ac_on = in;
    };

    // Recurso: mensagem de alerta no Thinger.io
    thing["alert"] >> [](pson &out) {
        out = alert_message;
    };
}

void loop() {
    static unsigned long previousMillis = 0;
    const long interval = 2000; // Intervalo de 2 segundos
    unsigned long currentMillis = millis();

    thing.handle();  // Processa as requisições do Thinger.io

    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;

        if (device_on) {
            digitalWrite(YELLOW_LED_PIN, HIGH);  // Liga LED amarelo
            readTemperatureHumidity();          // Lê temperatura e umidade
            controlPresenceLED();               // Verifica presença
        } else {
            digitalWrite(YELLOW_LED_PIN, LOW);  // Desliga LED amarelo
            digitalWrite(RED_LED_PIN, LOW);     // Desliga LED vermelho
        }

        // Controle do LED amarelo (apaga se não houver presença)
        if (!presence_detected || !device_on) {
            digitalWrite(YELLOW_LED_PIN, LOW);  // Desliga o LED amarelo se não houver presença ou dispositivo desligado
        } else {
            digitalWrite(YELLOW_LED_PIN, HIGH);  // Acende o LED amarelo quando houver presença e dispositivo ligado
        }
    }
}

void readTemperatureHumidity() {
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    if (isnan(temperature) || isnan(humidity)) {
        Serial.println("Falha ao ler do sensor DHT!");
        return;
    }

    Serial.print("Umidade: ");
    Serial.print(humidity);
    Serial.print("%  Temperatura: ");
    Serial.print(temperature);
    Serial.println("°C");

    controlLEDs(temperature);

    // Define mensagens de alerta no Thinger.io
    if (temperature >= 38) {
        alert_message = "Muito quente!";
        Serial.println(alert_message);
    } else if (temperature <= 16) {
        alert_message = "Muito frio!";
        Serial.println(alert_message);
    } else {
        alert_message = "Temperatura normal";
    }
}

void controlPresenceLED() {
    presence_detected = digitalRead(PIR_PIN) == HIGH;

    if (presence_detected) {
        Serial.println("Presença detectada!");
        digitalWrite(BLUE_LED_PIN, HIGH);  // Acende LED azul
        digitalWrite(RED_LED_PIN, HIGH);   // Acende LED vermelho
    } else {
        Serial.println("Sem presença.");
        digitalWrite(BLUE_LED_PIN, LOW);   // Apaga LED azul
        digitalWrite(RED_LED_PIN, LOW);    // Apaga LED vermelho (garantir que apaga quando não houver presença)
    }
}

void setLEDState(int pin, bool state) {
    digitalWrite(pin, state ? HIGH : LOW);
}

void controlLEDs(float temperature) {
    setLEDState(ORANGE_LED_PIN, temperature >= 38);  // LED laranja para altas temperaturas
    setLEDState(CYAN_LED_PIN, temperature <= 16);    // LED ciano para baixas temperaturas
    setLEDState(RED_LED_PIN, device_on && ac_on);    // LED vermelho se dispositivo e AC estiverem ligados
}
