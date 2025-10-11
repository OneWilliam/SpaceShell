#!/bin/bash

BUILD_TOOL="make"

mkdir -p build
cd build

if [ ! -f "Makefile" ]; then
  echo "[Setup] Generando archivos de compilación con CMake..."
  cmake ..
fi

echo "[Setup] Compilando el proyecto..."
$BUILD_TOOL

if [ $? -eq 0 ]; then
  echo "[Setup] Corriendo SpaceShell..."
  ./spaceshell
else
  echo "[SETUP] ERROR de compilación"
fi
