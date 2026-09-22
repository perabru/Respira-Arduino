
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <ESP32Servo.h>
#include <time.h>
#include <math.h>

// ==========================================
// REDE WI-FI
// ==========================================

const char* WIFI_SSID = "BRUNO";
const char* WIFI_SENHA = "123456789";

// ==========================================
// HIVEMQ CLOUD
// ==========================================

const char* MQTT_HOST =
  "125e4c5cf1a84e15a472be4cda4c9761.s1.eu.hivemq.cloud";

const int MQTT_PORT = 8883;

const char* MQTT_USER = "perabru2";
const char* MQTT_SENHA = "123456789";

// Topicos MQTT
const char* TOPICO_DADOS =
  "projeto/controle_ar/telemetria";

const char* TOPICO_STATUS =
  "projeto/controle_ar/status";

// ==========================================
// CERTIFICADO RAIZ ISRG ROOT X1
// Troque pela CA correta se o broker usar
// outra cadeia de certificados.
// ==========================================

static const char CERTIFICADO_CA[] = R"EOF(
-----BEGIN CERTIFICATE-----
MIIFazCCA1OgAwIBAgIRAIIQz7DSQONZRGPgu2OCiwAwDQYJKoZIhvcNAQELBQAw
TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh
cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwHhcNMTUwNjA0MTEwNDM4
WhcNMzUwNjA0MTEwNDM4WjBPMQswCQYDVQQGEwJVUzEpMCcGA1UEChMgSW50ZXJu
ZXQgU2VjdXJpdHkgUmVzZWFyY2ggR3JvdXAxFTATBgNVBAMTDElTUkcgUm9vdCBY
MTCCAiIwDQYJKoZIhvcNAQEBBQADggIPADCCAgoCggIBAK3oJHP0FDfzm54rVygc
h77ct984kIxuPOZXoHj3dcKi/vVqbvYATyjb3miGbESTtrFj/RQSa78f0uoxmyF+
0TM8ukj13Xnfs7j/EvEhmkvBioZxaUpmZmyPfjxwv60pIgbz5MDmgK7iS4+3mX6U
A5/TR5d8mUgjU+g4rk8Kb4Mu0UlXjIB0ttov0DiNewNwIRt18jA8+o+u3dpjq+sW
T8KOEUt+zwvo/7V3LvSye0rgTBIlDHCNAymg4VMk7BPZ7hm/ELNKjD+Jo2FR3qyH
B5T0Y3HsLuJvW5iB4YlcNHlsdu87kGJ55tukmi8mxdAQ4Q7e2RCOFvu396j3x+UC
B5iPNgiV5+I3lg02dZ77DnKxHZu8A/lJBdiB3QW0KtZB6awBdpUKD9jf1b0SHzUv
KBds0pjBqAlkd25HN7rOrFleaJ1/ctaJxQZBKT5ZPt0m9STJEadao0xAH0ahmbWn
OlFuhjuefXKnEgV4We0+UXgVCwOPjdAvBbI+e0ocS3MFEvzG6uBQE3xDk3SzynTn
jh8BCNAw1FtxNrQHusEwMFxIt4I7mKZ9YIqioymCzLq9gwQbooMDQaHWBfEbwrbw
qHyGO0aoSCqI3Haadr8faqU9GY/rOPNk3sgrDQoo//fb4hVC1CLQJ13hef4Y53CI
rU7m2Ys6xt0nUW7/vGT1M0NPAgMBAAGjQjBAMA4GA1UdDwEB/wQEAwIBBjAPBgNV
HRMBAf8EBTADAQH/MB0GA1UdDgQWBBR5tFnme7bl5AFzgAiIyBpY9umbbjANBgkq
hkiG9w0BAQsFAAOCAgEAVR9YqbyyqFDQDLHYGmkgJykIrGF1XIpu+ILlaS/V9lZL
ubhzEFnTIZd+50xx+7LSYK05qAvqFyFWhfFQDlnrzuBZ6brJFe+GnY+EgPbk6ZGQ
3BebYhtF8GaV0nxvwuo77x/Py9auJ/GpsMiu/X1+mvoiBOv/2X/qkSsisRcOj/KK
NFtY2PwByVS5uCbMiogziUwthDyC3+6WVwW6LLv3xLfHTjuCvjHIInNzktHCgKQ5
ORAzI4JMPJ+GslWYHb4phowim57iaztXOoJwTdwJx4nLCgdNbOhdjsnvzqvHu7Ur
TkXWStAmzOVyyghqpZXjFaH3pO3JLF+l+/+sKAIuvtd7u+Nxe5AW0wdeRlN8NwdC
jNPElpzVmbUq4JUagEiuTDkHzsxHpFKVK7q4+63SM1N95R1NbdWhscdCb+ZAJzVc
oyi3B43njTOQ5yOf+1CceWxG1bQVs5ZufpsMljq4Ui0/1lvh+wjChP4kqKOJ2qxq
4RgqsahDYVvTH9w7jXbyLeiNdd8XM2w9U/t7y0Ff/9yi0GE44Za4rF2LN9d11TPA
mRGunUHBcnWEvgJBQl9nJEiU0Zsnvgc/ubhPgXRR4Xq37Z0j4r7g1SgEEzwxA57d
emyPxgcYxn/eR44/KJ4EBs+lVDR3veyJm+kXQ99b21/+jh5Xos1AnX5iItreGCc=
-----END CERTIFICATE-----
)EOF";

