# To build and run this project :

# First step : Clone the project

# Second step : Install assimp
```bash
  path/to/your/project/folder
  cd external
  git clone https://github.com/assimp/assimp.git
  cd assimp
  cmake CMakeLists.txt
  cmake --build .
```

if you have the error : 
```bash
cannot find -lassimp, no such file or directory
```
modify the CMakeLists.txt of the project into the "include directories" section by adding the absolute path to the assimp library that you previously installed :
```bash
-L/absolute/path/to/assimp/folder/bin -lassimp
```

# Third step : Build project
```bash
  path/to/your/project/folder
  mkdir build
  cd build
  cmake ..
  make -j
```
# Fourth step : Launch project
```bash
   path/to/your/project/folder
   cd build
   make -j
  ./launch-TP1.sh
```
