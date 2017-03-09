#ifndef RT_CHAR_H
#define RT_CHAR_H

#include "layer000/RtWin32ExTypes.h"

/**
 * @file
 * Characters management.
 *
 * <p>
 * Functions must never rely on an heap.<br>
 * Introduce new char header if necessary.
 * </p>
 */

#define RT_CHAR_EMPTY_STRING _R("")
#define RT_CHAR_EMPTY_STRING8 ""

/* String size that can be used once in a function and avoid stack probe. */
#define RT_CHAR_BIG_STRING_SIZE 1920
/* String size that can be used twice in a function and avoid stack probe. */
#define RT_CHAR_HALF_BIG_STRING_SIZE 960
/* String size that can be used three times in a function and avoid stack probe. */
#define RT_CHAR_THIRD_BIG_STRING_SIZE 640

RT_B RT_API RtConvertIntegerToString(RT_N nInput, RT_CHAR* lpBuffer, RT_UN unBufferSize, RT_UN* lpWritten);

RT_B RT_API RtConvertUIntegerToString(RT_UN unInput, RT_CHAR* lpBuffer, RT_UN unBufferSize, RT_UN* lpWritten);

RT_B RT_API RtConvertStringToInteger(RT_CHAR* lpInput, RT_N* lpResult);

/**
 * Return 0 if the Strings are equals, a negative value if the first string is less than the second one, a positive value otherwise.<br>
 * Characters are compared as unsigned characters.
 */
RT_N RT_API RtCompareStrings(RT_CHAR* lpString1, RT_CHAR* lpString2);

RT_UN RT_API RtGetStringSize(RT_CHAR* lpInput);

/**
 * @return The index of the searched string in given string, or RT_TYPE_MAX_UN if not found.
 */
RT_UN RT_API RtSearchString(RT_CHAR* lpString, RT_CHAR* lpSearched);

/**
 * @return Count occurrences of <tt>lpSearched</tt> in <tt>lpString</tt>.
 */
RT_UN RT_API RtCountStringOccurrences(RT_CHAR* lpString, RT_CHAR* lpSearched);

RT_B RT_API RtReplaceString(RT_CHAR* lpString, RT_CHAR* lpSearched, RT_CHAR* lpReplacement, RT_CHAR* lpBuffer, RT_UN unBufferSize, RT_UN* lpWritten);

/**
 * Copy <tt>unSize</tt> characters from <tt>lpSource</tt> to <tt>lpBuffer</tt>.<br>
 * Add a trailing zero character.<br>
 * There is no check on the presence of zeros in lpSource.
 *
 * @param lpSource Its size must be at least <tt>unSize</tt>.
 * @param unSize Can be zero but not negative.
 */
RT_B RT_API RtCopyStringWithSize(RT_CHAR* lpSource, RT_UN unSize, RT_CHAR* lpBuffer, RT_UN unBufferSize, RT_UN* lpWritten);

/**
 * Copy from lpSource to lpBuffer.
 * If the buffer is too small then unBufferSize - 1 is returned and RtSetLastError is called.
 * If the buffer size is negative or zero, then zero is returned.
 */
RT_B RT_API RtCopyString(RT_CHAR* lpSource, RT_CHAR* lpBuffer, RT_UN unBufferSize, RT_UN* lpWritten);

/**
 * Write <tt>nChar</tt> as first character of given buffer and zero as second character.
 */
RT_B RT_API RtCopyChar(RT_CHAR nChar, RT_CHAR* lpBuffer, RT_UN unBufferSize, RT_UN* lpWritten);

/**
 * Manage only characters from 'A' to 'Z'.<br>
 * Leave other characters unchanged.
 *
 * @return The size of the string.
 */
RT_UN RT_API RtFastLowerString(RT_CHAR* lpString);

/**
 * Manage only characters from 'A' to 'Z'.<br>
 * Leave other characters unchanged.
 */
RT_CHAR RT_API RtFastLowerChar(RT_CHAR nChar);

/**
 * Use this function in place of <tt>RtLeftPadStringWithSize</tt> if you do not know the size of <tt>lpInput</tt>.
 */
RT_B RT_API RtLeftPadString(RT_CHAR* lpInput, RT_CHAR nChar, RT_UN unSize, RT_CHAR* lpBuffer, RT_UN unBufferSize, RT_UN* lpWritten);

/**
 * Add some <tt>nChar</tt> at the left of <tt>lpInput</tt> to ensure that <tt>lpBuffer</tt> size is at least <tt>unSize</tt>.
 *
 * <p>
 * <tt>lpBuffer</tt> can be the same as <tt>lpInput</tt>.
 * </p>
 */
RT_B RT_API RtLeftPadStringWithSize(RT_CHAR* lpInput, RT_UN unInputSize, RT_CHAR nChar, RT_UN unSize, RT_CHAR* lpBuffer, RT_UN unBufferSize, RT_UN* lpWritten);

RT_B RT_CDECL_API RtConcatStrings(RT_CHAR* lpBuffer, RT_UN unBufferSize, RT_UN* lpWritten, ...);

RT_B RT_API RtVConcatStrings(va_list lpVaList, RT_CHAR* lpBuffer, RT_UN unBufferSize, RT_UN* lpWritten);

RT_B RT_API RtStringEndsWith(RT_CHAR* lpString, RT_CHAR* lpSearched);

RT_B RT_API RtStringEndsWithWithSize(RT_CHAR* lpString, RT_UN unStringSize, RT_CHAR* lpSearched, RT_UN unSearchedSize);

RT_UN RT_API RtComputeStringHash(RT_CHAR* lpString);

/**
 * Use RtRightTrimStringWithSize when you know the size of the string.
 */
RT_B RT_API RtRightTrimString(RT_CHAR* lpString, RT_UN *lpWritten);

/**
 * Remove trailing characters <= ' '.
 */
RT_B RT_API RtRightTrimStringWithSize(RT_CHAR* lpString, RT_UN unStringSize, RT_UN *lpWritten);

#endif /* RT_CHAR_H */
