
#include <FranquetteGL/EGL/fegl_errors.h>

// By repeating the prototype of an inline function, the C compiler will create code for the function.
void fegl_Success();
EGLint fegl_GetDebugTypeMask(EGLenum messageType);

/// Status of the last EGL function call.
fgl_THREAD_LOCAL EGLint _fegl_lastStatus = EGL_SUCCESS;

/// User function to be called on error.
static EGLDEBUGPROCKHR _fegl_debugProc = NULL;

/// User preferences for debug messages.
static EGLint _fegl_debugFlags = fegl_DEBUG_CRITICAL_BIT | fegl_DEBUG_ERROR_BIT;

/// Current thread label.
static fgl_THREAD_LOCAL EGLLabelKHR _fegl_threadLabel = NULL;

void fegl_Info(char const* command, EGLLabelKHR objectLabel, char const* message)
{
    if (_fegl_debugProc == NULL)
        return; // debug messages disabled
    if ((fegl_DEBUG_INFO_BIT & _fegl_debugFlags) == 0)
        return; // filtered out
    (*_fegl_debugProc)(EGL_SUCCESS, command, EGL_DEBUG_MSG_INFO_KHR, _fegl_threadLabel, objectLabel, message);
}

void fegl_Warn(char const* command, EGLLabelKHR objectLabel, char const* message)
{
    if (_fegl_debugProc == NULL)
        return; // debug messages disabled
    if ((fegl_DEBUG_WARN_BIT & _fegl_debugFlags) == 0)
        return; // filtered out
    (*_fegl_debugProc)(EGL_SUCCESS, command, EGL_DEBUG_MSG_WARN_KHR, _fegl_threadLabel, objectLabel, message);
}

void fegl_Error(EGLenum error, char const* command, EGLLabelKHR objectLabel, char const* message)
{
    _fegl_lastStatus = error;
    if (_fegl_debugProc == NULL)
        return; // debug messages disabled

    EGLint messageType = 0;
    switch (error) {
    case EGL_SUCCESS: messageType = EGL_DEBUG_MSG_INFO_KHR; break;
    case EGL_BAD_ALLOC:
    case EGL_CONTEXT_LOST: messageType = EGL_DEBUG_MSG_CRITICAL_KHR; break;
    default: messageType = EGL_DEBUG_MSG_ERROR_KHR; break;
    }

    if ((fegl_GetDebugTypeMask(messageType) & _fegl_debugFlags) == 0)
        return; // filtered out
    (*_fegl_debugProc)(error, command, messageType, _fegl_threadLabel, objectLabel, message);
}

EGLint fegl_CurrentDebugTypeMask() { return _fegl_debugFlags; }

/// @addtogroup EGL_KHR_debug
/// @sa - Specification: https://registry.khronos.org/EGL/extensions/KHR/EGL_KHR_debug.txt
/// @{

/// Controls debug output functionality. Debug messages are passed to @a callback.
/// Messages can be filtered by types with @a attrib_list .
/// @param callback User function to be called when a message is outputted.
/// If NULL, the debug messages are disabled and the type filter is reset.
/// @param attrib_list Modifies the current type filter.
/// Expects a list of pairs (KEY,VALUE) terminated by EGL_NONE.
///
/// For each (KEY,VALUE) in @a attrib_list , KEY is the message type to filter,
/// and VALUE is either EGL_TRUE to enable it or EGL_FALSE to disable it.
/// | Accepted keys for @a attrib_list ||Default|
/// |----------------------------------||-------|
/// | EGL_DEBUG_MSG_CRITICAL_KHR | Internal EGL driver failures, i.e. EGL_BAD_ALLOC, EGL_CONTEXT_LOST     | EGL_TRUE  |
/// | EGL_DEBUG_MSG_ERROR_KHR    | Input and bad match errors, i.e. EGL_BAD_CONTEXT, EGL_BAD_PARAMETER... | EGL_TRUE  |
/// | EGL_DEBUG_MSG_WARN_KHR     | Warnings, code is EGL_SUCCESS, but deprecated or inefficient operation | EGL_FALSE |
/// | EGL_DEBUG_MSG_INFO_KHR     | Verbose operations, object creation, destruction, state change...      | EGL_FALSE |
///
EGLint eglDebugMessageControlKHR(EGLDEBUGPROCKHR callback, EGLAttrib const* attrib_list)
{
    if (callback == NULL) {
        _fegl_debugProc = NULL;
        _fegl_debugFlags = fegl_DEBUG_CRITICAL_BIT | fegl_DEBUG_ERROR_BIT;
        return EGL_SUCCESS;
    }

    if (attrib_list == NULL || attrib_list[0] == EGL_NONE) {
        _fegl_debugProc = callback;
        return EGL_SUCCESS;
    }

    EGLint debugFlags = _fegl_debugFlags;

    for (int i = 0; attrib_list[i] != EGL_NONE; i += 2) {
        EGLAttrib messageType = attrib_list[i];
        EGLAttrib messageEnabled = attrib_list[i + 1];

        EGLint mask = fegl_GetDebugTypeMask(messageType);
        if (mask == 0 || (messageEnabled != EGL_TRUE && messageEnabled != EGL_FALSE))
            return EGL_BAD_ATTRIBUTE;

        debugFlags = (messageEnabled ? (debugFlags | mask) : (debugFlags & ~mask));
    }

    _fegl_debugProc = callback;
    _fegl_debugFlags = debugFlags;
    return EGL_SUCCESS;
}

