FROM archlinux:latest

# Install packages from pacman
RUN pacman -Syu --noconfirm && \
  pacman -S --noconfirm \
  git python cmake ccache ninja clang cppcheck codespell doxygen \
  gtest yaml-cpp && \
  pacman -Scc --noconfirm

# Build and install magic_enum
RUN git clone https://github.com/Neargye/magic_enum.git /tmp/magic_enum && \
  cd /tmp/magic_enum && \
  cmake -S . -B build/ -G Ninja -DCMAKE_BUILD_TYPE=Release && \
  cmake --build build/ -j$(nproc) && \
  cmake --install build/ && \
  cd / && rm -rf /tmp/magic_enum

# Create python venv and install python packages
ENV VIRTUAL_ENV=/opt/venv
RUN python -m venv $VIRTUAL_ENV
ENV PATH="$VIRTUAL_ENV/bin:$PATH"
RUN pip install sphinx breathe sphinx-furo sphinx-inline-tabs myst-parser git+https://github.com/matus-chochlik/ctcache.git
