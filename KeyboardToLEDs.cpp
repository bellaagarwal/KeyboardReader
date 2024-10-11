#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <linux/input.h>
#include <unistd.h>
#include <fcntl.h>
#include "parse.h"
#include "De1.h"

#define VAL_PRESS 1
#define VAL_RELEASE 0
#define VAL_HOLD 2

using namespace std;

#include "Board.h";

class Counter {

protected:
//TODO put private members here
      unsigned int count;
      unsigned int max;

public:
 virtual void setCount(unsigned int c) {
       //TODO update the counter's value to be c
       if( c <= max) {
       count = c;
       }
 }
 
 Counter(){
       count = 0;
       max = 1023;
 }
 
 Counter(int c, int m) {
       count = c <= m ? c : m;
       max = m;
 }
 
 void increment() {
       setCount(count + 1);
 }
void decrement() {
      setCount(count - 1);
}
 
void shiftLeft() {
  setCount(count >> 1);
}

void shiftRight() {
  setCount(count << 1);
}
 
 //TODO put constructor, destructor
 //and other public members here - always use setCount
 //to update the counter state
};

//This is the subclass of Counter, which should
//act JUST LIKE Counter, except any time the counter's
//internal value it should display it on an SoC's LEDS
class LEDCounter : public Counter {
private:
Soc* board;
public:

  LEDCounter(Soc* b, int c, int m): Counter(c, m) {
    board = b;
  }

      void setCount(unsigned int c) {
            Counter :: setCount(c);
            board->RegisterWrite(LEDR_BASE, count);
      }
//TODO, override the setCount method
//to first call its parent, and then update
//board's LEDS to reflect the counter's value.
};


class Keyboard {
  int fd;
public:
  Keyboard(char* dev) {
    //TODO fill this in
    //exit the application if the Keyboard object cannot be created
      fd = open(dev, O_RDONLY);
      if (fd == -1) {
            cout << "ERROR: could not open " << endl;
            exit(1);
            }
  }

  ~Keyboard() {
    //TODO fill this in
      close(fd);
  }

  /**
   * A static method (called via Keyboard::printEvent(e))
   * that is useful for debugging. This should print out
   * a useful message including the type, code, and value of the event
   * @param e an input_event object to be displayed on std::cout
  */
  static void printEvent(input_event e) {
    //TODO fill this in
    //hint: keynames[e.code] evaluates to a string name of the given keycode
    // e.g., keynames[16] evaluates to "KEY_Q"
    // keynames is defined in parse.h
      if(e.type == EV_KEY) {
                  cout << keynames[e.code].name<< endl;
      }
}

  /**
   * Returns the next event received from this keyboard device.
   * This is a blocking method, which means that it should not
   * return until some EV_KEY event is read. If an event
   * of any other type is read, skip it and read the next event.
   * If there is an error reading from the keyboard, print a warning to std::cerr;
   * in this case this method may return any value.
   * @return the next input_event whose type is EV_KEY
  */
  input_event getNextEvent() {
    input_event result;
    //TODO fill in the rest of this
      while(true) {
            int ret_val = read(fd, &result, sizeof(input_event));
   if (ret_val==-1)
     cout << "could not read from usb\n";
   else if (ret_val == 0)
     cout << "nothingto read anymore\n";
            if(result.type == EV_KEY) {
                  return result;
            }
      }
 }

  /**
   * Behaves the same as getNextEvent()
   * except only returns the next event whose code is keycode,
   * and ignores all other events.
   * @param keycode - the code of the next event to return
   * @return an input event with type EV_KEY and code keycode
   */
  input_event getNextEvent(int keycode) {
      while(true) {
            input_event res = getNextEvent();
            if(res.code == keycode) {
                  return res;
            }
      }
    //TODO fill this in
  }


  /**
   * Wait until the next key is pressed down and return the
   * the keycode of the pressed key.
   * @return the code of the key that was next pressed down
   */
  int getKeyDown() {
    int result = getNextEvent().code;
    //TODO fill in this code
    return result;
  }


};
/**
 * This program should be run with a input device path
 * as the first argument in the shell
*/
int main(int argc, char**argv) {
  if (argc < 2) {
    cerr << "Usage: main <path to usb keyboard input device>" << endl;
    exit(1);
  }
  Keyboard kbd = Keyboard(argv[1]); //pass the first command-line argument to the Keyboard constructor
  //TODO fill in the remainder of this application
  LEDCounter count = LEDCounter(NULL, 0, 1023);
  bool run = true;
  input_event x = kbd.getNextEvent();
  while(run) {
    input_event x = kbd.getNextEvent(x.code);
    int val = keynames[x.code].value;
    int release = x.value;
    if(release == VAL_RELEASE) {
        kbd.printEvent(x);
    }
    //cout << keynames[x.code].value << endl;
    if(val == 103) {
    count->increment();
    }
    if(val == 105) {
    count->decrement();
    }
    if(val == 106) {
    count->shiftLeft();
    }
    if(val == 108) {
       count->shiftRight();
    }
   
   if(val == 28) {
     run = false;
      break;
    }
  }

  return 0;
}

