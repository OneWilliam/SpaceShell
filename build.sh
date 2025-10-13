#!/bin/bash

BUILD_TOOL="make"
TARGET=${1:-all}

mkdir -p build
cd build

if [ ! -f "Makefile" ]; then
  echo "[Setup] Generando archivos de compilación con CMake..."
  cmake ..
fi

echo "[Setup] Ejecutando target: $TARGET ..."
$BUILD_TOOL $TARGET
BUILD_EXIT_CODE=$?

if [ "$TARGET" = "all" ]; then
  if [ $BUILD_EXIT_CODE -eq 0 ]; then
    echo "[Setup] Corriendo SpaceShell..."
    ./spaceshell
  else
    echo "[SETUP] ERROR de compilación"
  fi
else
  if [ $BUILD_EXIT_CODE -eq 0 ]; then
    echo "[Setup] Target '$TARGET' ejecutado exitosamente"
  else
    echo "[SETUP] ERROR ejecutando target '$TARGET'"
  fi
fi