// ==========================================
// PINAGEM DO ESP32
// ==========================================

#define PINO_SERVO       5
#define PINO_POTEN       12
#define PINO_DHT         15

#define PINO_VERDE       25
#define PINO_AMARELO     26
#define PINO_VERMELHO    27

#define TIPO_DHT DHT11

// ==========================================
// OBJETOS
// ==========================================

DHT dht(PINO_DHT, TIPO_DHT);

Servo meuServo;

WiFiClientSecure clienteSeguro;
PubSubClient mqtt(clienteSeguro);

// ==========================================
// VARIAVEIS
// ==========================================

float leituraFiltrada = 0;

int anguloServo = 0;
int ultimoAngulo = -1;

float temperatura = NAN;
float umidade = NAN;

unsigned long ultimaLeituraDHT = 0;
unsigned long ultimaLeituraPot = 0;
unsigned long ultimaTentativaMQTT = 0;
unsigned long ultimaTentativaWiFi = 0;

bool horarioSincronizado = false;

char mqttClientId[40];

// ==========================================
// CONTROLE DOS LEDS
// ==========================================

const char* atualizarLEDs(int angulo) {

  digitalWrite(PINO_VERDE, LOW);
  digitalWrite(PINO_AMARELO, LOW);
  digitalWrite(PINO_VERMELHO, LOW);

  if (angulo <= 60) {

    digitalWrite(PINO_VERDE, HIGH);

    return "VERDE";

  } else if (angulo <= 120) {

    digitalWrite(PINO_AMARELO, HIGH);

    return "AMARELO";

  } else {

    digitalWrite(PINO_VERMELHO, HIGH);

    return "VERMELHO";

  }
}

// ==========================================
// CONEXAO WI-FI
// ==========================================

void conectarWiFi() {

  if (WiFi.status() == WL_CONNECTED) {
    return;
  }

  if (millis() - ultimaTentativaWiFi < 10000) {
    return;
  }

  ultimaTentativaWiFi = millis();

  Serial.println();
  Serial.println("Conectando ao Wi-Fi...");

  WiFi.disconnect();
  WiFi.begin(WIFI_SSID, WIFI_SENHA);
}

// ==========================================
// SINCRONIZACAO DO RELOGIO
// Necessaria para validar os certificados TLS
// ==========================================

void sincronizarHorario() {

  if (WiFi.status() != WL_CONNECTED) {
    return;
  }

  if (horarioSincronizado) {
    return;
  }

  static bool ntpIniciado = false;

  if (!ntpIniciado) {

    configTime(
      0,
      0,
      "pool.ntp.org",
      "time.google.com"
    );

    ntpIniciado = true;

    Serial.println("Sincronizando horario...");
  }

  if (time(nullptr) > 1700000000) {

    horarioSincronizado = true;

    Serial.println("Horario sincronizado!");
  }
}

// ==========================================
// CONEXAO MQTT
// ==========================================

void conectarMQTT() {

  if (WiFi.status() != WL_CONNECTED) {
    return;
  }

  if (!horarioSincronizado) {
    return;
  }

  if (mqtt.connected()) {
    return;
  }

  if (millis() - ultimaTentativaMQTT < 5000) {
    return;
  }

  ultimaTentativaMQTT = millis();

  Serial.println("Conectando ao HiveMQ Cloud...");

  bool conectado = mqtt.connect(
    mqttClientId,
    MQTT_USER,
    MQTT_SENHA,
    TOPICO_STATUS,
    0,
    true,
    "offline"
  );

  if (conectado) {

    Serial.println("MQTT conectado!");

    mqtt.publish(
      TOPICO_STATUS,
      "online",
      true
    );

  } else {

    Serial.print("Falha MQTT. Estado: ");
    Serial.println(mqtt.state());

  }
}

// ==========================================
// CONTROLE DO SERVO
// ==========================================

void controlarServo() {

  if (millis() - ultimaLeituraPot < 30) {
    return;
  }

  ultimaLeituraPot = millis();

  int leitura = analogRead(PINO_POTEN);

  // Filtro para suavizar a leitura
  leituraFiltrada =
    leituraFiltrada * 0.8f +
    leitura * 0.2f;

  // Converte potenciometro para angulo
  anguloServo = map(
    (int)leituraFiltrada,
    0,
    4095,
    0,
    180
  );

  anguloServo = constrain(
    anguloServo,
    0,
    180
  );

  if (anguloServo != ultimoAngulo) {

    meuServo.write(anguloServo);

    atualizarLEDs(anguloServo);

    ultimoAngulo = anguloServo;

    Serial.print("Servo: ");
    Serial.print(anguloServo);
    Serial.println(" graus");

  }
}

