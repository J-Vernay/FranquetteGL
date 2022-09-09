# Public headers {#headers}

All public headers from FranquetteGL can be found in the `include` directory.
This documentation helps with which headers to include in your own projects.

At its core, FranquetteGL is a software-rendering library, independent of any standard.
Multiple standard APIs (OpenGL, OpenGL ES, EGL) are implemented on top of this core.
You can either choose to use directly the FranquetteGL API, or to use the standard headers
which will make easier to change rendering implementation if needed.
Note that the standard headers have redundancy, as most functions are found in multiple
OpenGL versions and in OpenGL ES. So by including the headers corresponding to a given API,
the entire API is available and you need not to keep track of which version has a specific feature.

The official documentation on which headers are required for each API can be found
on the Khronos Group website:

- [EGL (context creation)](https://registry.khronos.org/EGL/)
- [OpenGL (desktop rendering)](https://registry.khronos.org/OpenGL/index_gl.php)
- [OpenGL ES (embedded rendering)](https://registry.khronos.org/OpenGL/index_es.php)

For convenience, here is a summary of the headers roles.
Note that the `xxxplatform.h` headers are not meant to be included directly,
but to provide platform-independent fundations for the other headers.


## EGL (context creation)

EGL is the Khronos API to create contexts which are then used for OpenGL and OpenGL ES.

- `EGL/egl.h` contains the EGL 1.5 specification. It contains all standard APIs for EGL.
- `EGL/eglext.h` contains the extensions, which may or may not be available depending on your system.

`egl.h` is always needed, and you will require `eglext.h` for extensions.

## OpenGL (desktop rendering)

OpenGL is the Khronos API to render geometric figures into discrete pixels.

- `GL/glext.h` contains the entire OpenGL specification, and all registered extensions which may
  or may not be available depending on your system.
- `GL/glcorearb.h` contains the entire OpenGL specification and the registered extensions, but
  the deprecated functions since OpenGL 3 (glBegin() and such) are removed.

You should choose only one of them. When in doubt, `glcorearb.h` should be preferred for learning 
purposes as it exposes an API closer to the inner workings and performances of a graphics card.

## OpenGL ES (embedded rendering)

OpenGL ES is the Khronos API to render geometric figures into discrete pixels on limited hardware
(game consoles, phones and such). Headers depend on the API version.

For the main OpenGL ES specification, use one of these:

- `GLES3/gl32.h` for the 3.2 version
- `GLES3/gl31.h` for the 3.1 version
- `GLES3/gl3.h` for the 3.0 version
- `GLES2/gl2.h` for the 2.0 version
- `GLES/gl.h` for the 1.1 version and older

For the extensions, use one of these:

- `GLES2/gl2ext.h` for versions 2.0 and newer (including 3.x)
- `GLES/glext.h` for versions 1.1 and older

## FranquetteGL (core implementation)

These headers are not standard and they expose the inner workings of the FranquetteGL core.
Each individual header exposes one specific part of the API, and this documentation
should help you know which headers are required for which features.

- `FranquetteGL/fgl.h` contains the entire FranquetteGL public API
  (by including transitively all other FranquetteGL headers)

