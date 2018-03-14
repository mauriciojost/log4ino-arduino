# LOG4INO README

This is a basic library for logging control in Arduino.

If you like it, feel free to **star the project!**

## Use

Just copy the `src/log4ino/` directory somewhere in your project.

Then include the headers: 

```
#include <log4ino/Log.h>
```

and use: 

```
#define CLASS "Main"
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

## License

Note that this project is released under the [GNU General Public License v3](https://www.gnu.org/licenses/gpl.txt). 

## Contact

For any information regarding this project send me an e-mail to _mauriciojostx@gmail.com_. 

