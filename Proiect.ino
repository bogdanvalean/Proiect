#include <LiquidCrystal.h>
#include <avr/eeprom.h>
//includem fisierul cu definițiile pentru tonuri
#include "pitches.h"
#include <TimerOne.h>
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
String intrebare[] = {"Cand a avut loc atentatul de la Sarajevo?",
                      "In ce an a murit John F. Kennedy?",
                      "In ce an s-a nascut Albert Einstein?",
                      "Cati ani avea Nicolae Ceausescu cand a fost asasinat?",
                      "Cate titluri Champions League are Real Madrid?",
                      "Ce inaltime are varful Everest?",
                      "Din cate state este compusa UE? ",
                      "In ce an s-au pus bazele companiei Apple?",
                      "Cand a devenit Albania independenta?",
                      "Care este numarul casei familiei Simpson?",
                      "Ce lungime(in mile) masoare Marele Zid Chinezesc?",
                      "In ce an a murit Printesa Diana?", "In ce an a fost lansat Google?",
                      "In ce an a fost construit Zidul Berlinului?", "De la ce numar incepe jocul de darts?",
                      "In ce an s-au tinut Jocurile Olimpice la Barcelona?",
                      "In ce an a devenit orasul Bonn capitala Germaniei de Vest?",
                      "In ce an a murit rebelul mexican, Pancho Villa?",
                      "In ce an a devenit independenta Islanda?",
                      "In ce an a fost infiintata compania Walmart?",
                      "In ce an a fost inaugurat muzeul Joan Miro din Barcelona?"
                     };


int raspuns[] = {1914, 1963, 1879, 71, 11, 8848, 28, 1976, 1913, 742, 4000, 1997, 1998, 501, 1992, 1949, 1923, 1944, 1962, 1975};

byte happy[8] = {
  B00000,
  B11011,
  B11011,
  B00000,
  B00000,
  B10001,
  B01110,
};
byte sad[8] = {
  B00000,
  B11011,
  B11011,
  B00000,
  B00000,
  B01110,
  B10001,
};

int nrCurrent = 0;
int nrQ = 5;
int valIntermediar = 0;
int scor = 0;
int pos = 0;
float mult = 1.0;
boolean begin1 = false;
boolean terminat = false;
boolean rasp = false;
boolean gataTot = false;
boolean rasp2 = true; //pentru cifre - litere
char nume[] = "";
struct data_y {
  int data_scor;
  char data_nume[] = "";
} data;
struct input_y {
  int input_scor;
  char input_nume[];
} input;

int nr2 = 0, nr3 = 0, nr4 = 0, nr5 = 0, nr6 = 0, nr7 = 0, nr8 = 0, nr9 = 0;
// melodia ca lista de note
int melody[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};
//introducem durata pentru fiecare nota din melodie
int noteDurations[] = {4, 8, 8, 4, 4, 4, 4, 4 };

void setup() {
  Serial.begin(9600);
  analogReference(DEFAULT);
  pinMode(A8, INPUT);
  digitalWrite(A8, HIGH);
  pinMode(A10, OUTPUT);
  pinMode(21 , INPUT);
  lcd.begin(16, 2);
  lcd.createChar(1, happy);
  lcd.createChar(2, sad);
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("HIGHSCORE");
  eeprom_read_block((void*)&input, (void*)0, sizeof(input));
  lcd.setCursor(3, 1);
  lcd.print(input.input_scor);
  delay(2000);
  lcd.clear();
  lcd.setCursor(5, 0);
  lcd.print("QUIZ");
  lcd.setCursor(0, 1);
  lcd.print("PRESS 1 TO START");
}

