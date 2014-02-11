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
#include <string.h>

#include <string>
#include <iostream>
#include <fstream>

#include <tokenize.h>
#include <process.h>
#include <filesystem.h>
#include <stringmanip.h>

using namespace std;

// from libiberty
#ifndef DMGL_PARAMS
#define DMGL_PARAMS     (1 << 0)        /**< Include function args */
#endif
#ifndef DMGL_ANSI
#define DMGL_ANSI       (1 << 1)        /**< Include const, volatile, etc */
#endif
extern "C" char * cplus_demangle(char const * mangled, int options);

static const string NM_DEFINED = "nm --print-file-name --line-numbers --format=posix --defined-only ";
static const string NM_UNDEFINED = "nm --print-file-name --line-numbers --format=posix --undefined-only ";

/**
 *  Definition of a single object file.
 */
struct Object
{
   string name;
   string path;
   vector<string> externals;
   vector<string> internals;
   vector<Object*> dependencies;
};

/**
 * Definition of a library file.
 */
struct Library
{
   string name;
   vector<Object*> objects;
};

/**
 * List of libraries.
 */
vector<Library> libraries;

/**
 * List of all known objects.
 */
vector<Object*> objects;

template<typename T>
bool ExistsInVector(const vector<T>& v, const T& search)
{
   bool ret = false;
   for (int x = 0; x < v.size() && !ret; x++)
   {
      if (v[x] == search)
      {
         ret = true;
      }
   }
   return ret;
}

/**
 * Parse an nm output line.
 */
void
ParseNm(const string& line, string& libName, string& objName, string& symbol)
{
   libName = "";
   objName = "";
   symbol = "";

   string token;
   int state = 0;
   for (unsigned int x =
#ifdef WIN32
           2
#else
           0
#endif
           ; x < line.size() && state != 3; x++)
   {
      switch (line[x])
      {
      case ':':
         if (state == 0 || state == 2)
         {
            if (state == 0)
            {
               replace(token,string("//"),string("/"));
               objName = Basename(token);
            }

            // grab the symbol off the line
            vector<string> tokens = explode(line.substr(x+2,line.size()-2-x)," \t");

            if (tokens.size() == 0)
            {
               cerr << "Error: Could not parse nm output (1)" << endl;
               exit(1);
               return;
            }

            char* demangled = cplus_demangle(tokens[0].c_str(), DMGL_ANSI | DMGL_PARAMS);

            // if we can't demangle it just use it as is
            if (demangled != 0)
            {
               symbol = demangled;
            }
            else
            {
               symbol = tokens[0];
            }

            delete demangled;
            state = 3;
            break;
         }
      case '[':
         if (state == 0)
         {
            state++;
            replace(token,"//","/");
            libName = Basename(token);
            token = "";
            break;
         }
      case ']':
         if (state == 1)
         {
            state++;
            replace(token,"//","/");
            objName = Basename(token);
            token = "";
            break;
         }
      default:
         token = token + line[x];
      }
   }

   if (state != 3)
   {
      cerr << "Error: Could not parse nm output (3)" << endl;
      exit(1);
   }
}

/**
 *  Locate an object that has a symbol.
 */
Object*
FindSymbolObject(const string& symbol)
{
   for (unsigned int x = 0; x < objects.size(); x++)
   {
      for (unsigned int y = 0; y < objects[x]->internals.size(); y++)
      {
         if (objects[x]->internals[y] == symbol)
         {
            return objects[x];
         }
      }
   }

   return 0;
}

Object*
FindObject(const string& name)
{
   for (unsigned int x = 0; x < objects.size(); x++)
   {
      if (objects[x]->name == name)
      {
         return objects[x];
      }
   }

   return 0;
}

Library*
FindLibrary(const string& name)
{
   for (unsigned int x = 0; x < libraries.size(); x++)
   {
      if (libraries[x].name == name)
      {
         return &libraries[x];
      }
   }

   return 0;
}

/**
 * Generate all the dependencies for all objects.
 */
void
GenDependencies(bool showUndefined)
{
   for (unsigned int x = 1; x < objects.size(); x++)
   {
      for (unsigned int y = 0; y < objects[x]->externals.size(); y++)
      {
         Object* find = FindSymbolObject(objects[x]->externals[y]);
         if (find != 0 && find != objects[x])
         {
            if (!ExistsInVector(objects[x]->dependencies,find))
            {
               objects[x]->dependencies.push_back(find);
            }
         }
         else
         {
            cerr << "symbol not found: " << objects[x]->externals[y] << endl;
            if (showUndefined)
            {
               if (!ExistsInVector(objects[x]->dependencies,objects[0]))
               {
                  objects[x]->dependencies.push_back(objects[0]);
               }
            }
         }
      }
   }
}

/**
 *  Load an object or library file.
 */
