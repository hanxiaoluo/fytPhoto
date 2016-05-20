# fytPhoto
image process, softglow

3rd:FreeImage

IDE: VC++ 6.0

refer: GIMP


GIMP的柔光(softglow)源码分析 

doc: http://blog.chinaunix.net/uid-15063109-id-3499466.html



前言：柔光效果可以把一张照片美白化，影楼应该用的比较多。
打算把柔光效果用在自己软件中，看GIMP的代码，开始感觉牵扯的比较多，另外找了几个，比如tinyimage，imagestone等。效果也不错，不过看到处理图像大量使用了gdi+,stl,感觉不太合适。再回去认真看GIMP代码，最终成功移植到vc和iOS下。
效果：     


采用的是美图秀秀软件下面的一张图，如果有版权问题，请联系，及时删掉。不过说实话，美图秀秀还有很大可优化空间。尤其是处理图片的时候，能不能先对缩略图处理一下？直接对大图动手，什么CPU也受不了。
处理的参数是 SoftglowVals svals = {10, 0.5, 0.6};
有兴趣的朋友可以用GIMP自己处理到满意的程度，记住参数，再用程序来实现。这里只是演示功能，不考虑美观效果，有可能处理后比处理前还难看。

一 gimp softglow(柔光)分析

代码在 gimp-2.8.0\plug-ins\common\softglow.c里面。
1 选中显示图像的一部分
2 对选中部分进行亮度，锐度计算，保存在 width X height数组内。(图像的原始数据区是width X height X bytes per pixel)
3 高斯模糊计算


//  Calculate the standard deviations  
  radius  = fabs (svals.glow_radius) + 1.0;
  std_dev = sqrt (-(radius * radius) / (2 * log (1.0 / 255.0)));


  //  derive the constants for calculating the gaussian from the std dev  
  find_constants(n_p, n_m, d_p, d_m, bd_p, bd_m, std_dev);
根据半径算出高斯模板的矩阵值。find_constants里面计算的是5x5矩阵。
4 再根据原始图像数据，计算出目标图像数据。计算公式：
255 - INT_MULT((255 - src_ptr[col * bytes + b]), (255 - blur_ptr[col]), tmp);


5 把目标数据保存成图，就得到了柔光的结果。


总结：
算法比较简单，需要注意的是，原程序里读原始图像数据，使用的是gimp提供的api,比如图像数据是drawable，需要改写成从DC里取值，或直接从文件里读。


二 改造
毕竟gimp不是通常用c/c++库，如果用在自己项目里，是需要进行一些处理的。
改造从几个方面进行：
1 读写图像：推荐用FreeImage
2 显示图像：使用到drawable和rgn的地方要改。
3 算法：基本不用动，只是要加一些注释，源码过于简洁，比如
double       n_p[5], n_m[5];
double       d_p[5], d_m[5];
double       bd_p[5], bd_m[5];


static void
find_constants (gdouble n_p[],
                gdouble n_m[],
                gdouble d_p[],
                gdouble d_m[],
                gdouble bd_p[],
                gdouble bd_m[],
                gdouble std_dev)
{
  gint    i;
  gdouble constants [8];
  gdouble div;


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
  memcpy(d_m, d_p, 5 * sizeof(gdouble));
#else
  for (i = 0; i <= 4; i++)
    d_m [i] = d_p [i];
#endif


  n_m[0] = 0.0;
  for (i = 1; i <= 4; i++)
    n_m [i] = n_p[i] - d_p[i] * n_p[0];


  {
    gdouble sum_n_p, sum_n_m, sum_d;
    gdouble a, b;


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


总结：刚看gimp代码时，的确一头雾水。这时需要九方皋相马的方式，透过现象看本质。
比如：
guchar *src_ptr  = src_rgn.data;
guchar *dest_ptr = dest_rgn.data;
如果修改成标准c的写法就是：
uint8_t* src_ptr = image_src.accessPixels();
uint8_t*dest_ptr = new uint8_t[image_src.getImageSize()];
//以上假设是对整图处理，如果对部分处理的话，自己定义个矩形，选中图像数据，比如可以使用FreeImage的crop函数读选中区域。
