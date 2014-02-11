/**
 * @file process.h
 * @brief Read, write, and manage process function definitions.
*/
#ifndef DCCL_PROCESS_H
#define DCCL_PROCESS_H

#include <string>

/**
 * @brief Open a process and read it like a file.
 *
 * This is a a popen() wrapper without error handling and stderr is still
 *   allowed to be output.
 *
 * @param command The shell command.
 * @return The complete string result of executing that command (except stderr!).
*/
std::string ReadProcess(const std::string& command);

#endif
