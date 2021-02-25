#ifndef RT_WIN32_EX_GUI_OS_DEFINES_H
#define RT_WIN32_EX_GUI_OS_DEFINES_H

#include <rpr.h>

/**
 * @file
 * This special header is used internally to include Windows and gdi+ headers.
 *
 * <p>
 * It must be included only in internal C sources files and never in headers files.
 * </p>
 * <p>
 * It has no associated .c file.
 * </p>
 */

/* Only unicode is supported by the library under Windows. */
#ifndef UNICODE
#define UNICODE
#endif

#if (!defined(_WIN32_WINNT)) || (_WIN32_WINNT < 0x0501)
#define _WIN32_WINNT 0x0501
#endif

#if (!defined(WINVER)) || (WINVER < 0x0500)
#define WINVER 0x0500
#endif

#if (!defined(_WIN32_IE)) || (_WIN32_IE < 0x0600)
#define _WIN32_IE 0x0600
#endif

#include <windows.h>
#include <commctrl.h>
#include <shlobj.h>

#ifdef RT_DEFINE_GCC
#include <gdiplus.h>
#else /* NOT RT_DEFINE_GCC */

typedef float REAL;
typedef DWORD ARGB;
typedef INT PixelFormat;
typedef DWORD GraphicsContainer;
typedef DWORD GraphicsState;

typedef void IDirectDrawSurface7;

/* Callback function types */
/* FIXME: need a correct definition for these function pointer types */
typedef void *DebugEventProc;
typedef void *EnumerateMetafileProc;
typedef void *DrawImageAbort;
typedef void *GetThumbnailImageAbort;

typedef enum GpStatus {
		Ok = 0,
		GenericError = 1,
		InvalidParameter = 2,
		OutOfMemory = 3,
		ObjectBusy = 4,
		InsufficientBuffer = 5,
		NotImplemented = 6,
		Win32Error = 7,
		WrongState = 8,
		Aborted = 9,
		FileNotFound = 10,
		ValueOverflow = 11,
		AccessDenied = 12,
		UnknownImageFormat = 13,
		FontFamilyNotFound = 14,
		FontStyleNotFound = 15,
		NotTrueTypeFont = 16,
		UnsupportedGdiplusVersion = 17,
		GdiplusNotInitialized = 18,
		PropertyNotFound = 19,
		PropertyNotSupported = 20
} GpStatus;
/* No Status enum, only GpStatus. */

typedef enum BrushType {
	BrushTypeSolidColor = 0,
	BrushTypeHatchFill = 1,
	BrushTypeTextureFill = 2,
	BrushTypePathGradient = 3,
	BrushTypeLinearGradient = 4
} BrushType;

typedef enum CombineMode {
	CombineModeReplace = 0,
	CombineModeIntersect = 1,
	CombineModeUnion = 2,
	CombineModeXor = 3,
	CombineModeExclude = 4,
	CombineModeComplement = 5
} CombineMode;

typedef enum CompositingMode {
	CompositingModeSourceOver = 0,
	CompositingModeSourceCopy = 1
} CompositingMode;

typedef enum CompositingQuality {
	CompositingQualityDefault = 0,
	CompositingQualityHighSpeed = 1,
	CompositingQualityHighQuality = 2,
	CompositingQualityGammaCorrected = 3,
	CompositingQualityAssumeLinear = 4
} CompositingQuality;

typedef enum CoordinateSpace {
	CoordinateSpaceWorld = 0,
	CoordinateSpacePage = 1,
	CoordinateSpaceDevice = 2
} CoordinateSpace;

typedef enum CustomLineCapType {
	CustomLineCapTypeDefault = 0,
	CustomLineCapTypeAdjustableArrow = 1
} CustomLineCapType;

typedef enum DashCap {
	DashCapFlat = 0,
	DashCapRound = 2,
	DashCapTriangle = 3
} DashCap;

typedef enum DashStyle {
	DashStyleSolid = 0,
	DashStyleDash = 1,
	DashStyleDot = 2,
	DashStyleDashDot = 3,
	DashStyleDashDotDot = 4,
	DashStyleCustom = 5
} DashStyle;

typedef enum DitherType {
	DitherTypeNone = 0,
	DitherTypeSolid = 1,
	DitherTypeOrdered4x4 = 2,
	DitherTypeOrdered8x8 = 3,
	DitherTypeOrdered16x16 = 4,
	DitherTypeOrdered91x91 = 5,
	DitherTypeSpiral4x4 = 6,
	DitherTypeSpiral8x8 = 7,
	DitherTypeDualSpiral4x4 = 8,
	DitherTypeDualSpiral8x8 = 9,
	DitherTypeErrorDiffusion = 10
} DitherType;

typedef enum DriverStringOptions {
	DriverStringOptionsCmapLookup = 1,
	DriverStringOptionsVertical = 2,
	DriverStringOptionsRealizedAdvance = 4,
	DriverStringOptionsLimitSubpixel = 8
} DriverStringOptions;