void
ReadFile(const string& path)
{
   cout << "Parsing file " << path << " ..." << endl;

   // first parse the defined symbols
   string defined = ReadProcess(NM_DEFINED + path);
   vector<string> lines = explode(defined,"\n");

   Object* object;

   for (unsigned int x = 0; x < lines.size(); x++)
   {
      string libName;
      string objName;
      string symbol;

      ParseNm(lines[x],libName,objName,symbol);

      if ((object = FindObject(objName)) != 0)
      {
         // if the object exists, just add the symbols to it
         if (!ExistsInVector(object->internals,symbol))
         {
            object->internals.push_back(symbol);
         }

         if (libName != "")
         {
            Library* lib = FindLibrary(libName);
            if (lib != 0)
            {
               if (!ExistsInVector(lib->objects,object))
               {
                  lib->objects.push_back(object);
               }
            }
            else
            {
               cout << "Added library " << libName << endl;
               lib = new Library;
               lib->name = libName;
               lib->objects.push_back(object);
               libraries.push_back(*lib);
            }
         }
      }
      else
      {
         // add a new object
         object = new Object();
         object->path = path;
         object->name = objName;
         object->internals.push_back(symbol);
         objects.push_back(object);

         if (libName != "")
         {
            Library* lib = FindLibrary(libName);
            if (lib != 0)
            {
               if (!ExistsInVector(lib->objects,object))
               {
                  lib->objects.push_back(object);
               }
            }
            else
            {
               cout << "Added library " << libName << endl;
               lib = new Library;
               lib->name = libName;
               lib->objects.push_back(object);
               libraries.push_back(*lib);
            }
         }
         else
         {
            if (!ExistsInVector(libraries[0].objects,object))
            {
               libraries[0].objects.push_back(object);
            }
         }

         cout << "Added object " << objName << endl;
      }
   }

   // now parse the undefined symbols
   string undefined = ReadProcess(NM_UNDEFINED + path);
   lines = explode(undefined,"\n");

   for (unsigned int x = 0; x < lines.size(); x++)
   {
      string libName;
      string objName;
      string symbol;

      ParseNm(lines[x],libName,objName,symbol);

      if ((object = FindObject(objName)) != 0)
      {
         // if the object exists, just add the symbols to it
         if (!ExistsInVector(object->externals,symbol))
         {
            object->externals.push_back(symbol);
         }

         if (libName != "")
         {
            Library* lib = FindLibrary(libName);
            if (lib != 0)
            {
               if (!ExistsInVector(lib->objects,object))
               {
                  lib->objects.push_back(object);
               }
            }
            else
            {
               cout << "Added library " << libName << endl;
               lib = new Library;
               lib->name = libName;
               lib->objects.push_back(object);
               libraries.push_back(*lib);
            }
         }
      }
      else
      {
         // add a new object
         object = new Object();
         object->path = path;
         object->name = objName;
         object->externals.push_back(symbol);
         objects.push_back(object);

         if (libName != "")
         {
            Library* lib = FindLibrary(libName);
            if (lib != 0)
            {
               if (!ExistsInVector(lib->objects,object))
               {
                  lib->objects.push_back(object);
               }
            }
            else
            {
               lib = new Library;
               lib->name = libName;
               lib->objects.push_back(object);
               libraries.push_back(*lib);
               cout << "Added library " << libName << endl;
            }
         }
         else
         {
            if (!ExistsInVector(libraries[0].objects,object))
            {
               libraries[0].objects.push_back(object);
            }
         }

         cout << "Added object " << objName << endl;
      }
   }
}

void
LoadDir(const string& path, bool recursive, bool parseObjects, bool parseLibs)
{
   DIR* handle = OpenDirectory(path);
   if (handle != NULL)
   {
      char* filename;
      while ((filename = NextFilename(handle)) != 0)
      {
         string name = filename;
         string filepath = path + "/" + name;
         if (name.size() > 2)
         {
            if (FileType(filepath.c_str()) == FILE_TYPE_DIRECTORY &&
                recursive)
            {
               LoadDir(filepath,recursive,parseObjects,parseLibs);
            }
            else if (((name.substr(name.size()-2,2) == ".o" && parseObjects)
                      ||
                      (name.substr(name.size()-2,2) == ".a" && parseLibs)))
            {
               ReadFile(filepath);
            }
         }
      }

      CloseDirectory(handle);
   }
   else
   {
      cerr << "Error: Unable to open directory " << path << endl;
   }
}

