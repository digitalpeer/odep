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
#include "process.h"
#include "utils.h"

#include <cstdio>

#ifdef  __cplusplus
extern "C" {
#endif

#include <unistd.h>

#ifdef  __cplusplus
}
#endif

/*
Join stdout and stderr into one so we can get all of their output.
I have been fooling around with and just don't know what the right solution is.
*/
//#define JOIN_STDERR_STDOUT 1

using namespace std;

string
ReadProcess(const string& command)
{
   string answer;
   const int BUFFER_SIZE = 255;
   char buffer[BUFFER_SIZE];
   FILE* process;

   string realCommand = command;

#ifdef JOIN_STDERR_STDOUT
   realCommand = realCommand + " 2>&1";
#endif

   // flush ALL current output streams, otherwise the output from this (mainly stderr)
   // will get all mixed in with the current program output
   fflush(0);

   //open the process
   process = popen(realCommand.c_str(),"r");

   if (process)
   {
      // read all the output
      while (fgets(buffer,BUFFER_SIZE,process))
      {
         answer += buffer;
      }

      // close the process and check the status
      int status = pclose(process);
      if (status != 0)
      {
         DEBUG_VAL("Command '" << command << "' exited with error status: " << status << ".")
      }
   }
   else
   {
      DEBUG_VAL("Unable to open comand: " << command);
   }

   return answer;
}

