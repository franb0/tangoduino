/*
  TAKE ON ME - a-ha
  Adaptación monofónica 8-bit para Arduino Nano

  Generada desde el MIDI proporcionado.

  Selección musical:
    - riff instrumental de la introducción
    - melodía vocal principal desde su entrada
    - voz transpuesta una octava hacia arriba para mayor claridad

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

  Potenciómetro lineal de 10 kΩ:
    extremo 1 -> GND
    terminal central -> A0
    extremo 2 -> 5V

  DATOS
  ------------------------------------------------------------
  Tempo original: 169 BPM
  Eventos melódicos: 286
  Duración aproximada a velocidad normal: 205.1 s
  Registro MIDI usado: 57 a 88

  Se reproduce una vez al encender o presionar RESET.
*/

#include <Arduino.h>
#include <avr/pgmspace.h>

const uint8_t PIN_PIEZO = 10;
const uint8_t LED_GRAVE = 3;
const uint8_t LED_MEDIO = 4;
const uint8_t LED_AGUDO = 5;
const uint8_t PIN_POTENCIOMETRO = A0;

// Rango del control de velocidad.
// Centro: aproximadamente 100 %.
const uint16_t VELOCIDAD_MINIMA = 60;
const uint16_t VELOCIDAD_MAXIMA = 140;

// Rangos de los LEDs, expresados como notas MIDI.
const uint8_t LIMITE_MEDIO = 72;  // DO5
const uint8_t LIMITE_AGUDO = 79;  // SOL5

struct Evento {
  uint8_t notaMidi;
  uint16_t duracionSonido;
  uint16_t duracionPausa;
};

#define E(nota, sonido, pausa) {nota, sonido, pausa}

