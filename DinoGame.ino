#include <LiquidCrystal.h>

//pines displays
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);  

// sprites en piexeles (8 bits)
byte DINO_PARADO_PARTE_1[8] = {B00000, B00000, B00010, B00010, B00011, B00011, B00001, B00001}; 
byte DINO_PARADO_PARTE_2[8] = {B00111, B00111, B00111, B00100, B11100, B11100, B11000, B01000};
byte DINO_PIE_DERE_PART_1[8] = {B00000, B00000, B00010, B00010, B00011, B00011, B00001, B00001};
byte DINO_PIE_DERE_PART_2[8] = {B00111, B00111, B00111, B00100, B11100, B11100, B11000, B00000};
byte DINO_PIE_IZQU_PART_1[8] = {B00000, B00000, B00010, B00010, B00011, B00011, B00001, B00000};
byte DINO_PIE_IZQU_PART_2[8] = {B00111, B00111, B00111, B00100, B11100, B11100, B11000, B01000};
byte DOS_RAMAS_PART_1[8] = {B00000, B00100, B00100, B10100, B10100, B11100, B00100, B00100};
byte DOS_RAMAS_PART_2[8] = {B00100, B00101, B00101, B10101, B11111, B00100, B00100, B00100};
byte AVE_ALAS_PART1[8] = {B00001, B00001, B00001, B00001, B01001, B11111, B00000, B00000};
byte AVE_ALAS_PART2[8] = {B00000, B10000, B11000, B11100, B11110, B11111, B00000, B00000};

int columna_dino1 = 1;
int columna_dino2 = 2;
int fila_dino = 1;
unsigned long reloj = 0; // Reloj 
int periodo = 100; // Periodo en milisegundos
int flag = 1;
int fila_rama = 0;
int columna_rama = 13;
int periodo2 = 100;
unsigned long reloj2 = 0;
unsigned long reloj3 = 0;
int numerorandom = 0;
int columnaave = 13;
int fila_ave = 1;
int senalactual = 0;
int senalantigua = 0;
int aceleracion = 1;
int puntos = 0;
int punto2 = 0;

// columnnas para las colisiones
int b = 1;
int c = 2;
int d = 0;

void setup() {
  lcd.begin(16, 2);               
  lcd.createChar(0, DINO_PARADO_PARTE_1);      
  lcd.createChar(1, DINO_PARADO_PARTE_2);               
  lcd.createChar(2, DINO_PIE_DERE_PART_1);      
  lcd.createChar(3, DINO_PIE_DERE_PART_2);          
  lcd.createChar(4, DINO_PIE_IZQU_PART_1);      
  lcd.createChar(5, DINO_PIE_IZQU_PART_2);          
  lcd.createChar(6, DOS_RAMAS_PART_1);      
  lcd.createChar(7, DOS_RAMAS_PART_2);
}

void loop() {
  manejarPiesDinosaurio();
  manejarCactusYAve();
  verificarColisiones();
  manejarPuntos();
  manejarEstadoBoton();
  actualizarPantalla();
}

//animación pies
void manejarPiesDinosaurio() {
  if (millis() > reloj + periodo) {
    reloj = millis();
    flag = (flag == 1) ? 2 : 1; // Alternar entre los dos estados de los pies
  }

  if (d == 0) {
    if (flag == 1) {
      lcd.setCursor(columna_dino1, fila_dino); 
      lcd.write(byte(2));
      lcd.setCursor(columna_dino2, fila_dino);
      lcd.write(byte(3));
    } else {
      lcd.setCursor(columna_dino1, fila_dino); 
      lcd.write(byte(4));
      lcd.setCursor(columna_dino2, fila_dino);
      lcd.write(byte(5));
    }
  }
}

// Función para manejar las ramas y el ave
void manejarCactusYAve() {
  if (millis() > reloj2 + periodo2) {
    reloj2 = millis();
    columna_rama--;

    if (columna_rama < 0) {
      columna_rama = 13;
      periodo2 -= aceleracion; // Aceleración
      numerorandom = random(0, 3); // Generación aleatoria
    }

    limpiarPantalla(columna_rama);
    generarCactusYAve();
  }
}

// Función para limpiar las posiciones anteriores de las ramas o el ave
void limpiarPantalla(int columna) {
  lcd.setCursor(columna + 1, 1);
  lcd.print(" ");
  lcd.setCursor(columna + 1, 0);
  lcd.print(" ");
  lcd.setCursor(0, 1);
  lcd.print(" ");
  lcd.setCursor(0, 0);
  lcd.print(" ");
}

// generación de cactus o pajaros
void generarCactusYAve() {
  if (numerorandom == 1) {
    fila_rama = 1;
    lcd.createChar(6, DOS_RAMAS_PART_1);
    lcd.setCursor(columna_rama, fila_rama);
    lcd.write(byte(6));
  } else if (numerorandom == 2) {
    fila_rama = 1;
    lcd.createChar(7, DOS_RAMAS_PART_2);
    lcd.setCursor(columna_rama, fila_rama);
    lcd.write(byte(7));
  } else {
    columnaave = columna_rama - 1;
    fila_rama = 0;
    lcd.createChar(6, AVE_ALAS_PART1);
    lcd.setCursor(columnaave, fila_rama); 
    lcd.write(byte(6));

    lcd.createChar(7, AVE_ALAS_PART2);  
    lcd.setCursor(columna_rama, fila_rama); 
    lcd.write(byte(7));
  }
}

// verificación de colisiones
void verificarColisiones() {
  if (digitalRead(13) == HIGH && (columna_rama == 1 || columna_rama == 2 || columnaave == 1 || columnaave == 2) && fila_rama == 0) {
    gameOver();
  }

  if ((columna_rama == b || columna_rama == c) && fila_rama == 1) { // Rama
    gameOver();
  }
}

// pantalla del fin del juego
void gameOver() {
  lcd.clear();
  lcd.setCursor(5, 0);					
  lcd.print("GAME OVER");
  delay(2000);
  lcd.clear();
  columna_rama = 15;
  periodo2 = 100;
  puntos = 0;
  punto2 = 0;
  periodo2 = 100;
}

// salto del dinosaurio
void manejarEstadoBoton() {
  if (digitalRead(13) == HIGH) {
    b = 50; // Evita la colisión
    c = 50;

    if (d == 0) {
      lcd.setCursor(0, 1); // Limpia la fila inferior
      lcd.print("    ");
    }
    d = 1; 

    lcd.setCursor(columna_dino1, 0); 
    lcd.write(byte(2));
    lcd.setCursor(columna_dino2, 0);
    lcd.write(byte(3));
  } else {
    b = 1; // Restaurar los valores de inicio
    c = 2;
    d = 0;
  }
}

// acomulación de puntos
void manejarPuntos() {
  if (millis() > reloj3 + 100) { // Intervalo para actualizar los puntos
    reloj3 = millis();
    lcd.setCursor(14, 1);
    lcd.print(puntos);
    puntos++;

    if (puntos == 100) {
      puntos = 0;
      punto2++;
      if (punto2 == 100) {
        punto2 = 0;
      }
    }
  }
}

// actualizacion de pantalla
void actualizarPantalla() {
  lcd.setCursor(14, 1);
  lcd.print(puntos);
  lcd.setCursor(14, 0);
  lcd.print(punto2);
  senalactual = digitalRead(13);

  // cambios en el botón
  if (senalactual != senalantigua) {
    lcd.setCursor(1, 0);					
    lcd.print("  "); 
  }
  senalantigua = senalactual;
}
