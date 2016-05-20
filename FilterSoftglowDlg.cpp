// FilterSoftglowDlg.cpp : implementation file
//

#include "stdafx.h"
#include "fytPhoto.h"
#include "FilterSoftglowDlg.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFilterSoftglowDlg dialog

#ifndef CLAMP
#define CLAMP(x,l,u) ((x)<(l)?(l):((x)>(u)?(u):(x)))
#endif

/**
 * CLAMP0255:
 * @a: the value to be clamped.
 *
 * This macro clamps its argument @a, an int32-range int, between 0
 * and 255 inclusive.
 **/
#define CLAMP0255(a)  CLAMP(a,0,255)

#define G_PI    3.14159265358979323846

#ifndef MAX
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif
#ifndef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

#define TILE_CACHE_SIZE 48
#define SIGMOIDAL_BASE   2
#define SIGMOIDAL_RANGE 20

#define INT_MULT(a,b,t)  ((t) = (a) * (b) + 0x80, ((((t) >> 8) + (t)) >> 8))

typedef struct
{
  double glow_radius;
  double brightness;
  double sharpness;
} SoftglowVals;

#define ROUND(x) ((int) ((x) + 0.5))

SoftglowVals svals;

static void
find_constants (double n_p[],
                double n_m[],
                double d_p[],
                double d_m[],
                double bd_p[],
                double bd_m[],
                double std_dev);
static void transfer_pixels (double *src1,
                 double *src2,
                 BYTE  *dest,
                 int     jump,
                 int     width);

CFilterSoftglowDlg::CFilterSoftglowDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFilterSoftglowDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFilterSoftglowDlg)
	m_strValue1 = _T("");
	m_strValue2 = _T("");
	m_strValue3 = _T("");
	//}}AFX_DATA_INIT
}


void CFilterSoftglowDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFilterSoftglowDlg)
	DDX_Control(pDX, IDC_EDIT3, m_Edit3);
	DDX_Control(pDX, IDC_EDIT2, m_Edit2);
	DDX_Control(pDX, IDC_EDIT1, m_Edit1);
	DDX_Text(pDX, IDC_EDIT1, m_strValue1);
	DDX_Text(pDX, IDC_EDIT2, m_strValue2);
	DDX_Text(pDX, IDC_EDIT3, m_strValue3);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFilterSoftglowDlg, CDialog)
	//{{AFX_MSG_MAP(CFilterSoftglowDlg)
	ON_WM_PAINT()
	ON_EN_CHANGE(IDC_EDIT1, OnChangeEdit1)
	ON_EN_CHANGE(IDC_EDIT2, OnChangeEdit2)
	ON_EN_CHANGE(IDC_EDIT3, OnChangeEdit3)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFilterSoftglowDlg message handlers

BOOL CFilterSoftglowDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	//bool b = m_image.load("c:\\1.jpg");

	m_wndLineTrackBar1.SubclassDlgItem(IDC_CUSTOM1, this);
	m_wndLineTrackBar1.InitControl (IDC_EDIT1, RGB(0,0,255), 1, 25,50,4);

	m_wndLineTrackBar2.SubclassDlgItem(IDC_CUSTOM2, this);
	m_wndLineTrackBar2.InitControl (IDC_EDIT2,RGB(255,0,0),0, 50,100,2);

	m_wndLineTrackBar3.SubclassDlgItem(IDC_CUSTOM3, this);
	m_wndLineTrackBar3.InitControl (IDC_EDIT3,RGB(0,255,0),0, 50,100,2);

	//SoftglowVals svals = {10, 0.5, 0.6};
	svals.glow_radius = 10;
    svals.brightness = 0.5;
	svals.sharpness = 0.6;

	softglow();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



/**
 * gimp_rgb_to_l_int:
 * @red: Red channel
 * @green: Green channel
 * @blue: Blue channel
 *
 * Calculates the lightness value of an RGB triplet with the formula
 * L = (max(R, G, B) + min (R, G, B)) / 2
 *
 * Return value: Luminance vaue corresponding to the input RGB value
 **/
int
gimp_rgb_to_l_int (int red,
                   int green,
                   int blue)
{
  int min, max;

  if (red > green)
    {
      max = MAX (red,   blue);
      min = MIN (green, blue);
    }
  else
    {
      max = MAX (green, blue);
      min = MIN (red,   blue);
    }

  return ROUND ((max + min) / 2.0);
}

BOOL g_fSaveDataToBitmap(BITMAPINFOHEADER *pBmpFileInfo, TCHAR *szFileName, LPVOID pBuffer, int iBufLeng)
{
	BITMAPFILEHEADER bmpFileHeader;
	HANDLE hFile = NULL;
	DWORD dwTotalWriten = 0;
	DWORD dwWriten;

	bmpFileHeader.bfType = 0x4d42; //'BM';
	bmpFileHeader.bfOffBits=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);
	bmpFileHeader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)+ iBufLeng;
