# Naming conventions {#naming}

FranquetteGL includes multiple parts and interfaces. Naming conventions are used to distinguish
between the different parts of FranquetteGL.

## Khronos Naming

OpenGL, OpenGL ES and EGL all uses the following prefixes:
- `EGL_xxx`, `GL_xxx` for enumeration values.
- `EGLxxx`, `GLxxx` for type names.
- `eglxxx`, `glxxx` for function names.

Additionally, the following extension names have special meaning:
- `KHR`, `ARB`: a Khronos-approved extension, likely to enter later versions.
- `EXT`: generic extension, which may be implemented by multiple vendors.

## FranquetteGL Naming

The naming conventions for FranquetteGL API reflect multiple aspects:

|            FranqueteGL Naming          ||
|:--------------------------------------:||
|                Visibility              ||
|   Public identifiers   | `fgl_public`   |
|  Private identifiers   | `_fgl_private` |
|                   Kind                 ||
|         Types          | `FGL_Type`     |
|   Enumeration values   | `fgl_CONSTANT` |
|       Functions        | `fgl_Function` |
|    Global variables    | `fgl_global`   |
|                  Module                ||
|   Core FranquetteGL    | `FGL`, `fgl`   |
|    Helpers for EGL     | `FEGL`, `fegl` |
|   Helpers for OpenGL   | `FOGL`, `fogl` |
|  Helpers for OpenGL ES | `FOES`, `foes` |