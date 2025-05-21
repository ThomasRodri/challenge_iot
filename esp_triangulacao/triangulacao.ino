#include <WiFi.h>

struct AccessPoint {
  const char* ssid;
  float x, y;
  int rssi;
  float distance;
};

AccessPoint aps[3] = {
  {"AP1", 0.0, 0.0, 0, 0},
  {"AP2", 5.0, 0.0, 0, 0},
  {"AP3", 2.5, 4.33, 0, 0}
};

const int A = -40;
const float n = 2.5;

float calculateDistance(int rssi) {
  return pow(10.0, ((A - rssi) / (10.0 * n)));
}

void trilaterate(float& x, float& y) {
  float x1 = aps[0].x, y1 = aps[0].y, r1 = aps[0].distance;
  float x2 = aps[1].x, y2 = aps[1].y, r2 = aps[1].distance;
  float x3 = aps[2].x, y3 = aps[2].y, r3 = aps[2].distance;

  float A = 2 * (x2 - x1);
  float B = 2 * (y2 - y1);
  float C = r1 * r1 - r2 * r2 - x1 * x1 + x2 * x2 - y1 * y1 + y2 * y2;
  float D = 2 * (x3 - x2);
  float E = 2 * (y3 - y2);
  float F = r2 * r2 - r3 * r3 - x2 * x2 + x3 * x3 - y2 * y2 + y3 * y3;

  x = (C * E - F * B) / (A * E - D * B);
  y = (A * F - D * C) / (A * E - D * B);
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  delay(100);
}

void loop() {
  Serial.println("\n🔍 Escaneando redes Wi-Fi...");
  int networks = WiFi.scanNetworks();

  if (networks == 0) {
    Serial.println("❌ Nenhuma rede encontrada.");
    return;
  }

  for (int i = 0; i < 3; i++) {
    aps[i].rssi = -1000;
    
    for (int j = 0; j < networks; j++) {
      if (WiFi.SSID(j) == aps[i].ssid) {
        aps[i].rssi = WiFi.RSSI(j);
        aps[i].distance = calculateDistance(aps[i].rssi);
        break;
      }
    }

    if (aps[i].rssi == -1000) {
      Serial.printf("⚠️ AP \"%s\" não encontrado!\n", aps[i].ssid);
      return;
    }

    Serial.printf("📶 AP \"%s\": RSSI = %d dBm → Distância ≈ %.2f m\n",
                  aps[i].ssid, aps[i].rssi, aps[i].distance);
  }

  float x, y;
  trilaterate(x, y);
  Serial.printf("📍 Posição estimada do ESP32: (x = %.2f, y = %.2f)\n", x, y);

  delay(10000);
}
