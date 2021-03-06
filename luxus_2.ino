 /*  
 *   LUXUS
 *  Projet de robot sensible à la Lumière
 *  Auteur : Philk
 *  Licence : CC by NC-SA 
 */

 // Définition des ports
const byte pin1Motor1 = 2;  // Moteur gauche, marche avant
const byte pin2Motor1 = 4;  // Moteur gauche, marche aarrière
const byte pinPMotor1 = 3;  // Moteur droit, marche avant
const byte pin3Motor2 = 7;  // Moteur droit, marche arrière
const byte pin4Motor2 = 8;  // Vitesse du moteur gauche
const byte pinPMotor2 = 6;  // vitesse du moteur droit
const int sensorPin = A5;   // Capteur photorésistance
const int potardPin = A4;   // Potentiomètre
const int t_min = 200;      // duree min de rotation
const int t_max = 1500;     // duree max de rotation
const int alea = 75;        // valeur du facteur aleatoire pour la demarche en mode "j'ai trouve"

// Constantes
const byte speedSearch = 100; // vitesse pour le mode recherche
const byte speedLight = 255;  // vitesse pour le mode lumière trouvée

// Configurations
void setup() {
  Serial.begin(9600);
  pinMode(pin1Motor1, OUTPUT);
  pinMode(pin2Motor1, OUTPUT);
  pinMode(pinPMotor1, OUTPUT);
  pinMode(pin3Motor2, OUTPUT);
  pinMode(pin4Motor2, OUTPUT);
  pinMode(pinPMotor2, OUTPUT);
  for(int i = 2; i <= 13; i++) digitalWrite(i, LOW);  // mise au niveau bas de tous les ports
}

// Boucle principale
void loop() {
  int seuil = illum();    // Lecture du seuil du niveau de lumière
  search(seuil);   // Mode recherche
  light();    // More lumière trouvée
}

// Réglage du niveau de seuil lumineux
int illum() {
  int seuil = analogRead(potardPin);
  return(seuil);
}

// Mode recherche : Luxus cherche une source lumineuse en tournant sur lui-même
void search(int seuil) {
  int timer = random(t_min, t_max);
  unsigned long startTime = millis();
  byte etatMotors = 1;
  bool sens = random(2);
  int lum = analogRead(sensorPin);
  while(lum < seuil) {
    // affichage(seuil, lum, timer, sens);
    motors(etatMotors, sens);
    unsigned long actualTime = millis();
    if(actualTime - startTime > timer) {
      etatMotors++;
      etatMotors = etatMotors % 2;
      sens = random(2);
      startTime = actualTime;
      timer = random(t_min, t_max);
    }
    lum = analogRead(sensorPin);
    seuil = illum();
    affichage(seuil, lum, timer, sens);
  }
}

// Mode : J'ai trouvé la lumière !
void light() {
  byte etatMotors = 2;
  motors(etatMotors, 0);
  Serial.print("Lumière ! "); Serial.println(analogRead(sensorPin));
}

// Machinerie : Fonctionnement des moteurs
void motors(int etat, bool sens) {
  if (etat == 0) { // mode arrêt
    digitalWrite(pin1Motor1, LOW);
    digitalWrite(pin2Motor1, LOW);
    digitalWrite(pin3Motor2, LOW);
    digitalWrite(pin4Motor2, LOW);
    analogWrite(pinPMotor1, 0);
    analogWrite(pinPMotor2, 0);
  }
  else if (etat == 1 && sens == 0) { // mode recherche en sens avant
    digitalWrite(pin1Motor1, HIGH);
    digitalWrite(pin2Motor1, LOW);
    digitalWrite(pin3Motor2, LOW);
    digitalWrite(pin4Motor2, HIGH);
    analogWrite(pinPMotor1, speedSearch);
    analogWrite(pinPMotor2, speedSearch);
  }
    else if (etat == 1 && sens == 1) { // mode recherche en sens arrière
    digitalWrite(pin1Motor1, LOW);
    digitalWrite(pin2Motor1, HIGH);
    digitalWrite(pin3Motor2, HIGH);
    digitalWrite(pin4Motor2, LOW);
    analogWrite(pinPMotor1, speedSearch);
    analogWrite(pinPMotor2, speedSearch);
  }
  else if (etat == 2) { // mode lumière trouvée
    digitalWrite(pin1Motor1, HIGH);
    digitalWrite(pin2Motor1, LOW);
    digitalWrite(pin3Motor2, HIGH);
    digitalWrite(pin4Motor2, LOW);
    analogWrite(pinPMotor1, speedLight-random(alea));
    analogWrite(pinPMotor2, speedLight-random(alea));
  }
}

void affichage(int seuil, int lum, int timer, bool sens) {
  Serial.print("S = ");
  Serial.print(seuil);
  Serial.print(" L = ");
  Serial.print(lum);
  Serial.print(" T = ");
  Serial.print(timer);
  Serial.print(" D = ");
  if(sens == 1) Serial.println("A droite");
  if(sens == 0) Serial.println("A gauche");
}