/// Query an attribute from the debug layer.
/// @param attribute Which attribute to query the value. Possible values are:
/// - EGL_DEBUG_CALLBACK_KHR
/// - EGL_DEBUG_MSG_CRITICAL_KHR
/// - EGL_DEBUG_MSG_ERROR_KHR
/// - EGL_DEBUG_MSG_WARN_KHR
/// - EGL_DEBUG_MSG_INFO_KHR
/// @param value Pointer to location where the result is written on success:
/// - The current function callback if @a attribute is EGL_DEBUG_CALLBACK_KHR
/// - Either EGL_TRUE or EGL_FALSE when @a attribute is a message type
/// @returns EGL_TRUE on success, and EGL_FALSE on failure.
///
/// **Errors**
/// - EGL_BAD_ATTRIBUTE if @a attribute is not one of the possible values.
///
EGLBoolean eglQueryDebugKHR(EGLint attribute, EGLAttrib* value)
{
    if (attribute == EGL_DEBUG_CALLBACK_KHR) {
        *value = (EGLAttrib)_fegl_debugProc;
        return EGL_TRUE;
    }
    EGLint mask = fegl_GetDebugTypeMask(attribute);
    if (mask == 0) {
        fegl_Error(EGL_BAD_ATTRIBUTE, __func__, NULL, "Unknown attribute");
        return EGL_FALSE;
    }
    *value = (bool)(_fegl_debugFlags & mask);
    return EGL_TRUE;
}

/// Associates a user-provided debug label to an object owned by EGL.
/// You are free to pass any pointer as a label, either to a string or a struct.
/// The pointer will never be read by EGL, it will only be passed to the callback installed with
/// eglDebugMessageControlKHR(). All objects have initially a NULL label.
///
/// @todo Not implemented
EGLint eglLabelObjectKHR(EGLDisplay display, EGLenum objectType, EGLObjectKHR object, EGLLabelKHR label)
{
    switch (objectType) {
    case EGL_OBJECT_THREAD_KHR: _fegl_threadLabel = label; return EGL_SUCCESS;
    }
    return EGL_BAD_PARAMETER;
}

/// @}

/// @addtogroup EGL
/// @{

/// Returns the status (success or failure) of **the last EGL function call** in the current
/// thread.
/// @note Because eglGetError() is itself an EGL function, when calling the function twice, the
/// second call will always return EGL_SUCCESS, indicating the status of the first call.
/// @retval EGL_SUCCESS
/// Function succeeded.
/// @retval EGL_NOT_INITIALIZED
/// EGL is not initialized, or could not be initialized, for the specified display.
/// Any command may generate this error.
/// @retval EGL_BAD_ACCESS
/// EGL cannot access a requested resource (for example, a context is bound in another thread).
/// Any command accessing a named resource may generate this error.
/// @retval EGL_BAD_ALLOC
/// EGL failed to allocate resources for the requested operation.
/// Any command allocating resources may generate this error.
/// @retval EGL_BAD_ATTRIBUTE
/// An unrecognized attribute or attribute value was passed in an attribute list.
/// Any command taking an attribute parameter or attribute list may generate this error.
/// @retval EGL_BAD_CONTEXT
/// An EGLContext argument does not name a valid EGLContext.
/// Any command taking an EGLContext parameter may generate this error.
/// @retval EGL_BAD_CONFIG
/// An EGLConfig argument does not name a valid EGLConfig.
/// Any command taking an EGLConfig parameter may generate this error.
/// @retval EGL_BAD_CURRENT_SURFACE
/// The current surface of the calling thread is a window, pbuffer or pixmap
/// that is no longer valid.
/// @retval EGL_BAD_DISPLAY
/// An EGLDisplay argument does not name a valid EGLDisplay.
/// Any command taking an EGLDisplay parameter may generate this error.
/// @retval EGL_BAD_SURFACE
/// An EGLSurface argument does not name a valid surface (window, pbuffer or pixmap)
/// configured for rendering. Any command taking an EGLSurface parameter may generate this
/// error.
/// @retval EGL_BAD_MATCH
/// Arguments are inconsistent; for example, an otherwise valid context requires buffers
/// (e.g. depth or stencil) not allocated by an otherwise valid surface.
/// @retval EGL_BAD_PARAMETER
/// One or more argument values are invalid. Any command taking parameters may generate this
/// error.
/// @retval EGL_BAD_NATIVE_PIXMAP
/// An EGLNativePixmapType argument does not refer to a valid native pixmap.
/// Any command taking an EGLNativePixmapType parameter may generate this error.
/// @retval EGL_BAD_NATIVE_WINDOW
/// An EGLNativeWindowType argument does not refer to a valid native window.
/// Any command taking an EGLNativeWindowType parameter may generate this error.
/// @retval EGL_CONTEXT_LOST
/// A power management event has occured. The application must destroy all contexts and
/// reinitialise client API state and objects to continue rendering. Any command may generate
/// this error.
EGLint eglGetError()
{
    EGLint lastStatus = _fegl_lastStatus;
    return fegl_Success(), lastStatus;
}

/// @}