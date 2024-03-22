# Linux Signals

| Signal Name | Signal Value | Description                            | Can be Handled? | Default Action                  | Possible Sender/Source                              |
|-------------|--------------|----------------------------------------|-----------------|---------------------------------|-----------------------------------------------------|
| SIGABRT     | 6            | Abnormal termination                   | Yes             | Terminate process               | Process itself (e.g., abort() call)                 |
| SIGALRM     | 14           | Alarm clock                            | Yes             | Terminate process               | Kernel timer                                        |
| SIGHUP      | 1            | Hangup (terminal disconnect)           | Yes             | Terminate process               | Controlling terminal disconnection                  |
| SIGILL      | 4            | Illegal instruction                    | No              | Terminate process               | Invalid machine code instruction                    |
| SIGINT      | 2            | Interrupt                              | Yes             | Terminate process               | Keyboard Ctrl+c signal                              |
| SIGQUIT     | 3            | Quit                                   | Yes             | Core dump and terminate process | Keyboard Ctrl+\ signal                              |
| SIGSEGV     | 11           | Segmentation violation                 | No              | Terminate process               | Memory access violation                             |
| SIGTERM     | 15           | Termination signal                     | Yes             | Terminate process               | Any process with appropriate permissions            |
| SIGTSTP     | 20           | Terminal stop signal                   | Yes             | Stop process                    | Keyboard Ctrl+z signal                              |
| SIGPIPE     | 13           | Broken pipe                            | Yes             | Terminate process               | Writing to a closed pipe                            |
| SIGXCPU     | 24           | CPU time limit exceeded                | Yes             | Terminate process               | CPU time limit exceeded                             |
| SIGXFSZ     | 26           | File size limit exceeded               | Yes             | Terminate process               | File size limit exceeded                            |
| SIGCHLD     | 17           | Child process terminated or stopped    | Yes             | Ignore                          | Child process upon termination or stop              |
| SIGCONT     | 19           | Continue a stopped process             | No              | None                            | Keyboard Ctrl+q signal                              |
| SIGURG      | 23           | Urgent data available on socket        | Yes             | None                            | Out-of-band data arrival on socket                  |
| SIGWINCH    | 28           | Window resize signal                   | Yes             | None                            | Terminal window size change                         |
| SIGVTALRM   | 27           | Virtual alarm clock (4.2BSD)           | Yes             | Terminate process               | Virtual timer expiration                            |
| SIGPROF     | 24           | Profiling timer expired                | Yes             | Terminate process               | Profiling timer expiration                          |
| SIGPOLL     | 27           | Pollable event occurred                | Yes             | None                            | Varies depending on implementation                  |
| SIGUSR1     | 10           | User-defined signal 1                  | Yes             | Terminate process               | Application-specific                                |
| SIGUSR2     | 12           | User-defined signal 2                  | Yes             | Terminate process               | Application-specific                                |
| SIGPWR      | 30           | Power failure                          | No              | Terminate process               | Kernel upon imminent power failure                  |
| SIGSYS      | 12           | Bad system call                        | No              | Terminate process               | Invalid system call invocation                      |
| SIGLOST     | 16           | File lock lost (unused)                | No              | Terminate process               | File locking error (deprecated)                     |
| SIGSTKFLT   | 10           | Stack fault                            | No              | Terminate process               | Stack overflow or underflow                         |
| SIGTRAP     | 5            | Trace/breakpoint trap                  | Yes             | Terminate process               | Tracepoint or breakpoint activation                 |
| SIGTTIN     | 21           | Terminal input for background process  | Yes             | Stop background process         | Background process attempting to read from terminal |
| SIGTTOU     | 22           | Terminal output for background process | Yes             | Stop background process         | Background process attempting to write to terminal  |
| SIGKILL     | 9            | Kill                                   | No              | Terminate process               | Any process with appropriate permissions            |
| SIGSTOP     | 17           | Stop a process                         | Yes             | Stop process                    | Keyboard Ctrl+z signal                              |