#define GDIP_WMF_RECORD_TO_EMFPLUS(meta) ((meta) | 0x10000)
#define GDIP_EMFPLUS_RECORD_BASE (0x4000)
typedef enum EmfPlusRecordType {
	WmfRecordTypeSetBkColor										= GDIP_WMF_RECORD_TO_EMFPLUS(META_SETBKCOLOR),
	WmfRecordTypeSetBkMode										 = GDIP_WMF_RECORD_TO_EMFPLUS(META_SETBKMODE),
	WmfRecordTypeSetMapMode										= GDIP_WMF_RECORD_TO_EMFPLUS(META_SETMAPMODE),
	WmfRecordTypeSetROP2											 = GDIP_WMF_RECORD_TO_EMFPLUS(META_SETROP2),
	WmfRecordTypeSetRelAbs										 = GDIP_WMF_RECORD_TO_EMFPLUS(META_SETRELABS),
	WmfRecordTypeSetPolyFillMode							 = GDIP_WMF_RECORD_TO_EMFPLUS(META_SETPOLYFILLMODE),
	WmfRecordTypeSetStretchBltMode						 = GDIP_WMF_RECORD_TO_EMFPLUS(META_SETSTRETCHBLTMODE),
	WmfRecordTypeSetTextCharExtra							= GDIP_WMF_RECORD_TO_EMFPLUS(META_SETTEXTCHAREXTRA),
	WmfRecordTypeSetTextColor									= GDIP_WMF_RECORD_TO_EMFPLUS(META_SETTEXTCOLOR),
	WmfRecordTypeSetTextJustification					= GDIP_WMF_RECORD_TO_EMFPLUS(META_SETTEXTJUSTIFICATION),
	WmfRecordTypeSetWindowOrg									= GDIP_WMF_RECORD_TO_EMFPLUS(META_SETWINDOWORG),
	WmfRecordTypeSetWindowExt									= GDIP_WMF_RECORD_TO_EMFPLUS(META_SETWINDOWEXT),
	WmfRecordTypeSetViewportOrg								= GDIP_WMF_RECORD_TO_EMFPLUS(META_SETVIEWPORTORG),
	WmfRecordTypeSetViewportExt								= GDIP_WMF_RECORD_TO_EMFPLUS(META_SETVIEWPORTEXT),
	WmfRecordTypeOffsetWindowOrg							 = GDIP_WMF_RECORD_TO_EMFPLUS(META_OFFSETWINDOWORG),
	WmfRecordTypeScaleWindowExt								= GDIP_WMF_RECORD_TO_EMFPLUS(META_SCALEWINDOWEXT),
	WmfRecordTypeOffsetViewportOrg						 = GDIP_WMF_RECORD_TO_EMFPLUS(META_OFFSETVIEWPORTORG),
	WmfRecordTypeScaleViewportExt							= GDIP_WMF_RECORD_TO_EMFPLUS(META_SCALEVIEWPORTEXT),
	WmfRecordTypeLineTo												= GDIP_WMF_RECORD_TO_EMFPLUS(META_LINETO),
	WmfRecordTypeMoveTo												= GDIP_WMF_RECORD_TO_EMFPLUS(META_MOVETO),
	WmfRecordTypeExcludeClipRect							 = GDIP_WMF_RECORD_TO_EMFPLUS(META_EXCLUDECLIPRECT),
	WmfRecordTypeIntersectClipRect						 = GDIP_WMF_RECORD_TO_EMFPLUS(META_INTERSECTCLIPRECT),
	WmfRecordTypeArc													 = GDIP_WMF_RECORD_TO_EMFPLUS(META_ARC),
	WmfRecordTypeEllipse											 = GDIP_WMF_RECORD_TO_EMFPLUS(META_ELLIPSE),
	WmfRecordTypeFloodFill										 = GDIP_WMF_RECORD_TO_EMFPLUS(META_FLOODFILL),
	WmfRecordTypePie													 = GDIP_WMF_RECORD_TO_EMFPLUS(META_PIE),
	WmfRecordTypeRectangle										 = GDIP_WMF_RECORD_TO_EMFPLUS(META_RECTANGLE),
	WmfRecordTypeRoundRect										 = GDIP_WMF_RECORD_TO_EMFPLUS(META_ROUNDRECT),
	WmfRecordTypePatBlt												= GDIP_WMF_RECORD_TO_EMFPLUS(META_PATBLT),
	WmfRecordTypeSaveDC												= GDIP_WMF_RECORD_TO_EMFPLUS(META_SAVEDC),
	WmfRecordTypeSetPixel											= GDIP_WMF_RECORD_TO_EMFPLUS(META_SETPIXEL),
	WmfRecordTypeOffsetClipRgn								 = GDIP_WMF_RECORD_TO_EMFPLUS(META_OFFSETCLIPRGN),
	WmfRecordTypeTextOut											 = GDIP_WMF_RECORD_TO_EMFPLUS(META_TEXTOUT),
	WmfRecordTypeBitBlt												= GDIP_WMF_RECORD_TO_EMFPLUS(META_BITBLT),
	WmfRecordTypeStretchBlt										= GDIP_WMF_RECORD_TO_EMFPLUS(META_STRETCHBLT),
	WmfRecordTypePolygon											 = GDIP_WMF_RECORD_TO_EMFPLUS(META_POLYGON),
	WmfRecordTypePolyline											= GDIP_WMF_RECORD_TO_EMFPLUS(META_POLYLINE),
	WmfRecordTypeEscape												= GDIP_WMF_RECORD_TO_EMFPLUS(META_ESCAPE),
	WmfRecordTypeRestoreDC										 = GDIP_WMF_RECORD_TO_EMFPLUS(META_RESTOREDC),
	WmfRecordTypeFillRegion										= GDIP_WMF_RECORD_TO_EMFPLUS(META_FILLREGION),
	WmfRecordTypeFrameRegion									 = GDIP_WMF_RECORD_TO_EMFPLUS(META_FRAMEREGION),
	WmfRecordTypeInvertRegion									= GDIP_WMF_RECORD_TO_EMFPLUS(META_INVERTREGION),
	WmfRecordTypePaintRegion									 = GDIP_WMF_RECORD_TO_EMFPLUS(META_PAINTREGION),
	WmfRecordTypeSelectClipRegion							= GDIP_WMF_RECORD_TO_EMFPLUS(META_SELECTCLIPREGION),
	WmfRecordTypeSelectObject									= GDIP_WMF_RECORD_TO_EMFPLUS(META_SELECTOBJECT),
	WmfRecordTypeSetTextAlign									= GDIP_WMF_RECORD_TO_EMFPLUS(META_SETTEXTALIGN),
	WmfRecordTypeDrawText											= GDIP_WMF_RECORD_TO_EMFPLUS(0x062F),
	WmfRecordTypeChord												 = GDIP_WMF_RECORD_TO_EMFPLUS(META_CHORD),
	WmfRecordTypeSetMapperFlags								= GDIP_WMF_RECORD_TO_EMFPLUS(META_SETMAPPERFLAGS),
	WmfRecordTypeExtTextOut										= GDIP_WMF_RECORD_TO_EMFPLUS(META_EXTTEXTOUT),
	WmfRecordTypeSetDIBToDev									 = GDIP_WMF_RECORD_TO_EMFPLUS(META_SETDIBTODEV),
	WmfRecordTypeSelectPalette								 = GDIP_WMF_RECORD_TO_EMFPLUS(META_SELECTPALETTE),
	WmfRecordTypeRealizePalette								= GDIP_WMF_RECORD_TO_EMFPLUS(META_REALIZEPALETTE),
	WmfRecordTypeAnimatePalette								= GDIP_WMF_RECORD_TO_EMFPLUS(META_ANIMATEPALETTE),
	WmfRecordTypeSetPalEntries								 = GDIP_WMF_RECORD_TO_EMFPLUS(META_SETPALENTRIES),
	WmfRecordTypePolyPolygon									 = GDIP_WMF_RECORD_TO_EMFPLUS(META_POLYPOLYGON),
	WmfRecordTypeResizePalette								 = GDIP_WMF_RECORD_TO_EMFPLUS(META_RESIZEPALETTE),
	WmfRecordTypeDIBBitBlt										 = GDIP_WMF_RECORD_TO_EMFPLUS(META_DIBBITBLT),
	WmfRecordTypeDIBStretchBlt								 = GDIP_WMF_RECORD_TO_EMFPLUS(META_DIBSTRETCHBLT),
	WmfRecordTypeDIBCreatePatternBrush				 = GDIP_WMF_RECORD_TO_EMFPLUS(META_DIBCREATEPATTERNBRUSH),
	WmfRecordTypeStretchDIB										= GDIP_WMF_RECORD_TO_EMFPLUS(META_STRETCHDIB),
	WmfRecordTypeExtFloodFill									= GDIP_WMF_RECORD_TO_EMFPLUS(META_EXTFLOODFILL),
	WmfRecordTypeSetLayout										 = GDIP_WMF_RECORD_TO_EMFPLUS(0x0149),
	WmfRecordTypeResetDC											 = GDIP_WMF_RECORD_TO_EMFPLUS(0x014C),
	WmfRecordTypeStartDoc											= GDIP_WMF_RECORD_TO_EMFPLUS(0x014D),
	WmfRecordTypeStartPage										 = GDIP_WMF_RECORD_TO_EMFPLUS(0x004F),
	WmfRecordTypeEndPage											 = GDIP_WMF_RECORD_TO_EMFPLUS(0x0050),
	WmfRecordTypeAbortDoc											= GDIP_WMF_RECORD_TO_EMFPLUS(0x0052),
	WmfRecordTypeEndDoc												= GDIP_WMF_RECORD_TO_EMFPLUS(0x005E),
	WmfRecordTypeDeleteObject									= GDIP_WMF_RECORD_TO_EMFPLUS(META_DELETEOBJECT),
	WmfRecordTypeCreatePalette								 = GDIP_WMF_RECORD_TO_EMFPLUS(META_CREATEPALETTE),
	WmfRecordTypeCreateBrush									 = GDIP_WMF_RECORD_TO_EMFPLUS(0x00F8),
	WmfRecordTypeCreatePatternBrush						= GDIP_WMF_RECORD_TO_EMFPLUS(META_CREATEPATTERNBRUSH),
	WmfRecordTypeCreatePenIndirect						 = GDIP_WMF_RECORD_TO_EMFPLUS(META_CREATEPENINDIRECT),
	WmfRecordTypeCreateFontIndirect						= GDIP_WMF_RECORD_TO_EMFPLUS(META_CREATEFONTINDIRECT),
	WmfRecordTypeCreateBrushIndirect					 = GDIP_WMF_RECORD_TO_EMFPLUS(META_CREATEBRUSHINDIRECT),
	WmfRecordTypeCreateBitmapIndirect					= GDIP_WMF_RECORD_TO_EMFPLUS(0x02FD),
	WmfRecordTypeCreateBitmap									= GDIP_WMF_RECORD_TO_EMFPLUS(0x06FE),
	WmfRecordTypeCreateRegion									= GDIP_WMF_RECORD_TO_EMFPLUS(META_CREATEREGION),
	EmfRecordTypeHeader												= EMR_HEADER,
	EmfRecordTypePolyBezier										= EMR_POLYBEZIER,
	EmfRecordTypePolygon											 = EMR_POLYGON,
	EmfRecordTypePolyline											= EMR_POLYLINE,
	EmfRecordTypePolyBezierTo									= EMR_POLYBEZIERTO,
	EmfRecordTypePolyLineTo										= EMR_POLYLINETO,
	EmfRecordTypePolyPolyline									= EMR_POLYPOLYLINE,
	EmfRecordTypePolyPolygon									 = EMR_POLYPOLYGON,
	EmfRecordTypeSetWindowExtEx								= EMR_SETWINDOWEXTEX,
	EmfRecordTypeSetWindowOrgEx								= EMR_SETWINDOWORGEX,
	EmfRecordTypeSetViewportExtEx							= EMR_SETVIEWPORTEXTEX,
	EmfRecordTypeSetViewportOrgEx							= EMR_SETVIEWPORTORGEX,
	EmfRecordTypeSetBrushOrgEx								 = EMR_SETBRUSHORGEX,
	EmfRecordTypeEOF													 = EMR_EOF,
	EmfRecordTypeSetPixelV										 = EMR_SETPIXELV,
	EmfRecordTypeSetMapperFlags								= EMR_SETMAPPERFLAGS,
	EmfRecordTypeSetMapMode										= EMR_SETMAPMODE,
	EmfRecordTypeSetBkMode										 = EMR_SETBKMODE,
	EmfRecordTypeSetPolyFillMode							 = EMR_SETPOLYFILLMODE,
	EmfRecordTypeSetROP2											 = EMR_SETROP2,
	EmfRecordTypeSetStretchBltMode						 = EMR_SETSTRETCHBLTMODE,
	EmfRecordTypeSetTextAlign									= EMR_SETTEXTALIGN,
	EmfRecordTypeSetColorAdjustment						= EMR_SETCOLORADJUSTMENT,
	EmfRecordTypeSetTextColor									= EMR_SETTEXTCOLOR,
	EmfRecordTypeSetBkColor										= EMR_SETBKCOLOR,
	EmfRecordTypeOffsetClipRgn								 = EMR_OFFSETCLIPRGN,
	EmfRecordTypeMoveToEx											= EMR_MOVETOEX,
	EmfRecordTypeSetMetaRgn										= EMR_SETMETARGN,
	EmfRecordTypeExcludeClipRect							 = EMR_EXCLUDECLIPRECT,
	EmfRecordTypeIntersectClipRect						 = EMR_INTERSECTCLIPRECT,
	EmfRecordTypeScaleViewportExtEx						= EMR_SCALEVIEWPORTEXTEX,
	EmfRecordTypeScaleWindowExtEx							= EMR_SCALEWINDOWEXTEX,
	EmfRecordTypeSaveDC												= EMR_SAVEDC,
	EmfRecordTypeRestoreDC										 = EMR_RESTOREDC,
	EmfRecordTypeSetWorldTransform						 = EMR_SETWORLDTRANSFORM,
	EmfRecordTypeModifyWorldTransform					= EMR_MODIFYWORLDTRANSFORM,
	EmfRecordTypeSelectObject									= EMR_SELECTOBJECT,
	EmfRecordTypeCreatePen										 = EMR_CREATEPEN,
	EmfRecordTypeCreateBrushIndirect					 = EMR_CREATEBRUSHINDIRECT,
	EmfRecordTypeDeleteObject									= EMR_DELETEOBJECT,
	EmfRecordTypeAngleArc											= EMR_ANGLEARC,
	EmfRecordTypeEllipse											 = EMR_ELLIPSE,
	EmfRecordTypeRectangle										 = EMR_RECTANGLE,
	EmfRecordTypeRoundRect										 = EMR_ROUNDRECT,
	EmfRecordTypeArc													 = EMR_ARC,
	EmfRecordTypeChord												 = EMR_CHORD,
	EmfRecordTypePie													 = EMR_PIE,
	EmfRecordTypeSelectPalette								 = EMR_SELECTPALETTE,
	EmfRecordTypeCreatePalette								 = EMR_CREATEPALETTE,
	EmfRecordTypeSetPaletteEntries						 = EMR_SETPALETTEENTRIES,
	EmfRecordTypeResizePalette								 = EMR_RESIZEPALETTE,
	EmfRecordTypeRealizePalette								= EMR_REALIZEPALETTE,
	EmfRecordTypeExtFloodFill									= EMR_EXTFLOODFILL,
	EmfRecordTypeLineTo												= EMR_LINETO,
	EmfRecordTypeArcTo												 = EMR_ARCTO,
	EmfRecordTypePolyDraw											= EMR_POLYDRAW,
	EmfRecordTypeSetArcDirection							 = EMR_SETARCDIRECTION,
	EmfRecordTypeSetMiterLimit								 = EMR_SETMITERLIMIT,
	EmfRecordTypeBeginPath										 = EMR_BEGINPATH,
	EmfRecordTypeEndPath											 = EMR_ENDPATH,
	EmfRecordTypeCloseFigure									 = EMR_CLOSEFIGURE,
	EmfRecordTypeFillPath											= EMR_FILLPATH,
	EmfRecordTypeStrokeAndFillPath						 = EMR_STROKEANDFILLPATH,
	EmfRecordTypeStrokePath										= EMR_STROKEPATH,
	EmfRecordTypeFlattenPath									 = EMR_FLATTENPATH,
	EmfRecordTypeWidenPath										 = EMR_WIDENPATH,
	EmfRecordTypeSelectClipPath								= EMR_SELECTCLIPPATH,
	EmfRecordTypeAbortPath										 = EMR_ABORTPATH,
	EmfRecordTypeReserved_069									= 69,
	EmfRecordTypeGdiComment										= EMR_GDICOMMENT,
	EmfRecordTypeFillRgn											 = EMR_FILLRGN,
	EmfRecordTypeFrameRgn											= EMR_FRAMERGN,
	EmfRecordTypeInvertRgn										 = EMR_INVERTRGN,
	EmfRecordTypePaintRgn											= EMR_PAINTRGN,
	EmfRecordTypeExtSelectClipRgn							= EMR_EXTSELECTCLIPRGN,
	EmfRecordTypeBitBlt												= EMR_BITBLT,
	EmfRecordTypeStretchBlt										= EMR_STRETCHBLT,
	EmfRecordTypeMaskBlt											 = EMR_MASKBLT,
	EmfRecordTypePlgBlt												= EMR_PLGBLT,
	EmfRecordTypeSetDIBitsToDevice						 = EMR_SETDIBITSTODEVICE,
	EmfRecordTypeStretchDIBits								 = EMR_STRETCHDIBITS,
	EmfRecordTypeExtCreateFontIndirect				 = EMR_EXTCREATEFONTINDIRECTW,
	EmfRecordTypeExtTextOutA									 = EMR_EXTTEXTOUTA,
	EmfRecordTypeExtTextOutW									 = EMR_EXTTEXTOUTW,
	EmfRecordTypePolyBezier16									= EMR_POLYBEZIER16,
	EmfRecordTypePolygon16										 = EMR_POLYGON16,
	EmfRecordTypePolyline16										= EMR_POLYLINE16,
	EmfRecordTypePolyBezierTo16								= EMR_POLYBEZIERTO16,
	EmfRecordTypePolylineTo16									= EMR_POLYLINETO16,
	EmfRecordTypePolyPolyline16								= EMR_POLYPOLYLINE16,
	EmfRecordTypePolyPolygon16								 = EMR_POLYPOLYGON16,
	EmfRecordTypePolyDraw16										= EMR_POLYDRAW16,
	EmfRecordTypeCreateMonoBrush							 = EMR_CREATEMONOBRUSH,
	EmfRecordTypeCreateDIBPatternBrushPt			 = EMR_CREATEDIBPATTERNBRUSHPT,
	EmfRecordTypeExtCreatePen									= EMR_EXTCREATEPEN,
	EmfRecordTypePolyTextOutA									= EMR_POLYTEXTOUTA,
	EmfRecordTypePolyTextOutW									= EMR_POLYTEXTOUTW,
	EmfRecordTypeSetICMMode										= 98,
	EmfRecordTypeCreateColorSpace							= 99,
	EmfRecordTypeSetColorSpace								 = 100,
	EmfRecordTypeDeleteColorSpace							= 101,
	EmfRecordTypeGLSRecord										 = 102,
	EmfRecordTypeGLSBoundedRecord							= 103,
	EmfRecordTypePixelFormat									 = 104,
	EmfRecordTypeDrawEscape										= 105,
	EmfRecordTypeExtEscape										 = 106,
	EmfRecordTypeStartDoc											= 107,
	EmfRecordTypeSmallTextOut									= 108,
	EmfRecordTypeForceUFIMapping							 = 109,
	EmfRecordTypeNamedEscape									 = 110,
	EmfRecordTypeColorCorrectPalette					 = 111,
	EmfRecordTypeSetICMProfileA								= 112,
	EmfRecordTypeSetICMProfileW								= 113,
	EmfRecordTypeAlphaBlend										= 114,
	EmfRecordTypeSetLayout										 = 115,
	EmfRecordTypeTransparentBlt								= 116,
	EmfRecordTypeReserved_117									= 117,
	EmfRecordTypeGradientFill									= 118,
	EmfRecordTypeSetLinkedUFIs								 = 119,
	EmfRecordTypeSetTextJustification					= 120,
	EmfRecordTypeColorMatchToTargetW					 = 121,
	EmfRecordTypeCreateColorSpaceW						 = 122,
	EmfRecordTypeMax													 = 122,
	EmfRecordTypeMin													 = 1,
	EmfPlusRecordTypeInvalid									 = GDIP_EMFPLUS_RECORD_BASE,
	EmfPlusRecordTypeHeader,
	EmfPlusRecordTypeEndOfFile,
	EmfPlusRecordTypeComment,
	EmfPlusRecordTypeGetDC,
	EmfPlusRecordTypeMultiFormatStart,
	EmfPlusRecordTypeMultiFormatSection,
	EmfPlusRecordTypeMultiFormatEnd,
	EmfPlusRecordTypeObject,
	EmfPlusRecordTypeClear,
	EmfPlusRecordTypeFillRects,
	EmfPlusRecordTypeDrawRects,
	EmfPlusRecordTypeFillPolygon,
	EmfPlusRecordTypeDrawLines,
	EmfPlusRecordTypeFillEllipse,
	EmfPlusRecordTypeDrawEllipse,
	EmfPlusRecordTypeFillPie,
	EmfPlusRecordTypeDrawPie,
	EmfPlusRecordTypeDrawArc,
	EmfPlusRecordTypeFillRegion,
	EmfPlusRecordTypeFillPath,
	EmfPlusRecordTypeDrawPath,
	EmfPlusRecordTypeFillClosedCurve,
	EmfPlusRecordTypeDrawClosedCurve,
	EmfPlusRecordTypeDrawCurve,
	EmfPlusRecordTypeDrawBeziers,
	EmfPlusRecordTypeDrawImage,
	EmfPlusRecordTypeDrawImagePoints,
	EmfPlusRecordTypeDrawString,
	EmfPlusRecordTypeSetRenderingOrigin,
	EmfPlusRecordTypeSetAntiAliasMode,
	EmfPlusRecordTypeSetTextRenderingHint,
	EmfPlusRecordTypeSetTextContrast,
	EmfPlusRecordTypeSetGammaValue,
	EmfPlusRecordTypeSetInterpolationMode,
	EmfPlusRecordTypeSetPixelOffsetMode,
	EmfPlusRecordTypeSetCompositingMode,
	EmfPlusRecordTypeSetCompositingQuality,
	EmfPlusRecordTypeSave,
	EmfPlusRecordTypeRestore,
	EmfPlusRecordTypeBeginContainer,
	EmfPlusRecordTypeBeginContainerNoParams,
	EmfPlusRecordTypeEndContainer,
	EmfPlusRecordTypeSetWorldTransform,
	EmfPlusRecordTypeResetWorldTransform,
	EmfPlusRecordTypeMultiplyWorldTransform,
	EmfPlusRecordTypeTranslateWorldTransform,
	EmfPlusRecordTypeScaleWorldTransform,
	EmfPlusRecordTypeRotateWorldTransform,
	EmfPlusRecordTypeSetPageTransform,
	EmfPlusRecordTypeResetClip,
	EmfPlusRecordTypeSetClipRect,
	EmfPlusRecordTypeSetClipPath,
	EmfPlusRecordTypeSetClipRegion,
	EmfPlusRecordTypeOffsetClip,
	EmfPlusRecordTypeDrawDriverString,
	EmfPlusRecordTypeStrokeFillPath,
	EmfPlusRecordTypeSerializableObject,
	EmfPlusRecordTypeSetTSGraphics,
	EmfPlusRecordTypeSetTSClip,
	EmfPlusRecordTotal,
	EmfPlusRecordTypeMax = EmfPlusRecordTotal-1,
	EmfPlusRecordTypeMin = EmfPlusRecordTypeHeader
} EmfPlusRecordType;

