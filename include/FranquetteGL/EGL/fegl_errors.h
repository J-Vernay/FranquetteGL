/// @file fegl_errors.h
/// @author Julien Vernay, 2022
/// @copyright Under GNU General Public License v3 or later.
/// @brief Error handling API for EGL.

#ifndef FRANQUETTEGL_EGL_ERRORS_H
#define FRANQUETTEGL_EGL_ERRORS_H

#define EGL_EGLEXT_PROTOTYPES
#include "../../EGL/egl.h"
#include "../../EGL/eglext.h"
#include "../fgl_api.h"

/// @name Related official EGL interface
/// @{

EGLint eglDebugMessageControlKHR(EGLDEBUGPROCKHR callback, EGLAttrib const* attrib_list);
EGLBoolean eglQueryDebugKHR(EGLint attribute, EGLAttrib* value);
EGLint eglLabelObjectKHR(EGLDisplay display, EGLenum objectType, EGLObjectKHR object, EGLLabelKHR label);
EGLint eglGetError();

/// @}

/// @name Global EGL status modification
/// In EGL, each thread stores the status of the last EGL function call.
/// Additionally, in EGL_KHR_debug extension, the user can register a callback to be called on certain events.
/// Using these functions ensures that everything is taken care of.
/// @{

/// Sets the global EGL status as EGL_SUCCESS. Inlined to make the correct path faster.
inline void fegl_Success()
{
    extern fgl_THREAD_LOCAL EGLint _fegl_lastStatus;
    _fegl_lastStatus = EGL_SUCCESS;
}

/// To be called to notify the user that the EGL state has changed.
/// @param command Name of the function emitting the message
/// @param objectLabel Label associated with the EGL object, or NULL.
/// @param message Short explanatory string, or NULL.
void fegl_Info(char const* command, EGLLabelKHR objectLabel, char const* message);

/// To be called on command success to indicate that the logic should be replaced because it is
/// deprecated or inefficient.
/// @param command Name of the function emitting the message
/// @param objectLabel Label associated with the EGL object, or NULL.
/// @param message Short explanatory string, or NULL.
void fegl_Warn(char const* command, EGLLabelKHR objectLabel, char const* message);

/// To be called on command failure. The message type (CRITICAL or ERROR) is determined with @a error.
/// @param error Status code, which determines the severity.
/// @param command Name of the function emitting the message
/// @param objectLabel Label associated with the EGL object, or NULL.
/// @param message Short explanatory string, or NULL.
void fegl_Error(EGLenum error, char const* command, EGLLabelKHR objectLabel, char const* message);

/// Bitset representing the interest of user in the different message types.
enum FEGL_DebugTypeMask {
    fegl_DEBUG_CRITICAL_BIT = 1, ///< Internal EGL failures (BAD_ALLOC, CONTEXT_LOST)
    fegl_DEBUG_ERROR_BIT = 2,    ///< Input and bad match errors (BAD_PARAMETER...)
    fegl_DEBUG_WARN_BIT = 4,     ///< Warnings, deprecated or inefficient operation
    fegl_DEBUG_INFO_BIT = 8,     ///< Verbose, object creation or change in state
};

/// Conversion from EGL enumeration values to FEGL_debugMask. Returns 0 on unknown value.
///
/// Possible values:
/// - EGL_DEBUG_MSG_CRITICAL_KHR
/// - EGL_DEBUG_MSG_ERROR_KHR
/// - EGL_DEBUG_MSG_WARN_KHR
/// - EGL_DEBUG_MSG_INFO_KHR
inline EGLint fegl_GetDebugTypeMask(EGLenum messageType)
{
    switch (messageType) {
    case EGL_DEBUG_MSG_CRITICAL_KHR: return fegl_DEBUG_CRITICAL_BIT;
    case EGL_DEBUG_MSG_ERROR_KHR: return fegl_DEBUG_ERROR_BIT;
    case EGL_DEBUG_MSG_WARN_KHR: return fegl_DEBUG_WARN_BIT;
    case EGL_DEBUG_MSG_INFO_KHR: return fegl_DEBUG_INFO_BIT;
    }
    return 0;
}

/// Returns a bitwise-or combination of FEGL_DebugTypeMask values indicating
/// which message types are passed to the user.
EGLint fegl_CurrentDebugTypeMask();

/// @}

#endif