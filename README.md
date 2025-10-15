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

### Prompt personalizado
   - Mostrar un prompt propio y leer la línea de comando.
   <img width="777" height="198" alt="image" src="https://github.com/user-attachments/assets/38914d1e-9211-4dd6-8bbe-105a1e53f217" />

   
### Resolución de rutas
   - Si el usuario escribe una ruta absoluta debe ejecutarse tal cual.
   <img width="791" height="210" alt="image" src="https://github.com/user-attachments/assets/e9a30be8-31bb-40f6-b798-0804a2de505b" />
   
   - Si no es una ruta absoluta, asumir que se llama desde */bin*
   <img width="774" height="166" alt="image" src="https://github.com/user-attachments/assets/24018409-d8fb-4639-826e-314731c16b53" />

   - Manejar errores cuando el ejecutable no exista o no tenga permisos.
   <img width="858" height="218" alt="image" src="https://github.com/user-attachments/assets/771dc832-cc18-4979-8492-fd24cb50bfba" />

   
### Ejecución mediante procesos
   - La invocación debe realizarse con `fork()` y luego `exec()`, desde el proceso hijo.
   - El proceso padre (el intérprete) espera la finalización del hijo con `wait()` / `waitpid()` antes de aceptar el siguiente comando.
  <img width="829" height="189" alt="image" src="https://github.com/user-attachments/assets/b6da378c-95b5-4a2b-9344-50a4e865dfde" />
 
   
### Manejo de Errores
   - Mensajes claros cuando el comando no exista, la ruta no sea válida o exec falle (incluir errno/perror cuando sea útil).
   <img width="970" height="224" alt="image" src="https://github.com/user-attachments/assets/ace8eca3-c431-4d77-b466-4b4fbc04ded8" />

    
### Redirección de salida estándar (>)
   - Si el usuario ingresa: nombrePrograma arg2 > archivo
   - Debe redirigir stdout del proceso hijo a archivo (crearlo/truncarlo). Nada debe mostrarse en pantalla para esa ejecución.
   - Nota: Todos los tokens (incluido >) están separados por espacios.
 <img width="871" height="236" alt="image" src="https://github.com/user-attachments/assets/965fb5ba-b166-4471-85dd-313549f0876a" />
  
### Comando de salida
   - El intérprete termina al ingresar la palabra *salir* o presionar *Ctrl + D*.
   <img width="889" height="219" alt="image" src="https://github.com/user-attachments/assets/24902307-8ad1-41e5-855b-9243c7021c2e" />

   
### Pipes
   - Detectar y ejecutar pipes simples (|): cmd1 | cmd2.
   <img width="985" height="319" alt="image" src="https://github.com/user-attachments/assets/78b59f4e-fd2c-44ba-b192-840457e4b5df" />

   
### Tareas en segundo plano
   - Invocando con ampersand (&), el comando no debe bloquear el prompt, mostrar *waitpid no bloqueante* o recolección diferida.
   <img width="966" height="193" alt="image" src="https://github.com/user-attachments/assets/74c56f63-6f4f-420c-8e6d-36975d1b9693" />

### Redireccionamiento
   - Integrar redirecciones de entrada (<) y salida (>>).
   <img width="871" height="236" alt="image" src="https://github.com/user-attachments/assets/6e11643b-6068-4f9c-affe-56adca1dcbbc" />

### Comandos Internos Built-ins
   - Comandos básicos como cd, pwd
   <img width="1033" height="324" alt="image" src="https://github.com/user-attachments/assets/d43fb674-252d-473a-bfbe-6d3a97c62fce" />

### Concurrencia con hilos
   - Built-in PARALLEL que ejecute n comandos en paralelo con pthread_create, con sincronización.
  <img width="1006" height="273" alt="image" src="https://github.com/user-attachments/assets/470e5534-26b8-40af-83e4-cbbf93a3cd3f" />

   
### Manejo de señales
   - Ignorar o capturar SIGINT en el padre (sin matar la SHELL accidentalmente); pasar señales a hijos cuando corresponda.
 <img width="993" height="232" alt="image" src="https://github.com/user-attachments/assets/ed26beee-b148-40b5-b9ab-12d62ba47f0f" />



