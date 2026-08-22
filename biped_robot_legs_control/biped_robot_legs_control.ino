/*
    EquipoDinamita 15/08/2026
    El conceso de la definiciones de los pines será:
    Los servos de la derecha tendrá los canales pares
    Los servos de la izquierda tendrá los canales impares

    Respetando que los pines desde el numero mas chico hasta el mas grande
    Asi representando que va desde el pie hasta la cadera (de abajo hacia arriba)

    Mapeo de los canales
  -------------------------------------|
    Pie derecho:        [0]| Columna 4 |
    Rodilla derecha:    [2]| Columna 2 |
    Cadera derecha:     [4]| Columna 5 |
  -------------------------|-----------|
    Pie izquierda:      [1]| Columna 3 |
    Rodilla izquierda:  [3]| Columna 1 |
    Cadera izquierda:   [5]| Columna 6 |
  -------------------------------------|
*/

#include <Adafruit_PWMServoDriver.h>
#include <Wire.h>

#define SERVOMIN 150
#define SERVOMAX 450
#define SERVO_FREQ 50

const int servoPieDer = 0;
const int servoPieIzq = 1;
const int servoRodillaDer = 2;
const int servoRodillaIzq = 3;
const int servoCaderaDer = 4;
const int servoCaderaIzq = 5;

Adafruit_PWMServoDriver servos = Adafruit_PWMServoDriver();

// --- Máquina de estados de la secuencia ---
enum EstadoSecuencia {
  PIE_IDA, PIE_PAUSA1, PIE_VUELTA, PIE_PAUSA2,
  RODILLA_IDA, RODILLA_PAUSA1, RODILLA_VUELTA, RODILLA_PAUSA2,
  CADERA_IDA, CADERA_PAUSA1, CADERA_VUELTA, CADERA_PAUSA2
};

EstadoSecuencia estadoActual = PIE_IDA;

// --- Variables del movimiento en curso ---
int posActual;       // posición actual (grados) del servo "izquierdo" de referencia
int posObjetivo;      // hacia dónde va
int canalIzqActual, canalDerActual;
unsigned long ultimoPaso = 0;
unsigned long inicioPausa = 0;
const int velocidadDelay = 50;   // ms entre cada grado (reemplaza al delay(velocidadDelay))
const int duracionPausa = 1000;  // ms de pausa (reemplaza al delay(1000))

void setup()
{
  servos.begin();
  servos.setPWMFreq(SERVO_FREQ);

  for (int i = 0; i < 6; i++)
  {
      servos.setPWM(i, 0, gradosAPulsos(90));
  }

  iniciarMovimiento(servoPieIzq, servoPieDer, 90, 175);
}

void loop()
{
  actualizarSecuencia();
}

int gradosAPulsos(int grados)
{
  return map(grados, 0, 180, SERVOMIN, SERVOMAX);
}

// Prepara un nuevo movimiento (equivalente a llamar moverPiernasEspejo, pero sin bloquear)
void iniciarMovimiento(int canalIzq, int canalDer, int inicio, int fin)
{
  canalIzqActual = canalIzq;
  canalDerActual = canalDer;
  posActual = inicio;
  posObjetivo = fin;
  ultimoPaso = millis();
}

// Devuelve true cuando el movimiento actual terminó
bool actualizarMovimiento()
{
  if (millis() - ultimoPaso < velocidadDelay) return false; // todavía no toca avanzar

  ultimoPaso = millis();

  // Aplicar la posición actual a ambos servos (lógica de espejo)
  servos.setPWM(canalIzqActual, 0, gradosAPulsos(posActual));
  servos.setPWM(canalDerActual, 0, gradosAPulsos(180 - posActual));

  if (posActual == posObjetivo) return true; // llegó

  posActual += (posObjetivo > posActual) ? 1 : -1;
  return false;
}

void actualizarSecuencia()
{
  switch (estadoActual)
  {
    case PIE_IDA:
      if (actualizarMovimiento()) {
        inicioPausa = millis();
        estadoActual = PIE_PAUSA1;
      }
      break;

    case PIE_PAUSA1:
      if (millis() - inicioPausa >= duracionPausa) {
        iniciarMovimiento(servoPieIzq, servoPieDer, 175, 90);
        estadoActual = PIE_VUELTA;
      }
      break;

    case PIE_VUELTA:
      if (actualizarMovimiento()) {
        inicioPausa = millis();
        estadoActual = PIE_PAUSA2;
      }
      break;

    case PIE_PAUSA2:
      if (millis() - inicioPausa >= duracionPausa) {
        iniciarMovimiento(servoRodillaIzq, servoRodillaDer, 90, 5);
        estadoActual = RODILLA_IDA;
      }
      break;

    case RODILLA_IDA:
      if (actualizarMovimiento()) {
        inicioPausa = millis();
        estadoActual = RODILLA_PAUSA1;
      }
      break;

    case RODILLA_PAUSA1:
      if (millis() - inicioPausa >= duracionPausa) {
        iniciarMovimiento(servoRodillaIzq, servoRodillaDer, 5, 90);
        estadoActual = RODILLA_VUELTA;
      }
      break;

    case RODILLA_VUELTA:
      if (actualizarMovimiento()) {
        inicioPausa = millis();
        estadoActual = RODILLA_PAUSA2;
      }
      break;

    case RODILLA_PAUSA2:
      if (millis() - inicioPausa >= duracionPausa) {
        iniciarMovimiento(servoCaderaIzq, servoCaderaDer, 90, 175);
        estadoActual = CADERA_IDA;
      }
      break;

    case CADERA_IDA:
      if (actualizarMovimiento()) {
        inicioPausa = millis();
        estadoActual = CADERA_PAUSA1;
      }
      break;

    case CADERA_PAUSA1:
      if (millis() - inicioPausa >= duracionPausa) {
        iniciarMovimiento(servoCaderaIzq, servoCaderaDer, 175, 90);
        estadoActual = CADERA_VUELTA;
      }
      break;

    case CADERA_VUELTA:
      if (actualizarMovimiento()) {
        inicioPausa = millis();
        estadoActual = CADERA_PAUSA2;
      }
      break;

    case CADERA_PAUSA2:
      if (millis() - inicioPausa >= duracionPausa) {
        // Reinicia el ciclo completo
        iniciarMovimiento(servoPieIzq, servoPieDer, 90, 175);
        estadoActual = PIE_IDA;
      }
      break;
  }
}
