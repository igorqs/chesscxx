FROM archlinux:latest

# Install dependencies
RUN pacman -Syu --noconfirm && pacman -S cppcheck make cmake clang gtest yaml-cpp --noconfirm
