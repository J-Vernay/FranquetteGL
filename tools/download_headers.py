from pathlib import Path
import urllib.request, os

# HEADERS: download base url -> relative header paths
HEADERS = {
    "https://raw.githubusercontent.com/KhronosGroup/EGL-Registry/main/api/":
    [
        "KHR/khrplatform.h",
        "EGL/eglplatform.h",
        "EGL/egl.h",
        "EGL/eglext.h",
    ],
    "https://raw.githubusercontent.com/KhronosGroup/OpenGL-Registry/main/api/":
    [
        "GL/glext.h",
        "GL/glcorearb.h",

        "GLES/glplatform.h",
        "GLES/gl.h",
        "GLES/glext.h",
        "GLES/egl.h",

        "GLES2/gl2platform.h",
        "GLES2/gl2.h",
        "GLES2/gl2ext.h",

        "GLES3/gl3platform.h",
        "GLES3/gl3.h",
        "GLES3/gl31.h",
        "GLES3/gl32.h",
    ]
}

HEADER_PREFIX = b"""
/// @file {header}
/// @author Khronos Group
/// @copyright Under GNU General Public License v3 or later.
/// @brief Common API definitions required for everything else.
"""

def main():
    if not Path("include/FranquetteGL").exists():
        raise RuntimeError("The script must be run from FranquetteGL's root directory.")

    total_count = sum(len(paths) for paths in HEADERS.values())
    count = 1
    for base_url, header_paths in HEADERS.items():
        for header_path in header_paths:
            print(f"{count:2}/{total_count}: Downloading {header_path} ...")
            with urllib.request.urlopen(base_url + header_path) as response:
                destpath = Path("include")/header_path
                destpath.parent.mkdir(parents=True,exist_ok=True)
                with open(destpath, "wb") as destfile:
                    destfile.write(response.read())
            count += 1
    print(f"Done downloading {total_count} headers.")
    

if __name__ == "__main__":
    try:
        main()
    except RuntimeError as e:
        print(f"ERROR: {e}")

            