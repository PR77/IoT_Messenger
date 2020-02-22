#include "gameTetris.h"


//=============================================================================
// Tetris theme tune
//=============================================================================

/*
 https://dragaosemchama.com/en/2019/02/songs-for-arduino/
*/

/*
 Tetris theme - (Korobeiniki)
 Connect a piezo buzzer or speaker to pin 11 or select a new pin.
 More songs available at https://github.com/robsoncouto/arduino-songs

 Robson Couto, 2019
*/

// notes of the moledy followed by the duration. A 4 means a quarter note,
// 8 an eighteenth , 16 sixteenth, so on. A negative numbers are used to
// represent dotted notes, so -4 means a dotted quarter note, that is, a
// quarter plus an eighteenth!!

const int16_t melody[] = {

    // Based on the arrangement at https://www.flutetunes.com/tunes.php?id=192

    NOTE_E5, 4, NOTE_B4, 8, NOTE_C5, 8, NOTE_D5, 4, NOTE_C5, 8, NOTE_B4, 8,
    NOTE_A4, 4, NOTE_A4, 8, NOTE_C5, 8, NOTE_E5, 4, NOTE_D5, 8, NOTE_C5, 8,
    NOTE_B4, -4, NOTE_C5, 8, NOTE_D5, 4, NOTE_E5, 4,
    NOTE_C5, 4, NOTE_A4, 4, NOTE_A4, 8, NOTE_A4, 4, NOTE_B4, 8, NOTE_C5, 8,

    NOTE_D5, -4, NOTE_F5, 8, NOTE_A5, 4, NOTE_G5, 8, NOTE_F5, 8,
    NOTE_E5, -4, NOTE_C5, 8, NOTE_E5, 4, NOTE_D5, 8, NOTE_C5, 8,
    NOTE_B4, 4, NOTE_B4, 8, NOTE_C5, 8, NOTE_D5, 4, NOTE_E5, 4,
    NOTE_C5, 4, NOTE_A4, 4, NOTE_A4, 4, REST, 4,

    NOTE_E5, 4, NOTE_B4, 8, NOTE_C5, 8, NOTE_D5, 4, NOTE_C5, 8, NOTE_B4,8,
    NOTE_A4, 4, NOTE_A4, 8, NOTE_C5, 8, NOTE_E5, 4, NOTE_D5, 8, NOTE_C5,8,
    NOTE_B4, -4, NOTE_C5, 8, NOTE_D5, 4, NOTE_E5, 4,
    NOTE_C5, 4, NOTE_A4, 4, NOTE_A4, 8, NOTE_A4, 4, NOTE_B4, 8, NOTE_C5,8,

    NOTE_D5, -4, NOTE_F5, 8, NOTE_A5, 4, NOTE_G5, 8, NOTE_F5, 8,
    NOTE_E5, -4, NOTE_C5, 8, NOTE_E5, 4, NOTE_D5, 8, NOTE_C5, 8,
    NOTE_B4, 4, NOTE_B4, 8, NOTE_C5, 8, NOTE_D5, 4, NOTE_E5, 4,
    NOTE_C5, 4, NOTE_A4, 4, NOTE_A4, 4, REST, 4,

    NOTE_E5, 2, NOTE_C5, 2,
    NOTE_D5, 2, NOTE_B4, 2,
    NOTE_C5, 2, NOTE_A4, 2,
    NOTE_GS4, 2, NOTE_B4, 4, REST, 8,
    NOTE_E5, 2, NOTE_C5, 2,
    NOTE_D5, 2, NOTE_B4, 2,
    NOTE_C5, 4, NOTE_E5, 4, NOTE_A5, 2,
    NOTE_GS5, 2,

};

//=============================================================================
// Tetris tune defines and constants
//=============================================================================

#define NOTES_COUNT             sizeof(melody) / (sizeof(melody[0]) / 2)
#define TEMPO                   144

// Whole note duration is based on 60 seconds / TEMP, 4 beats
#define WHOLE_NOTE_DURATION     (60000 * 4) / TEMPO

#define BUZZER_PIN              13

//=============================================================================
// Global object
//=============================================================================

gameTetrisObject_s gameTetrisObject;

//=============================================================================
// Tetris game global object initialisation
//=============================================================================

void gameTetrisInitialise(void) {

    gameTetrisObject.currentNoteIndex = 0;
    gameTetrisObject.refreshTimer = 0;
    gameTetrisObject.noteDelayTimer = 0;
}

void gameTetrisLoopMusic(void) {
    int divider = 0, noteDuration = 0;

    // calculates the duration of each note
    divider = melody[gameTetrisObject.currentNoteIndex + 1];
    if (divider > 0) {
        // regular note, just proceed
        noteDuration = (WHOLE_NOTE_DURATION) / divider;
    } else if (divider < 0) {
        // dotted notes are represented with negative durations!!
        noteDuration = (WHOLE_NOTE_DURATION) / abs(divider);
        noteDuration *= 1.5; // increases the duration in half for dotted notes
    }

    // stop the waveform generation before the next note.
    noTone(BUZZER_PIN);

    // we only play the note for 90% of the duration, leaving 10% as a pause
    tone(BUZZER_PIN, melody[gameTetrisObject.currentNoteIndex], noteDuration * 0.9);

    if (gameTetrisObject.currentNoteIndex < (NOTES_COUNT * 2)) {
        gameTetrisObject.currentNoteIndex += 2;
    } else {
        gameTetrisObject.currentNoteIndex = 0;
    }

    gameTetrisObject.noteDelayTimer = noteDuration;
}

//=============================================================================
// Tetris game
//=============================================================================

gameTetrisState_e gameTetrisCyclic(OLEDDisplay *display, ClickButton *rotateForward, ClickButton *rotateBack, ClickButton *drop, ClickButton *exit, uint16_t *gameRemainingBudget) {
    uint32_t currentTime = millis();

    if (gameTetrisObject.gameTetrisState == gameTetrisStateIdle) {

        gameTetrisInitialise();

        display->clear();
        display->setFont(ArialMT_Plain_16);
        display->setTextAlignment(TEXT_ALIGN_CENTER);
        display->drawString(64, 8, "Korobeiniki");
        display->display();

        gameTetrisObject.gameTetrisState = gameTetrisStateRunning;
    }

    if (gameTetrisObject.gameTetrisState == gameTetrisStateRunning) {
        if ((currentTime - gameTetrisObject.refreshTimer) >=  gameTetrisObject.noteDelayTimer) {
            gameTetrisObject.refreshTimer = currentTime;
            gameTetrisLoopMusic();
        }
    }

    *gameRemainingBudget = gameTetrisObject.noteDelayTimer;

    if (exit->clicks > 0) {
        gameTetrisObject.gameTetrisState = gameTetrisStateIdle;
        return (gameTetrisStateExit);
    }

    return (gameTetrisObject.gameTetrisState);
}
