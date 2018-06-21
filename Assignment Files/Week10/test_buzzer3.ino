#define BUZZER 3

void setup() {
  // put your setup code here, to run once:
  pinMode(BUZZER, OUTPUT);
  playNotes();
}

void loop() {
  // put your main code here, to run repeatedly:
}

void playNotes(){
  tone(BUZZER, 350, 250);
  delay(200);
  tone(BUZZER, 300, 250);
  delay(200);
  tone(BUZZER, 350, 250);
  delay(200);
}


