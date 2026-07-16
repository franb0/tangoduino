/*
  Y NOS DIERON LAS DIEZ - Joaquin Sabina
  Velocidad regulable con potenciómetro lineal de 10 kΩ
  Adaptación monofónica 8-bit para Arduino Nano
  Generada desde el MIDI proporcionado.

  Se utiliza exclusivamente la pista "Melodía".
  Se descartan piano, bajo, batería, vibráfono, acordeón
  y violines porque tone() solo reproduce una nota simultánea.

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
    terminal central (cursor) -> A0
    extremo 2 -> 5V

  Girar el cursor hacia 5V aumenta la velocidad.

  RANGOS VISUALES RELATIVOS A ESTA CANCIÓN
  ------------------------------------------------------------
  Grave: DO5 hasta FA#5
  Medio: SOL5 hasta LA5
  Agudo: LA#5 en adelante

  DATOS DEL MIDI
  ------------------------------------------------------------
  Tempo: 156 BPM
  Eventos melódicos: 573
  Duración de la melodía: 278.3 segundos

  La introducción instrumental del MIDI no se reproduce.
  La melodía comienza directamente al terminar la animación inicial.

  Se reproduce una vez al encender o presionar RESET.
*/

#include <Arduino.h>
#include <avr/pgmspace.h>

const uint8_t PIN_PIEZO = 10;
const uint8_t LED_GRAVE = 3;
const uint8_t LED_MEDIO = 4;
const uint8_t LED_AGUDO = 5;

const uint8_t PIN_POTENCIOMETRO = A0;

// Rango permitido por el potenciómetro.
// En la posición central se obtiene aproximadamente 100 %.
const uint16_t VELOCIDAD_MINIMA = 60;
const uint16_t VELOCIDAD_MAXIMA = 140;

// Límites expresados como números de nota MIDI.
const uint8_t LIMITE_MEDIO = 79;  // SOL5
const uint8_t LIMITE_AGUDO = 82;  // LA#5

struct Evento {
  uint8_t notaMidi;
  uint16_t duracionSonido;
  uint16_t duracionPausa;
};

#define E(nota, sonido, pausa) {nota, sonido, pausa}

