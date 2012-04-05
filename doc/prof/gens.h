#ifndef GENS_H
#define GENS_H

extern void mcount();
extern void monstartup(unsigned int low, unsigned int high);
extern void moncontrol(int mode); // mode=0 stops profiling, any other value starts it
extern void moncleanup();
extern void gens_log(const char *msg);

#endif
