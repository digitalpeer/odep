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
#include "filesystem.h"
#include "process.h"
#include "stringmanip.h"
#include "utils.h"

#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <string.h>
#include <iostream>

#ifdef  __cplusplus
extern "C" {
#endif

#include <libgen.h> //used for stripping filenames or directories from a path
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>

#ifdef  __cplusplus
}
#endif

using namespace std;

string
HumanFileSize(unsigned int bytes)
{
   string answer;
   if (bytes > 1024)
   {
      if (bytes > 1024*1024)
      {
         if (bytes > 1024*1024*1024)
         {
            ssprintf(answer,"%.2fGb",(double)(bytes)/1024/1024/1024);
         }
         else
         {
            ssprintf(answer,"%.2fMb",(double)(bytes)/1024/1024);
         }
      }
      else
      {
         ssprintf(answer,"%.2fKb",(double)(bytes)/1024);
      }
   }
   else
   {
      ssprintf(answer,"%.2fb",(double)(bytes));
   }

   return answer;
}

string
AbsPath(const string& path)
{
   string answer;
   if (path.size() > 2)
   {
      char temp[path.size()+1];
      char temp2[path.size()+1];
      strcpy(temp,path.c_str());
      string file = ::basename(temp);
      realpath(::dirname(temp),temp2);
      string dir = temp2;
      answer  = dir + "/" + file;
   }
   return answer;
}

string
Dirize(const string& dir)
{
   string answer = dir;
   if (answer.size())
   {
      if (answer[answer.size()-1] != '/')
      {
         answer += "/";
      }
   }
   return answer;
}

string
Basename(const string& path)
{
   char temp[path.size()+1];
   strcpy(temp,path.c_str());
   string base = ::basename(temp);
   return base;
}

string
Dirname(const string& path)
{
   char temp[path.size()+1];
   strcpy(temp,path.c_str());
   string base = ::dirname(temp);
   base += "/";
   return base;
}

inline
int
Unlink(const string& path)
{
   return ::unlink(path.c_str());
}

bool
FileExists(const string& path)
{
   struct stat sbuf;
   // exists && is a regular file!
   return ::stat(path.c_str(), &sbuf) == 0 && S_ISREG(sbuf.st_mode);
}

inline
int
Link(const string& o, const string& n)
{
   return ::link(o.c_str(), n.c_str());
}

inline
int
Mkdir(const string& path, mode_t mode)
{
   return ::mkdir(path.c_str(), mode);
}

inline
int
Stat(const string& path, struct stat* buf)
{
   return ::stat(path.c_str(), buf);
}

vector<string>
ReadDir(const string& dir, int(*select)(const struct dirent *))
{
   vector<string> listing;

   struct direct **files;

   int nfiles = scandir(dir.c_str(), &files, select, alphasort);

   if (nfiles < 0)
   {
      cerr << "Error reading directory " << dir.c_str() << endl;
   }

   for (int x = 0; x < nfiles; x++)
   {
      listing.push_back(files[x]->d_name);
   }

   return listing;
}

int
FileType(const string& name)
{
   struct stat statbuf;

   if (Stat(name, &statbuf) < 0)
   {
      if (errno != ENOENT)
      {
         cerr << "Failed to stat file " << name.c_str() << endl;
      }
      return FILE_TYPE_NONE;
   }

   if (S_ISREG(statbuf.st_mode))
   {
      return FILE_TYPE_FILE;
   }

   if (S_ISDIR(statbuf.st_mode))
   {
      return FILE_TYPE_DIRECTORY;
   }

   return FILE_TYPE_UNKNOWN;
}

DIR*
OpenDirectory(const string& name)
{
   DIR* dir;

   dir = opendir(name.c_str());
   if (!dir)
   {
      return NULL;
   }

   return dir;
}

void
CloseDirectory(DIR* dir)
{
   closedir(dir);
}

char*
NextFilename(DIR* dir)
{
   struct dirent* entry;

   do
   {
      entry = readdir(dir);
      if (entry == NULL)
      {
         return NULL;
      }
   }
   while ((entry->d_name[0] == '.' &&
           (entry->d_name[1] == '\0'
            ||
            (entry->d_name[1] == '.' && entry->d_name[2] == '\0'))));

   return entry->d_name;
}