const Evento TAKE_ON_ME[] PROGMEM = {
  E(78, 78, 92),
  E(78, 107, 74),
  E(74, 107, 71),
  E(71, 96, 259),
  E(71, 111, 244),
  E(76, 96, 259),
  E(76, 107, 248),
  E(76, 115, 70),
  E(80, 85, 89),
  E(80, 96, 85),
  E(81, 100, 74),
  E(83, 111, 74),
  E(81, 85, 85),
  E(81, 100, 81),
  E(81, 89, 89),
  E(76, 96, 259),
  E(74, 85, 270),
  E(78, 96, 259),
  E(78, 100, 259),
  E(78, 96, 85),
  E(76, 100, 78),
  E(76, 100, 78),
  E(78, 85, 89),
  E(76, 104, 81),
  E(78, 85, 89),
  E(78, 96, 85),
  E(74, 96, 78),
  E(71, 81, 278),
  E(71, 100, 255),
  E(76, 107, 248),
  E(76, 118, 237),
  E(76, 107, 78),
  E(80, 89, 85),
  E(80, 89, 92),
  E(81, 85, 89),
  E(83, 96, 89),
  E(81, 70, 100),
  E(81, 89, 92),
  E(81, 67, 107),
  E(76, 78, 281),
  E(74, 67, 288),
  E(78, 85, 270),
  E(78, 92, 267),
  E(78, 96, 85),
  E(76, 85, 89),
  E(76, 111, 70),
  E(78, 92, 82),
  E(76, 104, 85),
  E(78, 74, 92),
  E(78, 115, 70),
  E(74, 104, 70),
  E(71, 96, 259),
  E(71, 111, 244),
  E(76, 100, 259),
  E(76, 111, 244),
  E(76, 111, 70),
  E(80, 89, 89),
  E(80, 92, 86),
  E(81, 104, 70),
  E(83, 115, 74),
  E(81, 81, 89),
  E(81, 96, 85),
  E(81, 85, 89),
  E(78, 96, 263),
  E(74, 85, 270),
  E(78, 96, 259),
  E(78, 96, 259),
  E(78, 96, 89),
  E(76, 100, 74),
  E(76, 100, 81),
  E(76, 85, 266),
  E(74, 429, 104),
  E(74, 309, 50),
  E(73, 143, 38),
  E(71, 1635, 144),
  E(73, 137, 37),
  E(73, 240, 115),
  E(73, 309, 50),
  E(69, 244, 466),
  E(78, 144, 215),
  E(78, 96, 85),
  E(78, 305, 50),
  E(76, 305, 50),
  E(74, 436, 97),
  E(74, 126, 55),
  E(74, 137, 37),
  E(73, 305, 50),
  E(71, 1309, 296),
  E(73, 141, 37),
  E(73, 218, 137),
  E(73, 309, 50),
  E(71, 305, 50),
  E(69, 305, 50),
  E(71, 305, 50),
  E(73, 146, 39),
  E(71, 305, 50),
  E(69, 322, 388),
  E(74, 473, 60),
  E(74, 129, 52),
  E(74, 122, 52),
  E(74, 244, 1720),
  E(69, 111, 63),
  E(69, 107, 74),
  E(69, 104, 70),
  E(69, 100, 81),
  E(69, 100, 74),
  E(69, 100, 81),
  E(69, 141, 37),
  E(68, 305, 50),
  E(66, 440, 455),
  E(57, 1324, 100),
  E(68, 1324, 100),
  E(69, 1324, 100),
  E(76, 444, 85),
  E(78, 384, 63),
  E(76, 388, 63),
  E(69, 1321, 99),
  E(76, 1324, 100),
  E(78, 1328, 100),
  E(76, 444, 92),
  E(78, 378, 62),
  E(76, 384, 63),
  E(73, 1324, 100),
  E(80, 1324, 100),
  E(81, 1654, 125),
  E(83, 143, 38),
  E(85, 309, 50),
  E(83, 137, 37),
  E(81, 270, 92),
  E(88, 5294, 398),
  E(74, 429, 107),
  E(74, 309, 50),
  E(73, 141, 37),
  E(71, 1635, 144),
  E(73, 141, 37),
  E(73, 237, 122),
  E(73, 305, 50),
  E(69, 240, 474),
  E(78, 137, 218),
  E(78, 100, 78),
  E(78, 305, 50),
  E(76, 305, 50),
  E(74, 436, 100),
  E(74, 122, 56),
  E(74, 143, 38),
  E(73, 305, 50),
  E(71, 1305, 296),
  E(73, 141, 37),
  E(73, 214, 141),
  E(73, 309, 50),
  E(71, 305, 50),
  E(69, 305, 50),
  E(71, 309, 50),
  E(73, 141, 37),
  E(71, 305, 50),
  E(69, 322, 388),
  E(74, 481, 59),
  E(74, 126, 48),
  E(74, 122, 56),
  E(74, 240, 1720),
  E(69, 111, 67),
  E(69, 104, 74),
  E(69, 107, 74),
  E(69, 96, 78),
  E(69, 100, 81),
  E(69, 92, 82),
  E(69, 143, 38),
  E(68, 305, 50),
  E(66, 444, 447),
  E(57, 1324, 100),
  E(68, 1324, 100),
  E(69, 1324, 100),
  E(76, 444, 92),
  E(78, 378, 62),
  E(76, 388, 63),
  E(69, 1324, 100),
  E(76, 1321, 99),
  E(78, 1328, 100),
  E(76, 444, 92),
  E(78, 378, 62),
  E(76, 384, 63),
  E(73, 1324, 100),
  E(80, 1324, 100),
  E(81, 1654, 125),
  E(83, 143, 38),
  E(85, 309, 50),
  E(83, 137, 37),
  E(81, 270, 92),
  E(88, 5680, 39886),
  E(74, 305, 50),
  E(74, 152, 40),
  E(74, 309, 50),
  E(73, 141, 37),
  E(71, 1635, 144),
  E(73, 141, 37),
  E(73, 237, 122),
  E(73, 305, 50),
  E(69, 244, 470),
  E(78, 137, 218),
  E(78, 100, 78),
  E(78, 305, 50),
  E(76, 305, 50),
  E(74, 440, 96),
  E(74, 126, 52),
  E(74, 143, 38),
  E(73, 305, 50),
  E(71, 1305, 296),
  E(73, 141, 37),
  E(73, 218, 137),
  E(73, 309, 50),
  E(71, 305, 50),
  E(69, 305, 50),
  E(71, 309, 50),
  E(73, 141, 37),
  E(71, 305, 50),
  E(69, 325, 385),
  E(74, 481, 55),
  E(74, 129, 49),
  E(74, 126, 52),
  E(74, 244, 1716),
  E(69, 111, 67),
  E(69, 104, 74),
  E(69, 107, 74),
  E(69, 96, 78),
  E(69, 100, 81),
  E(69, 92, 86),
  E(69, 141, 37),
  E(68, 305, 50),
  E(66, 444, 447),
  E(57, 1328, 100),
  E(68, 1324, 100),
  E(69, 1324, 100),
  E(76, 444, 89),
  E(78, 382, 62),
  E(76, 388, 63),
  E(69, 1324, 100),
  E(76, 1324, 100),
  E(78, 1324, 100),
  E(76, 440, 89),
  E(78, 388, 63),
  E(76, 384, 63),
  E(73, 1321, 99),
  E(80, 1328, 100),
  E(81, 1654, 125),
  E(83, 137, 37),
  E(85, 309, 50),
  E(83, 143, 38),
  E(81, 266, 93),
  E(88, 1321, 99),
  E(68, 1324, 100),
  E(69, 1324, 100),
  E(76, 4, 529),
  E(78, 4, 536),
  E(76, 305, 50),
  E(69, 1324, 100),
  E(76, 1324, 100),
  E(78, 1328, 100),
  E(76, 436, 93),
  E(78, 384, 63),
  E(76, 384, 63),
  E(73, 1324, 100),
  E(80, 1324, 100),
  E(81, 1654, 125),
  E(83, 137, 37),
  E(85, 309, 50),
  E(83, 143, 38),
  E(81, 270, 89),
  E(88, 1321, 99),
  E(68, 1324, 100),
  E(69, 1328, 100),
  E(76, 440, 89),
  E(78, 384, 63),
  E(76, 384, 63),
  E(69, 1324, 100),
  E(76, 1328, 100),
  E(78, 1324, 100),
  E(76, 440, 89),
  E(78, 359, 88),
  E(76, 388, 63),
  E(73, 1321, 99),
  E(80, 1328, 100),
  E(81, 1654, 125),
  E(83, 137, 37),
  E(85, 305, 50),
  E(83, 143, 38),
  E(81, 232, 38),
};

