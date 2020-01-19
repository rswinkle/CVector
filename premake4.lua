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
		
		configuration "Debug"
			defines { "DEBUG" }
			flags { "Symbols" }

		configuration "Release"
			defines { "NDEBUG" }
			flags { "Optimize" }

		configuration { "linux", "gmake" }
			buildoptions { "-ansi", "-pedantic-errors", "-fno-strict-aliasing", "-Wunused-variable", "-Wreturn-type" }

	--  Same as main testing project except not -ansi so I can use POSIX strdup to make sure
	--  the CVEC_STRDUP macro works when defined before cvector.h inclusion
	project "cvec_strdup"
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
		links { "cunit" }
		targetdir "build"

		defines { "USE_POSIX_STRDUP" }
		
		configuration "Debug"
			defines { "DEBUG" }
			flags { "Symbols" }

		configuration "Release"
			defines { "NDEBUG" }
			flags { "Optimize" }

		configuration { "linux", "gmake" }
			buildoptions { "-pedantic-errors", "-fno-strict-aliasing", "-Wunused-variable", "-Wreturn-type" }

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