typedef enum EmfToWmfBitsFlags {
	EmfToWmfBitsFlagsDefault = 0,
	EmfToWmfBitsFlagsEmbedEmf = 1,
	EmfToWmfBitsFlagsIncludePlaceable = 2,
	EmfToWmfBitsFlagsNoXORClip = 4
} EmfToWmfBitsFlags;

typedef enum EmfType {
	EmfTypeEmfOnly = 3,
	EmfTypeEmfPlusOnly = 4,
	EmfTypeEmfPlusDual = 5
} EmfType;

typedef enum EncoderParameterValueType {
	EncoderParameterValueTypeByte = 1,
	EncoderParameterValueTypeASCII = 2,
	EncoderParameterValueTypeShort = 3,
	EncoderParameterValueTypeLong = 4,
	EncoderParameterValueTypeRational = 5,
	EncoderParameterValueTypeLongRange = 6,
	EncoderParameterValueTypeUndefined = 7,
	EncoderParameterValueTypeRationalRange = 8,
	EncoderParameterValueTypePointer = 9
} EncoderParameterValueType;

typedef enum EncoderValue {
	EncoderValueColorTypeCMYK = 0,
	EncoderValueColorTypeYCCK = 1,
	EncoderValueCompressionLZW = 2,
	EncoderValueCompressionCCITT3 = 3,
	EncoderValueCompressionCCITT4 = 4,
	EncoderValueCompressionRle = 5,
	EncoderValueCompressionNone = 6,
	EncoderValueScanMethodInterlaced = 7,
	EncoderValueScanMethodNonInterlaced = 8,
	EncoderValueVersionGif87 = 9,
	EncoderValueVersionGif89 = 10,
	EncoderValueRenderProgressive = 11,
	EncoderValueRenderNonProgressive = 12,
	EncoderValueTransformRotate90 = 13,
	EncoderValueTransformRotate180 = 14,
	EncoderValueTransformRotate270 = 15,
	EncoderValueTransformFlipHorizontal = 16,
	EncoderValueTransformFlipVertical = 17,
	EncoderValueMultiFrame = 18,
	EncoderValueLastFrame = 19,
	EncoderValueFlush = 20,
	EncoderValueFrameDimensionTime = 21,
	EncoderValueFrameDimensionResolution = 22,
	EncoderValueFrameDimensionPage = 23
} EncoderValue;