// ==========================================
// ENVIO DE DADOS MQTT
// ==========================================

void publicarDados() {

  if (!mqtt.connected()) {

    Serial.println(
      "MQTT desconectado. Leitura nao enviada."
    );

    return;
  }

  if (isnan(temperatura) || isnan(umidade)) {
    return;
  }

  const char* estadoLED = atualizarLEDs(anguloServo);

  char mensagem[256];

  snprintf(
    mensagem,
    sizeof(mensagem),

    "{"
      "\"temperatura\":%.1f,"
      "\"umidade\":%.1f,"
      "\"servo\":%d,"
      "\"led\":\"%s\","
      "\"wifi_rssi\":%ld,"
      "\"uptime_ms\":%lu"
    "}",

    temperatura,
    umidade,
    anguloServo,
    estadoLED,
    (long)WiFi.RSSI(),
    millis()
  );

  bool enviado = mqtt.publish(
    TOPICO_DADOS,
    mensagem
  );

  if (enviado) {

    Serial.println("Dados enviados ao MQTT:");
    Serial.println(mensagem);

  } else {

    Serial.println("Falha ao publicar MQTT.");

  }
}

// ==========================================
// LEITURA DO DHT11
// ==========================================

void monitorarAmbiente() {

  if (millis() - ultimaLeituraDHT < 2000) {
    return;
  }

  ultimaLeituraDHT = millis();

  float novaTemperatura = dht.readTemperature();
  float novaUmidade = dht.readHumidity();

  if (
    isnan(novaTemperatura) ||
    isnan(novaUmidade)
  ) {

    temperatura = NAN;
    umidade = NAN;

    Serial.println("Erro na leitura do DHT11!");

    return;
  }

  temperatura = novaTemperatura;
  umidade = novaUmidade;

  Serial.println();
  Serial.println("========================");

  Serial.print("Temperatura: ");
  Serial.print(temperatura);
  Serial.println(" C");

  Serial.print("Umidade: ");
  Serial.print(umidade);
  Serial.println(" %");

  Serial.print("Servo: ");
  Serial.print(anguloServo);
  Serial.println(" graus");

  Serial.println("========================");

  publicarDados();
}

// ==========================================
// CONFIGURACAO INICIAL
// ==========================================

void setup() {

  Serial.begin(115200);

  pinMode(PINO_VERDE, OUTPUT);
  pinMode(PINO_AMARELO, OUTPUT);
  pinMode(PINO_VERMELHO, OUTPUT);

  digitalWrite(PINO_VERDE, LOW);
  digitalWrite(PINO_AMARELO, LOW);
  digitalWrite(PINO_VERMELHO, LOW);

  analogReadResolution(12);

  dht.begin();

  // Configuracao do servo
  meuServo.setPeriodHertz(50);
  meuServo.attach(PINO_SERVO, 500, 2400);

  leituraFiltrada = analogRead(PINO_POTEN);

  anguloServo = constrain(
    map(
      (int)leituraFiltrada,
      0,
      4095,
      0,
      180
    ),
    0,
    180
  );

  meuServo.write(anguloServo);

  atualizarLEDs(anguloServo);

  ultimoAngulo = anguloServo;

  // Configuracao MQTT seguro
  clienteSeguro.setCACert(CERTIFICADO_CA);

  mqtt.setServer(
    MQTT_HOST,
    MQTT_PORT
  );

  mqtt.setBufferSize(512);

  mqtt.setKeepAlive(30);

  // ID unico baseado no MAC do ESP32
  uint64_t mac = ESP.getEfuseMac();

  snprintf(
    mqttClientId,
    sizeof(mqttClientId),
    "controle-ar-%04X%08X",
    (unsigned int)(mac >> 32),
    (unsigned int)mac
  );

  // Conexao inicial
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_SENHA);

  Serial.println();
  Serial.println("========================");
  Serial.println("CONTROLE DE AR - MQTT");
  Serial.println("========================");

  Serial.print("ID: ");
  Serial.println(mqttClientId);

  Serial.println("ESP32 inicializado!");
}

// ==========================================
// LOOP PRINCIPAL
// ==========================================

void loop() {

  // Controle local independente da rede
  controlarServo();

  // Manutencao do Wi-Fi
  conectarWiFi();

  // Atualiza o horario para TLS
  sincronizarHorario();

  // Manutencao da conexao MQTT
  conectarMQTT();

  // Processamento MQTT
  if (mqtt.connected()) {
    mqtt.loop();
  }

  // Leitura e publicacao do DHT11
  monitorarAmbiente();

}