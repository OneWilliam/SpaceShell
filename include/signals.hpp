#pragma once
#include <csignal>

extern volatile sig_atomic_t g_signal_recv;

class Signals {

  public:
    
    static void setup_handlers();

  private:

    static void handle_sigchld(int signum);
    static void handle_sigint(int signum);
};
