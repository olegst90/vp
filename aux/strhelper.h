#ifndef STRHELPER_H
#define STRHELPER_H

#include <string.h>

#define streq(s1,s2) (strcmp(s1,s2) == 0)
#define streqn(s1,s2,n) (strncmp(s1,s2,n) == 0)
#define lastch(s) ((s)[strlen(s) - 1])


#endif //STRHELPER_H
