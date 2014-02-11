/*
 * odep - Generate Object Dependency Graph
 *
 * Copyright (C) 2003-2005, J.D. Henderson <www.digitalpeer.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#ifndef COMMON_FILESYSTEM_H
#define COMMON_FILESYSTEM_H

#include <string>
#include <vector>

#ifdef  __cplusplus
extern "C" {
#endif

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/dir.h>    // directory reading

#ifdef  __cplusplus
}
#endif

/**
 * File types returned by FileType.
*/
enum
{
   FILE_TYPE_NONE,
   FILE_TYPE_FILE,
   FILE_TYPE_DIRECTORY,
   FILE_TYPE_UNKNOWN
};

/**
 * Converts bytes into a human readable format.
 * @return A string of the formatted size.
*/
std::string HumanFileSize(unsigned int bytes);

/**
 * Convert a file/path to it's absolute path.
*/
std::string AbsPath(const std::string& path);

/**
 * Append a / or \ onto a directory if it doesn't already exist!
*/
std::string Dirize(const std::string& dir);

/**
 * Get the base name of a path.
*/
std::string Basename(const std::string& path);

/**
 * Get the directory of a file path.
*/
std::string Dirname(const std::string& path);

/**
 * Unlink/delete a file.
*/
inline int Unlink(const std::string& path);

/**
 * Check if a file exists.
*/
bool FileExists(const std::string& path);

/**
 * Link a file.
*/
inline int Link(const std::string& o, const std::string& n);

/**
 * Create a directory.
*/
inline int Mkdir(const std::string& path, mode_t mode);

/**
 * Stat a file.
*/
inline int Stat(const std::string& path, struct stat* buf);

/**
 * Return a list of files from a directory.
 * The second parameter expects a function pointer to a function like this:
 *    int dir_filter(const struct direct *entry)
 * which returns a 0 if the file is not to be included and non zero if it is.
*/
std::vector<std::string> ReadDir(const std::string& dir,int(*select)(const struct dirent *));

/**
 * Returns the type of file.
*/
int FileType(const std::string& name);

/**
 * Opens a directory handle.
*/
DIR* OpenDirectory(const std::string& name);

/**
 * Closes a directory handle.
*/
void CloseDirectory(DIR* dir);

/**
 * Returns the next filename on the directory.
*/
char* NextFilename(DIR* dir);


#endif
