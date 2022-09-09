
# Entire code pipeline

- Get OpenGL XML
- Parse OpenGL XML and FGL headers to know what is missing and potential errors.
  Maybe complete documentation if not written? Maybe special documentation?
- Compile FGL
- Doxygen FGL
- Compile tests
- Run tests
- Compile demo
- Let user check demos


build/
    CMake artifacts

include/
    franquettegl/
        Public headers

src/
    Implementation code and details

tests/
    unit/
    rationales/
    integration/
    Rationales, system tests, unit tests

examples/
    Entire applications based on FranquetteGL

data/
    gl.xml
    implemented.md

tools/
    gen-api/
        Generation of headers according to gl.xml and implemented.md

docs/
    Doxyfile
    doxygen-awesome.css
    html/
        Doxygen result

README.md
CMakeLists.txt