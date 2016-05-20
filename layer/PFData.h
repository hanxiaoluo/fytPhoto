#if !defined (PD_DATA_HEAD_FILE_SEAGULL_000314)
#define PD_DATA_HEAD_FILE_SEAGULL_000314
//PhotoFamily data

//Manage part




//Editor part

#define WM_PH_USER						1500
#define IDW_EDIT_TBWND                  WM_PH_USER + 1
#define IDW_EDIT_PREVWND                WM_PH_USER + 2
#define IDW_EDIT_IMAGELISTWND           WM_PH_USER + 3
#define IDW_EDIT_SAMPLEWND				WM_PH_USER + 4
#define IDW_EDIT_TEMPLATEWND			WM_PH_USER + 5

#define IDT_TOOLBAR_ADJ                 WM_PH_USER + 10
#define IDT_TOOLBAR_EFF                 WM_PH_USER + 11
#define IDT_TOOLBAR_DIS                 WM_PH_USER + 12
#define IDT_TOOLBAR_PHO                 WM_PH_USER + 13

#define IDT_ADJ_ROTATE                  WM_PH_USER + 15
#define IDT_ADJ_RESIZE                  WM_PH_USER + 16
#define IDT_ADJ_LIGHT                   WM_PH_USER + 17
#define IDT_ADJ_COLOR                   WM_PH_USER + 18
#define IDT_ADJ_SATURA                  WM_PH_USER + 19
#define IDT_EFF_SHARP                   WM_PH_USER + 20
#define IDT_EFF_MOSAIC                  WM_PH_USER + 21
#define IDT_EFF_EMBOSS                  WM_PH_USER + 22
#define IDT_DIS_SLOPE                   WM_PH_USER + 25
#define IDT_DIS_SPHESI                  WM_PH_USER + 26
#define IDT_DIS_PINCH                   WM_PH_USER + 27
#define IDT_DIS_TWIRL                   WM_PH_USER + 28
#define IDT_DIS_WAVE                    WM_PH_USER + 29
#define IDT_PHO_EDGES                   WM_PH_USER + 30
#define IDT_PHO_FRAMES                  WM_PH_USER + 31
#define IDT_PHO_CARDS                   WM_PH_USER + 32
#define IDT_PHO_CALENDAR                WM_PH_USER + 33
#define IDT_PHO_STATIONARY              WM_PH_USER + 34

#define IDC_ADV_LIGHT                   WM_PH_USER + 40
#define IDC_ADV_COLOR                   WM_PH_USER + 41
#define IDC_ADV_SATURA                  WM_PH_USER + 42
#define IDC_ADV_SHARP                   WM_PH_USER + 43
#define IDC_ADV_MOSAIC                  WM_PH_USER + 44
#define IDC_ADV_EMBOSS                  WM_PH_USER + 45
#define IDC_ADV_SLOPE                   WM_PH_USER + 46
#define IDC_ADV_SPHESI                  WM_PH_USER + 47
#define IDC_ADV_PINCH                   WM_PH_USER + 48
#define IDC_ADV_TWIRL                   WM_PH_USER + 49
#define IDC_ADV_WAVE                    WM_PH_USER + 50

#define IDC_EDIT_TEXT					WM_PH_USER + 55
#define IDC_EDIT_CALENDAR				WM_PH_USER + 56
#define IDC_EDIT_TEXT_RETN				WM_PH_USER + 57
#define IDC_EDIT_CALEND_RETN			WM_PH_USER + 58

#define PREVWNDWIDTH		448			//preview window width
#define PREVWNDHEIGH		428			//preview window heigth

#define EDITUNDOMAXCOUNT    5			//Edit undo list max count

// scaling when preview object
// 1/10, 1/8, 1/6, 1/4, 1/3, 1/2, 1, 2, 3, 4, 5, 6, 8, 10, 12, 16  Total : 16 values
static double dScaling[] = { 0.100, 0.125, 0.167, 0.250, 0.333, 0.50, 
							1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 8.0, 10.0, 12.0, 16.0 };

// Define operation panel edit type
enum EDITTYPE
{
	EDIT_NONE = 0, EDIT_ROTATE, EDIT_RESIZE, EDIT_LIGHT, EDIT_COLOR, EDIT_SATURATION,
	EDIT_SHARPNESS, EDIT_MOSAIC, EDIT_EMBOSS,
	EDIT_SLOPE, EDIT_SPHESIZE, EDIT_PINCH, EDIT_TWIRL, EDIT_WAVE,
	EDIT_LIGHT_SAMPLE, EDIT_COLOR_SAMPLE, EDIT_SATURATION_SAMPLE,
	EDIT_SHARPNESS_SAMPLE, EDIT_MOSAIC_SAMPLE, EDIT_EMBOSS_SAMPLE,
	EDIT_SLOPE_SAMPLE, EDIT_SPHESIZE_SAMPLE, EDIT_PINCH_SAMPLE, EDIT_TWIRL_SAMPLE, EDIT_WAVE_SAMPLE,
	EDIT_EDGE, EDIT_FRAME, EDIT_CARD, EDIT_CALENDAR, EDIT_STATIONARY,
	EDIT_TEXT, EDIT_CALENDAR_SET,
};

// Define object type
enum OBJECTTYPE
{
	OBJECT_NONE = 0, OBJECT_ALGIMG, OBJECT_EDGE, OBJECT_FRAME,
	OBJECT_CARD, OBJECT_CALENDAR, OBJECT_STATIONARY
};

// Define task type
enum TASKTYPE
{
	TASK_NONE = 0, TASK_ADJUST, TASK_EFFECT, TASK_DISTORT, TASK_PHOTOFUN,
	TASK_CARD, TASK_CALENDAR
};

// Define preview type
enum PREVIEWTYPE
{
	PREVIEW_NONE = 0, PREVIEW_ACTUALSIZE, PREVIEW_FITINWND, PREVIEW_ZOOM
};

// Define *.APH FileHeader;
typedef struct tag_APH_FILEHEADER
{
	TCHAR		signature[8];	/* must equal to "PHOTOFUN" */
	WORD		wVersion;		/* Version */
	DWORD		dwreserved;		/* Reserved */
	OBJECTTYPE	Object_Type;	/* Object type */
	DWORD		dwWidth;		/* Template Width */
	DWORD		dwHeight;		/* Template Height */
	DWORD		dwSize;			/* Object image size, 0 mean no object image */
}_APH_FILEHEADER;

#endif