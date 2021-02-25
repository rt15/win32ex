#ifndef RT_IMAGE_H
#define RT_IMAGE_H

#include <rpr.h>

/**
 * @file
 * Windows images utility functions.
 */

/**
 * Create a device context compatible with the screen from the given bitmap.
 *
 * <p>
 * DeleteDC must be used when the device context is not used anymore.
 * </p>
 *
 * @return RT_NULL in case of error.
 */
rt_h RtCreateDcFromBitmap(rt_h hBitmap);

/**
 * Fill the DC with the given color.
 */
rt_s RtFillDc(rt_h hDc, rt_n32 nWidth, rt_n32 nHeight, rt_un32 unColor);

/**
 * Can be used to resize and image.
 */
rt_s RtStretchImage(rt_h hSourceDc, rt_n32 nSourceWidth, rt_n32 nSourceHeight, rt_h hDestinationDc, rt_n32 nDestinationWidth, rt_n32 nDestinationHeight);

/**
 * Mainly for debugging purpose.
 */
rt_s RtDrawImageList(rt_h hImageList, rt_h hDc, rt_n32 nX, rt_n32 nY);

/**
 * <p>
 * Call DeleteObject on the returned handle when the bitmap is no more necessary.
 * <p>
 * @param nWidth Both the width of the icon and the resulting bitmap.
 * @param nHeight Both the height of the icon and the resulting bitmap.
 */
rt_h RtConvertIconToBitmap(rt_h hIcon, rt_n32 nWidth, rt_n32 nHeight, rt_un32 unBackgroundColor);

/**
 * Retrieve the pixels of the given bitmap.
 *
 * <p>
 * The size of the buffer must be sizeof(rt_un32) * nWidth * nHeight.
 * </p>
 */
rt_s RtGetPixels(rt_h hBitmap, rt_n32 nWidth, rt_n32 nHeight, rt_un32 *buffer, rt_h hDc);

/**
 * Pre-multiply the color.
 *
 * <p>
 * Pre-multiplied colors are used because they are faster to render.<br>
 * The color components are pre-multiplied by the alpha value.
 * </p>
 */
rt_un32 RtConvertArgbColorToPargbColor(rt_un32 unColor);

/**
 *
 * <p>
 * The size of the buffer must be sizeof(rt_un32) * nWidth * nHeight.
 * </p>
 *
 * <p>
 * In a PARGB bitmap, RGB values are pre-multiplied by the transparency.
 * <p/>
 */
rt_h RtConvertIconToPargbBitmap(rt_h hIcon, rt_n32 nWidth, rt_n32 nHeight, rt_un32 *lpMaskBuffer, rt_un32 *lpColorBuffer, rt_h hDc);

/**
 * Load an icon.
 *
 * <p>
 * The size of the returned icon is based on the size of the resource while LoadIcon is based on SM_CXICON and SM_CYICON.
 * No need to destroy the returned icon.<br>
 * </p>
 */
rt_h RtLoadIcon(rt_un unResourceIndex, rt_h hInstance);

#endif /* RT_IMAGE_H */
