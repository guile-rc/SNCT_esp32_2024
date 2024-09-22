// TODO: Investigar erro no DHT: imprime NaNs e não há como checar se problema é no hardware ou software por enquanto

//#include <Wire.h> // Não usado
//#include <Wifi.h>
#include <Adafruit_BMP085.h>

/*#include <DHT.h>
#define DHTPIN 56
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);*/

// Inicializa o sensor BMP085
Adafruit_BMP085 bmp;

void setup() {
  Serial.begin(115200);

  // Verifica se o sensor BMP foi encontrado
  if (!bmp.begin()) {
    Serial.println("Não foi possível encontrar o sensor BMP");
    while(1); // Para o código se não foi encontrado
  }

  //dht.begin();
}

void loop() {
  delay(2000);

  // Lê temperatura, pressão e (futuramente) umidade
  float temperatura = lerTemperatura();
  int pressao = lerPressao();
  //float umidade = lerUmidade();

  // Verifica se leituras são válidas; se forem, imprime-as 
  if (isnan(temperatura)) {
    Serial.println("Erro ao ler temperatura!");
  } else {
    Serial.print("Temperatura: ");
    Serial.print(temperatura);
    Serial.println(" °C");
  }

  if (isnan(pressao)) {
    Serial.println("Erro ao ler pressão!");
  } else {
    Serial.print("Pressão: ");
    Serial.print(pressao);
    Serial.println(" hPa");
  }

  /*if (isnan(umidade)) {
    Serial.println("Erro ao ler umidade!");
  } else {
    Serial.print("Umidade: ");
    Serial.print(umidade);
    Serial.println(" %");
  }*/

  Serial.println("---");
}

/* 
 *  Função para ler a temperatura do sensor BMP085
 */
float lerTemperatura() {
  return bmp.readTemperature();
}

/* 
 *  Função para ler a pressão atmosférica do sensor BMP085
 */
float lerPressao() { 
  return bmp.readPressure() / 100; // converter para hPa
}

/* 
 *  Função para ler a umidade do sensor BMP085
 */
/*float lerUmidade() {
  return dht.readHumidity();
}*/
