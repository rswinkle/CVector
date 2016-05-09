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
		excludes
		{
			"vector_template*", "cvector.h", "vector_tests2.c"
		}
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
			buildoptions { "-ansi", "-pedantic-errors", "-fno-strict-aliasing", "-Wunused-variable", "-Wreturn-type" }


	project "cvector"
		location "build"
		kind "ConsoleApp"
		language "C"
		files
		{
			  "main.c",
			  "vector_tests.c",
			  "cvector.h",
			  "vector_short.h",
			  "vector_f_struct.h"
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


	project "vectorcpp"
		location "build"
		kind "ConsoleApp"
		language "C++"
		files
		{
			"main.cpp",
			"vector_i.c",
			"vector_d.c",
			"vector_str.c",
			"vector_void.c",
			"vector_f_struct.h",
			"vector_short.h"
		}
		--excludes { }
		--libdirs { }
		--links { "cunit" }
		targetdir "build"

		configuration "Debug"
			defines { "DEBUG" }
			flags { "Symbols" }

		configuration "Release"
			defines { "NDEBUG" }
			flags { "Optimize" }

		configuration { "linux", "gmake" }
			buildoptions { "-ansi", "-pedantic-errors", "-fno-strict-aliasing", "-Wunused-variable", "-Wreturn-type", "-x c++" }


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

