<div align="center">
<img width="256" height="256" alt="icon" src="https://github.com/user-attachments/assets/f0f979af-c51b-4e16-bfc1-69dd9963b397" />
<h1>SpaceShell</h1>
<h2><i>Una shell mínima, pero robusta, con customización, funcionalidades básicas y utilidades.</i></h2>
</div>


## Características

| Característica | Estado | Tipo |
| :--- | :---: | :--- |
| **Prompt personalizado y lectura de línea** | ✅ | Base |
| **Resolución de rutas (absolutas y en `/bin`)** | ✅ | Base |
| **Ejecución con `fork()` y `exec*()`** | ✅ | Base |
| **Espera de procesos con `wait()`/`waitpid()`** | ✅ | Base |
| **Manejo de errores en comandos y ejecución** | 🚧 | Base |
| **Redirección de salida (`>`)** | ✅ | Base |
| **Comando de salida (`salir`)** | ✅ | Base |
| **Pipes simples (`cmd1 \| cmd2`)** | ✅ | Extra |
| **Tareas en segundo plano (`&`)** | ✅ | Extra |
| **Redirección de entrada (`<`) y anexo (`>>`)** | ✅ | Extra |
| **Comandos internos (`cd`, `pwd`, `history`, etc.)** | ✅ | Extra |
| **Concurrencia con hilos (`parallel`)** | 🚧 | Extra |
| **Gestión de memoria (`meminfo`)** | 🚧 | Extra |
| **Manejo de señales (`SIGINT`)** | ✅ | Extra |

 Leyenda de Estado: ✅ Implementado | 🚧 En Progreso | ❌ No Iniciado

## Requisitos de Desarrollo
Para compilar, ejecutar y trabajar en SpaceShell, se necesita el siguiente conjunto de herramientas.

*   **Compilador C++:** Un compilador con soporte para C++11 o superior (ej. `g++`).
*   **CMake:** Versión 3.10 o superior, para la gestión del sistema de compilación.
*   **Git o Github CLI:** Para el control de versiones.


## Casos de prueba