/*
int size = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + iBufLeng;
	BYTE* pFile = new BYTE[size];
	memcpy(pFile, &bmpFileHeader, sizeof(BITMAPFILEHEADER));
	memcpy(pFile+sizeof(BITMAPFILEHEADER), &pBmpFileInfo, sizeof(BITMAPINFOHEADER));
	memcpy(pFile+sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER), pBuffer, iBufLeng);

	fipMemoryIO memIO(pFile, size);*/


	hFile = ::CreateFile(szFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == NULL) return FALSE;

	::WriteFile(hFile, &bmpFileHeader, sizeof(BITMAPFILEHEADER), &dwWriten,NULL);
	::WriteFile(hFile, pBmpFileInfo, sizeof(BITMAPINFOHEADER ), &dwWriten,NULL);
	::WriteFile(hFile, pBuffer, iBufLeng, &dwWriten, NULL);
	::CloseHandle(hFile);
	
	return TRUE;
}

#define WIDTHBYTES(bits) ((DWORD)(((bits)+31) & (~31)) / 8)

void CFilterSoftglowDlg::softglow()
{
  int          width, height;
  int          bytes;
  bool         has_alpha;
  BYTE         *dest;
  BYTE         *src, *sp_p, *sp_m;
  double       n_p[5], n_m[5];
  double       d_p[5], d_m[5];
  double       bd_p[5], bd_m[5];
  double       *val_p, *val_m, *vp, *vm;
  int          x1, y1, x2, y2;
  int          i, j;
  int          row, col, b;
  int          terms;
  int          progress, max_progress;
  int          initial_p[4];
  int          initial_m[4];
  int          tmp;
  double       radius;
  double       std_dev;
  double       val;



  bool preview = true;
  //得到图像宽，高及左上，右下两点坐标
  if (!m_image.isValid())
	  return;

  BYTE *src_ptr  = m_image.accessPixels();
  bytes = m_image.getBitsPerPixel()/8;

  int n = WIDTHBYTES(m_image.getBitsPerPixel());

  has_alpha = m_image.isTransparent();

  fipWinImage image2 = m_image;

  width = m_image.getWidth();
  height = m_image.getHeight();
  int size = width*height*3;
  size = m_image.getImageSize();

  BITMAPINFO* info = m_image.getInfo();
    
  //uint16       bpp;  the bytes per pixel (1, 2, 3 or 4) 
   
  val_p = new double[MAX(width, height)];
  val_m = new double[MAX(width, height)];

  //保存blur的计算结果
  dest = new BYTE[width*height];
  

  progress = 0;
  max_progress = width * height * 3;


  BYTE *dest_ptr = dest;//new BYTE[m_image.getImageSize()];
  
  for (row = 0; row < height; row++)
  {
	  for (col = 0; col < width; col++)
	  {
		  // desaturate 
		 if (bytes > 2)
			  dest_ptr[col] = (BYTE) gimp_rgb_to_l_int(src_ptr[col * bytes + 0],
			  src_ptr[col * bytes + 1],
			  src_ptr[col * bytes + 2]);
		  else
			  dest_ptr[col] = (BYTE) src_ptr[col * bytes];
		  
		  // compute sigmoidal transfer 
		  val = dest_ptr[col] / 255.0;
		  val = 255.0 / (1 + exp (-(SIGMOIDAL_BASE + (svals.sharpness * SIGMOIDAL_RANGE)) * (val - 0.5)));
		  val = val * svals.brightness;
		  dest_ptr[col] = (BYTE) CLAMP (val, 0, 255);
	  }
	  
	  src_ptr  += width * bytes;
	  dest_ptr += width;
  }

  

  //  Calculate the standard deviations  
  radius  = fabs (svals.glow_radius) + 1.0;
  std_dev = sqrt (-(radius * radius) / (2 * log (1.0 / 255.0)));

  //  derive the constants for calculating the gaussian from the std dev  
  find_constants(n_p, n_m, d_p, d_m, bd_p, bd_m, std_dev);

  //  First the vertical pass  
  for (col = 0; col < width; col++)
    {
      memset (val_p, 0, height * sizeof (double));
      memset (val_m, 0, height * sizeof (double));

      src  = dest + col;
      sp_p = src;
      sp_m = src + width * (height - 1);
      vp   = val_p;
      vm   = val_m + (height - 1);

      //  Set up the first vals  
      initial_p[0] = sp_p[0];
      initial_m[0] = sp_m[0];

      for (row = 0; row < height; row++)
        {
          double *vpptr, *vmptr;

          terms = (row < 4) ? row : 4;

          vpptr = vp; vmptr = vm;
          for (i = 0; i <= terms; i++)
            {
              *vpptr += n_p[i] * sp_p[-i * width] - d_p[i] * vp[-i];
              *vmptr += n_m[i] * sp_m[i * width] - d_m[i] * vm[i];
            }
          for (j = i; j <= 4; j++)
            {
              *vpptr += (n_p[j] - bd_p[j]) * initial_p[0];
              *vmptr += (n_m[j] - bd_m[j]) * initial_m[0];
            }

          sp_p += width;
          sp_m -= width;
          vp ++;
          vm --;
        }

      transfer_pixels(val_p, val_m, dest + col, width, height);

    }

  for (row = 0; row < height; row++)
    {
      memset (val_p, 0, width * sizeof (double));
      memset (val_m, 0, width * sizeof (double));

      src = dest + row * width;

      sp_p = src;
      sp_m = src + width - 1;
      vp = val_p;
      vm = val_m + width - 1;

      //  Set up the first vals  
      initial_p[0] = sp_p[0];
      initial_m[0] = sp_m[0];

      for (col = 0; col < width; col++)
        {
          double *vpptr, *vmptr;

          terms = (col < 4) ? col : 4;

          vpptr = vp; vmptr = vm;

          for (i = 0; i <= terms; i++)
            {
              *vpptr += n_p[i] * sp_p[-i] - d_p[i] * vp[-i];
              *vmptr += n_m[i] * sp_m[i] - d_m[i] * vm[i];
            }

          for (j = i; j <= 4; j++)
            {
              *vpptr += (n_p[j] - bd_p[j]) * initial_p[0];
              *vmptr += (n_m[j] - bd_m[j]) * initial_m[0];
            }

          sp_p ++;
          sp_m --;
          vp ++;
          vm --;
        }

      transfer_pixels (val_p, val_m, dest + row * width, 1, width);
    }


  {
	  BYTE *src_ptr  = m_image.accessPixels();
	  BYTE *dest_ptr = new BYTE[m_image.getImageSize()];
	  BYTE* dest_line = dest_ptr;
	  BYTE *blur_ptr = dest ;
	  
	  for (row = 0; row < height; row++)
	  {
		  for (col = 0; col < width; col++)
		  {
			  // screen op 
			  for (b = 0; b < (has_alpha ? (bytes - 1) : bytes); b++)
			  {
				  BYTE bb = 255 - INT_MULT((255 - src_ptr[col * bytes + b]), (255 - blur_ptr[col]), tmp);
				  dest_line[col * bytes + b] = bb;

				  //dest_line[col * bytes + b] = src_ptr[col * bytes + b];
			  }
			  if (has_alpha)
				  dest_line[col * bytes + b] = src_ptr[col * bytes + b];
		  }
		  
		  src_ptr  += width*bytes;
		  dest_line += width*bytes;
		  blur_ptr += width;
	  }


	  char* szFile = "3.bmp";
	  g_fSaveDataToBitmap(&(info->bmiHeader), szFile, dest_ptr, m_image.getImageSize());
	  m_imageDest.load(szFile);
	  if (!m_imageDest.isValid())
	  {
		  AfxMessageBox("处理失败!");
	  }

	  /*fipMemoryIO memIO;
	  fipWinImage image3;
	  image3.setSize(FIT_BITMAP, width, height, info->bmiHeader.biBitCount);
	  image3.saveToMemory(FIF_BMP, memIO);*/
	  delete []dest_ptr;

  }


  delete []val_p;
  delete []val_m;
  delete []dest;
  
 // FIBITMAP* dib = (FIBITMAP *)(&image2);
  //dest = image2.accessPixels();
  //memset(dest, 0, image2.getImageSize());
  //image2.save("c:\\2.jpg");

  Invalidate();
  
}

