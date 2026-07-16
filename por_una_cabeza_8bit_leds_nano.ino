/*
  POR UNA CABEZA - Carlos Gardel
  Adaptación monofónica 8-bit para Arduino Nano
  Generada a partir del MIDI proporcionado.

  El MIDI original contiene melodía armonizada y acompañamiento.
  Esta versión conserva la nota superior de cada acorde como
  línea melódica principal.

  CONEXIONES
  ------------------------------------------------------------
  Piezo pasivo:
    D10 -> resistencia de 100 a 220 ohm -> positivo
    GND -> negativo

  LED grave:
    D3 -> resistencia de 220 a 330 ohm -> ánodo
    cátodo -> GND

  LED medio:
    D4 -> resistencia de 220 a 330 ohm -> ánodo
    cátodo -> GND

  LED agudo:
    D5 -> resistencia de 220 a 330 ohm -> ánodo
    cátodo -> GND

  RANGOS VISUALES
  ------------------------------------------------------------
  Grave: menor que LA#4
  Medio: LA#4 hasta FA#5
  Agudo: SOL5 en adelante

  DURACIÓN APROXIMADA
  ------------------------------------------------------------
  140.0 segundos.

  Se reproduce una vez al encender o presionar RESET.
*/

#include <Arduino.h>
#include <avr/pgmspace.h>

const uint8_t PIN_PIEZO = 10;
const uint8_t LED_GRAVE = 3;
const uint8_t LED_MEDIO = 4;
const uint8_t LED_AGUDO = 5;

// Permite ajustar la velocidad global.
// 100 = velocidad original.
// 110 = 10 % más rápido.
// 90  = 10 % más lento.
const uint16_t VELOCIDAD_PORCENTAJE = 100;

// Límites MIDI elegidos para repartir visualmente los tres LEDs.
const uint8_t LIMITE_MEDIO = 70;  // LA#4
const uint8_t LIMITE_AGUDO = 78;  // FA#5 / SOL5

struct Evento {
  uint8_t notaMidi;
  uint16_t duracionSonido;
  uint16_t duracionPausa;
};

#define E(nota, sonido, pausa) {nota, sonido, pausa}

