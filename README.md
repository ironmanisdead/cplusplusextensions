# cplusplusextensions
A new take on c++ extensions with extra flexibility

PREREQUISITES:
A machine that runs Unix or Linux
bash (version 4.4 or higher, however I have not tested with lower versions)
gcc 10 or higher
make (must be capable of supporting global export statements)
sha256

## Objective and reasoning:
In C++ there is several different tools at your disposal, but some things,
(like vectors) don't use any type-information, or have many conversions.

Say you are using a std::vector&lt;int&gt; and want to convert it to
std::vector&lt;double&gt;. You have to make a foreach function for converting it,
or a reusable lambda. Even though not difficult, it feels slightly weird that
you as a programmer are responsible for making an automatic conversion

#### This library has many benefits, but there is a catch:
CPPExtensions::Vector does not support std::vector, and vice versa. Same with
CPPExtensions::String and CPPExtensions::Array (although Array might be convertible implicitly.)
This project is designed to have as little dependencies on the parent library as possible.

## Installation, Uninstallation, and test files:
To install, simply run "<i>make install</i>", and to uninstall run "<i>make uninstall</i>"
reinstallation is simple, as uninstalls aren't typically required. 
Just download an updated version, and run "<i>make install</i>" like during initial installation

If all you want to do are tests, then by all means, fill up your clone of this project with
as many .cpp files as you wish, just create the corresponding programs with "<i>make tests</i>"
The project installation directory can be modified by setting <b>installdir</b> in the make parameter list,
or the environmental variables

## Notice:
This project is not yet completely stable, but it is deprecating features rather than simply replacing or
removing them. This doesn't yet mean that all types from my library are 100% safe.
Just don't use too many types from the Utils namespace, if at all possible.
At the moment, my project is mostly stable.
