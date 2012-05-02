#ifndef YODA_FORMATTING_H
#define YODA_FORMATTING_H

#include <iostream>
#include <unistd.h>


#define MSG(msg)                                \
  do {                                          \
    std::cout << msg  << std::endl;             \
  } while (0)


#define COLOR_(msg, code)                       \
  (isatty(1) ? code : "") << msg << (isatty(1) ? "\033[0m" : "")


#define RED(msg)     COLOR_(msg, "\033[0;31m")
#define GREEN(msg)   COLOR_(msg, "\033[0;32m")
#define YELLOW(msg)  COLOR_(msg, "\033[0;33m")
#define BLUE(msg)    COLOR_(msg, "\033[0;34m")

#define MSG_RED(x)     MSG(RED(x))
#define MSG_GREEN(x)   MSG(GREEN(x))
#define MSG_YELLOW(x)  MSG(YELLOW(x))
#define MSG_BLUE(x)    MSG(BLUE(x))


#endif