typedef enum FillMode {
	FillModeAlternate = 0,
	FillModeWinding = 1
} FillMode;

typedef enum FlushIntention {
	FlushIntentionFlush = 0,
	FlushIntentionSync = 1
} FlushIntention;

typedef enum FontStyle {
	FontStyleRegular = 0,
	FontStyleBold = 1,
	FontStyleItalic = 2,
	FontStyleBoldItalic = 3,
	FontStyleUnderline = 4,
	FontStyleStrikeout = 8
} FontStyle;

typedef enum HatchStyle {
	HatchStyleHorizontal = 0,
	HatchStyleVertical = 1,
	HatchStyleForwardDiagonal = 2,
	HatchStyleBackwardDiagonal = 3,
	HatchStyleCross = 4,
	HatchStyleLargeGrid = 4,
	HatchStyleDiagonalCross = 5,
	HatchStyle05Percent = 6,
	HatchStyle10Percent = 7,
	HatchStyle20Percent = 8,
	HatchStyle25Percent = 9,
	HatchStyle30Percent = 10,
	HatchStyle40Percent = 11,
	HatchStyle50Percent = 12,
	HatchStyle60Percent = 13,
	HatchStyle70Percent = 14,
	HatchStyle75Percent = 15,
	HatchStyle80Percent = 16,
	HatchStyle90Percent = 17,
	HatchStyleLightDownwardDiagonal = 18,
	HatchStyleLightUpwardDiagonal = 19,
	HatchStyleDarkDownwardDiagonal = 20,
	HatchStyleDarkUpwardDiagonal = 21,
	HatchStyleWideDownwardDiagonal = 22,
	HatchStyleWideUpwardDiagonal = 23,
	HatchStyleLightVertical = 24,
	HatchStyleLightHorizontal = 25,
	HatchStyleNarrowVertical = 26,
	HatchStyleNarrowHorizontal = 27,
	HatchStyleDarkVertical = 28,
	HatchStyleDarkHorizontal = 29,
	HatchStyleDashedDownwardDiagonal = 30,
	HatchStyleDashedUpwardDiagonal = 31,
	HatchStyleDashedHorizontal = 32,
	HatchStyleDashedVertical = 33,
	HatchStyleSmallConfetti = 34,
	HatchStyleLargeConfetti = 35,
	HatchStyleZigZag = 36,
	HatchStyleWave = 37,
	HatchStyleDiagonalBrick = 38,
	HatchStyleHorizontalBrick = 39,
	HatchStyleWeave = 40,
	HatchStylePlaid = 41,
	HatchStyleDivot = 42,
	HatchStyleDottedGrid = 43,
	HatchStyleDottedDiamond = 44,
	HatchStyleShingle = 45,
	HatchStyleTrellis = 46,
	HatchStyleSphere = 47,
	HatchStyleSmallGrid = 48,
	HatchStyleSmallCheckerBoard = 49,
	HatchStyleLargeCheckerBoard = 50,
	HatchStyleOutlinedDiamond = 51,
	HatchStyleSolidDiamond = 52,
	HatchStyleTotal = 53,
	HatchStyleMin = HatchStyleHorizontal,
	HatchStyleMax = HatchStyleTotal - 1
} HatchStyle;

