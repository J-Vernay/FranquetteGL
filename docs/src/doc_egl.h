
#include <stdint.h>

/// @addtogroup EGL
/// @{

/// In EGL, all rendering state is relative to an EGLDisplay.
/// The state representation is unspecified by EGL : each implementation
/// chooses the content and layout of the underlying structure.
///
/// The conceptual representation of EGL is also unspecified:
/// it may be a device per screen, or per graphics card, or per computer.
/// Khronos recommends to have only one EGLDisplay per system, as it makes
/// sharing resources between two displays difficult or impossible, and most
/// applications do not even support multiple EGLDisplay.
///
/// @FGL
/// Only one EGLDisplay can exist.
typedef void* EGLDisplay;

/// Type representing integer or pointer, used in generic interfaces with multiple possible types.
/// The actual meaning of an EGLAttrib depends on the function.
///
/// A common pattern in EGL is to have an array of EGLAttrib as argument (`EGLAttrib const*`).
/// In these cases, the array represents a sequence of key-value pairs terminated by EGL_NONE:
/// ```c
/// EGLAttrib attribs[] = { keyA, valueA, keyB, valueB, EGL_NONE };
/// ```
/// Keys are usually enum values, and which values are expected depends on the function and key.
///
/// Example with eglDebugMessageControlKHR from EGL_KHR_debug extension:
/// ```c
/// EGLAttrib attribs[] = {
///     EGL_DEBUG_MSG_WARN_KHR, EGL_TRUE,
///     EGL_DEBUG_MSG_INFO_KHR, EGL_FALSE,
///     EGL_NONE };
/// eglDebugMessageControlKHR(callback, attribs);
/// ```
typedef intptr_t EGLAttrib;

/// @}
