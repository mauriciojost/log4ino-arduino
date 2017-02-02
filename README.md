# MAIN4INO README

This is a library for the configuration and control of software.

If you like it, feel free to **star the project!**

## Use

Your goal is to instantiate the `Bot` class. 

For this you will create:

- `Configurable`s: they will extend the class Configurable, and will be expose their configuration to the user. Example: a humidity sensor, whose alarm threshold must be configured by the user. Also any module used by other actors can be a `Configurable`.

- `Actor`s: they will extend the class `Actor` (and indirectly `Configurable`), and will be triggered with the specified frequency to act. Example: the same humidity sensor of above, which will act to actually measure humidity regularly, based on what has been configured by the user.

## Develop

The project makes use of [PlatformIO IoT open-source ecosystem](http://platformio.org/) for building and testing purposes.

Use _platformio v3.1.0_ or higher. As it comes with CLI tools, compiling and testing can be launched even without IDE.

Installation of _PlatformIO CLI_ is much easier and predictable when installing it together with the _Atom IDE_ ([download _deb_ file provided as _Atom for PlatformIO_ here](http://platformio.org/platformio-ide)).

If you really want an IDE (I do): I personally use _Intellij IDEA 14_ with the _C/C++_ plugin, and it works very well. Also I have used _bluefish_ and I was pretty satisfied with it.

About the formatter, I recommend using _clang-format_ tool. Use [this script](misc/format.bash) for automatic formatting.

To **compile** the project execute: 

```
platformio run
```

To execute the **unit tests** (that will run on your PC) execute: 

```
bash misc/test.bash
```

Note that for the execution of these tests on a CI platform (such as **Travis CI**) you will need to
create an account in PlatformIO Plus by doing: 

```
platformio account register
platformio account login
platformio account token
```

Retrieve the token and use it (hidden) in your CI platform by exporting: 

```
PLATFORMIO_AUTH_TOKEN=<token>
```

## License

Note that this project is released under the [GNU General Public License v3](https://www.gnu.org/licenses/gpl.txt). 

## Contact

For any information regarding this project send me an e-mail to _mauriciojostx@gmail.com_. 