const Evento CANCION[] PROGMEM = {
  E(79, 80, 106),
  E(79, 80, 122),
  E(79, 317, 61),
  E(77, 161, 35),
  E(76, 109, 80),
  E(79, 240, 850),
  E(79, 87, 109),
  E(79, 90, 166),
  E(79, 199, 67),
  E(77, 292, 788),
  E(77, 122, 199),
  E(81, 147, 109),
  E(79, 282, 160),
  E(79, 192, 129),
  E(77, 256, 138),
  E(76, 651, 2811),
  E(79, 87, 115),
  E(79, 103, 73),
  E(79, 285, 39),
  E(77, 186, 74),
  E(76, 141, 51),
  E(79, 208, 939),
  E(76, 122, 67),
  E(77, 154, 48),
  E(79, 99, 84),
  E(79, 119, 73),
  E(79, 109, 90),
  E(81, 119, 73),
  E(81, 109, 80),
  E(81, 122, 70),
  E(79, 109, 154),
  E(79, 87, 102),
  E(81, 112, 74),
  E(81, 131, 129),
  E(81, 128, 132),
  E(81, 231, 157),
  E(77, 433, 3282),
  E(83, 112, 144),
  E(83, 112, 144),
  E(83, 317, 68),
  E(81, 147, 42),
  E(79, 115, 81),
  E(83, 173, 856),
  E(79, 153, 33),
  E(81, 141, 51),
  E(83, 157, 35),
  E(83, 64, 840),
  E(79, 96, 87),
  E(81, 161, 35),
  E(83, 192, 68),
  E(81, 253, 71),
  E(79, 155, 34),
  E(83, 189, 61),
  E(83, 288, 39),
  E(79, 330, 3516),
  E(83, 112, 80),
  E(83, 96, 93),
  E(83, 301, 90),
  E(81, 314, 64),
  E(79, 186, 67),
  E(77, 253, 648),
  E(74, 122, 77),
  E(76, 115, 74),
  E(77, 96, 96),
  E(77, 83, 106),
  E(77, 96, 96),
  E(79, 112, 84),
  E(79, 125, 67),
  E(79, 122, 64),
  E(77, 131, 65),
  E(77, 128, 68),
  E(77, 125, 131),
  E(81, 253, 68),
  E(81, 112, 141),
  E(79, 256, 71),
  E(81, 196, 57),
  E(79, 510, 3061),
  E(82, 74, 118),
  E(82, 77, 115),
  E(82, 237, 128),
  E(81, 112, 71),
  E(79, 112, 112),
  E(82, 279, 695),
  E(81, 189, 138),
  E(79, 115, 138),
  E(82, 189, 67),
  E(82, 160, 920),
  E(82, 179, 90),
  E(84, 167, 134),
  E(84, 128, 157),
  E(84, 167, 86),
  E(82, 194, 43),
  E(81, 423, 3308),
  E(86, 157, 164),
  E(86, 109, 141),
  E(86, 253, 61),
  E(83, 154, 51),
  E(81, 122, 54),
  E(78, 324, 830),
  E(81, 128, 77),
  E(83, 112, 77),
  E(84, 103, 93),
  E(84, 83, 109),
  E(84, 99, 90),
  E(86, 109, 77),
  E(86, 135, 67),
  E(86, 125, 54),
  E(84, 135, 137),
  E(84, 85, 27),
  E(84, 138, 131),
  E(86, 189, 132),
  E(86, 112, 144),
  E(86, 186, 70),
  E(86, 199, 57),
  E(84, 153, 33),
  E(83, 452, 2885),
  E(79, 122, 131),
  E(79, 196, 67),
  E(79, 321, 64),
  E(77, 157, 35),
  E(76, 112, 80),
  E(79, 231, 859),
  E(79, 80, 109),
  E(79, 96, 157),
  E(79, 202, 64),
  E(77, 298, 792),
  E(77, 119, 198),
  E(81, 147, 106),
  E(79, 285, 39),
  E(79, 192, 129),
  E(77, 256, 122),
  E(76, 657, 2939),
  E(79, 96, 112),
  E(79, 103, 67),
  E(79, 285, 39),
  E(77, 157, 35),
  E(76, 151, 80),
  E(79, 202, 977),
  E(76, 119, 67),
  E(77, 154, 51),
  E(79, 103, 86),
  E(79, 122, 67),
  E(79, 99, 97),
  E(81, 115, 77),
  E(81, 96, 93),
  E(81, 119, 73),
  E(79, 99, 97),
  E(79, 189, 119),
  E(79, 221, 48),
  E(81, 151, 105),
  E(81, 138, 115),
  E(81, 192, 64),
  E(81, 256, 129),
  E(77, 439, 3029),
  E(83, 109, 80),
  E(83, 106, 83),
  E(83, 321, 70),
  E(81, 151, 38),
  E(79, 115, 77),
  E(83, 173, 596),
  E(79, 144, 39),
  E(81, 112, 90),
  E(83, 207, 46),
  E(83, 205, 949),
  E(79, 115, 145),
  E(83, 192, 68),
  E(81, 247, 131),
  E(79, 189, 67),
  E(83, 189, 71),
  E(83, 213, 47),
  E(79, 436, 3410),
  E(83, 109, 83),
  E(83, 99, 90),
  E(83, 301, 80),
  E(81, 157, 35),
  E(79, 161, 35),
  E(77, 253, 901),
  E(74, 112, 77),
  E(76, 115, 74),
  E(77, 103, 96),
  E(77, 80, 112),
  E(77, 103, 93),
  E(79, 109, 80),
  E(79, 125, 67),
  E(79, 128, 61),
  E(77, 128, 68),
  E(77, 125, 64),
  E(77, 213, 47),
  E(81, 256, 68),
  E(81, 112, 67),
  E(79, 253, 77),
  E(81, 253, 68),
  E(79, 513, 3058),
  E(82, 77, 115),
  E(82, 80, 119),
  E(82, 231, 138),
  E(81, 112, 61),
  E(79, 128, 96),
  E(82, 279, 686),
  E(81, 202, 128),
  E(79, 122, 141),
  E(82, 186, 67),
  E(82, 160, 917),
  E(82, 260, 70),
  E(86, 163, 90),
  E(84, 213, 47),
  E(84, 192, 68),
  E(82, 317, 64),
  E(81, 417, 3173),
  E(86, 154, 170),
  E(86, 109, 147),
  E(86, 253, 61),
  E(83, 151, 45),
  E(81, 128, 64),
  E(78, 311, 846),
  E(81, 115, 74),
  E(83, 119, 70),
  E(84, 103, 93),
  E(84, 87, 96),
  E(84, 109, 96),
  E(86, 109, 80),
  E(86, 131, 68),
  E(84, 125, 64),
  E(84, 128, 64),
  E(86, 125, 61),
  E(86, 128, 61),
  E(86, 155, 34),
  E(86, 205, 61),
  E(86, 189, 132),
  E(86, 253, 68),
  E(83, 449, 2878),
  E(84, 253, 148),
  E(84, 196, 173),
  E(84, 550, 75),
  E(83, 117, 37),
  E(81, 295, 99),
  E(84, 497, 70),
  E(83, 125, 67),
  E(81, 298, 83),
  E(84, 321, 70),
  E(84, 186, 785),
  E(84, 176, 45),
  E(86, 160, 68),
  E(86, 131, 93),
  E(86, 160, 119),
  E(88, 269, 112),
  E(84, 179, 170),
  E(81, 208, 199),
  E(79, 505, 69),
  E(77, 157, 35),
  E(76, 205, 164),
  E(79, 904, 1038),
  E(74, 141, 61),
  E(76, 112, 71),
  E(77, 160, 48),
  E(76, 106, 80),
  E(77, 163, 36),
  E(76, 103, 73),
  E(77, 168, 37),
  E(76, 112, 80),
  E(77, 157, 35),
  E(76, 99, 93),
  E(77, 109, 147),
  E(76, 189, 64),
  E(77, 192, 58),
  E(76, 192, 68),
  E(74, 196, 70),
  E(72, 458, 16975),
  E(79, 125, 122),
  E(79, 199, 61),
  E(79, 324, 64),
  E(77, 163, 36),
  E(76, 109, 80),
  E(79, 240, 914),
  E(79, 80, 109),
  E(79, 90, 112),
  E(79, 311, 833),
  E(77, 122, 74),
  E(77, 119, 202),
  E(81, 144, 109),
  E(79, 282, 39),
  E(79, 189, 132),
  E(77, 256, 132),
  E(76, 647, 2946),
  E(79, 93, 109),
  E(79, 103, 73),
  E(79, 282, 39),
  E(77, 157, 35),
  E(76, 151, 86),
  E(79, 192, 987),
  E(76, 119, 64),
  E(77, 154, 45),
  E(79, 106, 86),
  E(79, 112, 80),
  E(79, 99, 90),
  E(81, 119, 77),
  E(81, 106, 86),
  E(81, 115, 81),
  E(79, 103, 89),
  E(79, 183, 125),
  E(79, 221, 48),
  E(81, 144, 109),
  E(81, 135, 125),
  E(81, 186, 70),
  E(81, 250, 128),
  E(77, 439, 3026),
  E(83, 112, 84),
  E(83, 109, 80),
  E(83, 321, 60),
  E(81, 151, 38),
  E(79, 119, 77),
  E(83, 176, 850),
  E(83, 141, 55),
  E(81, 112, 87),
  E(83, 256, 632),
  E(79, 109, 147),
  E(79, 103, 153),
  E(83, 192, 68),
  E(81, 256, 132),
  E(79, 189, 64),
  E(83, 192, 64),
  E(83, 210, 46),
  E(79, 446, 3403),
  E(83, 103, 89),
  E(83, 96, 90),
  E(83, 301, 90),
  E(81, 186, 199),
  E(79, 186, 61),
  E(77, 381, 523),
  E(74, 119, 73),
  E(76, 119, 77),
  E(77, 106, 83),
  E(77, 80, 112),
  E(77, 103, 83),
  E(79, 112, 84),
  E(79, 125, 67),
  E(79, 128, 64),
  E(77, 128, 68),
  E(77, 122, 70),
  E(77, 125, 131),
  E(81, 253, 68),
  E(81, 115, 141),
  E(79, 256, 65),
  E(81, 202, 51),
  E(79, 513, 2952),
  E(82, 213, 47),
  E(82, 154, 99),
  E(82, 339, 46),
  E(81, 510, 131),
  E(79, 189, 67),
  E(82, 192, 68),
  E(81, 122, 67),
  E(79, 125, 61),
  E(82, 189, 64),
  E(82, 327, 1151),
  E(82, 189, 77),
  E(86, 167, 134),
  E(84, 128, 164),
  E(84, 163, 87),
  E(82, 194, 43),
  E(81, 420, 3423),
  E(86, 157, 51),
  E(86, 106, 80),
  E(86, 244, 131),
  E(83, 157, 39),
  E(81, 131, 58),
  E(78, 321, 833),
  E(81, 125, 74),
  E(83, 115, 84),
  E(84, 96, 96),
  E(84, 83, 106),
  E(84, 99, 93),
  E(86, 109, 74),
  E(86, 135, 67),
  E(86, 125, 54),
  E(84, 135, 137),
  E(84, 85, 27),
  E(84, 138, 131),
  E(86, 192, 125),
  E(86, 115, 141),
  E(86, 189, 71),
  E(86, 250, 74),
  E(83, 567, 2632),
  E(79, 250, 131),
  E(79, 317, 74),
  E(79, 321, 64),
  E(77, 161, 35),
  E(76, 109, 83),
  E(79, 237, 850),
  E(79, 83, 106),
  E(79, 96, 160),
  E(79, 196, 73),
  E(77, 295, 968),
  E(77, 96, 106),
  E(77, 87, 99),
  E(77, 135, 64),
  E(76, 99, 97),
  E(77, 176, 77),
  E(76, 205, 3769),
  E(79, 96, 106),
  E(79, 103, 73),
  E(79, 285, 39),
  E(77, 157, 35),
  E(76, 147, 84),
  E(79, 202, 977),
  E(76, 115, 81),
  E(77, 112, 74),
  E(79, 103, 99),
  E(79, 80, 106),
  E(79, 96, 96),
  E(79, 112, 84),
  E(81, 119, 73),
  E(81, 125, 64),
  E(79, 128, 68),
  E(79, 256, 129),
  E(79, 122, 70),
  E(79, 256, 58),
  E(81, 119, 73),
  E(81, 213, 47),
  E(81, 163, 36),
  E(81, 157, 35),
  E(77, 506, 2946),
  E(83, 115, 74),
  E(83, 112, 84),
  E(83, 321, 73),
  E(81, 154, 35),
  E(79, 112, 80),
  E(83, 176, 593),
  E(79, 147, 42),
  E(81, 109, 83),
  E(83, 210, 46),
  E(83, 212, 945),
  E(79, 109, 141),
  E(83, 192, 64),
  E(81, 256, 132),
  E(79, 192, 68),
  E(83, 189, 67),
  E(83, 213, 47),
  E(79, 442, 3398),
  E(83, 112, 84),
  E(83, 99, 97),
  E(83, 295, 80),
  E(81, 161, 35),
  E(79, 157, 35),
  E(77, 253, 901),
  E(74, 122, 70),
  E(76, 115, 77),
  E(77, 99, 97),
  E(77, 80, 112),
  E(77, 96, 100),
  E(79, 109, 77),
  E(79, 125, 71),
  E(79, 122, 61),
  E(77, 131, 71),
  E(77, 125, 61),
  E(77, 128, 132),
  E(77, 125, 135),
  E(81, 186, 61),
  E(79, 256, 74),
  E(81, 250, 67),
  E(79, 516, 3064),
  E(82, 71, 108),
  E(82, 83, 122),
  E(82, 234, 135),
  E(81, 112, 61),
  E(79, 122, 102),
  E(82, 276, 689),
  E(81, 128, 74),
  E(79, 128, 71),
  E(82, 154, 1372),
  E(82, 135, 73),
  E(82, 122, 57),
  E(86, 155, 34),
  E(84, 216, 47),
  E(84, 192, 129),
  E(82, 314, 67),
  E(81, 417, 3112),
  E(86, 154, 170),
  E(86, 109, 151),
  E(86, 244, 77),
  E(83, 144, 42),
  E(81, 128, 58),
  E(78, 321, 842),
  E(81, 119, 67),
  E(83, 119, 77),
  E(84, 103, 93),
  E(84, 80, 116),
  E(84, 96, 90),
  E(86, 115, 77),
  E(86, 125, 71),
  E(86, 125, 61),
  E(84, 125, 135),
  E(84, 97, 31),
  E(84, 128, 132),
  E(86, 192, 129),
  E(86, 112, 144),
  E(86, 192, 64),
  E(86, 256, 65),
  E(83, 577, 2619),
  E(84, 256, 141),
  E(84, 199, 173),
  E(84, 553, 75),
  E(83, 112, 35),
  E(81, 295, 96),
  E(84, 505, 69),
  E(83, 125, 67),
  E(81, 189, 199),
  E(84, 317, 68),
  E(84, 183, 788),
  E(84, 183, 41),
  E(86, 154, 67),
  E(86, 112, 122),
  E(86, 151, 112),
  E(88, 272, 122),
  E(84, 176, 173),
  E(81, 205, 202),
  E(79, 505, 69),
  E(77, 155, 34),
  E(76, 208, 161),
  E(79, 904, 1042),
  E(74, 144, 58),
  E(76, 112, 74),
  E(77, 151, 48),
  E(76, 106, 80),
  E(77, 163, 36),
  E(76, 99, 77),
  E(77, 167, 38),
  E(76, 112, 80),
  E(77, 161, 35),
  E(76, 96, 93),
  E(77, 112, 148),
  E(76, 189, 67),
  E(77, 189, 58),
  E(76, 192, 74),
  E(74, 189, 67),
  E(72, 458, 555),
  E(84, 256, 141),
  E(84, 205, 173),
  E(84, 545, 74),
  E(83, 117, 37),
  E(81, 298, 90),
  E(84, 510, 70),
  E(83, 125, 67),
  E(81, 189, 192),
  E(84, 317, 68),
  E(84, 192, 782),
  E(84, 176, 48),
  E(86, 151, 77),
  E(86, 112, 119),
  E(86, 154, 109),
  E(88, 276, 109),
  E(84, 183, 176),
  E(81, 205, 202),
  E(79, 499, 68),
  E(77, 153, 33),
  E(76, 212, 157),
  E(79, 910, 1042),
  E(74, 138, 64),
  E(76, 112, 77),
  E(77, 147, 42),
  E(76, 106, 90),
  E(77, 157, 35),
  E(76, 103, 83),
  E(77, 163, 39),
  E(76, 112, 80),
  E(77, 153, 33),
  E(76, 106, 83),
  E(77, 112, 154),
  E(76, 186, 70),
  E(77, 186, 70),
  E(76, 186, 70),
  E(74, 189, 64),
  E(72, 407, 55),
};