void loop() {
  if (!gataTot) {
    if (!terminat) {
      if (!begin1) {
        int val = analogRead(A8);
        val = getTasta(val);
        if (val == 1) begin1 = true;
        Serial.print(val);
        delay(200);
      }

      else {
        lcd.clear();
        delay(1000);
        lcd.setCursor(9, 1);
        lcd.print("S:");
        lcd.print(scor);
        while (!rasp) {
          printLine(intrebare[nrCurrent]);
          int val = analogRead(A8);
          val = getTasta(val);
          if (val != 12) {
            lcd.setCursor(0, 1);
            lcd.print("R: ");
            if (val > 0) {
              if (val == 10) val = 0;
              if (val == 11) {
                valIntermediar = (int)valIntermediar / 10;
                lcd.setCursor(3, 1);
                lcd.print(" ");
                lcd.setCursor(4, 1);
                lcd.print(" ");
                lcd.setCursor(5, 1);
                lcd.print(" ");
                lcd.setCursor(6, 1);
                lcd.print(" ");
                lcd.setCursor(7, 1);
                lcd.print(" ");
                lcd.setCursor(3, 1);
              }
              else
                valIntermediar = valIntermediar * 10 + val;
            }
            Serial.println(valIntermediar);
            lcd.print(valIntermediar);
            delay(500);
          }
          else {
            if (valIntermediar == raspuns[nrCurrent]) {
              lcd.clear();
              lcd.setCursor(5, 0);
              lcd.print("CORECT");
              lcd.setCursor(7, 1);
              lcd.write(1);
              scor += 100 * mult;
              mult += 0.25;
              rasp = true;
              nrCurrent++;
              valIntermediar = 0;
            }
            else {
              lcd.clear();
              lcd.setCursor(4, 0);
              lcd.print("INCORECT");
              lcd.setCursor(7, 1);
              lcd.write(2);
              mult = 1;
              rasp = true;
              nrCurrent++;
              valIntermediar = 0;
            }
          }

        }
        pos = 0;
        rasp = false;
        delay(1000);
        //daca s-a ajuns la final
        if (nrQ == nrCurrent) {
          nrCurrent = 0;
          //begin1=false;
          lcd.clear();
          lcd.setCursor(5, 0);
          lcd.print("FINAL");
          lcd.setCursor(3, 1);
          lcd.print("SCOR: ");
          lcd.print(scor);
          // digitalWrite(21, LOW);
          // attachInterrupt(digitalPinToInterrupt(21),PlaySong, LOW);
          rasp2 = false;
          delay(2000);
        }
        lcd.clear();
        //  modificam functionalitatea tastaturii
        while (!rasp2) {
          int val = analogRead(A8);
          val = getTasta(val);
          lcd.setCursor(0, 1);
          lcd.print(nume);
          if (val != 12) {
            lcd.setCursor(0, 0);
            printLine("Introduceti numele:");


            if (val > 0) {
              //if(val==10) val=0;
              if (val == 2) {
                nr2++;
                lcd.setCursor(15, 1);
                lcd.print(nr2);
              }
              if (val == 3) {
                nr3++;
                lcd.setCursor(15, 1);
                lcd.print(nr3);
              }
              if (val == 4) {
                nr4++;
                lcd.setCursor(15, 1);
                lcd.print(nr4);
              }
              if (val == 5) {
                nr5++;
                lcd.setCursor(15, 1);
                lcd.print(nr5);
              }
              if (val == 6) {
                nr6++;
                lcd.setCursor(15, 1);
                lcd.print(nr6);
              }
              if (val == 7) {
                nr7++;
                lcd.setCursor(15, 1);
                lcd.print(nr7);
              }
              if (val == 8) {
                nr8++;
                lcd.setCursor(15, 1);
                lcd.print(nr8);
              }
              if (val == 9) {
                nr9++;
                lcd.setCursor(15, 1);
                lcd.print(nr9);
              }
              if (val == 11) {
                //            p[strlen(p)-1] = 0;
              }
              delay(500);
            }


            delay(500);
          }
          else {

            if (nr2 == 1) {

              strcat(nume, "A");
              nr2 = 0;
              lcd.setCursor(15, 1);
              lcd.print(" ");
            }
            else if (nr2 == 2) {
              strcat(nume, "B");
              nr2 = 0;
              lcd.setCursor(15, 1);
              lcd.print(" ");
            }
            else if (nr2 == 3) {
              strcat(nume, "C");
              nr2 = 0;
              lcd.setCursor(15, 1);
              lcd.print(" ");
            }
            else if (nr2 > 3) {
              nr2 = 0;
              lcd.setCursor(15, 1);
              lcd.print(nr2);
            }

            if (nr3 == 1) {

              strcat(nume, "D");
              nr3 = 0;
              lcd.setCursor(15, 1);
              lcd.print(" ");
            }
            else if (nr3 == 2) {
              strcat(nume, "E");
              nr3 = 0;
              lcd.setCursor(15, 1);
              lcd.print(" ");
            }
            else if (nr3 == 3) {
              strcat(nume, "F");
              nr3 = 0;
              lcd.setCursor(15, 1);
              lcd.print(" ");
            }
            else if (nr3 > 3) {
              nr3 = 0;
              lcd.setCursor(15, 1);
              lcd.print(nr3);
            }

            if (nr4 == 1) {

              strcat(nume, "G");
              nr4 = 0;
              lcd.setCursor(15, 1);
              lcd.print(" ");
            }
            else if (nr4 == 2) {
              strcat(nume, "H");
              nr4 = 0;
              lcd.setCursor(15, 1);
              lcd.print(" ");
            }
            else if (nr4 == 3) {
              strcat(nume, "I");
              nr4 = 0;
              lcd.setCursor(15, 1);
              lcd.print(" ");
            }
            else if (nr4 > 3) {
              nr4 = 0;
              lcd.setCursor(15, 1);
              lcd.print(nr4);
            }

            if (nr5 == 1) {

              strcat(nume, "J");
              nr5 = 0;
              lcd.setCursor(15, 1);
              lcd.print(" ");
            }
            else if (nr5 == 2) {
              strcat(nume, "K");
              nr5 = 0;
              lcd.setCursor(15, 1);
              lcd.print(" ");
            }
            else if (nr5 == 3) {
              strcat(nume, "L");
              nr5 = 0;
              lcd.setCursor(15, 1);
              lcd.print(" ");
            }
            else if (nr5 > 3) {
              nr5 = 0;
              lcd.setCursor(15, 1);
              lcd.print(nr5);
            }

            if (nr6 == 1) {

              strcat(nume, "M");
              nr6 = 0;
              lcd.setCursor(15, 1);
              lcd.print(" ");
            }
            else if (nr6 == 2) {
              strcat(nume, "N");
              nr6 = 0;
              lcd.setCursor(15, 1);
              lcd.print(" ");
            }
            else if (nr6 == 3) {
              strcat(nume, "O");
              nr6 = 0;
              lcd.setCursor(15, 1);
              lcd.print(" ");
            }
            else if (nr6 > 3) {
              nr6 = 0;
              lcd.setCursor(15, 1);
              lcd.print(nr6);
            }

            if (nr7 == 1) {

              strcat(nume, "P");
              nr7 = 0;
              lcd.setCursor(15, 1);
              lcd.print(" ");
            }
            else if (nr7 == 2) {
              strcat(nume, "Q");
              nr7 = 0;
              lcd.setCursor(15, 1);
              lcd.print(" ");
            }
            else if (nr7 == 3) {
              strcat(nume, "R");
              nr7 = 0;
              lcd.setCursor(15, 1);
              lcd.print(" ");
            }
            else if (nr7 == 4) {
              strcat(nume, "S");
              nr7 = 0;
              lcd.setCursor(15, 1);
              lcd.print(" ");
            }
            else if (nr7 > 4) {
              nr7 = 0;
              lcd.setCursor(15, 1);
              lcd.print(nr7);
            }

            if (nr8 == 1) {
              strcat(nume, "T");
              nr8 = 0;
              lcd.setCursor(15, 1);
              lcd.print(" ");
            }
            else if (nr8 == 2) {
              strcat(nume, "U");
              nr8 = 0;
              lcd.setCursor(15, 1);
              lcd.print(" ");
            }
            else if (nr8 == 3) {
              strcat(nume, "V");
              nr8 = 0;
              lcd.setCursor(15, 1);
              lcd.print(" ");
            }
            else if (nr8 > 3) {
              nr8 = 0;
              lcd.setCursor(15, 1);
              lcd.print(nr5);
            }

            if (nr9 == 1) {

              strcat(nume, "W");
              nr9 = 0;
              lcd.setCursor(15, 1);
              lcd.print(" ");
            }
            else if (nr9 == 2) {
              strcat(nume, "X");
              nr9 = 0;
              lcd.setCursor(15, 1);
              lcd.print(" ");
            }
            else if (nr9 == 3) {
              strcat(nume, "Y");
              nr9 = 0;
              lcd.setCursor(15, 1);
              lcd.print(" ");
            }
            else if (nr9 == 4) {
              strcat(nume, "Z");
              nr9 = 0;
              lcd.setCursor(15, 1);
              lcd.print(" ");
            }
            else if (nr9 > 4) {
              nr9 = 0;
              lcd.setCursor(15, 1);
              lcd.print(nr7);
            }
            delay(500);
          }
          int gata = analogRead(A8);
          gata = getTasta(gata);
          if (gata == 10) {
            rasp2 = true;
            terminat = true;
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print(nume);
            lcd.setCursor(0, 1);
            lcd.print(scor);
            PlaySong();
            if (scor > input.input_scor) {
              lcd.clear();
              lcd.setCursor(1, 0);
              lcd.print("NEW HIGHSCORE!");
              input.input_scor = scor;
              data.data_scor = scor;
              strcpy(data.data_nume, nume);
              eeprom_write_block((const void*)&data, (void*)0, sizeof(data));
              delay(500);
            }
          }
        }

      }
    }


    else {

      lcd.setCursor(0, 0);
      lcd.print("1- PLAY AGAIN");
      lcd.setCursor(0, 1);
      lcd.print("2- END GAME");
      int val = analogRead(A8);
      val = getTasta(val);
      if (val == 1) {
        begin1 = true;
        nrCurrent = 0;
        valIntermediar = 0;
        scor = 0;
        pos = 0;
        terminat = false;
        rasp = false;
        rasp2 = true;
        mult = 1.0;
        strcpy(nume, "");
        lcd.clear();
      }
      if (val == 2) {
        lcd.clear();
        lcd.setCursor(4, 0);
        lcd.print("BYE BYE!");
        delay(1000);
        lcd.clear();
        gataTot = true;
      }
      //Serial.print(val);
      delay(1000);
    }

  }
}

