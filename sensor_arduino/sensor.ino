#define TRIG 9      
#define ECHO 10     
#define LED 3       

void setup() {
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(LED, OUTPUT);
  Serial.begin(9600);  
}

void loop() {
  long duracao;
  float distancia;

  
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  
  duracao = pulseIn(ECHO, HIGH);
  distancia = duracao * 0.034 / 2;

  Serial.print("Distância: ");
  Serial.print(distancia);
  Serial.println(" cm");

  
  if (distancia < 20.0) {
    digitalWrite(LED, HIGH); 
  } else {
    digitalWrite(LED, LOW);  
  }

  delay(500); 
}
