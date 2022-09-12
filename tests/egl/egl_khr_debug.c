#include "FranquetteGL/EGL/fegl_errors.h"

#define EGL_EGLEXT_PROTOTYPES
#include <EGL/egl.h>
#include <EGL/eglext.h>

#define TEST_NO_MAIN
#include "../acutest.h"

static bool wasErrorHandled = false;
static EGLenum lastError = EGL_SUCCESS;
static EGLLabelKHR lastThreadLabel = NULL;
static EGLLabelKHR lastObjectLabel = NULL;

static void ErrorHandler(EGLenum error, const char* command, EGLint messageType, EGLLabelKHR threadLabel,
                         EGLLabelKHR objectLabel, const char* message)
{
    wasErrorHandled = true;
    lastError = error;
    lastThreadLabel = threadLabel;
    lastObjectLabel = objectLabel;
}

void test_EGL_KHR_debug()
{
    EGLAttrib attrib;
    EGLint result;
    EGLBoolean isSuccess;

    // Checking NULL state.

    result = eglDebugMessageControlKHR(NULL, NULL);
    TEST_CHECK(result == EGL_SUCCESS);
    TEST_CHECK(eglQueryDebugKHR(EGL_DEBUG_CALLBACK_KHR, &attrib) == EGL_TRUE);
    TEST_CHECK(attrib == (EGLAttrib)NULL);

    // Message filtering should be reset, only ERROR and CRITICAL are accepted.

    TEST_CHECK(eglQueryDebugKHR(EGL_DEBUG_MSG_CRITICAL_KHR, &attrib) == EGL_TRUE);
    TEST_CHECK(attrib == EGL_TRUE);
    TEST_CHECK(eglQueryDebugKHR(EGL_DEBUG_MSG_ERROR_KHR, &attrib) == EGL_TRUE);
    TEST_CHECK(attrib == EGL_TRUE);
    TEST_CHECK(eglQueryDebugKHR(EGL_DEBUG_MSG_WARN_KHR, &attrib) == EGL_TRUE);
    TEST_CHECK(attrib == EGL_FALSE);
    TEST_CHECK(eglQueryDebugKHR(EGL_DEBUG_MSG_INFO_KHR, &attrib) == EGL_TRUE);
    TEST_CHECK(attrib == EGL_FALSE);

    TEST_CHECK(fegl_CurrentDebugTypeMask() == (fegl_DEBUG_CRITICAL_BIT | fegl_DEBUG_ERROR_BIT));

    // Custom handling.

    EGLAttrib attribs[] = {EGL_DEBUG_MSG_CRITICAL_KHR, EGL_FALSE, EGL_DEBUG_MSG_WARN_KHR, EGL_TRUE, EGL_NONE};
    result = eglDebugMessageControlKHR(&ErrorHandler, attribs);
    TEST_CHECK(result == EGL_SUCCESS);
    TEST_CHECK(fegl_CurrentDebugTypeMask() == (fegl_DEBUG_ERROR_BIT | fegl_DEBUG_WARN_BIT));
    wasErrorHandled = false;

    // Only triggered by critical errosr and warnings.

    fegl_Error(EGL_BAD_ALLOC, NULL, NULL, NULL); // Critical error
    TEST_CHECK(wasErrorHandled == false);
    wasErrorHandled = false;

    fegl_Error(EGL_BAD_PARAMETER, NULL, (EGLLabelKHR)1205, NULL); // Non-critical error
    TEST_CHECK(wasErrorHandled == true);
    TEST_CHECK(lastError == EGL_BAD_PARAMETER);
    TEST_CHECK(lastThreadLabel == NULL);
    TEST_CHECK(lastObjectLabel == (EGLLabelKHR)1205);
    wasErrorHandled = false;

    fegl_Warn(NULL, NULL, NULL);
    TEST_CHECK(wasErrorHandled == true);
    wasErrorHandled = false;

    fegl_Info(NULL, NULL, NULL);
    TEST_CHECK(wasErrorHandled == false);
    wasErrorHandled = false;

    // Passing a new attribute list will not reset the state.

    EGLAttrib attribs2[] = {EGL_DEBUG_MSG_INFO_KHR, EGL_TRUE, EGL_NONE};
    result = eglDebugMessageControlKHR(&ErrorHandler, attribs2);
    TEST_CHECK(result == EGL_SUCCESS);
    TEST_CHECK(fegl_CurrentDebugTypeMask() == (fegl_DEBUG_ERROR_BIT | fegl_DEBUG_WARN_BIT | fegl_DEBUG_INFO_BIT));
    wasErrorHandled = false;

    // Thread label

    EGLDisplay disp = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    eglLabelObjectKHR(disp, EGL_OBJECT_THREAD_KHR, NULL, (EGLLabelKHR)1234);

    fegl_Warn(NULL, (EGLLabelKHR)999, NULL);
    TEST_CHECK(wasErrorHandled == true);
    TEST_CHECK(lastThreadLabel == (EGLLabelKHR)1234);
    TEST_CHECK(lastObjectLabel == (EGLLabelKHR)999);
    wasErrorHandled = false;

    // Display label

    eglLabelObjectKHR(disp, EGL_OBJECT_DISPLAY_KHR, disp, (EGLLabelKHR)10000);

    eglQueryString(disp, EGL_VERSION); // 'disp' is not initialized: EGL_NOT_INITIALIZED error
    TEST_CHECK(wasErrorHandled == true);
    TEST_CHECK(lastThreadLabel == (EGLLabelKHR)1234);
    TEST_CHECK(lastObjectLabel == (EGLLabelKHR)10000);
    wasErrorHandled = false;
}