static void transfer_pixels (double *src1,
                 double *src2,
                 BYTE  *dest,
                 int     jump,
                 int     width)
{
  int    i = 0;
  double sum = 0;

  for (i = 0; i < width; i++)
    {
      sum = src1[i] + src2[i];

      sum = CLAMP0255 (sum);

      *dest = (BYTE) sum;
      dest += jump;
    }
}

static void
find_constants (double n_p[],
                double n_m[],
                double d_p[],
                double d_m[],
                double bd_p[],
                double bd_m[],
                double std_dev)
{
  int    i;
  double constants [8];
  double div;

  /*  The constants used in the implemenation of a casual sequence
   *  using a 4th order approximation of the gaussian operator
   */

  div = sqrt(2 * G_PI) * std_dev;

  constants [0] = -1.783  / std_dev;
  constants [1] = -1.723  / std_dev;
  constants [2] =  0.6318 / std_dev;
  constants [3] =  1.997  / std_dev;
  constants [4] =  1.6803 / div;
  constants [5] =  3.735  / div;
  constants [6] = -0.6803 / div;
  constants [7] = -0.2598 / div;

  n_p [0] = constants[4] + constants[6];
  n_p [1] = exp (constants[1]) *
    (constants[7] * sin (constants[3]) -
     (constants[6] + 2 * constants[4]) * cos (constants[3])) +
       exp (constants[0]) *
         (constants[5] * sin (constants[2]) -
          (2 * constants[6] + constants[4]) * cos (constants[2]));
  n_p [2] = 2 * exp (constants[0] + constants[1]) *
    ((constants[4] + constants[6]) * cos (constants[3]) * cos (constants[2]) -
     constants[5] * cos (constants[3]) * sin (constants[2]) -
     constants[7] * cos (constants[2]) * sin (constants[3])) +
       constants[6] * exp (2 * constants[0]) +
         constants[4] * exp (2 * constants[1]);
  n_p [3] = exp (constants[1] + 2 * constants[0]) *
    (constants[7] * sin (constants[3]) - constants[6] * cos (constants[3])) +
      exp (constants[0] + 2 * constants[1]) *
        (constants[5] * sin (constants[2]) - constants[4] * cos (constants[2]));
  n_p [4] = 0.0;

  d_p [0] = 0.0;
  d_p [1] = -2 * exp (constants[1]) * cos (constants[3]) -
    2 * exp (constants[0]) * cos (constants[2]);
  d_p [2] = 4 * cos (constants[3]) * cos (constants[2]) * exp (constants[0] + constants[1]) +
    exp (2 * constants[1]) + exp (2 * constants[0]);
  d_p [3] = -2 * cos (constants[2]) * exp (constants[0] + 2 * constants[1]) -
    2 * cos (constants[3]) * exp (constants[1] + 2 * constants[0]);
  d_p [4] = exp (2 * constants[0] + 2 * constants[1]);

#ifndef ORIGINAL_READABLE_CODE
  memcpy(d_m, d_p, 5 * sizeof(double));
#else
  for (i = 0; i <= 4; i++)
    d_m [i] = d_p [i];
#endif

  n_m[0] = 0.0;
  for (i = 1; i <= 4; i++)
    n_m [i] = n_p[i] - d_p[i] * n_p[0];

  {
    double sum_n_p, sum_n_m, sum_d;
    double a, b;

    sum_n_p = 0.0;
    sum_n_m = 0.0;
    sum_d   = 0.0;

    for (i = 0; i <= 4; i++)
      {
        sum_n_p += n_p[i];
        sum_n_m += n_m[i];
        sum_d += d_p[i];
      }

#ifndef ORIGINAL_READABLE_CODE
    sum_d++;
    a = sum_n_p / sum_d;
    b = sum_n_m / sum_d;
#else
    a = sum_n_p / (1 + sum_d);
    b = sum_n_m / (1 + sum_d);
#endif

    for (i = 0; i <= 4; i++)
      {
        bd_p[i] = d_p[i] * a;
        bd_m[i] = d_m[i] * b;
      }
  }
}

void CFilterSoftglowDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// Do not call CDialog::OnPaint() for painting messages
	if (m_image.isValid())
	{
		CRect rt1;
		GetDlgItem(IDC_STATIC_SRC)->GetWindowRect(rt1);
		ScreenToClient(rt1);

		CRect rtNew = CalcRect(m_image.getWidth(), m_image.getHeight(), &rt1);
		m_image.draw(dc.m_hDC, rtNew);
	}
	
	if (m_imageDest.isValid())
	{
		CRect rt2;
		GetDlgItem(IDC_STATIC_DEST)->GetWindowRect(rt2);
		ScreenToClient(rt2);

		CRect rtNew = CalcRect(m_imageDest.getWidth(), m_imageDest.getHeight(), &rt2);
		m_imageDest.draw(dc.m_hDC, rtNew);
	}
}

void CFilterSoftglowDlg::OnChangeEdit1() 
{
	UpdateData();
	int m_nNum1=atoi(m_strValue1);
	m_wndLineTrackBar1.SetPos (m_nNum1);

	svals.glow_radius = m_nNum1;

	UpdateData(FALSE);
	softglow();
}

void CFilterSoftglowDlg::OnChangeEdit2() 
{
	UpdateData();
	int m_nNum1=atoi(m_strValue2);
	m_wndLineTrackBar2.SetPos (m_nNum1);

	svals.brightness = (double)m_nNum1/(double)100;
	UpdateData(FALSE);
	softglow();
}

void CFilterSoftglowDlg::OnChangeEdit3() 
{
	UpdateData();
	int m_nNum1=atoi(m_strValue3);
	m_wndLineTrackBar3.SetPos (m_nNum1);
	svals.sharpness = (double)m_nNum1/(double)100;

	UpdateData(FALSE);
	softglow();
	
}