void
WriteDot(ostream& out, bool showUndefined)
{
   out << "digraph G {" << endl;
   out << "   ratio=fill;" << endl;
   out << "   overlap=scale;" /* false */<< endl;
   out << "   nodesep=0.5;" << endl;
   out << "   ranksep=1;" << endl;
   out << "   splines=true;" << endl;
   out << "   fontsize=\"16\";" << endl;
   out << "   node [fontsize=10, style=filled, color=blanchedalmond]" << endl;
   out << endl;

   for (unsigned int x = 1; x < objects.size(); x++)
   {
      for (unsigned int y = 0; y < objects[x]->dependencies.size(); y++)
      {
         out << "   \"" << objects[x]->name << "\" -> \"" << objects[x]->dependencies[y]->name << "\";" << endl;
      }
   }

   cout << endl;

   for (unsigned int y = 0; y < libraries[0].objects.size(); y++)
   {
      out << "   \"" << libraries[0].objects[y]->name << "\";" << endl;
   }

   for (unsigned int x = 1; x < libraries.size(); x++)
   {
      out << "   subgraph \"cluster_" << x << "\" {" << endl;
      out << "      label=\"" << libraries[x].name << "\";" << endl;
      out << "      color=blue;" << endl;
      out << "      bgcolor=grey;" << endl;
      for (unsigned int y = 0; y < libraries[x].objects.size(); y++)
      {
         out << "      \"" << libraries[x].objects[y]->name << "\";" << endl;
      }
      out << "   }" << endl;
   }

   if (showUndefined)
   {
      out << "   \"___undef___\" [label=\"Undefined Symbols\", color=red, bgcolor=pink1];" << endl;
   }

   out << endl << "}" << endl;
}

void
PrintMenu(char* path)
{
   cout << "Generate dependency dot graph from object files." << endl << endl;
   cout << "usage: " << path << " -o OUTPUT [OPTIONS] PATH ..." << endl << endl;
   cout << "-o OUTPUT     Output dot file" << endl;
   cout << "PATH          Path to a directory or file" << endl;
   cout << "-r, -R        Recursive search" << endl;
   cout << "-a            Parse library files (.a)" << endl;
   cout << "-j            Parse object files (.o) (default)" << endl;
   cout << "-u            Graph dependencies on undefined symbols" << endl;
   //cout << "-l, -L        Only show library deps; default shows object deps" << endl;
   cout << "-h, -help     Show this menu" << endl << endl;
   cout << "J.D. Henderson <www.digitalpeer.com>" << endl << endl;
}

int
main(int argv, char** argc)
{
   vector<string> paths;
   bool recursive = false;
   bool parseLibs = false;
   bool parseObjects = true;
   bool showLibDeps = false;
   bool showUndefined = false;
   string output;

   for (unsigned int x = 1; x < argv; x++)
   {
      if (argc[x][0] == '-')
      {
         if (strlen(argc[x]) < 2)
         {
            cerr << "Error: Unknown option: " << argc[x] << endl;
            return 1;
         }

         switch (argc[x][1])
         {
         case 'r':
         case 'R':
            recursive = !recursive;
            break;
         case 'o':
            if (x >= argv-1)
            {
               cerr << "Error: Missing parameter to option -o" << endl;
               return 1;
            }
            output = argc[++x];
            break;
         case 'a':
            parseLibs = !parseLibs;
            break;
         case 'j':
            parseObjects = !parseObjects;
            break;
         case 'l':
         case 'L':
            showLibDeps = !showLibDeps;
            break;
         case 'h':
            PrintMenu(argc[0]);
            return 0;
         case 'u':
            showUndefined = !showUndefined;
            break;
         default:
            cerr << "Error: Unknown option: " << argc[x] << endl;
            PrintMenu(argc[0]);
            return 1;
         }
      }
      else
      {
         paths.push_back(argc[x]);
      }

   }

   if (paths.size() == 0 || output == "")
   {
      if (argv < 2)
      {
         cerr << "Error: Missing required options" << endl;
      }
      PrintMenu(argc[0]);
      return 1;
   }

   // add on the global namespace library
   libraries.push_back(Library());

   // add on the undefined symbol object
   Object* undef = new Object;
   undef->name = "___undef___";
   objects.push_back(undef);

   for (unsigned int x = 0; x < paths.size(); x++)
   {
      if (FileType(paths[x].c_str()) == FILE_TYPE_DIRECTORY)
      {
         LoadDir(paths[x],recursive,parseObjects,parseLibs);
      }
      else if (FileType(paths[x].c_str()) == FILE_TYPE_FILE &&
               ((paths[x].substr(paths[x].size()-2,2) == ".o" && parseObjects)
                ||
                (paths[x].substr(paths[x].size()-2,2) == ".a" && parseLibs)))
      {
         ReadFile(paths[x]);
      }
      else
      {
         cerr << "Error: Invalid path: " << paths[x] << endl;
      }
   }

   GenDependencies(showUndefined);

   // write the output dot file
   ofstream out(output.c_str());
   WriteDot(out,showUndefined);
   out.close();

#if 1
   for (unsigned int x = 0; x < libraries.size(); x++)
   {
      cout << "library: " << libraries[x].name << endl;
      for (unsigned int y = 0; y < libraries[x].objects.size(); y++)
      {
         cout << "     object: " << libraries[x].objects[y]->name << endl;
         for (unsigned int z = 0; z < libraries[x].objects[y]->dependencies.size(); z++)
         {
            cout << "          " << libraries[x].objects[y]->dependencies[z]->name << endl;
         }
      }
      cout << endl;
   }

#endif
   return 0;
}
