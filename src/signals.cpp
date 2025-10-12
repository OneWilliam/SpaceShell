#include "signals.hpp"
#include <iostream>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <cstring>

volatile sig_atomic_t g_signal_recv = 0;

void Signals::handle_sigchld(int signum) {
    int saved_errno = errno;
    while (waitpid(-1, nullptr, WNOHANG) > 0) {
        // TODO: Implementación de salida de  finalización de procesos child...
    }
    errno = saved_errno;
}

void Signals::handle_sigint(int signum) {
    g_signal_recv = signum;
}


void Signals::setup_handlers() {
    struct sigaction sa_chld, sa_int;

    sa_chld.sa_handler = &Signals::handle_sigchld;
    sigemptyset(&sa_chld.sa_mask);
    sa_chld.sa_flags = SA_RESTART | SA_NOCLDSTOP;
    if (sigaction(SIGCHLD, &sa_chld, 0) == -1) {
        perror("[SHELL] Error al configurar SIGCHLD");
    }

    sa_int.sa_handler = &Signals::handle_sigint;
    sigemptyset(&sa_int.sa_mask);
    sa_int.sa_flags = 0;
    if (sigaction(SIGINT, &sa_int, 0) == -1) {
        perror("[SHELL] Error al configurar SIGINT");
    }
}
