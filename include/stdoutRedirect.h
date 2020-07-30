#ifndef STDOUTREDIRECT_H
#define STDOUTREDIRECT_H

void switchStdout(const char *newStream);

void revertStdout();

void switchStderr(const char *newStream);

void revertStderr();

void switchStdin(const char *newStream);

void revertStdin();

#endif // STDOUTREDIRECT_H
