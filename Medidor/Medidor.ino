#include <WiFi.h>
#include <HTTPClient.h>
#include <Adafruit_BMP085.h>
#include <DHT.h>

// Define os pinos e versão do DHT
#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// Define o pino do relé
#define RELAYPIN 5

// Inicializa o sensor BMP085
Adafruit_BMP085 bmp;

// SSID e senha do WiFi
const char* ssid = "IFNMG";
const char* password = "@ifnmg2015";

void setup() {
  // Inicializa a comunicação serial
  Serial.begin(115200);

  // Configura o pino do relé como saída e o desliga
  pinMode(RELAYPIN, OUTPUT);
  digitalWrite(RELAYPIN, LOW);

  // Espera pela conexão
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando ao WiFi…");
    Serial.println(WiFi.status());
  }
  Serial.println("Conectado ao WiFi");

  // Verifica se o sensor BMP foi encontrado
  if (!bmp.begin()) {
    Serial.println("Não foi possível encontrar o sensor BMP");
    while (1);
  }

  // Inicializa o DHT
  dht.begin();
}

void loop() {
  delay(2000);

  // Tenta reconectar em caso de perda de conexão
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Desconectado. Tentando reconectar…");
    WiFi.begin(ssid, password);
    
    while (WiFi.status() != WL_CONNECTED) {
      Serial.println("Reconectando…");
    }

    Serial.println("Conectado ao WiFi");
  }

  // Lê temperatura, pressão e umidade
  float temperatura = lerTemperatura();
  int pressao = lerPressao();
  float umidade = lerUmidade();

  // Liga o relé caso a umidade for baixa
  if (umidade < 50.0) {
    digitalWrite(RELAYPIN, HIGH);
    Serial.println("Relé ligado");
  } else {
    digitalWrite(RELAYPIN, LOW);
    Serial.println("Relé desligado");
  }

  // Conecta e envia dados ao site
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin("http://192.168.8.150:3000/api/data"); // URL do servidor
    http.addHeader("Content-Type", "application/json"); // Tipo de conteúdo

    // Cria um objeto JSON e o envia
    String jsonData = "{\"temperatura\":" + String(temperatura) + ", \"pressao\":" + String(pressao) + ", \"umidade\":" + String(umidade) + "}";
    int httpResponseCode = http.POST(jsonData);

    // Verifica se envio foi um sucesso ao conseguir a resposta do servidor
    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println(httpResponseCode);
      Serial.println(response);
      Serial.println("Dados enviados: " + jsonData);
    } else {
      Serial.print("Erro ao enviar POST: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  }
}

/*
    Função para ler a temperatura do sensor BMP085
    Retorna a temperatura em graus Celsius ou -999.0 em caso de erro
*/
float lerTemperatura() {
  float temperatura = bmp.readTemperature();
  return isnan(temperatura) ? -999.0 : temperatura;
}

/*
    Função para ler a pressão atmosférica do sensor BMP085
    Retorna a pressão em hectopascais ou -999.0 em caso de erro
*/
float lerPressao() {
  float pressao = bmp.readPressure() / 100;
  return isnan(pressao) ? -999.0 : pressao;
}

/*
    Função para ler a umidade do sensor BMP085
    Retorna a umidade ou -999.0 em caso de erro
*/
float lerUmidade() {
  float umidade = dht.readHumidity();
  return isnan(umidade) ? -999.0 : umidade;
  }
