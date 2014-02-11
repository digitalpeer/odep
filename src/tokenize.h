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
 * @file tokenize.h
 * @brief Tokenization function definitions.
*/
#ifndef COMMON_TOKENIZE_H
#define COMMON_TOKENIZE_H

#include <string>
#include <vector>

/**
 * @brief Tokenize a string into a string vector based on given delimeters.
 * @note Excludes all delimiters from output.
*/
std::vector<std::string> explode(const std::string& str,  const std::string& delimiters = " \t\n\r\0\x0B");

/**
 * @brief Reassemble tokens with delimiter *only* between them.
*/
std::string implode(const std::string& delimiter,const std::vector<std::string>& tokens);


#endif
