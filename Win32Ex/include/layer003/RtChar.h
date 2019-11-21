#ifndef RT_CHAR_H
#define RT_CHAR_H

#include "layer000/RtWin32ExTypes.h"

#include "layer003/RtArray.h"

/**
 * @file
 * Characters management.
 *
 * <p>
 * Functions must never rely on a heap.<br>
 * Introduce new char header if necessary.
 * </p>
 */

/* String size that can be used once in a function and avoid stack probe. */
#define RT_CHAR_BIG_STRING_SIZE 1920
/* String size that can be used twice in a function and avoid stack probe. */
#define RT_CHAR_HALF_BIG_STRING_SIZE 960
/* String size that can be used three times in a function and avoid stack probe. */
#define RT_CHAR_THIRD_BIG_STRING_SIZE 640
/* String size that can be used four times in a function and avoid stack probe. */
#define RT_CHAR_QUARTER_BIG_STRING_SIZE 480

/**
 * Create a String pointing on a CString.<br>
 *
 * <p>
 * The resulting array is immutable.<br>
 * The size of the array does not include the trailing zero.
 * </p>
 */
RT_ARRAY* RT_API RtChar_CreateString(RT_ARRAY* lpString, RT_CHAR* lpCString);

RT_ARRAY* RT_API RtChar_CreateStringWithSize(RT_ARRAY* lpString, RT_CHAR* lpCString, RT_UN unSize);

RT_UN RT_API RtChar_GetStringSize(RT_ARRAY* lpString);

RT_B RT_API RtChar_CopyCString(RT_ARRAY* lpString, RT_CHAR* lpCString);

RT_B RT_API RtChar_AppendCString(RT_ARRAY* lpString, RT_CHAR* lpCString);

RT_B RT_API RtChar_Append(RT_ARRAY* lpString, RT_CHAR nChar);

RT_UN RT_API RtChar_GetCStringSize(RT_CHAR* lpInput);

/**
 *
 * <p>
 * If <tt>lpSearched</tt> (or <tt>lpString</tt>) is empty, then RT_TYPE_MAX_UN is returned.
 * </p>
 *
 * @return The index of the searched string in given string, or RT_TYPE_MAX_UN if not found.
 */
RT_UN RT_API RtChar_SearchString(RT_ARRAY* lpString, RT_ARRAY* lpSearched);

/**
 * @return index of <tt>nSearched</tt> in <tt>lpString</tt> or RT_TYPE_MAX_UN if not found.
 */
RT_UN RT_API RtChar_Search(RT_ARRAY* lpString, RT_CHAR nSearched);

/**
 * Test two Strings for equality.
 *
 * <p>
 * This function can be faster than traditional strcmp because it first check that the strings have the same size.<br>
 * If the size is different, RT_FALSE is immediately returned. As a result, this function cannot return an RT_N32 with the difference like classical strcmp.
 * </p>
 *
 */
RT_B RT_API RtChar_StringEqualsString(RT_ARRAY* lpString, RT_ARRAY* lpOtherString);

RT_B RT_API RtChar_StringEqualsCString(RT_ARRAY* lpString, RT_CHAR* lpCString);

RT_B RT_API RtChar_ConvertIntegerToString(RT_N nInput, RT_ARRAY* lpString);

RT_B RT_API RtChar_ConvertUnsignedIntegerToString(RT_UN unInput, RT_ARRAY* lpString);

RT_B RT_API RtChar_ConvertStringToInteger(RT_ARRAY* lpInput, RT_N* lpResult);

RT_B RT_API RtChar_ConvertStringToUnsignedInteger(RT_ARRAY* lpInput, RT_UN* lpResult);

/**
 * Return 0 if the Strings are equals, a negative value if the first string is less than the second one, a positive value otherwise.<br>
 * The result is somehow lpString1 - lpString2.<br>
 * Characters are compared as unsigned characters.
 *
 * <p>
 * Can be used to sort strings.<br>
 * To check equality, prefer <tt>RtChar_StringEqualsString</tt>.
 * </p>
 *
 */
RT_N RT_API RtChar_CompareStrings(RT_ARRAY* lpString1, RT_ARRAY* lpString2);

RT_N RT_API RtChar_CompareCStrings(RT_CHAR* lpString1, RT_CHAR* lpString2);

/* 1337 */

/**
 * Return 0 if the Strings are equals, a negative value if the first string is less than the second one, a positive value otherwise.<br>
 * Characters are compared as unsigned characters.
 */
RT_N RT_API RtChar1337_CompareStrings(RT_CHAR* lpString1, RT_CHAR* lpString2);

RT_N RT_API RtChar_CompareStringsWithSize(RT_CHAR* lpString1, RT_CHAR* lpString2, RT_UN unSize);

/**
 * <p>
 * Null last like Oracle default.
 * </p>
 */
RT_N RT_API RtChar_CompareNullStrings(RT_CHAR* lpString1, RT_CHAR* lpString2);

/**
 * @return The index of the searched string in given string, or RT_TYPE_MAX_UN if not found.
 */
RT_UN RT_API RtChar1337_SearchString(RT_CHAR* lpString, RT_CHAR* lpSearched);

/**
 * @return Count occurrences of <tt>lpSearched</tt> in <tt>lpString</tt>.
 */
RT_UN RT_API RtChar_CountStringOccurrences(RT_CHAR* lpString, RT_CHAR* lpSearched);

