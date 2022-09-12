# FranquetteGL - C software implementation of OpenGL

FranquetteGL aims to be a usable implementation of modern OpenGL, done in software (as opposed to hardware graphic cards).

**Documentation:** https://j-vernay.github.io/FranquetteGL/html/index.html

Goals include, in order of priority:

- Being educational on the inner workings of an OpenGL implementation
- Well-documented readable code which can be used to reason about OpenGL
- Large-scale project organization
- Standard C with emphasis on inspectability and modularity
- Sufficiently performant for real-time graphics

## Project layout

```sh
README.md       # This file presenting the project.
CMakeLists.txt  # Main script used to coordinate all compilation stages.
.clang-format   # Format options, to ensure homogeneous code formatting.

build/
    # Compilation artifacts will end there
include/
    # Public API for inclusion by external projects
src/
    # Implementation code and details
tests/
    # Different projects used to verify implementation correctness
examples/
    # Projects using the public API, for demonstration purposes
data/
    # Non-code required files when building.
tools/
    # Projects used when building, for example code generation.
docs/
    # Documentation of the project
```