#ifndef FLATUSPEDIA_H
#define FLATUSPEDIA_H

#include <M5Cardputer.h>

class FlatusPedia {
public:
    FlatusPedia();
    void displayRandomFact();
private:
    String readRandomFact();
};

#endif
