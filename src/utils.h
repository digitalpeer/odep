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
#ifndef DCCL_UTILS_H
#define DCCL_UTILS_H

#include <iostream>
#include <string>
#include <vector>

#ifdef DEBUG

/**
 A debug macro for general developer debug print statements.
*/
#define DEBUG_VAL(text) \
    std::cout << "debug: " << __FILE__ << ":" << __LINE__; \
    std::cout << " " << text << std::endl;
/**
 A debug macro for dumping variables with their names.
*/
#define DEBUG_VAR(text) \
    std::cout << "debug: " << __FILE__ << ":" << __LINE__; \
    std::cout << " " << (#text) << "=" << text << std::endl;

#else
#define DEBUG_VAL(text)
#define DEBUG_VAR(text)
#endif

#endif
