# To build and run this project :

# First step : Build project
```bash
  mkdir build
  cd build
  cmake ..
```

# Second step : Install assimp
```bash
  path/to/your/project/folder
  cd build
  git clone https://github.com/assimp/assimp.git
  cd assimp
  cmake CMakeLists.txt
  cmake --build .
```

# Third step : Run project
```bash
  path/to/your/project/folder
  cd build
  make -j
  ./launch-TP1.sh
```

