#ifndef RT_GDIP_H
#define RT_GDIP_H

#include <rpr.h>

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
rt_s RtGdipInitialize();

/**
 * Must be called once when gdi+ will not be used anymore.<br>
 * Can be called even if RtGdipInitialize has never been called.<br>
 */
rt_s RtGdipCleanUp();

/**
 * Draw <tt>hBitmap</tt> into <tt>hDc</tt> with the given size.
 *
 * <p>
 * hBitmap must not be in use in a device context.
 * </p>
 */
rt_s RtGdipStretchBitmap(rt_h hBitmap, rt_h hDc, rt_n32 nWidth, rt_n32 nHeight, RT_GDIP_INTERPOLATION_MODE nInterpolationMode);

/**
 * Call rt_error_set_last/SetLastError with similar error code from GpStatus <tt>unStatus</tt> value.
 *
 * <p>
 * Can be uses when there is a gdi+ error to align the behavior with win32 errors.
 * </p>
 */
void RtGdipSetLastErrorFromGpStatus(rt_un unStatus);

#endif /* RT_GDIP_H */
