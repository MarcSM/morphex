/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   current_version_png;
    const int            current_version_pngSize = 345697;

    extern const char*   v0_1_0beta_png;
    const int            v0_1_0beta_pngSize = 50515;

    extern const char*   v0_2_0beta_png;
    const int            v0_2_0beta_pngSize = 94032;

    extern const char*   v0_3_0beta_png;
    const int            v0_3_0beta_pngSize = 345697;

    extern const char*   github_logo_png;
    const int            github_logo_pngSize = 36571;

    extern const char*   github_logo_small_png;
    const int            github_logo_small_pngSize = 22813;

    extern const char*   morphex_logo_png;
    const int            morphex_logo_pngSize = 44236;

    extern const char*   morphex_logo_original_png;
    const int            morphex_logo_original_pngSize = 41472;

    extern const char*   morphex_logo_small_png;
    const int            morphex_logo_small_pngSize = 1540;

    extern const char*   morphex_long_logo_png;
    const int            morphex_long_logo_pngSize = 86623;

    extern const char*   morphex_long_logo_svg;
    const int            morphex_long_logo_svgSize = 12506;

    extern const char*   morphex_long_logo_shadow_svg;
    const int            morphex_long_logo_shadow_svgSize = 12973;

    // Number of elements in the namedResourceList and originalFileNames arrays.
    const int namedResourceListSize = 12;

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