const Evento POR_UNA_CABEZA[] PROGMEM = {
  E(59, 280, 53),
  E(60, 280, 53),
  E(61, 130, 37),
  E(62, 130, 37),
  E(64, 280, 53),
  E(62, 333, 334),
  E(61, 280, 53),
  E(62, 280, 53),
  E(64, 130, 37),
  E(66, 130, 37),
  E(69, 280, 53),
  E(67, 333, 334),
  E(71, 280, 53),
  E(72, 280, 53),
  E(69, 130, 37),
  E(71, 130, 37),
  E(67, 280, 53),
  E(69, 280, 53),
  E(66, 130, 37),
  E(67, 130, 37),
  E(66, 333, 417),
  E(62, 210, 40),
  E(60, 333, 667),
  E(72, 280, 53),
  E(71, 280, 53),
  E(74, 130, 37),
  E(72, 130, 37),
  E(69, 333, 667),
  E(69, 280, 53),
  E(68, 280, 53),
  E(71, 130, 37),
  E(69, 130, 37),
  E(66, 333, 667),
  E(66, 280, 53),
  E(67, 280, 53),
  E(68, 130, 37),
  E(69, 130, 37),
  E(71, 280, 53),
  E(69, 280, 53),
  E(66, 130, 37),
  E(62, 130, 37),
  E(63, 130, 37),
  E(66, 130, 37),
  E(64, 333, 84),
  E(60, 210, 40),
  E(59, 250, 750),
  E(59, 280, 53),
  E(60, 280, 53),
  E(61, 130, 37),
  E(62, 130, 37),
  E(64, 280, 53),
  E(62, 333, 334),
  E(61, 280, 53),
  E(62, 280, 53),
  E(64, 130, 37),
  E(66, 130, 37),
  E(69, 280, 53),
  E(67, 333, 334),
  E(67, 280, 53),
  E(69, 280, 53),
  E(71, 130, 37),
  E(67, 130, 37),
  E(69, 280, 53),
  E(67, 280, 53),
  E(69, 130, 37),
  E(71, 130, 37),
  E(69, 333, 417),
  E(67, 210, 40),
  E(76, 250, 750),
  E(81, 280, 53),
  E(79, 280, 53),
  E(76, 130, 37),
  E(72, 130, 37),
  E(71, 280, 53),
  E(69, 280, 53),
  E(71, 130, 37),
  E(72, 130, 37),
  E(75, 280, 53),
  E(79, 280, 53),
  E(77, 130, 37),
  E(75, 130, 37),
  E(79, 280, 53),
  E(74, 333, 334),
  E(79, 280, 53),
  E(81, 280, 53),
  E(83, 130, 37),
  E(79, 130, 37),
  E(81, 280, 53),
  E(76, 130, 37),
  E(78, 280, 53),
  E(79, 280, 53),
  E(81, 130, 37),
  E(78, 130, 37),
  E(79, 500, 333),
  E(62, 130, 37),
  E(67, 450, 50),
  E(62, 500, 500),
  E(70, 333, 167),
  E(70, 280, 53),
  E(72, 280, 53),
  E(74, 280, 53),
  E(74, 333, 667),
  E(69, 333, 667),
  E(63, 333, 167),
  E(67, 333, 167),
  E(67, 280, 53),
  E(69, 280, 53),
  E(70, 280, 53),
  E(70, 333, 667),
  E(65, 333, 1167),
  E(63, 333, 167),
  E(63, 280, 53),
  E(65, 280, 53),
  E(67, 280, 53),
  E(67, 333, 667),
  E(67, 280, 53),
  E(69, 280, 53),
  E(70, 130, 37),
  E(67, 130, 37),
  E(69, 333, 667),
  E(67, 280, 53),
  E(69, 280, 53),
  E(70, 130, 37),
  E(67, 130, 37),
  E(70, 333, 667),
  E(69, 333, 1167),
  E(82, 333, 167),
  E(82, 280, 53),
  E(84, 280, 53),
  E(86, 280, 53),
  E(86, 333, 667),
  E(81, 333, 1167),
  E(79, 333, 167),
  E(79, 280, 53),
  E(81, 280, 53),
  E(82, 280, 53),
  E(82, 333, 667),
  E(77, 333, 1167),
  E(75, 333, 167),
  E(75, 280, 53),
  E(77, 280, 53),
  E(79, 280, 53),
  E(79, 333, 667),
  E(79, 280, 53),
  E(81, 280, 53),
  E(82, 130, 37),
  E(79, 130, 37),
  E(81, 333, 500),
  E(76, 130, 37),
  E(78, 280, 53),
  E(79, 280, 53),
  E(81, 130, 37),
  E(78, 130, 37),
  E(79, 333, 1167),
  E(71, 280, 53),
  E(72, 280, 53),
  E(73, 130, 37),
  E(74, 130, 37),
  E(76, 280, 53),
  E(74, 333, 334),
  E(73, 280, 53),
  E(74, 280, 53),
  E(76, 130, 37),
  E(78, 130, 37),
  E(81, 280, 53),
  E(79, 333, 334),
  E(83, 280, 53),
  E(84, 280, 53),
  E(81, 130, 37),
  E(83, 130, 37),
  E(79, 280, 53),
  E(81, 280, 53),
  E(78, 130, 37),
  E(79, 130, 37),
  E(78, 333, 417),
  E(74, 210, 40),
  E(72, 333, 667),
  E(84, 280, 53),
  E(83, 280, 53),
  E(86, 130, 37),
  E(84, 130, 37),
  E(81, 333, 667),
  E(81, 280, 53),
  E(80, 280, 53),
  E(83, 130, 37),
  E(81, 130, 37),
  E(78, 333, 667),
  E(78, 280, 53),
  E(79, 280, 53),
  E(80, 130, 37),
  E(81, 130, 37),
  E(83, 280, 53),
  E(81, 280, 53),
  E(78, 130, 37),
  E(74, 130, 37),
  E(75, 130, 37),
  E(78, 130, 37),
  E(76, 333, 84),
  E(72, 210, 40),
  E(71, 250, 750),
  E(71, 280, 53),
  E(72, 280, 53),
  E(73, 130, 37),
  E(74, 130, 37),
  E(76, 280, 53),
  E(74, 333, 334),
  E(73, 280, 53),
  E(74, 280, 53),
  E(76, 130, 37),
  E(78, 130, 37),
  E(81, 280, 53),
  E(79, 333, 334),
  E(79, 280, 53),
  E(81, 280, 53),
  E(83, 130, 37),
  E(79, 130, 37),
  E(81, 280, 53),
  E(79, 280, 53),
  E(81, 130, 37),
  E(83, 130, 37),
  E(81, 333, 417),
  E(79, 210, 40),
  E(88, 250, 750),
  E(81, 280, 53),
  E(79, 280, 53),
  E(76, 130, 37),
  E(72, 130, 37),
  E(71, 280, 53),
  E(69, 280, 53),
  E(71, 130, 37),
  E(72, 130, 37),
  E(75, 280, 53),
  E(79, 280, 53),
  E(77, 130, 37),
  E(75, 130, 37),
  E(79, 280, 53),
  E(74, 333, 334),
  E(79, 280, 53),
  E(81, 280, 53),
  E(83, 130, 37),
  E(79, 130, 37),
  E(81, 280, 53),
  E(76, 130, 37),
  E(78, 280, 53),
  E(79, 280, 53),
  E(81, 130, 37),
  E(78, 130, 37),
  E(79, 500, 333),
  E(62, 130, 37),
  E(67, 450, 50),
  E(62, 500, 500),
  E(70, 333, 167),
  E(70, 280, 53),
  E(72, 280, 53),
  E(74, 280, 53),
  E(74, 333, 667),
  E(69, 333, 667),
  E(63, 333, 167),
  E(67, 333, 167),
  E(67, 280, 53),
  E(69, 280, 53),
  E(70, 280, 53),
  E(70, 333, 667),
  E(65, 333, 1167),
  E(63, 333, 167),
  E(63, 280, 53),
  E(65, 280, 53),
  E(67, 280, 53),
  E(67, 333, 667),
  E(67, 280, 53),
  E(69, 280, 53),
  E(70, 130, 37),
  E(67, 130, 37),
  E(69, 333, 667),
  E(67, 280, 53),
  E(69, 280, 53),
  E(70, 130, 37),
  E(67, 130, 37),
  E(70, 333, 667),
  E(69, 333, 1167),
  E(82, 333, 167),
  E(82, 280, 53),
  E(84, 280, 53),
  E(86, 280, 53),
  E(86, 333, 667),
  E(81, 333, 1167),
  E(79, 333, 167),
  E(79, 280, 53),
  E(81, 280, 53),
  E(82, 280, 53),
  E(82, 333, 667),
  E(77, 333, 1167),
  E(75, 333, 167),
  E(75, 280, 53),
  E(77, 280, 53),
  E(79, 280, 53),
  E(79, 333, 667),
  E(79, 280, 53),
  E(81, 280, 53),
  E(82, 130, 37),
  E(79, 130, 37),
  E(81, 333, 500),
  E(76, 130, 37),
  E(78, 280, 53),
  E(79, 280, 53),
  E(81, 130, 37),
  E(78, 667, 1000),
  E(58, 130, 37),
  E(60, 130, 37),
  E(61, 130, 37),
  E(62, 130, 37),
  E(63, 130, 37),
  E(62, 130, 37),
  E(61, 130, 37),
  E(62, 130, 37),
  E(69, 130, 37),
  E(67, 130, 37),
  E(66, 130, 37),
  E(67, 130, 37),
  E(72, 130, 37),
  E(70, 130, 37),
  E(69, 130, 37),
  E(70, 130, 37),
  E(75, 130, 37),
  E(74, 130, 37),
  E(73, 130, 37),
  E(74, 280, 53),
  E(79, 333, 667),
  E(86, 450, 50),
  E(91, 6650, 350),
};

