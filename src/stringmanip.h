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
/**
 * @file stringmanip.h
 * @brief String manipilation function definitions.
 */
#ifndef COMMON_STRINGMANIP_H
#define COMMON_STRINGMANIP_H

#include <vector>
#include <string>
#include <algorithm>

/**
 * Convert a std::string to all lowercase.
 * @param original Target std::string.
 * @return Transformed std::string.
*/
std::string lowerc(const std::string& original);

/**
 * Convert a std::string to all uppercase
 * @param original Target std::string.
 * @return Transformed std::string.
*/
std::string upperc(const std::string& original);

std::vector<std::string> lowerc(const std::vector<std::string>& original);
std::vector<std::string> upperc(const std::vector<std::string>& original);

/**
 * @brief Clean all white space from a std::string.
 * @note This uses the STL whitespace delimiters.
*/
std::string cleanws(const std::string& str);

/**
 * @brief Find and replace every instance a std::string within another std::string.
*/
void replace(std::string& source,const std::string& find,std::string replace);

/**
 * @brief Cut everything to the right of,and including any of the listed delemiters
*/
std::string rcut(const std::string & source , const char& t = '$');

/**
 * @brief Trim delimiters off the right side of a std::string
*/
std::string rtrim(const std::string & source , const std::string & t = " \t\n\r\0\x0B");

/**
 * @brief Trim delimiters off the left side of a std::string
*/
std::string ltrim(const std::string & source , const std::string & t = " \t\n\r\0\x0B");

/**
 * @brief Trim delimiters off both sides of a std::string
*/
std::string trim(const std::string & source , const std::string & t = " \t\n\r\0\x0B");

/**
 * @brief An STL *safe* sprintf function.
*/
int ssprintf(std::string& target, const char* format, ...);

/**
 * @brief Do a *smart* test on whether a std::string should mean true or false
*/
int boolstr(const std::string& value);

/**
 * @todo Needs to be tested.
*/
std::string left(const std::string& value, int size);

#endif
