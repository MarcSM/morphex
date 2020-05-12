/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   sopranoE4_had;
    const int            sopranoE4_hadSize = 329671;

    extern const char*   violinB3_had;
    const int            violinB3_hadSize = 597328;

    extern const char*   morphex_long_logo_png;
    const int            morphex_long_logo_pngSize = 86600;

    extern const char*   morphex_logo_small_png;
    const int            morphex_logo_small_pngSize = 1540;

    extern const char*   morphex_logo_png;
    const int            morphex_logo_pngSize = 44236;

    extern const char*   github_logo_small_png;
    const int            github_logo_small_pngSize = 22813;

    extern const char*   github_logo_png;
    const int            github_logo_pngSize = 36571;

    // Number of elements in the namedResourceList and originalFileNames arrays.
    const int namedResourceListSize = 7;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Points to the start of a list of resource filenames.
    extern const char* originalFilenames[];

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes);

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding original, non-mangled filename (or a null pointer if the name isn't found).
    const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8);
}