void PlaySong()
{
  for (int thisNote = 0; thisNote < 8; thisNote++) {
    //calculam durata de afisare a notei
    int noteDuration = 1000 / noteDurations[thisNote];
    //apelam functia de tone pentru difuzorul atasat la pinul 8 si durata specificata
    tone(10, melody[thisNote], noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(10);
  }
}

void printLine(String message1) {
  //Serial.println(message1);
  //Serial.println(pos);
  //Serial.println(previous);
  String message = message1;
  lcd.setCursor(0, 0);
  char lcdTop[16];
  int copySize = 16;
  if (message.length() < 16)
  {
    copySize = message.length();
  }

  int tempPos = pos;
  if (tempPos < 0)
  {
    tempPos = -(tempPos);
  }

  memcpy(&lcdTop[0], &message[tempPos], copySize);
  lcd.print(lcdTop);//Print it from position 0

  pos += 1;
  if (pos + 16 > message.length())
  {
    pos = 0;
  }

}


int getTasta(int val)
{
  if (val > 50 && val < 80)return 12; //e apasata tasta 12
  else if (val > 90 && val < 120) return 11; //e apasata tasta 11
  else if (val > 120 && val < 160) return 10; // si asa mai departe…
  else if (val > 160 && val < 200) return 9;
  else if (val > 200 && val < 240)return 8;
  else if (val > 240 && val < 290) return 7;
  else if (val > 290 && val < 320) return 6;
  else if (val > 320 && val < 370) return 5;
  else if (val > 370 && val < 410) return 4;
  else if (val > 410 && val < 450) return 3;
  else if (val > 450 && val < 490) return 2;
  else if (val > 490 && val < 530) return 1;
  else return 0; //pentru orice alta valoare returnam 0 – tasta neapasata
}
