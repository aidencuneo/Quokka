// Signal Handlers

// List retrieved from http://www.yolinux.com/TUTORIALS/C++Signals.html

// 1
void sighupHandler()
{
    error("Received Hangup Signal", line_num);
}

// 2
void sigintHandler()
{
    // The following can be uncommented if the "conio.h" header file is in use: //

    // If Keyboard Interrupt is caught during an input() call
    // on a linux-based machine, call tcsetattr using OLDATTR
    /*
    #ifndef _WIN32
        tcsetattr(STDIN_FILENO, TCSANOW, &OLDATTR);
    #endif
    */

    error("Keyboard Interrupt", line_num);
}

// 3
void sigquitHandler()
{
    error("Received Quit Signal", line_num);
}

// 4
void sigillHandler()
{
    error("Received Illegal Instruction Signal", line_num);
}

// 5
void sigtrapHandler()
{
    error("Received Trace Trap Signal", line_num);
}

// 6
void sigabrtHandler()
{
    error("Aborted", line_num);
}

// 7
void sigbusHandler()
{
    error("Bus Error (invalid address accessed)", line_num);
}

// 8
void sigfpeHandler()
{
    error("Floating Point Arithmetic Exception", line_num);
}

// 11
void sigsegvHandler()
{
    error("Segmentation Fault (I/O operation outside of memory bounds)", line_num);
}

// 13
void sigpipeHandler()
{
    error("Broken Pipe", line_num);
}

// 15
void sigtermHandler()
{
    error("Terminated", line_num);
}

// 16
void sigstkfltHandler()
{
    error("Stack Fault", line_num);
}

// 17
void sigtstpHandler()
{
    cleanupAll();
    exit(1);
}

// 24
void sigxcpuHandler()
{
    error("CPU Limit Exceeded", line_num);
}

// 25
void sigxfszHandler()
{
    error("File Size Limit Exceeded", line_num);
}

// 30
void sigpwrHandler()
{
    error("Power Failure Restart", line_num);
}

// 31
void sigsysHandler()
{
    error("Bad System Call", line_num);
}
