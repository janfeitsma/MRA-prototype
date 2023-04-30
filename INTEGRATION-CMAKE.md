# CMake

*Disclaimer: this page and the code is a hacky mess. Expert input and pull requests would be appreciated. 
Author (Jan) does not want to come back and learn cmake, after having migrated away from cmake to bazel.*

This page tries to describe how to setup MRA for your CMake build system.

For general C++ coding examples using MRA: see [here](INTEGRATION.md#Coding).

The following chapters describe how to make `@MRA` available in your repository's CMake configuration.
1. [use only](#Use-only): no (explicit) git checkout, just download and use the code.
2. [use & develop via subrepo](#Use--develop-via-subrepo): setup a git sub-repository.
3. [use & develop via standalone repo](#Use--develop-via-standalone-repo): refer to a git repository checkout elsewhere on your system.

The [CMakeLists.txt](#cmakeliststxt) chapter describes how to depend on targets defined by MRA repo.

## Use only

TODO

Something with `FetchContent`??

## Use & develop via subrepo

Make a checkout at `some-path-to/MRA-prototype` and let CMake build it via `add_subdirectory`.

Put something like the following into a file like `cmake/MRAConfig.cmake` such that CMake can find all required things.

```
cmake_minimum_required(VERSION 3.5.1)

set(MRA_DIR some-path-to/MRA-prototype)
message("-- Found MRA (at ${MRA_DIR})")

# TODO: let MRA repo export below stuff somehow ... it's not nice to have the internals of MRA exposed here ...

include_directories(${MRA_DIR}) # for including external component headers
include_directories(${MRA_DIR}/base) # for including internal headers

set(MRA_BUILD_DIR build/some-path-to/MRA-prototype)
include_directories(${MRA_BUILD_DIR}) # for generated protobuf headers 

# dependency: json
include(FetchContent)
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)
include_directories(build/_deps/json-src/include/) # hack, dont know why it errors otherwise..

#link_directories(${MRA_BUILD_DIR})
```

## Use & develop via standalone repo

TODO

## CMakeLists.txt

To make MRA targets available in CMakeLists.txt, use
```
find_package(MRA)
```

Then, you can link against targets like `MRA-components` and `MRA-datatypes`, provided the dependency to MRA has been configured.

