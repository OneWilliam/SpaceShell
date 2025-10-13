#pragma once
#include <csignal>

/* Bandera global para comunicación segura desde 'handles' (o manejadores) de señales.
 *
 * Esta variable es de tipo 'volatile sig_atomic_t' para garantizar
 * que sea segura para ser escrita por un manejador de señales y leída por el
 * bucle principal sin problemas de optimización del compilador o condiciones de carrera.
 * Almacena el número de la señal recibida (ej. SIGINT).
 */
extern volatile sig_atomic_t g_signal_recv;

/**
 * Clase de utilidad estática para configurar handles de señales de la shell.
 */
class Signals {
 public:
  /*
   * Debe llamarse una sola vez al inicio de la ejecución de la shell.
   */
  static void setup_handlers();

 private:
  /**
   * Handle para la señal SIGCHLD.
   *
   * Se activa cuando un proceso hijo cambia de estado (termina).
   * @param signum El número de la señal recibida (SIGCHLD).
   */
  static void handle_sigchld(int signum);

  /**
   * Handle para la señal SIGINT (Ctrl+C).
   *
   * Levanta la bandera global 'g_signal_recv' para notificar al bucle
   * principal que la entrada del usuario fue interrumpida.
   * @param signum El número de la señal recibida (SIGINT).
   */
  static void handle_sigint(int signum);
};
