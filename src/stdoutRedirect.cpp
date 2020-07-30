#include "stdoutRedirect.h"

#include <fcntl.h>
#include <stdio.h>

static int outfd;
static int errfd;
static int infd;

static fpos_t outpos;
static fpos_t errpos;
static fpos_t inpos;

#if !defined(_WIN32) || defined(__MINGW32) || defined(__MINGW32__)
    #if defined(__MINGW32) || defined(__MINGW32__)
    #define DUP dup_
    #else
    #define DUP dup
    #endif
#include <unistd.h>
void switchStdout(const char *newStream)
{
  fflush(stdout);
  fgetpos(stdout, &outpos);
  outfd = DUP(STDOUT_FILENO);
  freopen(newStream, "w", stdout);
}

void revertStdout()
{
  fflush(stdout);
  dup2(outfd, STDOUT_FILENO);
  close(outfd);
  clearerr(stdout);
  fsetpos(stdout, &outpos);
}

void switchStderr(const char *newStream)
{
  fflush(stderr);
  fgetpos(stderr, &errpos);
  errfd = DUP(STDERR_FILENO);
  freopen(newStream, "w", stderr);
}

void revertStderr()
{
  fflush(stderr);
  dup2(errfd, STDERR_FILENO);
  close(errfd);
  clearerr(stderr);
  fsetpos(stderr, &errpos);
}


void switchStdin(const char *newStream)
{
  fflush(stdin);
  fgetpos(stdin, &inpos);
  infd = DUP(STDIN_FILENO);
  freopen(newStream, "w", stdin);
}


void revertStdin()
{
  fflush(stdin);
  dup2(infd, STDIN_FILENO);
  close(infd);
  clearerr(stdin);
  fsetpos(stdin, &inpos);
}
#else
#include <windows.h>
#include <io.h>

void switchStdout(const char *newStream)
{
    fflush(stdout);
    fgetpos(stdout, &outpos);
    outfd = _dup(_fileno(stdout));
    freopen(newStream, "w", stdout);
}

void revertStdout()
{
    fflush(stdout);
    _dup2(outfd, _fileno(stdout));
    _close(outfd);
    clearerr(stdout);
    fsetpos(stdout, &outpos);
}

void switchStderr(const char *newStream)
{
    fflush(stderr);
    fgetpos(stderr, &errpos);
    errfd = _dup(_fileno(stderr));
    freopen(newStream, "w", stderr);
}

void revertStderr()
{
    fflush(stderr);
    _dup2(errfd, _fileno(stderr));
    _close(errfd);
    clearerr(stderr);
    fsetpos(stderr, &errpos);
}


void switchStdin(const char *newStream)
{
    fflush(stdin);
    fgetpos(stdin, &inpos);
    infd = _dup(_fileno(stdin));
    freopen(newStream, "w", stdin);
}


void revertStdin()
{
    fflush(stdin);
    dup2(infd, _fileno(stdin));
    _close(infd);
    clearerr(stdin);
    fsetpos(stdin, &inpos);
}


#endif