const uint16_t CANTIDAD_EVENTOS =
  sizeof(POR_UNA_CABEZA) / sizeof(POR_UNA_CABEZA[0]);

void apagarLeds() {
  digitalWrite(LED_GRAVE, LOW);
  digitalWrite(LED_MEDIO, LOW);
  digitalWrite(LED_AGUDO, LOW);
}

void encenderLedSegunNota(uint8_t notaMidi) {
  apagarLeds();

  if (notaMidi < LIMITE_MEDIO) {
    digitalWrite(LED_GRAVE, HIGH);
  }
  else if (notaMidi < LIMITE_AGUDO) {
    digitalWrite(LED_MEDIO, HIGH);
  }
  else {
    digitalWrite(LED_AGUDO, HIGH);
  }
}

uint16_t frecuenciaDesdeMidi(uint8_t notaMidi) {
  /*
    Tabla de frecuencias para las notas MIDI 58 a 91,
    rango utilizado por esta melodía.
  */
  static const uint16_t FRECUENCIAS[] PROGMEM = {
     233,  247,  262,  277,  294,  311,  330,  349,
     370,  392,  415,  440,  466,  494,  523,  554,
     587,  622,  659,  698,  740,  784,  831,  880,
     932,  988, 1047, 1109, 1175, 1245, 1319, 1397,
    1480, 1568
  };

  if (notaMidi < 58 || notaMidi > 91) {
    return 440;
  }

  return pgm_read_word(&FRECUENCIAS[notaMidi - 58]);
}

