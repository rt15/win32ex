#ifndef RT_GDIP_H
#define RT_GDIP_H

#include <RtWin32Ex.h>

/**
 * @file
 * Windows GDI+ utility functions.
 */

/**
 * Same as InterpolationMode from gdiplusenums.h.
 *
 * <p>
 * Used in <tt>RtGdipStretchBitmap</tt> to define quality of the stretching.
 * </p>
 */
typedef enum RT_GDIP_INTERPOLATION_MODE {
	RT_GDIP_INTERPOLATION_MODE_DEFAULT = 0,
	RT_GDIP_INTERPOLATION_MODE_LOW_QUALITY = 1,
	RT_GDIP_INTERPOLATION_MODE_HIGH_QUALITY = 2,
	RT_GDIP_INTERPOLATION_MODE_BILINEAR = 3,
	RT_GDIP_INTERPOLATION_MODE_BICUBIC = 4,
	RT_GDIP_INTERPOLATION_MODE_NEAREST_NEIGHBOR = 5,
	RT_GDIP_INTERPOLATION_MODE_HIGH_QUALITY_BILINEAR = 6,
	RT_GDIP_INTERPOLATION_MODE_HIGH_QUALITY_BICUBIC = 7
} RT_GDIP_INTERPOLATION_MODE;

/**
 * Must be called before using gdi+.<br>
 * Can be called several times without issue.
 */
RT_B RT_API RtGdipInitialize();

/**
 * Must be called once when gdi+ will not be used anymore.<br>
 * Can be called even if RtGdipInitialize has never been called.<br>
 */
RT_B RT_API RtGdipCleanUp();

/**
 * Draw <tt>hBitmap</tt> into <tt>hDc</tt> with the given size.
 *
 * <p>
 * hBitmap must not be in use in a device context.
 * </p>
 */
RT_B RT_API RtGdipStretchBitmap(RT_H hBitmap, RT_H hDc, RT_N32 nWidth, RT_N32 nHeight, RT_GDIP_INTERPOLATION_MODE nInterpolationMode);

/**
 * Call RtSetLastError with the appropriate error code from GpStatus <tt>nStatus</tt> value.
 *
 * <p>
 * Can be uses when there is a gdi+ error to align the behavior with win32 errors.
 * </p>
 */
void RT_API RtGdipSetLastErrorFromGpStatus(RT_UN unStatus);

#endif /* RT_GDIP_H */
