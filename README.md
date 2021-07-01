# cplusplusextensions
A new take on c++ extensions with extra flexibility

PREREQUISITES:
A machine that runs Unix or Linux
gcc 8 (gcc 9 or greater preferred but definitely not nessecary)
make (this is how it installs)

## Objective and reasoning:
In C++ there is several different tools at your disposal, but some things,
(like vectors) don't use any type-information, or have many conversions.

Say you are using a std::vector&lt;int&gt; and want to convert it to
std::vector&lt;double&gt;. You have to make a foreach function for converting it,
or a reusable lambda. Even though not difficult, it feels slightly weird that
you as a programmer are responsible for making an automatic conversion

#### This library has many benefits, but there is a catch:
CustomUtils::Vector does not support std::vector, and vice versa. Same with
CustomUtils::String and CustomUtils::Array (although Array might be convertible implicitly.)
This project is designed to have as little dependencies on the parent library as possible.

## Installation, Uninstallation, and test files:
To install, simply run "<i>make install</i>", and to uninstall run "<i>make uninstall</i>"

If all you want to do are tests, then by all means, fill up your clone of this project with
as many .cpp files as you wish, just create the corresponding programs with "<i>make test</i>"
I currently have the project set up so it has to install inside /usr/local, but could fix that in
the future.
