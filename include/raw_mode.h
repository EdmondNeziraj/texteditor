#ifndef RAW_MODE_H
#define RAW_MODE_H

#include <termios.h>

class RawMode {
    public:
        RawMode();
        ~RawMode();

    private:
        static void enable();
        static void restore();
        static struct termios original_state_;
};

#endif // RAW_MODE_H