/* 
# Music Offering: 
  An instrumental C++ program to show music key switches;
# Brief Introduction: 
   This script helps to learn how music keys are shifted by the sharp & flat marks in sheet music. 
   You can control the shifts by adding p(sharp) or l(flat) notes, and check how the (logical) black & white keys are arranged on the piano. 
# Example:
   Try the following commands in console, and the logical pianos will be printed after RET (RET means Enter):
   - type C RET to switch to C-Major;
   - type p RET to sharpen to G-Major, or l RET to flatten to F-Major;
   - type F or Eb or G# to switch to each corresponding majors;
   - type C to go back to C-Major;
   - type ppppp RET to check that five sharp marks result in B-Major;
   - type C RET llll RET to check that four flat marks result in G#(Ab)-Major;
   - type s to toggle the #/b marks of the piano, or q to quit.
# Author and Lisence:
   - Author: Linghuei Guo. Henu, Zhengzhou, China. 16 Nov 2023.
   - Contact: [mailto:guolh2013@hotmail.com], gpg hash: 6819D81B0971C2C4
   - All rights reserved.
*/

#include <iostream>
#include <iomanip>
#include <cstring>

using namespace std;

struct NoteName {
  char n[3];
};

class Octave {
  int notes[12];
  static const NoteName names_sharp[12]; // Pitch names with #s
  static const NoteName names_flat[12];  // Pitch names with bs
  const NoteName* names;                 // The name style chooser

  static inline char* getOct(int i) {
    // get the octave index i belongs to, where 0 denotes the middle octave. 
    static char ocnotes[5][2] = {"_", ".", " ", "'", "^"};
    int j;
    if (i < 0)
      j = ((i + 1) / 12) - 1;
    else
      j = i / 12;
    j = j + 2;
    if (j < 0 || j > 4)
      j = 2;
    return ocnotes[j];
  }
  
  static inline int getIndex(int n) {
    return (n % 12 + 12) % 12;
  }
  int getNote(int n) {
    return notes[ getIndex(n) ];
  }
  const char* getName(int n) {
    int i = getIndex(n);
    return names[i].n;
  }
  int find7() {
    // find the index of 7 from current pitch list;
    int rise = 0, fulls = 0;
    for (int i = 0; i < 24; i++) {
      int x0 = i % 12, x1 = (i + 1) % 12;
      if (notes[x0] == 1) {
        if (notes[x1] == 0)
          rise ++;
        else {
          rise = 0;
          fulls = 0;
        }
      }
      else if (notes[x1] == 1) {
        if (rise > 0) {
          fulls ++;
          if (fulls >= 3)
            return x1;
        }
        rise = 0;
      }
      else {
        rise = 0;
        fulls = 0;
      }
    }
    return -1;
  }

  int find4() {
    // find the index of 4 in current pitch list;
    int i = find7();
    return (i + 6) % 12;
  }

public:
  Octave () 
    : notes {1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1}, names(names_sharp) {
  }
 
  void sharpen() {
    // Move the current 4 one semitone right .
    int i = find4();
    int i0 = (i + 1) % 12;
    swap(notes[i], notes[i0]);
  }
  
  void flatten() {
    // Move the current 7 one semitone left.
    int i = find7();
    int i0 = (i + 11) % 12;
    swap(notes[i], notes[i0]);
  }

  void raise() {
    // Right shift all tones.
    for (int i = 10; i >= 0; i--) {
      swap(notes[i], notes[i + 1]);
    }
  }

  void lower() {
    // Left shift all tones.
    for (int i = 1; i < 12; i++) {
      swap(notes[i - 1], notes[i]);
    }
  }

  void shifto(char c) {
    // switch to a certain major key.
    for (int i = 0; i < 12; i++) {
      const char *k = findMajor();
      if (k[0] == c && k[1] == '\0')
	return;
      sharpen();
    }
  }

  int switchNotes() {
    if (names == names_sharp) {
      names = names_flat;
      return -1;
    }
    else {
      names = names_sharp;
      return 1;
    }
  }
  
  void printBlack(int pos1, int pos2){
    int mark = 0;
    printf(" ");
    for (int i = pos1; i <= pos2; i++) {
      int ii = ((i % 12) + 12) % 12;
      if (notes[ii] == 0 ) {
        printf(" %1s%-2s ", getOct(i), names[ii].n);
        mark = 0;
      }
      else {
        if (mark++ > 0)
          printf("     ");
      }
    }
  }
  void printWhite(int pos1, int pos2){
    int mark = 1;
    for (int i = pos1; i <= pos2; i++) {
      int ii = ((i % 12) + 12) % 12;
      if (notes[ii] == 1 ) {
        printf("%1s%-2s  ", getOct(i), names[ii].n);
        mark = 0;
      }
      else {
        if (mark++ > 0)
          printf("     ");
      }

    }
  }

  const char* findMajor() {
    return names[(find7() + 1) % 12].n;
  }

  void print() {
    printf("\n---- Here is the logical piano ( 1 = %-2s):\n\n", findMajor());
    int pos1 = -24, pos2 = 35;
    // int pos1 = 0, pos2 = 11;
    printBlack(pos1, pos2);
    cout<<endl;
    printWhite(pos1, pos2);
    cout<<endl;
  }
  static void test() {
    Octave sc;
    sc.print();
    cout<<endl;
    for (int i = 0; i < 12; i++) {
      sc.sharpen();
      sc.print();
    }
  }
  static void run() {
    cout<<" \n---  Note:\n   This script helps to learn how music keys are shifted by the sharp & flat marks in music sheets. \n   You can control the shift by adding p(sharp) or l(flat) notes \n    and check how the (logical) black & white keys are arranged on the piano. \n\n";

    Octave sc;

    
    sc.print();
    char c;
    while ( 1 ) {
      cout<<endl<<"---- Type p / l to sharpen/flatten, < / > to shift left/right, s to toggle #/b, or the major name in capital: ";
      cin>>c;
      if (c == 'p')
        sc.sharpen();
      else if (c == 'l')
        sc.flatten();
      else if (c == '>' || c == '#')
	sc.raise();
      else if (c == '<' || c == 'b')
	sc.lower();
      else if (c == 's')
	sc.switchNotes();
      else if (c == 'q') 
	break;
      else {
	// Shifting to "X#" or "Yb" will have to take two steps since only one char is read each loop...
	sc.shifto(c);
      }
      
      sc.print();
    }
  }
};

const NoteName Octave::names_sharp[12] = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};
const NoteName Octave::names_flat[12]  = {"C", "Db", "D", "Eb", "E", "F", "Gb", "G", "Ab", "A", "Bb", "B"};

int main () {
  Octave::run();
  return 0;
}
