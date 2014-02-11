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
#include "stringmanip.h"


#include <sstream>
#include <cstdio>
#include <algorithm>
#include <cctype>
#include <cstdarg>
#include <string.h>

using namespace std;

string
lowerc(const string& original)
{
   string answer = original;
   string::iterator p = answer.begin();
   for (; p != answer.end(); ++p)
   {
      *p = tolower(*p);
   }

   // not ANSI/ISO standard
   //string answer;
   //transform(original.begin(), original.end(),answer.begin(),ToLower());

   return answer;
}

string
upperc(const string& original)
{
   string answer = original;
   string::iterator p = answer.begin();
   for (; p != answer.end(); ++p)
   {
      *p = toupper(*p);
   }

   //not ANSI/ISO standard to use transform like this:
   //transform(original.begin(), original.end(),answer.begin(),ToUpper());

   return answer;
}

vector<string>
lowerc(const vector<string>& original)
{
   vector<string> answer = original;
   for (uint x = 0; x < answer.size(); x++)
   {
      answer[x] = lowerc(answer[x]);
   }
   return answer;
}

vector<string>
upperc(const vector<string>& original)
{
   vector<string> answer = original;
   for (uint x = 0; x < answer.size(); x++)
   {
      answer[x] = upperc(answer[x]);
   }
   return answer;
}

string
cleanws(const string& str)
{
   string answer;
   string buffer;

   if (str == "")
   {
      return answer;
   }

   stringstream ss(str);

   while (ss >> buffer)
   {
      answer += buffer;
   }

   return answer;
}

void
replace(string& source,const string& find,string replace)
{
   if (source.size() == 0 || find.size() == 0)
   {
      return;
   }

   if (source == find)
   {
      source = replace;
      return;
   }

   size_t j;
   for (; (j = source.find(find)) != string::npos;)
   {
      source.replace(j, find.length(),replace);
   }
}

string
rcut(const string & source , const char& t)
{
   string str;
   for (uint x = 0; x < source.size(); x++)
   {
      if (source[x] == t)
      {
         break;
      }
      str += source[x];
   }
   return str;
}

string
rtrim(const string & source , const string & t)
{
   string str = source;
   return str.erase(str.find_last_not_of(t)+1) ;
}

string
ltrim(const string & source , const string & t)
{
   string str = source;
   return str.erase(0 ,source.find_first_not_of(t)) ;
}

string
trim(const string & source , const string & t)
{
   if (!source.size())
   {
      return "";
   }
   string str = source;
   return ltrim(rtrim(str,t),t);
}

int
ssprintf(string& target, const char* format, ...)
{
   const int PADDING = 64;
   // initialize the buffer and the va list.
   int bufLen  = strlen(format) + PADDING;
   char* buffer = new char[bufLen + 1];

   va_list(ap);

   // call the standard C vsnprintf.
   va_start(ap, format);

   // if the initial bufLen is too small, double it and retry
   int retVal;
   while ((retVal = vsnprintf(buffer, bufLen, format, ap)) >= bufLen)
   {
      bufLen *= 2;
      delete [] buffer;
      buffer = new char[bufLen + 1];
   }

   va_end(ap);

   buffer[bufLen] = '\0';

   // Write the result to the target string.
   target = string(buffer);

   delete [] buffer;

   return retVal;
}

int
boolstr(const string& value)
{
   int answer = 0;

   if (lowerc(value) == "no" || lowerc(value) == "false" || value == "0")
   {
      answer = 0;
   }
   else if (lowerc(value) == "yes" || lowerc(value) == "true" || value == "1")
   {
      answer = 1;
   }
   else
   {
      answer = -1;
   }

   return answer;
}

string left(const string& value, int size)
{
   string answer;
   if (value.size() < static_cast<uint>(size))
   {
      answer = value;
   }
   else
   {
      answer = value.substr(0,size);
   }
   return answer;
}

