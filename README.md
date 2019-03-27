# LOG4INO README

This is a basic but yet powerful library for logging in Arduino.

If you like it, feel free to **star the project!**

## Get started

Just copy the `src/log4ino/` directory somewhere in your project.

Then include the headers: 

```
#include <log4ino/Log.h>
```

and use: 

```
#define CLASS "MA"
...

void logCallback(const char* str) {
  // Serial.println(str);
  // or
  // LCD.display(str);
  // or
  // <your own>
}

void setup() {
  setupLog(logCallback);

  log(CLASS, Info, "Initialized correctly: %s", "now");
}
```

The levels are: 

- Debug (0)
- Info (1)
- Warn (2)
- Error (3)

Log level can be controlled via `LOG_LEVEL` numeric macro.
A value of 0 shows everything, 1 will ignore debug, 3 will only show error level. 

After these simple steps you can do your logging with the following powerful statements: 

```
log(CLASS, Warn, "This %s", "is");
log(CLASS, Info, "a very powerful");
log(CLASS, Debug, "logging framework -> %d %0.2f", 10, 5.0);
```
## License

Note that this project is released under the [GNU General Public License v3](https://www.gnu.org/licenses/gpl.txt). 

## Contact

For any information regarding this project send me an e-mail to _mauriciojostx@gmail.com_. 

