FROM archlinux:latest

# Install packages from pacman
RUN pacman -Syu --noconfirm && \
  pacman -S --noconfirm \
  git cmake ccache ninja gcc clang cppcheck codespell doxygen \
  python python-pipx python-sphinx python-breathe python-sphinx-furo python-sphinx-inline-tabs python-myst-parser \
  gtest yaml-cpp --noconfirm && \
  pacman -Scc --noconfirm

# Build and install magic_enum
RUN git clone https://github.com/Neargye/magic_enum.git /tmp/magic_enum && \
  cd /tmp/magic_enum && \
  cmake -S . -B build/ -G Ninja -DCMAKE_BUILD_TYPE=Release && \
  cmake --build build/ -j$(nproc) && \
  cmake --install build/ && \
  cd / && rm -rf /tmp/magic_enum

# Install ctcache with pipx
RUN pipx install git+https://github.com/matus-chochlik/ctcache.git
ENV PATH="/root/.local/bin:${PATH}"
