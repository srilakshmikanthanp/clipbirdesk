git clone https://github.com/KDE/extra-cmake-modules.git
cd extra-cmake-modules
git checkout v6.2.0
cmake -B ./build -DCMAKE_INSTALL_PREFIX="/opt/kderoot" -DCMAKE_BUILD_TYPE=Release -DQT_MAJOR_VERSION=6 -DCMAKE_PREFIX_PATH="${QT_CMAKE_DIR}"
cmake --build ./build --config Release --target install
cd build
make
sudo make install
cd ../..
rm -rf extra-cmake-modules

git clone https://github.com/KDE/plasma-wayland-protocols.git
cd plasma-wayland-protocols
cmake -B ./build -DCMAKE_INSTALL_PREFIX="/opt/kderoot" -DCMAKE_BUILD_TYPE=Release -DQT_MAJOR_VERSION=6 -DCMAKE_PREFIX_PATH="${QT_CMAKE_DIR}"
cmake --build ./build --config Release --target install
cd build
make
sudo make install
cd ../..
rm -rf plasma-wayland-protocols

git clone https://github.com/KDE/kguiaddons.git
cd kguiaddons
git checkout v6.2.0
cmake -B ./build -DCMAKE_INSTALL_PREFIX="/opt/kderoot" -DCMAKE_BUILD_TYPE=Release -DQT_MAJOR_VERSION=6 -DCMAKE_PREFIX_PATH="${QT_CMAKE_DIR}"
cmake --build ./build --config Release --target install
cd build
make
sudo make install
cd ../..
rm -rf kguiaddons
