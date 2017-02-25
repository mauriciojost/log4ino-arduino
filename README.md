# LOG4INO README

This is a basic library for logging control in Arduino.

If you like it, feel free to **star the project!**

## Use

Just copy somewhere in your project (for instance under `$ROOT_DIR/lib/`) and do:
```
cd $ROOT_DIR
ln -s lib/log4ino/src/log4ino src/log4ino
```

Then include the headers: 

```
#include <log4ino/Log.h>
```

and use: 

```
#define CLASS "Main"
...
log(CLASS, Info, "Initialized correctly");
```

Log level can be controlled via macros.

## License

Note that this project is released under the [GNU General Public License v3](https://www.gnu.org/licenses/gpl.txt). 

## Contact

For any information regarding this project send me an e-mail to _mauriciojostx@gmail.com_. 

