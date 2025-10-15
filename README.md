<div align="center">
<img width="256" height="256" alt="icon" src="https://github.com/user-attachments/assets/f0f979af-c51b-4e16-bfc1-69dd9963b397" />
<h1>SpaceShell</h1>
<h2><i>Una shell mínima con funcionalidades básicas y utilidades.</i></h2>
</div>


## Características

| Característica | Estado | Tipo |
| :--- | :---: | :--- |
| **Prompt personalizado y lectura de línea** | ✅ | Base |
| **Resolución de rutas (absolutas y en `/bin`)** | ✅ | Base |
| **Ejecución con `fork()` y `exec*()`** | ✅ | Base |
| **Espera de procesos con `wait()`/`waitpid()`** | ✅ | Base |
| **Manejo de errores en comandos y ejecución** | ✅ | Base |
| **Redirección de salida (`>`)** | ✅ | Base |
| **Comando de salida (`salir`)** | ✅ | Base |
| **Pipes simples (`cmd1 \| cmd2`)** | ✅ | Extra |
| **Tareas en segundo plano (`&`)** | ✅ | Extra |
| **Redirección de entrada (`<`) y anexo (`>>`)** | ✅ | Extra |
| **Comandos internos (`cd`, `pwd`, `history`, etc.)** | ✅ | Extra |
| **Concurrencia con hilos (`parallel`)** | ✅ | Extra |
| **Gestión de memoria (`meminfo`)** | ➖ | Extra |
| **Manejo de señales (`SIGINT`)** | ✅ | Extra |

 Leyenda de Estado: ✅ Implementado | 🚧 En Progreso | ❌ No Iniciado | ➖ No Planeado

## Requisitos de Desarrollo
Para compilar, ejecutar y trabajar en SpaceShell, se necesita el siguiente conjunto de herramientas.

*   **Compilador C++:** Un compilador con soporte para C++11 o superior (ej. `g++`).
*   **CMake:** Versión 3.10 o superior, para la gestión del sistema de compilación.
*   **Git o Github CLI:** Para el control de versiones.
*   **Clang-format (Opcional):** Para formatear el código automáticamente.

## Instrucciones de Compilación y Ejecución

El proyecto incluye un script de Bash (`build.sh`) que automatiza todo el proceso de compilación y ejecución.

1.  **Clona el repositorio:**
    ```bash
    git clone https://github.com/OneWilliam/SpaceShell.git
    cd SpaceShell
    ```

2.  **Da permisos de ejecución al script (solo la primera vez):**
    ```bash
    chmod +x build.sh
    ```

3.  **Para compilar el proyecto y lanzar la shell interactivamente, simplemente ejecuta:**
    ```bash
    ./build.sh
    ```
    
    El script se encargará de crear el directorio `build`, ejecutar `cmake` si es necesario, compilar con `make`, y finalmente lanzar `./spaceshell`.

4.  **(Opcional) Formatear el Código**
    El proyecto está configurado con `clang-format`. Para formatear todo el código fuente automáticamente, puedes pasar el target `format` al script:

    ```bash
    ./build.sh format
    ```
   

## Casos de prueba