const uint16_t CANTIDAD_EVENTOS =
  sizeof(TAKE_ON_ME) / sizeof(TAKE_ON_ME[0]);

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
  static const uint16_t FRECUENCIAS[] PROGMEM = {
    220, 233, 247, 262, 277, 294, 311, 330, 349, 370, 392, 415, 440, 466, 494, 523, 554, 587, 622, 659, 698, 740, 784, 831, 880, 932, 988, 1047, 1109, 1175, 1245, 1319
  };

  if (notaMidi < 57 || notaMidi > 88) {
    return 440;
  }

  return pgm_read_word(
    &FRECUENCIAS[notaMidi - 57]
  );
}

uint16_t leerVelocidadPorcentaje() {
  uint32_t suma = 0;

  // Promedio para evitar que pequeñas variaciones eléctricas
  // produzcan cambios bruscos de tempo.
  for (uint8_t i = 0; i < 8; i++) {
    suma += analogRead(PIN_POTENCIOMETRO);
  }

  const uint16_t lectura = suma / 8;

  return map(
    lectura,
    0,
    1023,
    VELOCIDAD_MINIMA,
    VELOCIDAD_MAXIMA
  );
}

uint16_t ajustarDuracion(
  uint16_t duracion,
  uint16_t velocidadPorcentaje
) {
  return (uint32_t)duracion * 100UL
       / velocidadPorcentaje;
}

void reproducirEvento(const Evento &evento) {
  const uint16_t velocidad =
    leerVelocidadPorcentaje();

  const uint16_t sonido =
    ajustarDuracion(
      evento.duracionSonido,
      velocidad
    );

  const uint16_t pausa =
    ajustarDuracion(
      evento.duracionPausa,
      velocidad
    );

  const uint16_t frecuencia =
    frecuenciaDesdeMidi(evento.notaMidi);

  encenderLedSegunNota(evento.notaMidi);
  tone(PIN_PIEZO, frecuencia);

  delay(sonido);

  noTone(PIN_PIEZO);
  apagarLeds();

  if (pausa > 0) {
    delay(pausa);
  }
}

void reproducirTakeOnMe() {
  for (uint16_t i = 0; i < CANTIDAD_EVENTOS; i++) {
    Evento evento;

    memcpy_P(
      &evento,
      &TAKE_ON_ME[i],
      sizeof(Evento)
    );

    reproducirEvento(evento);
  }

  noTone(PIN_PIEZO);
  apagarLeds();
}

void animacionInicial() {
  digitalWrite(LED_GRAVE, HIGH);
  delay(120);

  digitalWrite(LED_MEDIO, HIGH);
  delay(120);

  digitalWrite(LED_AGUDO, HIGH);
  delay(180);

  apagarLeds();
  delay(300);
}

void animacionFinal() {
  for (uint8_t i = 0; i < 4; i++) {
    digitalWrite(LED_GRAVE, HIGH);
    digitalWrite(LED_MEDIO, HIGH);
    digitalWrite(LED_AGUDO, HIGH);

    delay(100);

    apagarLeds();
    delay(100);
  }
}

void setup() {
  pinMode(PIN_PIEZO, OUTPUT);

  pinMode(LED_GRAVE, OUTPUT);
  pinMode(LED_MEDIO, OUTPUT);
  pinMode(LED_AGUDO, OUTPUT);
  pinMode(PIN_POTENCIOMETRO, INPUT);

  analogRead(PIN_POTENCIOMETRO);

  noTone(PIN_PIEZO);
  apagarLeds();

  delay(500);

  animacionInicial();
  reproducirTakeOnMe();
  animacionFinal();
}

void loop() {
  // Presionar RESET para volver a reproducir.
}
