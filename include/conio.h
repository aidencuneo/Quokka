// Useful functions to keep in case they're needed in the future

#include <termios.h>

struct termios OLDATTR, NEWATTR;

int getch(void)
{
    int ch;
    tcgetattr(STDIN_FILENO, &OLDATTR);
    NEWATTR = OLDATTR;
    NEWATTR.c_lflag &= ~( ICANON | ECHO );
    tcsetattr(STDIN_FILENO, TCSANOW, &NEWATTR);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &OLDATTR);
    return ch;
}

int getche(void)
{
    int ch;
    tcgetattr( STDIN_FILENO, &OLDATTR );
    NEWATTR = OLDATTR;
    NEWATTR.c_lflag &= ~( ICANON );
    tcsetattr( STDIN_FILENO, TCSANOW, &NEWATTR );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &OLDATTR );
    return ch;
}
