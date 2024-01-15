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
``
