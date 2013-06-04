-- A solution contains projects, and defines the available configurations
solution "CVector"
   configurations { "Debug", "Release" }
   location "build"
 
   -- A project defines one build target
   project "vector"
      location "build"
      kind "ConsoleApp"
      language "C"
      files { "*.h", "*.c" }
      excludes { "vector_template.*", "cvector.h", "vector_tests2.c" }
--      libdirs { }
      links { "cunit" } 
	  targetdir "build"
  
      configuration "Debug"
         defines { "DEBUG" }
         flags { "Symbols" }
 
      configuration "Release"
         defines { "NDEBUG" }
         flags { "Optimize" }
 
      configuration { "linux", "gmake" }
         buildoptions { "-ansi", "-pedantic-errors" }


   project "cvector"
      location "build"
      kind "ConsoleApp"
      language "C"
      files {
      	  "main.c", "vector_tests2.c",
      	  "vector_short.c", "vector_short.h"
      }
    --  excludes { "vector_template.*", "vector_tests.c" }
--      libdirs { }
      links { "cunit" } 
	  targetdir "build"
  
      configuration "Debug"
         defines { "DEBUG" }
         flags { "Symbols" }
 
      configuration "Release"
         defines { "NDEBUG" }
         flags { "Optimize" }
 
      configuration { "linux", "gmake" }
         buildoptions { "-ansi", "-pedantic-errors" }
