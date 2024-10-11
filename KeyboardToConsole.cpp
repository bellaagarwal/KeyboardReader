#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <linux/input.h>
#include <unistd.h>
#include <fcntl.h>
#include "parse.h"

#define VAL_PRESS 1
#define VAL_RELEASE 0
#define VAL_HOLD 2

using namespace std;

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
  bool run = true;
  while(run) {
    input_event x = kbd.getNextEvent();
    int val = keynames[x.code].value;
    if(val == getKeyDown()) {
        kbd.printEvent(x);
    }
    cout << keynames[x.code].value << endl;
   if(val == 28) {
     run = false;
      break;
    }
  }

  return 0;
}
