// TODO: Investigar erro no DHT: imprime NaNs e não há como checar se problema é no hardware ou software por enquanto

#include <WiFi.h>
#include <HTTPClient.h>
#include <Adafruit_BMP085.h>
#include <DHT.h>

#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// Inicializa o sensor BMP085
Adafruit_BMP085 bmp;

// SSID e senha do WiFi
const char* ssid = "IFNMG";
const char* password = "@ifnmg2015";

void setup() {
  Serial.begin(115200);
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
    while (1); // Para o código se não foi encontrado
  }

  dht.begin();
}

void loop() {
  delay(2000);

  // Lê temperatura, pressão e (futuramente) umidade
  float temperatura = lerTemperatura();
  int pressao = lerPressao();
  float umidade = lerUmidade();
  umidade = isnan(umidade) ? -1.0 : umidade;

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin("http://192.168.8.150:3000/api/data");
    http.addHeader("Content-Type", "application/json");

    // Cria um objeto JSON para o envio
    String jsonData = "{\"temperatura\":" + String(temperatura) + ", \"pressao\":" + String(pressao) + ", \"umidade\":" + String(umidade) + "}";
    int httpResponseCode = http.POST(jsonData);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println(httpResponseCode);
      Serial.println(response);
      Serial.println("Dados enviados: " + jsonData);
    } else {
      Serial.print("Error on sending POST: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  }
}

/*
    Função para ler a temperatura do sensor BMP085
*/
float lerTemperatura() {
  return bmp.readTemperature();
}

/*
    Função para ler a pressão atmosférica do sensor BMP085
*/
float lerPressao() {
  return bmp.readPressure() / 100; // converter para hPa
}

/*
    Função para ler a umidade do sensor BMP085
*/
float lerUmidade() {
  return dht.readHumidity();
  }
