/*connect ground to resistor to end 2 of the tempoPot.
  connect A1 to end 2 of the tempoPot  .
  connnect Vin to end 1 of the tempoPot
*/

#define MIDI_ON 144
#define MIDI_OFF 128
#define MIDI_PROGRAM 192

#define MIDI_CHANNEL_KICKS 0
#define MIDI_CHANNEL_SNARE 1
#define MIDI_CHANNEL_FILLER 2
#define MIDI_CHANNEL_CYMBAL 3

int tempoPotPin = 1;           // tempoPot pin
int MIDIvelPotPin = 2;           // tempoPot pin
int kickPin = 13;
int snarePin = 12;
int fillerPin = 11;
int cymbalPin = 10;
int lastkickButtonState = 0;
int lastsnareButtonState = 0;
int lastcymbalButtonState = 0;
int lastfillerButtonState = 0;

int status = 0;
int tempoPot = 0;
float temp = 0.0;
int MIDIvelPot = 0;
float temp1 = 0.0;
int tempo = 65; //initialise
int MIDIvelocity = 55;

boolean FillerIsON = false;
int noteNumberFiller = 60; //initialise
int stepcount = 0;

// midi2Max function declaration with optional arguments
void midi2Max( int cmd, int data1 = -1, int data2 = -1 );

unsigned long previousMillis = 0;        // will store last time it was updated