typedef enum HotkeyPrefix {
	HotkeyPrefixNone = 0,
	HotkeyPrefixShow = 1,
	HotkeyPrefixHide = 2
} HotkeyPrefix;

typedef enum ImageType {
	ImageTypeUnknown = 0,
	ImageTypeBitmap = 1,
	ImageTypeMetafile = 2
} ImageType;

typedef enum InterpolationMode {
	/*InterpolationModeInvalid = QualityModeInvalid,*/
	InterpolationModeDefault = 0,
	InterpolationModeLowQuality = 1,
	InterpolationModeHighQuality = 2,
	InterpolationModeBilinear = 3,
	InterpolationModeBicubic = 4,
	InterpolationModeNearestNeighbor = 5,
	InterpolationModeHighQualityBilinear = 6,
	InterpolationModeHighQualityBicubic = 7
} InterpolationMode;

typedef enum LinearGradientMode {
	LinearGradientModeHorizontal = 0,
	LinearGradientModeVertical = 1,
	LinearGradientModeForwardDiagonal = 2,
	LinearGradientModeBackwardDiagonal = 3
} LinearGradientMode;

typedef enum LineCap {
	LineCapFlat = 0,
	LineCapSquare = 1,
	LineCapRound = 2,
	LineCapTriangle = 3,
	LineCapNoAnchor = 16,
	LineCapSquareAnchor = 17,
	LineCapRoundAnchor = 18,
	LineCapDiamondAnchor = 19,
	LineCapArrowAnchor = 20,
	LineCapCustom = 255
} LineCap;

