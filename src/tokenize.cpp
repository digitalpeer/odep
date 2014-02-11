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
#include "tokenize.h"

#include <algorithm>

using namespace std;

vector<string>
explode(const string& str,const string& delimiters)
{
   vector<string> tokens;
   // skip delimiters at beginning.
   string::size_type lastPos = str.find_first_not_of(delimiters, 0);
   // find first "non-delimiter".
   string::size_type pos = str.find_first_of(delimiters, lastPos);

   while (string::npos != pos || string::npos != lastPos)
   {
      // found a token, add it to the vector.
      tokens.push_back(str.substr(lastPos, pos - lastPos));
      // skip delimiters.  Note the "not_of"
      lastPos = str.find_first_not_of(delimiters, pos);
      // find next "non-delimiter"
      pos = str.find_first_of(delimiters, lastPos);
   }

   return tokens;
}

std::string implode(const std::string& delimiter,const std::vector<std::string>& tokens)
{
   const unsigned int SIZE = tokens.size();
   std::string answer;
   for (unsigned int x = 0; x < SIZE; ++x)
   {
      answer += tokens[x];

      if (x != SIZE-1)
      {
         answer += delimiter;
      }
   }
   return answer;
}

