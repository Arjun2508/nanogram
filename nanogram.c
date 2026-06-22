#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <ctype.h>
#include <stdio.h>

struct termios orig_termios;

void disableRawMode() {
    tcsetattr(0, TCSAFLUSH, &orig_termios);
}

void enableRawMode() {
    struct termios raw;
    tcgetattr(0, &orig_termios); // tcgetattr function basically fetches the terminal configurations to be stored in a termios struct
    atexit(disableRawMode);

    raw = orig_termios;

    raw.c_iflag &= ~(ICRNL | IXON);                     // ICRNL for fixing Ctrl-M, IXON for Ctrl-S and Q
    raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);    // ECHO for not echoing out, ICANON for sending every input, ISIG for disabling Ctrl C and Z, IEXTEN is for disabling Ctrl V

    tcsetattr(0, TCSAFLUSH, &raw);
}

int main() {
    char c;

    enableRawMode();

    while (read(STDIN_FILENO, &c, 1) == 1 && c!= '@') { // can also be 0 instead of STDIN_FILENO, 1 for STDOUT_FILENO
        if (iscntrl(c)) { // control characters are characters that are not supposed to be displayed (enter, tab, backspace)
            if (c == 127) {
                // write(1, "\b \b", 1);
                printf("\b \b");
                fflush(stdout);
            }
            else if (c == 10) {
                // write(1, "\n", 1);
                printf("\n");
                fflush(stdout);
            }
            else if (c == 9) {
                // write(1, "\n", 1);
                printf("\t");
                fflush(stdout);
            }
            else{
                printf("%d", c);
                fflush(stdout);
            }
            continue;
        }

        // write(1, &c, 1);
        printf("%c", c);
        fflush(stdout);
    }

    // disableRawMode();
    
    return 0;
}