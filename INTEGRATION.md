# Integration

There are several ways of integrating MRA into your repository, depending on which build system you use, and what use cases you anticipate.

Chapter [Coding](#Coding) shows examples of how to make use of MRA in your C++ source code.

To get the code to compile, first consider which build system you use. 
1. **bazel**: integration with [bazel](https://bazel.build/) is easy, see [here](INTEGRATION-BAZEL.md).
2. **cmake**: integration with cmake is possible (but currently not straightforward!), see [here](INTEGRATION-CMAKE.md).
3. **other**: not supported - why not contribute, or make an issue/request?

Then, consider whether it is enough to just ***use*** MRA, or also allow **developing** (git checkout needed).
1. **use only**: no explicit) git checkout, just download and use the code.
2. **use & develop via subrepo**: setup a git sub-repository.
3. **use & develop via standalone repo**: refer to a git repository checkout elsewhere on your system.

The use cases are worked out on the detail pages for [bazel](INTEGRATION-BAZEL.md) resp. [cmake](INTEGRATION-CMAKE.md).

## Coding

### Includes

Example common datatype (protobuf-generated headers):
```
#include "datatypes/WorldState.pb.h"
```

Example component definition:
```
#include "components/falcons/getball_fetch/FalconsGetballFetch.hpp"
```

<span style="color:red">**TODO**</span>: maybe it is nicer to prefix with `MRA/`, similar to `boost` and `opencv` includes?
<span style="color:red">**TODO**</span>: maybe it is nicer to allow omitting `components/falcons/getball_fetch/`?

### Source code

Example implementation to make use of a component. See also: protobuf API.
```
// create protobuf data types
MRA::FalconsGetballFetch::InputType input; // input data, type generated from Input.proto
MRA::FalconsGetballFetch::ParamsType params; // configuration parameters, type generated from Params.proto
MRA::FalconsGetballFetch::OutputType output; // output data, type generated from Output.proto

// fill parameters
params.set_ballspeedthreshold(0.3); // protobuf generates a setter for each defined scalar parameter in Params.proto

// fill input worldstate, assuming the worldModel function produces a protobuf WorldState object
*(input.mutable_worldstate()) = myWorldModel->MRA_WorldState();

// call MRA implementation
int error_value = MRA::FalconsGetballFetch::FalconsGetballFetch().tick(
    input,
    params,
    output
);
```

