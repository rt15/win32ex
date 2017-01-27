#ifndef RT_IMAGE_H
#define RT_IMAGE_H

#include <RtWin32Ex.h>

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
RT_H RT_API RtCreateDcFromBitmap(RT_H hBitmap);

/**
 * Fill the DC with the given color.
 */
RT_B RT_API RtFillDc(RT_H hDc, RT_N nWidth, RT_N nHeight, RT_UN32 unColor);

/**
 * Can be used to resize and image.
 */
RT_B RT_API RtStretchImage(RT_H hSourceDc, RT_N nSourceWidth, RT_N nSourceHeight, RT_H hDestinationDc, RT_N nDestinationWidth, RT_N nDestinationHeight);

/**
 * Mainly for debugging purpose.
 */
RT_B RT_API RtDrawImageList(RT_H hImageList, RT_H hDc, RT_N nX, RT_N nY);

/**
 * <p>
 * Call DeleteObject on the returned handle when the bitmap is no more necessary.
 * <p>
 * @param nWidth Both the width of the icon and the resulting bitmap.
 * @param nHeight Both the height of the icon and the resulting bitmap.
 */
RT_H RT_API RtConvertIconToBitmap(RT_H hIcon, RT_N nWidth, RT_N nHeight, RT_UN32 unBackgroundColor);

/**
 * Retrieve the pixels of the given bitmap.
 *
 * <p>
 * The size of the buffer must be sizeof(RT_UN32) * nWidth * nHeight.
 * </p>
 */
RT_B RT_API RtGetPixels(RT_H hBitmap, RT_N nWidth, RT_N nHeight, RT_UN32* lpBuffer, RT_H hDc);

/**
 * Pre-multiply the color.
 *
 * <p>
 * Pre-multiplied colors are used because they are faster to render.<br>
 * The color components are pre-multiplied by the alpha value.
 * </p>
 */
RT_UN32 RT_API RtConvertArgbColorToPargbColor(RT_UN32 unColor);

/**
 *
 * <p>
 * The size of the buffer must be sizeof(RT_UN32) * nWidth * nHeight.
 * </p>
 *
 * <p>
 * In a PARGB bitmap, RGB values are pre-multiplied by the transparency.
 * <p/>
 */
RT_H RT_API RtConvertIconToPargbBitmap(RT_H hIcon, RT_N nWidth, RT_N nHeight, RT_UN32* lpMaskBuffer, RT_UN32* lpColorBuffer, RT_H hDc);

/**
 * Load an icon.
 *
 * <p>
 * The size of the returned icon is based on the size of the resource while LoadIcon is based on SM_CXICON and SM_CYICON.
 * No need to destroy the returned icon.<br>
 * </p>
 */
RT_H RT_API RtLoadIcon(RT_N nResourceIndex, RT_H hInstance);

#endif /* RT_IMAGE_H */
