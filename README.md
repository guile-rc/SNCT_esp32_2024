# SNCT_esp32_2024

Este projeto utiliza um microcontrolador ESP32 para monitorar a temperatura, umidade e pressão atmosférica, além de controlar um relé com base na umidade medida. Os dados são enviados para um servidor via HTTP.

## Componentes Necessários

- Microcontrolador ESP32
- Sensor de temperatura e umidade DHT22
- Sensor de pressão BMP085
- Módulo relé
- Fios de conexão
- Fonte de alimentação

## Bibliotecas Utilizadas

- ``WiFi.h``: Para conectar o microcontrolador à rede Wi-Fi.
- ``HTTPClient.h``: Para enviar dados via HTTP.
- ``Adafruit_BMP085.h``: Para interagir com o sensor BMP085.
- ``DHT.h``: Para interagir com o sensor DHT22.

## Configuração do Software

1. Clone ou baixe este repositório.
1. Abra o arquivo no Arduino IDE ou em outra IDE compatível.
1. Altere as variáveis ssid e password para as credenciais da sua rede Wi-Fi.
1. Altere a URL no código para o endereço do seu servidor onde os dados serão enviados.

## Funcionamento

- O código se conecta à rede Wi-Fi e inicializa os sensores.
- A cada 2 segundos, ele lê a temperatura, pressão e umidade.
- Se a umidade estiver abaixo de 50%, o relé é ativado.
- Os dados são enviados para o servidor especificado em formato JSON.

Funções Principais

- ``lerTemperatura()``: Lê a temperatura do sensor BMP085 e retorna o valor em graus Celsius.
- ``lerPressao()``: Lê a pressão atmosférica do sensor BMP085 e retorna o valor em hectopascais.
- ``lerUmidade()``: Lê a umidade do sensor DHT22 e retorna o valor em porcentagem.

## Exemplo de Dados Enviados

Os dados enviados para o servidor têm o seguinte formato JSON:

```json
{
  "temperatura": 25.0,
  "pressao": 1013.25,
  "umidade": 45.0
}
```
