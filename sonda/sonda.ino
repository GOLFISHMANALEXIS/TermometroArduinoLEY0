#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Adafruit_SSD1306.h>

#define ONE_WIRE_BUS 2                // Cable de datos del DS18B20 conectado al pin 2
#define OLED_RESET 4                  // Adafruit lo necesita, pero no lo usamos para I2C

DeviceAddress thermometerAddress;     // Tipo de arreglo personalizado para almacenar la dirección del dispositivo de 64 bits

Adafruit_SSD1306 display(OLED_RESET); // Crear una instancia de pantalla
OneWire oneWire(ONE_WIRE_BUS);        // Crear una instancia de oneWire para comunicarse con el sensor de temperatura
DallasTemperature tempSensor(&oneWire);  // Pasar la referencia de oneWire a la biblioteca Dallas Temperature

void setup()   {
  Serial.begin(9600);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // inicializar con la dirección I2C de la pantalla
  display.clearDisplay();                     // borrar el búfer de la pantalla
  display.display();                          // actualizar la pantalla

  Serial.println("Prueba del sensor de temperatura DS18B20");
  Serial.println("Buscando dispositivos...");
  tempSensor.begin();                         // inicializar el sensor de temperatura

  if (!tempSensor.getAddress(thermometerAddress, 0))
    Serial.println("No se pudo encontrar el dispositivo.");
  else {
    Serial.print("Dirección del Dispositivo 0: ");
    printAddress(thermometerAddress);
    Serial.println();
  }

  tempSensor.setResolution(thermometerAddress, 11);      // establecer la resolución de temperatura (9-12)
}

void loop() {
  tempSensor.requestTemperatures();                      // solicitar una muestra de temperatura del sensor en el bus de oneWire
  displayTemp(tempSensor.getTempC(thermometerAddress));  // mostrar la temperatura en la pantalla OLED

  delay(500);                                            // actualizar las lecturas cada medio segundo
}

void displayTemp(float temperatureReading) {             // la temperatura viene como un número decimal con 2 lugares decimales
  // configurar el tamaño del texto en OLED e imprimir los datos de temperatura
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Temp:");

  // mostrar la temperatura en °C
  display.print(temperatureReading, 1);  // redondeado a 1 lugar decimal
  display.print((char)247);              // símbolo de grado
  display.println("C");
  Serial.print(temperatureReading);      // salida de depuración por serie
  Serial.print("°");
  Serial.print("C  ");

  // mostrar la temperatura en °F
  display.print(DallasTemperature::toFahrenheit(temperatureReading), 1); // redondeado a 1 lugar decimal
  display.print((char)247);                                              // símbolo de grado
  display.println("F");
  Serial.print(DallasTemperature::toFahrenheit(temperatureReading));     // salida de depuración por serie
  Serial.print("°");
  Serial.println("F");

  display.display();                    // actualizar la pantalla OLED con todo el nuevo texto
}

// imprimir la dirección del dispositivo desde el arreglo de direcciones
void printAddress(DeviceAddress deviceAddress) {
  for (uint8_t i = 0; i < 8; i++) {
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}