typedef enum LineJoin {
	LineJoinMiter = 0,
	LineJoinBevel = 1,
	LineJoinRound = 2,
	LineJoinMiterClipped = 3
} LineJoin;

typedef enum MatrixOrder {
	MatrixOrderPrepend = 0,
	MatrixOrderAppend = 1
} MatrixOrder;

typedef enum MetafileFrameUnit {
	MetafileFrameUnitPixel = 2,
	MetafileFrameUnitPoint = 3,
	MetafileFrameUnitInch = 4,
	MetafileFrameUnitDocument = 5,
	MetafileFrameUnitMillimeter = 6,
	MetafileFrameUnitGdi = 7
} MetafileFrameUnit;

typedef enum MetafileType {
	MetafileTypeInvalid = 0,
	MetafileTypeWmf = 1,
	MetafileTypeWmfPlaceable = 2,
	MetafileTypeEmf = 3,
	MetafileTypeEmfPlusOnly = 4,
	MetafileTypeEmfPlusDual = 5
} MetafileType;

typedef enum ObjectType {
	ObjectTypeInvalid = 0,
	ObjectTypeBrush = 1,
	ObjectTypePen = 2,
	ObjectTypePath = 3,
	ObjectTypeRegion = 4,
	ObjectTypeFont = 5,
	ObjectTypeStringFormat = 6,
	ObjectTypeImageAttributes = 7,
	ObjectTypeCustomLineCap = 8,
	ObjectTypeGraphics = 9,
	ObjectTypeMin = ObjectTypeBrush,
	ObjectTypeMax = ObjectTypeGraphics
} ObjectType;

typedef enum PathPointType {
	PathPointTypeStart = 0x00,
	PathPointTypeLine = 0x01,
	PathPointTypeBezier = 0x03,
	PathPointTypeBezier3 = 0x03,
	PathPointTypePathTypeMask = 0x07,
	PathPointTypePathDashMode = 0x10,
	PathPointTypePathMarker = 0x20,
	PathPointTypeCloseSubpath = 0x80
} PathPointType;

typedef enum PenAlignment {
	PenAlignmentCenter = 0,
	PenAlignmentInset = 1
} PenAlignment;

typedef enum PenType {
	PenTypeUnknown = -1,
	PenTypeSolidColor = 0,
	PenTypeHatchFill = 1,
	PenTypeTextureFill = 2,
	PenTypePathGradient = 3,
	PenTypeLinearGradient = 4
} PenType;

typedef enum PixelOffsetMode {
	/*PixelOffsetModeInvalid = QualityModeInvalid,*/
	PixelOffsetModeDefault = 0,
	PixelOffsetModeHighSpeed = 1,
	PixelOffsetModeHighQuality = 2,
	PixelOffsetModeNone = 3,
	PixelOffsetModeHalf = 4
} PixelOffsetMode;

typedef enum QualityMode {
	/*QualityModeInvalid = ?,*/
	QualityModeDefault = 0,
	QualityModeLow = 1,
	QualityModeHigh = 2
} QualityMode;

typedef enum SmoothingMode {
	/*SmoothingModeInvalid = QualityModeInvalid,*/
	SmoothingModeDefault = 0,
	SmoothingModeHighSpeed = 1,
	SmoothingModeHighQuality = 2,
	SmoothingModeNone = 3,
	SmoothingModeAntiAlias8x4 = 4,
	SmoothingModeAntiAlias = 4,
	SmoothingModeAntiAlias8x8 = 5
} SmoothingMode;

typedef enum StringAlignment {
	StringAlignmentNear = 0,
	StringAlignmentCenter = 1,
	StringAlignmentFar = 2
} StringAlignment;

typedef enum StringDigitSubstitute {
	StringDigitSubstituteUser = 0,
	StringDigitSubstituteNone = 1,
	StringDigitSubstituteNational = 2,
	StringDigitSubstituteTraditional = 3
} StringDigitSubstitute;

typedef enum StringFormatFlags {
	StringFormatFlagsDirectionRightToLeft = 0x00000001,
	StringFormatFlagsDirectionVertical = 0x00000002,
	StringFormatFlagsNoFitBlackBox = 0x00000004,
	StringFormatFlagsDisplayFormatControl = 0x00000020,
	StringFormatFlagsNoFontFallback = 0x00000400,
	StringFormatFlagsMeasureTrailingSpaces = 0x00000800,
	StringFormatFlagsNoWrap = 0x00001000,
	StringFormatFlagsLineLimit = 0x00002000,
	StringFormatFlagsNoClip = 0x00004000
} StringFormatFlags;

typedef enum StringTrimming {
	StringTrimmingNone = 0,
	StringTrimmingCharacter = 1,
	StringTrimmingWord = 2,
	StringTrimmingEllipsisCharacter = 3,
	StringTrimmingEllipsisWord = 4,
	StringTrimmingEllipsisPath = 5
} StringTrimming;

typedef enum TextRenderingHint {
	TextRenderingHintSystemDefault = 0,
	TextRenderingHintSingleBitPerPixelGridFit = 1,
	TextRenderingHintSingleBitPerPixel = 2,
	TextRenderingHintAntiAliasGridFit = 3,
	TextRenderingHintAntiAlias = 4,
	TextRenderingHintClearTypeGridFit = 5
} TextRenderingHint;