const uint16_t CANTIDAD_EVENTOS =
  sizeof(CANCION) / sizeof(CANCION[0]);

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
    Frecuencias desde MIDI 72 hasta MIDI 88:
    DO5 hasta MI6.
  */
  static const uint16_t FRECUENCIAS[] PROGMEM = {
     523,  554,  587,  622,  659,  698,
     740,  784,  831,  880,  932,  988,
    1047, 1109, 1175, 1245, 1319
  };

  if (notaMidi < 72 || notaMidi > 88) {
    return 440;
  }

  return pgm_read_word(
    &FRECUENCIAS[notaMidi - 72]
  );
}

uint16_t leerVelocidadPorcentaje() {
  /*
    Promediamos varias lecturas para reducir pequeñas variaciones
    eléctricas que podrían hacer temblar el tempo.
  */
  uint32_t suma = 0;

  for (uint8_t i = 0; i < 8; i++) {
    suma += analogRead(PIN_POTENCIOMETRO);
  }

  const uint16_t lecturaPromedio = suma / 8;

  return map(
    lecturaPromedio,
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
  /*
    Más porcentaje implica menos tiempo por nota:
      100 % = velocidad original
      140 % = más rápido
       60 % = más lento
  */
  return (uint32_t)duracion * 100UL
       / velocidadPorcentaje;
}

void reproducirEvento(const Evento &evento) {
  const uint16_t frecuencia =
    frecuenciaDesdeMidi(evento.notaMidi);

  /*
    Se lee el potenciómetro al comienzo de cada nota.
    Esto permite modificar el tempo mientras suena la canción.
  */
  const uint16_t velocidadPorcentaje =
    leerVelocidadPorcentaje();

  const uint16_t sonido =
    ajustarDuracion(
      evento.duracionSonido,
      velocidadPorcentaje
    );

  const uint16_t pausa =
    ajustarDuracion(
      evento.duracionPausa,
      velocidadPorcentaje
    );

  encenderLedSegunNota(evento.notaMidi);
  tone(PIN_PIEZO, frecuencia);

  delay(sonido);

  noTone(PIN_PIEZO);
  apagarLeds();

  if (pausa > 0) {
    delay(pausa);
  }
}

void reproducirCancion() {
  for (uint16_t i = 0; i < CANTIDAD_EVENTOS; i++) {
    Evento evento;

    memcpy_P(
      &evento,
      &CANCION[i],
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
  pinMode(PIN_POTENCIOMETRO, INPUT);

  // Lectura inicial para estabilizar el conversor analógico.
  analogRead(PIN_POTENCIOMETRO);

  noTone(PIN_PIEZO);
  apagarLeds();

  delay(500);

  animacionInicial();
  reproducirCancion();
  animacionFinal();
}

void loop() {
  // Presionar RESET para volver a reproducir.
}
