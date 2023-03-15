-- A solution contains projects, and defines the available configurations
--
-- TODO DRY a lot.  Too much duplication
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
		}
		links { "cunit" }
		targetdir "build"

		defines { "USE_CVECTOR_H" }
		
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
		}
		links { "cunit" }
		targetdir "build"

		defines { "USE_CVECTOR_H", "USE_POSIX_STRDUP" }
		
		configuration "Debug"
			defines { "DEBUG" }
			flags { "Symbols" }

		configuration "Release"
			defines { "NDEBUG" }
			flags { "Optimize" }

		configuration { "linux", "gmake" }
			buildoptions { "-pedantic-errors", "-fno-strict-aliasing", "-Wunused-variable", "-Wreturn-type" }

	project "cvector_templates"
		location "build"
		kind "ConsoleApp"
		language "C"
		files
		{
			  "main.c",
			  "cvector_tests.c",
			  "cvector_all.c",
			  "cvector_short.h",
			  "cvector_f_struct.h"
		}
		links { "cunit" }
		targetdir "build"

		defines { "USE_TEMPLATES" }
		
		configuration "Debug"
			defines { "DEBUG" }
			flags { "Symbols" }

		configuration "Release"
			defines { "NDEBUG" }
			flags { "Optimize" }

		configuration { "linux", "gmake" }
			buildoptions { "-ansi", "-pedantic-errors", "-fno-strict-aliasing", "-Wunused-variable", "-Wreturn-type" }

	project "cvector_macro"
		location "build"
		kind "ConsoleApp"
		language "C"
		files
		{
			  "main.c",
			  "cvector_tests.c",
			  "cvector_all.c",
			  "cvector_macro.h"
		}
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

