-- A solution contains projects, and defines the available configurations
solution "CVector"
	configurations { "Debug", "Release" }
	location "build"

	project "cvector"
		location "build"
		kind "ConsoleApp"
		language "C"
		files
		{
			  "main.c",
			  "cvector_tests.c",
			  "cvector.h",
			  "cvector_short.h",
			  "cvector_f_struct.h"
		}
		--excludes { "vector_template.*", "vector_tests.c" }
		--libdirs { }
		links { "cunit" }
		targetdir "build"
		
		defines { "TESTING_CVECTOR_H" }

		configuration "Debug"
			defines { "DEBUG" }
			flags { "Symbols" }

		configuration "Release"
			defines { "NDEBUG" }
			flags { "Optimize" }

		configuration { "linux", "gmake" }
			buildoptions { "-ansi", "-pedantic-errors", "-fno-strict-aliasing", "-Wunused-variable", "-Wreturn-type" }

	project "cvectorcpp"
		location "build"
		kind "ConsoleApp"
		language "C++"
		files
		{
			  "main2.cpp",
			  "cvector.h"
		}
		--excludes { "vector_template.*", "vector_tests.c" }
		--libdirs { }
		links { "cunit" }
		targetdir "build"

		configuration "Debug"
			defines { "DEBUG" }
			flags { "Symbols" }

		configuration "Release"
			defines { "NDEBUG" }
			flags { "Optimize" }

		configuration { "linux", "gmake" }
			buildoptions { "-ansi", "-pedantic-errors", "-fno-strict-aliasing", "-Wunused-variable", "-Wreturn-type", "-x c++" }