typedef enum Unit {
	UnitWorld = 0,
	UnitDisplay = 1,
	UnitPixel = 2,
	UnitPoint = 3,
	UnitInch = 4,
	UnitDocument = 5,
	UnitMillimeter = 6
} Unit;

typedef enum WarpMode {
	WarpModePerspective = 0,
	WarpModeBilinear = 1
} WarpMode;

typedef enum WrapMode {
	WrapModeTile = 0,
	WrapModeTileFlipX = 1,
	WrapModeTileFlipY = 2,
	WrapModeTileFlipXY = 3,
	WrapModeClamp = 4
} WrapMode;

typedef enum GpTestControlEnum {
	TestControlForceBilinear = 0,
	TestControlForceNoICM = 1,
	TestControlGetBuildNumber = 2
} GpTestControlEnum;

typedef enum BrushType GpBrushType;
typedef enum CombineMode GpCombineMode;
typedef enum CompositingMode GpCompositingMode;
typedef enum CompositingQuality GpCompositingQuality;
typedef enum CoordinateSpace GpCoordinateSpace;
typedef enum CustomLineCapType GpCustomLineCapType;
typedef enum DashCap GpDashCap;
typedef enum DashStyle GpDashStyle;
typedef enum DitherType GpDitherType;
typedef enum DriverStringOptions GpDriverStringOptions;
typedef enum EmfPlusRecordType GpEmfPlusRecordType;
typedef enum EmfToWmfBitsFlags GpEmfToWmfBitsFlags;
typedef enum EmfType GpEmfType;
typedef enum EncoderParameterValueType GpEncoderParameterValueType;
typedef enum EncoderValue GpEncoderValue;
typedef enum FillMode GpFillMode;
typedef enum FlushIntention GpFlushIntention;
typedef enum FontStyle GpFontStyle;
typedef enum HatchStyle GpHatchStyle;
typedef enum HotkeyPrefix GpHotkeyPrefix;
typedef enum ImageType GpImageType;
typedef enum InterpolationMode GpInterpolationMode;
typedef enum LinearGradientMode GpLinearGradientMode;
typedef enum LineCap GpLineCap;
typedef enum LineJoin GpLineJoin;
typedef enum MatrixOrder GpMatrixOrder;
typedef enum MetafileFrameUnit GpMetafileFrameUnit;
typedef enum MetafileType GpMetafileType;
typedef enum ObjectType GpObjectType;
typedef enum PathPointType GpPathPointType;
typedef enum PenAlignment GpPenAlignment;
typedef enum PenType GpPenType;
typedef enum PixelOffsetMode GpPixelOffsetMode;
typedef enum QualityMode GpQualityMode;
typedef enum SmoothingMode GpSmoothingMode;
typedef enum StringAlignment GpStringAlignment;
typedef enum StringDigitSubstitute GpStringDigitSubstitute;
typedef enum StringFormatFlags GpStringFormatFlags;
typedef enum StringTrimming GpStringTrimming;
typedef enum TextRenderingHint GpTextRenderingHint;
typedef enum Unit GpUnit;
typedef enum WarpMode GpWarpMode;
typedef enum WrapMode GpWrapMode;

/* Imaging enums. */

typedef enum ImageCodecFlags {
	ImageCodecFlagsEncoder = 0x00000001,
	ImageCodecFlagsDecoder = 0x00000002,
	ImageCodecFlagsSupportBitmap = 0x00000004,
	ImageCodecFlagsSupportVector = 0x00000008,
	ImageCodecFlagsSeekableEncode = 0x00000010,
	ImageCodecFlagsBlockingDecode = 0x00000020,
	ImageCodecFlagsBuiltin = 0x00010000,
	ImageCodecFlagsSystem = 0x00020000,
	ImageCodecFlagsUser = 0x00040000
} ImageCodecFlags;

typedef enum ImageFlags {
	ImageFlagsNone = 0,
	ImageFlagsScalable = 0x00000001,
	ImageFlagsHasAlpha = 0x00000002,
	ImageFlagsHasTranslucent = 0x00000004,
	ImageFlagsPartiallyScalable = 0x00000008,
	ImageFlagsColorSpaceRGB = 0x00000010,
	ImageFlagsColorSpaceCMYK = 0x00000020,
	ImageFlagsColorSpaceGRAY = 0x00000040,
	ImageFlagsColorSpaceYCBCR = 0x00000080,
	ImageFlagsColorSpaceYCCK = 0x00000100,
	ImageFlagsHasRealDPI = 0x00001000,
	ImageFlagsHasRealPixelSize = 0x00002000,
	ImageFlagsReadOnly = 0x00010000,
	ImageFlagsCaching = 0x00020000
} ImageFlags;

typedef enum ImageLockMode {
	ImageLockModeRead = 1,
	ImageLockModeWrite = 2,
	ImageLockModeUserInputBuf = 4
} ImageLockMode;

typedef enum ItemDataPosition {
	ItemDataPositionAfterHeader = 0,
	ItemDataPositionAfterPalette = 1,
	ItemDataPositionAfterBits = 2
} ItemDataPosition;

typedef enum RotateFlipType {
	RotateNoneFlipNone = 0,
	Rotate90FlipNone = 1,
	Rotate180FlipNone = 2,
	Rotate270FlipNone = 3,
	RotateNoneFlipX = 4,
	Rotate90FlipX = 5,
	Rotate180FlipX = 6,
	Rotate270FlipX = 7,
	Rotate180FlipXY = 0,
	Rotate270FlipXY = 1,
	RotateNoneFlipXY = 2,
	Rotate90FlipXY = 3,
	Rotate180FlipY = 4,
	Rotate270FlipY = 5,
	RotateNoneFlipY = 6,
	Rotate90FlipY = 7
} RotateFlipType;

typedef struct BitmapData {
	UINT Width;
	UINT Height;
	INT Stride;
	INT PixelFormat;	/* MSDN: "PixelFormat PixelFormat;" */
	VOID *Scan0;
	UINT_PTR Reserved;
} BitmapData;

typedef struct EncoderParameter {
	GUID Guid;
	ULONG NumberOfValues;
	ULONG Type;
	VOID *Value;
} EncoderParameter;

typedef struct EncoderParameters {
	UINT Count;
	EncoderParameter Parameter[1];
} EncoderParameters;

typedef struct ImageCodecInfo {
	CLSID Clsid;
	GUID FormatID;
	WCHAR *CodecName;
	WCHAR *DllName;
	WCHAR *FormatDescription;
	WCHAR *FilenameExtension;
	WCHAR *MimeType;
	DWORD Flags;
	DWORD Version;
	DWORD SigCount;
	DWORD SigSize;
	BYTE *SigPattern;
	BYTE *SigMask;
} ImageCodecInfo;