const int nb_SnarePatterns = 20; //nb_SnarePatterns is to describe the number of pattern for snares
const int nb_Snaresteps = 16;
// different sequence pattern arrays for snares, all stored into SnarePatternStorage
//sequence bank for snare channel
int SnarePatternStorage[nb_SnarePatterns][nb_Snaresteps] = {
  {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0},
  {0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1},
  {0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1},
  {0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1},
  {1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 1},
  {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1},
  {1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 0},
  {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0},
  {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
  {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0},
  {1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0},
  {1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1, 0},
  {1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 1},
  {1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0},
  {1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 0},
  {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0},
  {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
  {1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1},
  {1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 0, 1},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};



const int nb_KickPatterns = 9; //nb_SnarePatterns is to describe the number of pattern for snares
const int nb_Kicksteps = 16;
//sequence bank for kick channel
int KickPatternStorage[nb_KickPatterns][nb_Kicksteps] = {
  {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0},
  {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0},
  {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1},
  {1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0},
  {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0},
  {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
  {1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1},
  {1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 0, 1},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};

const int nb_CymbalPatterns = 11; //nb_SnarePatterns is to describe the number of pattern for snares
const int nb_Cymbalsteps = 16;
//sequence bank for cymbal channel
int CymbalPatternStorage[nb_CymbalPatterns][nb_Cymbalsteps] = {
  {1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0},
  {1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 1},
  {1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 1},
  {1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0},
  {1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 0},
  {1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0},
  {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0},
  {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
  {1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1},
  {1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 0, 1},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

};

const int nb_FillerPatterns = 14; //nb_SnarePatterns is to describe the number of pattern for snares
const int nb_Fillersteps = 16;
//sequence bank for filler channel
int FillerPatternStorage[nb_FillerPatterns][nb_Fillersteps] = {
  {66, 0, 69, 0, 70, 0, 66, 0, 69, 0, 70, 0, 66, 0, 69, 0},
  {54, 0, 57, 0, 58, 0, 54, 0, 57, 0, 58, 0, 54, 0, 57, 0},
  {64, 0, 69, 0, 74, 0, 69, 0, 65, 0, 64, 0, 64, 0, 65, 0},
  {54, 0, 56, 0, 52, 0, 54, 0, 54, 0, 56, 0, 52, 0, 54, 0},
  {50, 0, 55, 0, 50, 0, 55, 0, 50, 0, 55, 0, 50, 0, 55, 0},
  {64, 0, 64, 0, 56, 0, 52, 0, 0, 0, 54, 0, 56, 0, 52, 0},
  {47, 0, 47, 0, 47, 0, 63, 64, 0, 0, 47, 47, 0, 64, 63, 0},
  {47, 0, 47, 0, 47, 0, 64, 0, 0, 0, 47, 0, 64, 63, 0, 0},
  {47, 0, 47, 0, 47, 0, 64, 0, 0, 0, 47, 0, 64, 0, 63, 0},
  {59, 0, 59, 0, 59, 0, 75, 76, 0, 0, 59, 59, 0, 76, 75, 0},
  {59, 0, 59, 0, 59, 0, 76, 0, 0, 0, 59, 0, 76, 75, 0, 0},
  {59, 0, 59, 0, 59, 0, 76, 0, 0, 0, 59, 0, 76, 0, 75, 0},
  {58, 57, 0, 62, 60, 59, 0, 0, 58, 57, 0, 62, 60, 59, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};


int SnareProgChange = 10; //default program Change
int KickProgChange = 3; //default program Change to BDRUM3
int CymbalProgChange = 10;

int SnarePattern = 10; //the default pattern for snares
int KickPattern = 0;
int FillerPattern = 1;
int CymbalPattern = 5;

int noteNumber = 60;
int noteNumber1 = 48;

void setup()
{
  pinMode(kickPin, INPUT);
  pinMode(snarePin, INPUT);
  pinMode(fillerPin, INPUT);
  pinMode(cymbalPin, INPUT);
  delay(1000);
  Serial.begin( 115200 );  // set the MIDI baud rate
  delay(10000);

  status = MIDI_PROGRAM + MIDI_CHANNEL_KICKS;
  midi2Max( status, KickProgChange);   // set the initial kick sample
  status = MIDI_PROGRAM + MIDI_CHANNEL_SNARE;
  midi2Max( status, SnareProgChange);  // set the initial snare sample
  status = MIDI_PROGRAM + MIDI_CHANNEL_CYMBAL;
  midi2Max( status, CymbalProgChange);  // set the initial cymbal sample
}

void loop()
{
 
  unsigned long currentMillis = millis();

  int kickButtonState = digitalRead(kickPin);
  int snareButtonState = digitalRead(snarePin);
  int fillerButtonState = digitalRead(fillerPin);
  int cymbalButtonState = digitalRead(cymbalPin);


  if ((kickButtonState != lastkickButtonState) && (kickButtonState == 1)) { //pattern change requested
    KickPattern += 1;
    if (KickPattern >= nb_KickPatterns) {
      KickPattern = 0;
    }
  }
  lastkickButtonState = kickButtonState;  // save the current state as the last state

  if ((snareButtonState != lastsnareButtonState) && (snareButtonState == 1)) { //pattern change requested
    SnarePattern += 1;
    if (SnarePattern >= nb_SnarePatterns) {
      SnarePattern = 0;
    }
  }
  lastsnareButtonState = snareButtonState;  // save the current state as the last state

  if ((fillerButtonState != lastfillerButtonState) && (fillerButtonState == 1)) { //pattern change requested
    FillerPattern += 1;
    if (FillerPattern >= nb_FillerPatterns) {
      FillerPattern = 0;
    }
  }
  lastfillerButtonState = fillerButtonState;  // save the current state as the last state

  if ((cymbalButtonState != lastcymbalButtonState) && (cymbalButtonState == 1)) { //pattern change requested
    CymbalPattern += 1;
    if (CymbalPattern >= nb_CymbalPatterns) {
      CymbalPattern = 0;
    }
  }
  lastcymbalButtonState = cymbalButtonState;  // save the current state as the last state


  int minVel = 0; // SET THE MIN VELOCITY FOR THE FILLER
  int maxVel = 110; // SET THE MAX VELOCITY FOR THE FILLER
  //scale to minVel-maxVel velocity
  MIDIvelPot = analogRead( MIDIvelPotPin );    // read the input pin (value from 0 to 1023)
  temp1 = MIDIvelPot;
  temp1 *= (maxVel - minVel);
  temp1 /= 1024;
  temp1 += minVel;                     // scale the value between ranges in velocity
  MIDIvelPot = temp1;
  MIDIvelocity = MIDIvelPot;


  int minBPM = 120; //SET THE MIN DESIRED BPM !!!
  int maxBPM = 150; //SET THE MAX DESIRED BPM !!!
  //scale to minBPM-maxBPM bpm
  tempoPot = analogRead( tempoPotPin );    // read the input pin (value from 0 to 1023)
  //find a way to convert log to linear values for the tempoPot
  temp = tempoPot;
  temp *= (maxBPM - minBPM);
  temp /= 1024;
  temp += minBPM;                     // scale the value between ranges in bpm
  tempoPot = temp;
  tempo = tempoPot;

  int quarterNoteTimems = 60000 / tempo; // scaled values in ms
 

  //SEQUENCER
  if (currentMillis - previousMillis >= quarterNoteTimems / 4) {
    previousMillis = currentMillis;

    //snares - check if theres a note to be played in the current step
    if (SnarePatternStorage[SnarePattern][stepcount] == 1) {
      status = MIDI_ON + MIDI_CHANNEL_SNARE; //signal note_on
      midi2Max( status, noteNumber, 100 );   // send a MIDI "note on" message
    }

    //Kicks - check if theres a note to be played in the current step
    if (KickPatternStorage[KickPattern][stepcount] == 1) {
      status = MIDI_ON + MIDI_CHANNEL_KICKS; //signal note_on
      midi2Max( status, noteNumber1, 100 );   // send a kick
    }

    //Cymbals - check if theres a note to be played in the current step
    if (CymbalPatternStorage[CymbalPattern][stepcount] == 1) {
      status = MIDI_ON + MIDI_CHANNEL_CYMBAL;
      midi2Max( status, noteNumber, 100 );   // send a cymbal
    }

    //filler
    if (FillerIsON == true) { //if the last step was a note_on, turn it off
      FillerIsON = false;
      status = MIDI_OFF + MIDI_CHANNEL_FILLER; //signal note_off
      midi2Max( status, noteNumberFiller, 0 );   // send a MIDI "note off" message
    }
    //now check if theres a note to be played in the current step
    if (FillerPatternStorage[FillerPattern][stepcount] != 0) {
      noteNumberFiller = FillerPatternStorage[FillerPattern][stepcount];
      status = MIDI_ON + MIDI_CHANNEL_FILLER; //signal note_on
      FillerIsON = true;
      midi2Max( status, noteNumberFiller, MIDIvelocity );   // send a MIDI "note on" message
    }


    //increment step for the sequence array-traversal
    if (stepcount >= 15) {
      stepcount = 0;
    }
    else {
      stepcount++;
    }
  }
}


// Send a MIDI message of 1 to 3 "bytes" (without checking values)
void midi2Max( int cmd, int data1, int data2 )
{
  Serial.print( cmd );
  if ( data1 >= 0 ) {
    Serial.print(' ');
    Serial.print( data1 );
  }
  if ( data2 >= 0 ) {
    Serial.print(' ');
    Serial.print( data2 );
  }
  Serial.println();
}
