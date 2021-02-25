#include "layer001/RtGdiP.h"

#include "layer000/RtWin32ExGuiOsDefines.h"

struct rt_fast_initialization rt_gdipInitialization = RT_FAST_INITIALIZATION_STATIC_INIT;

rt_un rt_nGdipToken;

rt_s RtGdipInitialize()
{
	GdiplusStartupInput gdipStartupInput;
	GpStatus status;
	rt_s ret;

	if (rt_fast_initialization_is_required(&rt_gdipInitialization)) {
		gdipStartupInput.GdiplusVersion = 1;
		gdipStartupInput.DebugEventCallback	= RT_NULL;
		gdipStartupInput.SuppressBackgroundThread = RT_FALSE;
		gdipStartupInput.SuppressExternalCodecs = RT_FALSE;
		status = GdiplusStartup(&rt_nGdipToken, &gdipStartupInput, RT_NULL);
		if (status != Ok) {
			RtGdipSetLastErrorFromGpStatus(status);
			ret = RT_FAILED;
		} else {
			ret = RT_OK;
		}

		rt_fast_initialization_notify_done(&rt_gdipInitialization);
	} else {
		ret = RT_OK;
	}
	return ret;
}

rt_s RtGdipCleanUp()
{
	if (rt_fast_initialization_is_done(&rt_gdipInitialization)) {
		/* Returns void. */
		GdiplusShutdown(rt_nGdipToken);
	}
	return RT_TRUE;
}

void RtGdipSetLastErrorFromGpStatus(rt_un unStatus)
{
	switch (unStatus)
	{
		case Ok:
			SetLastError(ERROR_SUCCESS);
			break;
		case InvalidParameter:
			rt_error_set_last(RT_ERROR_BAD_ARGUMENTS);
			break;
		case OutOfMemory:
			SetLastError(ERROR_OUTOFMEMORY);
			break;
		case ObjectBusy:
			SetLastError(ERROR_BUSY);
			break;
		case InsufficientBuffer:
			rt_error_set_last(RT_ERROR_INSUFFICIENT_BUFFER);
			break;
		case FileNotFound:
			SetLastError(ERROR_FILE_NOT_FOUND);
			break;
		case ValueOverflow:
			rt_error_set_last(RT_ERROR_ARITHMETIC_OVERFLOW);
			break;
		case AccessDenied:
			SetLastError(ERROR_ACCESS_DENIED);
			break;
		case Win32Error:
		case NotImplemented:
		case WrongState:
		case Aborted:
		case UnknownImageFormat:
		case FontFamilyNotFound:
		case FontStyleNotFound:
		case GdiplusNotInitialized:
		case PropertyNotFound:
		case PropertyNotSupported:
		case GenericError:
		case NotTrueTypeFont:
		case UnsupportedGdiplusVersion:
		default:
			 rt_error_set_last(RT_ERROR_FUNCTION_FAILED);
	}
}

rt_s RtGdipStretchBitmap(rt_h hBitmap, rt_h hDc, rt_n32 nWidth, rt_n32 nHeight, RT_GDIP_INTERPOLATION_MODE nInterpolationMode)
{
	GpGraphics *lpGraphics;
	GpBitmap *lpBitmap;
	GpBrush *lpBrush;
	GpStatus status;
	rt_s ret;

	lpGraphics = RT_NULL;
	lpBitmap = RT_NULL;
	lpBrush = RT_NULL;

	/* Create destination graphics hDc. */
	status = GdipCreateFromHDC(hDc, &lpGraphics);
	if (status) {
		RtGdipSetLastErrorFromGpStatus(status);
		goto error;
	}

	/* Adjust quality. */
	status = GdipSetInterpolationMode(lpGraphics, (InterpolationMode)nInterpolationMode);
	if (status) {
		RtGdipSetLastErrorFromGpStatus(status);
		goto error;
	}

	/* Create a brush to fill the background. */
	status = GdipCreateSolidFill(0xFFFFFFFF, &lpBrush);
	if (status) {
		RtGdipSetLastErrorFromGpStatus(status);
		goto error;
	}

	/* Fill the destination DC with a background. Necessary to avoid edges artifacts. */
	status = GdipFillRectangleI(lpGraphics, lpBrush, 0, 0, nWidth, nHeight);
	if (status) {
		RtGdipSetLastErrorFromGpStatus(status);
		goto error;
	}

	/* Create source image/bitmap from hBitmap. */
	status = GdipCreateBitmapFromHBITMAP(hBitmap, RT_NULL, &lpBitmap);
	if (status) {
		RtGdipSetLastErrorFromGpStatus(status);
		goto error;
	}

	/* Stretch. */
	status = GdipDrawImageRectI(lpGraphics, lpBitmap, 0, 0, nWidth, nHeight);
	if (status) {
		RtGdipSetLastErrorFromGpStatus(status);
		goto error;
	}

	ret = RT_OK;
	goto free;
error:
	ret = RT_FAILED;
free:
	if (lpGraphics) {
		status = GdipDeleteGraphics(lpGraphics);
		lpGraphics = RT_NULL;
		if (status && ret) {
			RtGdipSetLastErrorFromGpStatus(status);
			goto error;
		}
	}
	if (lpBitmap) {
		status = GdipDisposeImage(lpBitmap);
		lpBitmap = RT_NULL;
		if (status && ret) {
			RtGdipSetLastErrorFromGpStatus(status);
			goto error;
		}
	}
	if (lpBrush) {
		status = GdipDeleteBrush(lpBrush);
		lpBrush = RT_NULL;
		if (status && ret) {
			RtGdipSetLastErrorFromGpStatus(status);
			goto error;
		}
	}
	return ret;
}
