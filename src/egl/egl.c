/// @file egl.c
/// @author Julien Vernay, 2022
/// @copyright Under GNU General Public License v3 or later.
/// @brief Implementation of main EGL functions.

#include "EGL/egl.h"
#include <FranquetteGL/EGL/fegl_errors.h>

/// Only one EGLDisplay can exist in FranquetteGL.
static const EGLDisplay _fegl_ONLY_DISPLAY = (EGLDisplay)0xABCDEF;

/// @addtogroup EGL
/// @sa - Specification EGL 1.5: https://registry.khronos.org/EGL/specs/eglspec.1.5.pdf
/// @{

/// Returns an EGLDisplay from a platform-specific display.
/// When calling it multiple times with same arguments, the same EGLDisplay is returned.
/// @param platform Which platform is associated to @a native_display .
/// The set of supported platforms can be retrieved by calling
/// `eglQueryString(EGL_NO_DISPLAY, EGL_EXTENSIONS)`.
/// @param native_display Pointer to a platform-specific native object
/// @param attrib_list Extra arguments required by the platform.
/// Either NULL (empty) or it must point to an array of EGLAttrib,
/// which must be terminated by EGL_NONE .
/// @retval "Valid EGLDisplay" On success.
/// @retval EGL_NO_DISPLAY On failure.
///
/// @FGL
/// - @a platform must be EGL_NONE.
/// - @a native_display must be NULL.
/// - @a attrib_list must be empty.
/// - All valid calls will return the same EGLDisplay.
///
/// @EGL
/// - 1.5 Complete
EGLDisplay eglGetPlatformDisplay(EGLenum platform, void* native_display, EGLAttrib const* attrib_list)
{
    if (platform != EGL_NONE || native_display != NULL) {
        fegl_Error(EGL_BAD_PARAMETER, __func__, NULL, NULL);
        return EGL_NO_DISPLAY;
    }

    if (attrib_list != NULL && *attrib_list != EGL_NONE) {
        fegl_Error(EGL_BAD_ATTRIBUTE, __func__, NULL, NULL);
        return EGL_NO_DISPLAY;
    }

    fegl_Success();
    return _fegl_ONLY_DISPLAY;
}

/// Returns an EGLDisplay from an implementation-specific ID.
/// When calling it multiple times with same ID, the same EGLDisplay is returned.
/// @param display_id Implementation-specific value, which may be EGL_DEFAULT_DISPLAY.
/// @retval "Valid EGLDisplay" On success.
/// @retval EGL_NO_DISPLAY On failure.
///
/// @FGL
/// - @a display_id must be EGL_DEFAULT_DISPLAY.
/// - All valid calls will return the same EGLDisplay.
EGLDisplay eglGetDisplay(EGLNativeDisplayType display_id)
{
    if (display_id != EGL_DEFAULT_DISPLAY) {
        fegl_Success();
        return EGL_NO_DISPLAY;
    }
    fegl_Success();
    return _fegl_ONLY_DISPLAY;
}

/// @todo Not implemented
char const* eglQueryString(EGLDisplay dpy, EGLint name)
{
    fegl_Error(EGL_NOT_INITIALIZED, __func__, NULL, "Not implemented");
    return NULL;
}
/// @}