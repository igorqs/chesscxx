FROM archlinux:latest

# Install dependencies
RUN pacman -Syu --noconfirm && \
  pacman -S git \
  cmake ccache ninja mold clang cppcheck codespell doxygen \
  python-sphinx python-breathe python-sphinx-furo python-sphinx-inline-tabs python-myst-parser \
  gtest yaml-cpp --noconfirm && \
  git clone https://github.com/Neargye/magic_enum.git /tmp/magic_enum && \
  cd /tmp/magic_enum && \
  cmake -S . -B build/ -G Ninja -DCMAKE_BUILD_TYPE=Release && \
  cmake --build build/ -j$(nproc) && \
  cmake --install build/ && \
  cd / && rm -rf /tmp/magic_enum