/**
 * Replace all occurrences of <tt>lpSearched</tt> in <tt>lpString</tt> with <tt>lpReplacement</tt>.<br>
 * The result is written in <tt>lpBuffer</tt>. <tt>lpString</tt> is not modified.
 *
 * <p>
 * The result is written in a new buffer because doing it in place would be expensive if the size of <tt>lpReplacement</tt> was
 * greater than the size <tt>lpSearched</tt>.<br>
 * <tt>lpBuffer</tt> must be different than <tt>lpString</tt>.
 * </p>
 */
RT_B RT_API RtChar_ReplaceString(RT_CHAR* lpString, RT_UN unStringSize,
                                 RT_CHAR* lpSearched, RT_UN unSearchedSize,
                                 RT_CHAR* lpReplacement, RT_UN unReplacementSize,
                                 RT_CHAR* lpBuffer, RT_UN unBufferSize, RT_UN* lpOutputSize);

/**
 * Copy <tt>unSize</tt> characters from <tt>lpSource</tt> to <tt>lpBuffer</tt>.<br>
 * Add a trailing zero character.<br>
 * There is no check on the presence of zeros in lpSource.
 *
 * @param lpSource Its size must be at least <tt>unSize</tt>.
 * @param unSize Can be zero but not negative.
 */
RT_B RT_API RtChar_CopyStringWithSize(RT_CHAR* lpSource, RT_UN unSize, RT_CHAR* lpBuffer, RT_UN unBufferSize, RT_UN* lpOutputSize);

/**
 * Copy from lpSource to lpBuffer.
 * If the buffer is too small then unBufferSize - 1 is returned and RtSetLastError is called.
 * If the buffer size is negative or zero, then zero is returned.
 */
RT_B RT_API RtChar_CopyString(RT_CHAR* lpSource, RT_CHAR* lpBuffer, RT_UN unBufferSize, RT_UN* lpOutputSize);

/**
 * Write <tt>nChar</tt> as first character of given buffer and zero as second character.
 */
RT_B RT_API RtChar_Copy(RT_CHAR nChar, RT_CHAR* lpBuffer, RT_UN unBufferSize, RT_UN* lpOutputSize);

/**
 * Manage only characters from 'A' to 'Z'.<br>
 * Leave other characters unchanged.
 *
 * @return The size of the string.
 */
RT_UN RT_API RtChar_FastLowerString(RT_CHAR* lpString);

/**
 * Manage only characters from 'A' to 'Z'.<br>
 * Leave other characters unchanged.
 */
RT_CHAR RT_API RtChar_FastLower(RT_CHAR nChar);

/**
 * Manage only characters from 'a' to 'z'.<br>
 * Leave other characters unchanged.
 *
 * @return The size of the string.
 */
RT_UN RT_API RtChar_FastUpperString(RT_CHAR* lpString);

/**
 * Manage only characters from 'a' to 'z'.<br>
 * Leave other characters unchanged.
 */
RT_CHAR RT_API RtChar_FastUpper(RT_CHAR nChar);

/**
 * Add some <tt>nChar</tt> at the left of <tt>lpInput</tt> to ensure that <tt>lpBuffer</tt> size is at least <tt>unSize</tt>.
 *
 * <p>
 * <tt>lpBuffer</tt> can be the same as <tt>lpInput</tt>.<br>
 * This function takes an <tt>lpBuffer</tt> because we have to copy <tt>lpInput</tt> even if we pad in place.
 * </p>
 */
RT_B RT_API RtChar_LeftPadString(RT_CHAR* lpInput, RT_UN unInputSize, RT_CHAR nChar, RT_UN unSize, RT_CHAR* lpBuffer, RT_UN unBufferSize, RT_UN* lpOutputSize);

RT_B RT_CDECL_API RtChar_ConcatStrings(RT_CHAR* lpBuffer, RT_UN unBufferSize, RT_UN* lpOutputSize, ...);

RT_B RT_API RtChar_VConcatStrings(va_list lpVaList, RT_CHAR* lpBuffer, RT_UN unBufferSize, RT_UN* lpOutputSize);

RT_B RT_API RtChar_StringEndsWith(RT_CHAR* lpString, RT_CHAR* lpSearched);

RT_B RT_API RtChar_StringEndsWithWithSize(RT_CHAR* lpString, RT_UN unStringSize, RT_CHAR* lpSearched, RT_UN unSearchedSize);

RT_UN RT_API RtChar_GetStringHash(RT_CHAR* lpString);

/**
 * Remove trailing characters <= ' '.
 */
void RT_API RtChar_RightTrimString(RT_CHAR* lpString, RT_UN unStringSize, RT_UN *lpOutputSize);

/**
 * Search first string of <tt>lpStrings</tt> that is <tt>lpSearched</tt>.
 *
 * <p>
 * Should not be used with large arrays.<br>
 * Use sorted array and quicksort for large arrays.
 * </p>
 *
 * @param lpStrings Null terminated array of null terminated strings. Can be RT_NULL.
 * @return index of <tt>lpSearched</tt> in <tt>lpStrings</tt> or RT_TYPE_MAX_UN if not found.
 */
RT_UN RT_API RtChar_SearchStringInStrings(RT_CHAR* lpStrings[], RT_CHAR* lpSearched);

/**
 *
 * @param lpString Can be RT_NULL.
 * @return index of <tt>nSearched</tt> in <tt>lpString</tt> or RT_TYPE_MAX_UN if not found.
 */
RT_UN RT_API RtChar1337_Search(RT_CHAR* lpString, RT_CHAR nSearched);

#endif /* RT_CHAR_H */
