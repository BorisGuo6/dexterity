# USAGE

To use these libraries, copy the library folder into the 'lib' directory of your project as such:

|--lib
|  |--General_ESPNOW
|  |  |- General_ESPNOW.h
|  |--HapticGlove_ESPNOW
|  |  |- HapticGlove_ESPNOW.c
|  |  |- HapticGlove_ESPNOW.h
|  |--RoboticArm_ESPNOW
|  |  |- RoboticArm_ESPNOW.c
|  |  |- RoboticArm_ESPNOW.h
|--src
|  |- main.c
|- platformio.ini

and contents of `src/main.c`:
```
#include <HapticGlove_ESPNOW.h>
#include <RoboticArm_ESPNOW.h>

int main (void)
{
  ...
}

```

PlatformIO Library Dependency Finder will find automatically dependent
libraries scanning project source files.