uint16_t ajustarDuracion(uint16_t duracion) {
  return (uint32_t)duracion * 100UL / VELOCIDAD_PORCENTAJE;
}

void reproducirEvento(const Evento &evento) {
  const uint16_t frecuencia =
    frecuenciaDesdeMidi(evento.notaMidi);

  const uint16_t sonido =
    ajustarDuracion(evento.duracionSonido);

  const uint16_t pausa =
    ajustarDuracion(evento.duracionPausa);

  encenderLedSegunNota(evento.notaMidi);
  tone(PIN_PIEZO, frecuencia);

  delay(sonido);

  noTone(PIN_PIEZO);
  apagarLeds();

  if (pausa > 0) {
    delay(pausa);
  }
}

void reproducirPorUnaCabeza() {
  for (uint16_t i = 0; i < CANTIDAD_EVENTOS; i++) {
    Evento evento;

    memcpy_P(
      &evento,
      &POR_UNA_CABEZA[i],
      sizeof(Evento)
    );

    reproducirEvento(evento);
  }

  noTone(PIN_PIEZO);
  apagarLeds();
}

void animacionInicial() {
  digitalWrite(LED_GRAVE, HIGH);
  delay(160);

  digitalWrite(LED_MEDIO, HIGH);
  delay(160);

  digitalWrite(LED_AGUDO, HIGH);
  delay(220);

  apagarLeds();
  delay(400);
}

void animacionFinal() {
  for (uint8_t i = 0; i < 3; i++) {
    digitalWrite(LED_GRAVE, HIGH);
    digitalWrite(LED_MEDIO, HIGH);
    digitalWrite(LED_AGUDO, HIGH);

    delay(160);

    apagarLeds();
    delay(160);
  }
}

void setup() {
  pinMode(PIN_PIEZO, OUTPUT);

  pinMode(LED_GRAVE, OUTPUT);
  pinMode(LED_MEDIO, OUTPUT);
  pinMode(LED_AGUDO, OUTPUT);

  noTone(PIN_PIEZO);
  apagarLeds();

  delay(500);

  animacionInicial();
  reproducirPorUnaCabeza();
  animacionFinal();
}

void loop() {
  // Presionar RESET para volver a reproducir.
}
