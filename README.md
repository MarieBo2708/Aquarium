1st step :
Build project :
  mkdir build
  cd build
  cmake ..

2nd step :
Install assimp :
  path/to/your/project/folder
  cd build
  git clone https://github.com/assimp/assimp.git
  cd assimp
  cmake CMakeLists.txt
  cmake --build .

3rd step :
Run project :
  path/to/your/project/folder
  cd build
  make -j
  ./launch-TP1.sh

