# Bazel integration

This page describes how to setup MRA for your [bazel](https://bazel.build/) build system.

For general C++ coding examples using MRA: see [here](INTEGRATION.md#Coding).

The following chapters describe how to make `@MRA` available in your repository's `WORKSPACE` configuration.
1. [use only](#Use-only): no (explicit) git checkout, just download and use the code.
2. [use & develop via subrepo](#Use--develop-via-subrepo): setup a git sub-repository.
3. [use & develop via standalone repo](#Use--develop-via-standalone-repo): refer to a git repository checkout elsewhere on your system.

The [BUILD](#BUILD-files) chapter describes how to depend on bazel targets defined by MRA repo.

## Use only

Add something like the following to your `WORKSPACE` file, typically while updating to latest git hash.
```
load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

git_repository(
    name = "MRA",
    remote = "https://github.com/janfeitsma/MRA-prototype.git",
    commit = "f6dc7daabc7b6738c1bb4505755bd0571593b34c",
    shallow_since = "1682687444 +0200",
)
```

If not done already, you might also have to setup [protobuf](#protobuf).

## Use & develop via subrepo

Not yet attempted. (Why would we, standalone repo use case is trivial.)

## Use & develop via standalone repo

Add something like the following to your `WORKSPACE` file.
```
new_local_repository(
    name = "MRA",
    path = "/some-path-to/MRA-prototype",
    build_file_content = "",
)
```

## BUILD files

Example dependency to all common datatypes:
```
deps = [
    "@MRA//datatypes:MRA_cc_proto",
],
```

Example dependency to a specific component:
```
deps = [
    "@MRA//components/falcons/getball_fetch:implementation",
],
```

## Protobuf

To setup MRA protobuf dependency in your `WORKSPACE`:

```
# Google protobuf (for MRA)
http_archive(
    name = "com_google_protobuf",
    sha256 = "25680843adf0c3302648d35f744e38cc3b6b05a6c77a927de5aea3e1c2e36106",
    strip_prefix = "protobuf-3.19.4",
    urls = ["https://github.com/protocolbuffers/protobuf/archive/refs/tags/v3.19.4.zip"],
)

load("@com_google_protobuf//:protobuf_deps.bzl", "protobuf_deps")
protobuf_deps()
```

<span style="color:red">**TODO**</span>: would be nice to omit protobuf in WORKSPACE, instead let MRA configure/export it, then cleanup this chapter?

