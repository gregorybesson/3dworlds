Raytracing - CSFML based minigame
================================================

# Install csfml on Mac OSX

Download the following archives:

- CSFML-2.3-osx-clang-universal.tar.gz : http://www.sfml-dev.org/download/csfml/
- SFML-2.3.2-osx-clang-universal.tar.gz : http://www.sfml-dev.org/download/sfml/2.3.2/
- InstallNameToolGui.zip : https://github.com/garvankeeley/install-name-tool-gui

1. Put the include directory of CSFML in your project include directory
2. Put the lib directory of CSFML in your project lib directory
3. Put the lib directory of SFML in your project lib directory
4. Put the extlibs directory of SFML in your project lib directory
5. With InstallNameToolGui, change all paths of the dylib files included in your lib project: Change each line containing @rpath/ with @loader_path/lib/
@loader_path is used to change the dependency in a binary to use a relative path to locate the library (relative to the install location of the binary)

Once done, you're ready to code with CSFML !

To compile your c file:

```
gcc main.c -o prog -I./include -L./lib -lcsfml-window -lcsfml-graphics -lcsfml-system
```

(you can also grab the lib and include directories of this project as they're already modified for Mac OSX)

Enjoy !