/* FIXME: The order of fields is probably wrong. Please don't use this
 * structure until this problem is resolved!	Can't test because
 * ImageItemData is not supported by the redistributable GDI+ 1.0 DLL. */
typedef struct ImageItemData {
	UINT Size;
	UINT Position;
	VOID *Desc;
	UINT DescSize;
	UINT *Data;
	UINT DataSize;
	UINT Cookie;
} ImageItemData;

typedef struct PropertyItem {
	PROPID id;
	ULONG length;
	WORD type;
	VOID *value;
} PropertyItem;

/* Pixel formats enums. */

typedef enum PaletteFlags {
	PaletteFlagsHasAlpha = 1,
	PaletteFlagsGrayScale = 2,
	PaletteFlagsHalftone = 4
} PaletteFlags;

typedef enum PaletteType {
	PaletteTypeCustom = 0,
	PaletteTypeOptimal = 1,
	PaletteTypeFixedBW = 2,
	PaletteTypeFixedHalftone8 = 3,
	PaletteTypeFixedHalftone27 = 4,
	PaletteTypeFixedHalftone64 = 5,
	PaletteTypeFixedHalftone125 = 6,
	PaletteTypeFixedHalftone216 = 7,
	PaletteTypeFixedHalftone252 = 8,
	PaletteTypeFixedHalftone256 = 9
} PaletteType;

typedef struct ColorPalette {
	UINT Flags;
	UINT Count;
	ARGB Entries[1];
} ColorPalette;

/* Color matrix enums. */

typedef enum ColorAdjustType {
	ColorAdjustTypeDefault = 0,
	ColorAdjustTypeBitmap = 1,
	ColorAdjustTypeBrush = 2,
	ColorAdjustTypePen = 3,
	ColorAdjustTypeText = 4,
	ColorAdjustTypeCount = 5,
	ColorAdjustTypeAny = 6
} ColorAdjustType;

typedef enum ColorMatrixFlags {
	ColorMatrixFlagsDefault = 0,
	ColorMatrixFlagsSkipGrays = 1,
	ColorMatrixFlagsAltGray = 2
} ColorMatrixFlags;

typedef enum HistogramFormat {
	HistogramFormatARGB = 0,
	HistogramFormatPARGB = 1,
	HistogramFormatRGB = 2,
	HistogramFormatGray = 3,
	HistogramFormatB = 4,
	HistogramFormatG = 5,
	HistogramFormatR = 6,
	HistogramFormatA = 7
} HistogramFormat;

typedef enum ColorChannelFlags {
	ColorChannelFlagsC = 0,
	ColorChannelFlagsM = 1,
	ColorChannelFlagsY = 2,
	ColorChannelFlagsK = 3,
	ColorChannelFlagsLast = 4
} ColorChannelFlags;

typedef struct tagENHMETAHEADER3 {
	DWORD iType;
	DWORD size;
	RECTL rclBounds;
	RECTL rclFrame;
	DWORD dSignature;
	DWORD nVersion;
	DWORD nBytes;
	DWORD nRecords;
	WORD nHandles;
	WORD sReserved;
	DWORD nDescription;
	DWORD offDescription;
	DWORD nPalEntries;
	SIZEL szlDevice;
	SIZEL szlMillimeters;
} ENHMETAHEADER3,*LPENHMETAHEADER3;

typedef struct PWMFRect16 {
	INT16 Left;
	INT16 Top;
	INT16 Right;
	INT16 Bottom;
} PWMFRect16;

typedef struct WmfPlaceableFileHeader {
	UINT32 Key;
	INT16 Hmf;
	PWMFRect16 BoundingBox;
	INT16 Inch;
	UINT32 Reserved;
	INT16 Checksum;
} WmfPlaceableFileHeader;

typedef struct MetafileHeader {
	MetafileType Type;
	UINT Size;
	UINT Version;
	UINT EmfPlusFlags;
	REAL DpiX;
	REAL DpiY;
	INT X;
	INT Y;
	INT Width;
	INT Height;
	union {
		METAHEADER WmfHeader;
		ENHMETAHEADER3 EmfHeader;
	};
	INT EmfPlusHeaderSize;
	INT LogicalDpiX;
	INT LogicalDpiY;
} MetafileHeader;

typedef struct Color
{
	ARGB Value;
} Color;

typedef struct ColorMap {
	Color oldColor;
	Color newColor;
} ColorMap;

typedef struct ColorMatrix {
	REAL m[5][5];
} ColorMatrix;

typedef struct Point {
	INT X;
	INT Y;
} Point;
typedef Point GpPoint;

typedef struct PointF {
	REAL X;
	REAL Y;
} PointF;
typedef PointF GpPointF;

typedef struct Rect {
	INT X;
	INT Y;
	INT Width;
	INT Height;
} Rect;
typedef Rect GpRect;

typedef struct RectF {
	REAL X;
	REAL Y;
	REAL Width;
	REAL Height;
} RectF;
typedef RectF GpRectF;

typedef struct CharacterRange {
	INT First;
	INT Length;
} CharacterRange;

typedef struct PathData {
	INT Count;
	PointF *Points;
	BYTE *Types;
} PathData;

typedef struct GdiplusStartupInput {
	UINT32 GdiplusVersion;
	DebugEventProc DebugEventCallback;
	BOOL SuppressBackgroundThread;
	BOOL SuppressExternalCodecs;
} GdiplusStartupInput;

typedef GpStatus (__stdcall *NotificationHookProc)(ULONG_PTR *token);
typedef VOID (__stdcall *NotificationUnhookProc)(ULONG_PTR token);

typedef struct GdiplusStartupOutput {
	NotificationHookProc NotificationHook;
	NotificationUnhookProc NotificationUnhook;
} GdiplusStartupOutput;

typedef void CGpEffect;
typedef void GpAdjustableArrowCap;
typedef void GpBitmap;
typedef void GpBrush;
typedef void GpCachedBitmap;
typedef void GpCustomLineCap;
typedef void GpFont;
typedef void GpFontFamily;
typedef void GpFontCollection;
typedef void GpGraphics;
typedef void GpHatch;
typedef void GpImage;
typedef void GpImageAttributes;
typedef void GpLineGradient;
typedef void GpMatrix;
typedef void GpMetafile;
typedef void GpPath;
typedef void GpPathData;
typedef void GpPathGradient;
typedef void GpPathIterator;
typedef void GpPen;
typedef void GpRegion;
typedef void GpSolidFill;
typedef void GpStringFormat;
typedef void GpTexture;

/* From GdiPlusInit.h. */
GpStatus WINAPI GdiplusStartup(OUT ULONG_PTR *token, const GdiplusStartupInput *input, OUT GdiplusStartupOutput *output);
VOID WINAPI GdiplusShutdown(ULONG_PTR token);

/* This header is provided with "&" operators that must be replaced by "*". */
#include <GdiPlusFlat.h>

#endif


#endif /* RT_WIN32_EX_GUI_OS_DEFINES_H */
