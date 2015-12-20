    /***************************************************************************
  \file T_openGL.h
  \brief OpenGL related filters
  \author (C) 2011 Mean Fixounet@free.fr 
***************************************************************************/
#include "ADM_openGl.h"
#include "T_openGLFilter.h"
#include "ADM_default.h"
#include "DIA_coreToolkit.h"
static QGLWidget *thisWidget=NULL;
     

#define CHECK(x) if(!x) {GUI_Error_HIG("Missing extension "#x,#x" not defined");ADM_assert(0);}

/**

*/
bool ADM_setGlWidget(QGLWidget *w)
{
        thisWidget=w;
        return true;
}
/**

*/
QGLWidget *ADM_getGlWidget(void)
{
        return thisWidget;
        
}

/**
    \fn ctor
*/
ADM_coreVideoFilterQtGl::ADM_coreVideoFilterQtGl(ADM_coreVideoFilter *previous,CONFcouple *conf)
:ADM_coreVideoFilter(previous,conf),ADM_coreQtGl(ADM_getGlWidget())
{
    bufferARB=0;
    widget->makeCurrent();
    if(ADM_glHasARB())
            ADM_glExt::genBuffers(1,&bufferARB);        
    glProgramY=NULL;
    glProgramUV=NULL;
    fboY=NULL;
    fboUV=NULL;
    fboY = new QGLFramebufferObject(info.width,info.height);
    ADM_assert(fboY);
    fboUV = new QGLFramebufferObject(info.width/2,info.height/2);
    ADM_assert(fboUV);
    widget->doneCurrent();
}
/**
    \fn resizeFBO
*/
bool ADM_coreVideoFilterQtGl::resizeFBO(uint32_t w,uint32_t h)
{
    widget->makeCurrent();
    if(fboY) delete fboY;
    fboY=new QGLFramebufferObject(w,h);
    widget->doneCurrent();
    checkGlError("resizeFBO");
    return true;
}
/**
    \fn dtor
*/
ADM_coreVideoFilterQtGl::~ADM_coreVideoFilterQtGl()
{
    ADM_info("Gl filter : Destroying..\n");
   
    if(glProgramY) delete glProgramY;
    glProgramY=NULL;
    if(glProgramUV) delete glProgramUV;
    glProgramUV=NULL;
    if(fboY) delete fboY;
    fboY=NULL;
    if(fboUV) delete fboUV;
    fboUV=NULL;
    if(ADM_glHasARB())
        ADM_glExt::deleteBuffers(1,&bufferARB);
    bufferARB=0;
}
#if  1
#define TEX_Y_OFFSET 2
#define TEX_U_OFFSET 1
#define TEX_V_OFFSET 0  
#define TEX_A_OFFSET 3   
#else
#define TEX_Y_OFFSET 0
#define TEX_U_OFFSET 1
#define TEX_V_OFFSET 2   
#define TEX_A_OFFSET 3   
#endif

/**
    \fn downloadTexture
*/
bool ADM_coreVideoFilterQtGl::downloadTexture(ADMImage *image, ADM_PLANE plane,
        QGLFramebufferObject *fbo)
{
#ifdef BENCH_READTEXTURE
    {
    ADMBenchmark bench;
    for(int i=0;i<100;i++)
    {
        bench.start();
        QImage qimg(fbo->toImage());
        bench.end();
     }
    ADM_warning("convert to Qimage\n");
    bench.printResult();
    }
#endif

    QImage qimg(fbo->toImage()); // this is slow ! ~ 15 ms for a 720 picture (Y only).



    // Assume RGB32, read R or A
#ifdef BENCH_READTEXTURE
    ADMBenchmark bench;
    for(int i=0;i<100;i++)
    {
        bench.start();
#endif
    int stride=image->GetPitch(plane);
    uint8_t *to=image->GetWritePtr(plane);
    int width=image->GetWidth(plane);
    int height=image->GetHeight(plane);
    for(int y=0;y<height;y++)
    {
        const uchar *src=qimg.constScanLine(height-y-1);
        if(!src)
        {
            ADM_error("Can t get pointer to openGl texture\n");
            return false;
        }
        for(int x=0;x<width;x++)
            to[x]=src[x*4];
        to+=stride;
    }
#ifdef BENCH_READTEXTURE
        bench.end();
    }
    bench.printResult();

#endif
    return true;
}
typedef void typeGlYv444(const uint8_t *src,uint8_t *dst,const int width);
typedef void typeGlYUV444(const uint8_t *src,uint8_t *dstY,uint8_t *dstU, uint8_t *dstV,const int width);

/**
 */

/**
 * \fn glYUV444_ChromaC
 * \brief very stupid downsampler for U & V plane, one line is discarder
 * @param src
 * @param toU
 * @param toV
 * @param width
 */
static inline void glYUV444_ChromaC(const uint8_t *src, uint8_t *toU, uint8_t *toV, const int width)
{
       // ?
       const uchar *p=src;
       for(int x=0;x<width;x++) // Stupid subsample: 1 out of 2
        {
            if(!*(uint32_t *)p || !*(uint32_t *)(p+4))
            {
                toU[x]=128;
                toV[x]=128;
            }else
            {
                toU[x]  =  ((int)p[TEX_U_OFFSET]+(int)p[TEX_U_OFFSET+4])>>1;
                toV[x]  =  ((int)p[TEX_V_OFFSET]+(int)p[TEX_V_OFFSET+4])>>1;
            }
            p+=8;
        }
}
/**
 * 
 */
#ifdef ADM_CPU_X86
static inline void glYUV444_MMXInit(void)
{
   static uint64_t __attribute__((used)) FUNNY_MANGLE(mask) = 0x00ff000000ff0000LL;

    __asm__(" movq "Mangle(mask)", %%mm7\n" ::);
}
static inline void glYUV444_MMX(const uint8_t *src, uint8_t *dst, const int width)
{
 
    int count=width/8;
                    __asm__(
                        "1:\n"
                        "movq           (%0),%%mm0 \n"
                        "pand           %%mm7,%%mm0\n"
                        "movq           8(%0),%%mm1 \n"
                        "pand           %%mm7,%%mm1\n"

                        "movq           16(%0),%%mm2 \n"
                        "pand           %%mm7,%%mm2\n"
                        "movq           24(%0),%%mm3 \n"
                        "pand           %%mm7,%%mm3\n"

                        "packuswb       %%mm1,%%mm0\n"
                        "packuswb       %%mm3,%%mm2\n"
                        "psrlw          $8,%%mm0\n"
                        "psrlw          $8,%%mm2\n"
                        "packuswb       %%mm2,%%mm0\n"

                        "movq           %%mm0,(%1)  \n"  
                        "add            $32,%0      \n"
                        "add            $8,%1       \n"
                        "sub            $1,%2        \n"
                        "jnz             1b         \n"
                        
                        :: "r"(src),"r"(dst),"r"(count)
                        );
    if(width&7)
    {
        for(int i=count*8;i<width;i++)
            dst[i]  = src[i*4+TEX_Y_OFFSET];
    }
}
/**
 * 
 * @param src
 * @param dstY
 * @param dstU
 * @param dstV
 * @param width
 */
static inline void glYUV444_MMX_Chroma(const uint8_t *src, uint8_t *dstY, uint8_t *dstU, uint8_t *dstV,const int width)
{
 
    int count=width/8;
                    __asm__(
                        "1:\n"
                        "movq           (%0),%%mm0 \n"
                        "pmov           %mm0,%%mm4 \n"
                        "pand           %%mm7,%%mm0\n"
                        "movq           8(%0),%%mm1 \n"
                        "pmov           %mm1,%%mm5 \n"
                        "pand           %%mm7,%%mm1\n"

                        "movq           16(%0),%%mm2 \n"
                        "pmov           %mm2,%%mm6 \n"
                        "pand           %%mm7,%%mm2\n"
                        "movq           24(%0),%%mm3 \n"
                        "packuswb       %%mm1,%%mm0\n"
                        "pmov           %mm3,%%mm1 \n" // We have a copy in MM4/MM5/MM6/MM1
                        "pand           %%mm7,%%mm3\n"

                        // Pack luma
                        "packuswb       %%mm3,%%mm2\n"
                        "psrlw          $8,%%mm0\n"
                        "psrlw          $8,%%mm2\n"
                        "packuswb       %%mm2,%%mm0\n"
                        "movq           %%mm0,(%1)  \n"  
                            
                        // now do chroma, it is similar    
                            
                        // Next..
                        "add            $32,%0      \n"
                        "add            $8,%1       \n"
                        "sub            $1,%2        \n"
                        "jnz             1b         \n"
                        
                        :: "r"(src),"r"(dstY),"r"(dstU),"r"(dstV),"r"(count)
                        );
    if(width&7)
    {
        for(int i=count*8;i<width;i++)
            dstY[i]  = src[i*4+TEX_Y_OFFSET];
    }
}
/**
 */
static inline void glYUV444_YUVMMX(const uint8_t *src, uint8_t *toY,uint8_t *toU, uint8_t *toV, const int width)
{
    glYUV444_MMX(src,toY,width);
    glYUV444_ChromaC(src,toU,toV,width>>1);
    
    
}
#endif
static inline void glYUV444_C(const uint8_t *src, uint8_t *dst, const int width)
{
       for(int x=0;x<width;x++)
        {
            dst[x]  = src[x*4+TEX_Y_OFFSET];
        }
}

static inline void glYUV444_C_withChroma(const uint8_t *src, uint8_t *dstY,uint8_t *dstU, uint8_t *dstV, const int width)
{
       glYUV444_C(src,dstY,width);
       glYUV444_ChromaC(src,dstU,dstV,width>>1);
}

bool ADM_coreVideoFilterQtGl::downloadTextures(ADMImage *image,  QGLFramebufferObject *fbo)
{
#if 1 // With QT5, download QT is faster ..    
    if(ADM_glHasARB())
        return downloadTexturesDma(image,fbo);
#endif
    return downloadTexturesQt(image,fbo);
}
/**
    \fn downloadTexture
    \brief Download YUVA texture into a YV12 image
*/
bool ADM_coreVideoFilterQtGl::downloadTexturesQt(ADMImage *image,  QGLFramebufferObject *fbo)
{

    QImage qimg(fbo->toImage()); // this is slow ! ~ 15 ms for a 720 picture (Y only).
    // Assume RGB32, read R or A
    int strideY=image->GetPitch(PLANAR_Y);
    uint8_t *toY=image->GetWritePtr(PLANAR_Y);
    uint8_t *toU=image->GetWritePtr(PLANAR_U);
    uint8_t *toV=image->GetWritePtr(PLANAR_V);
    int      strideU=image->GetPitch(PLANAR_U);
    int      strideV=image->GetPitch(PLANAR_V);

    int width=image->GetWidth(PLANAR_Y);
    int height=image->GetHeight(PLANAR_Y);
    typeGlYv444  *luma=glYUV444_C;
    typeGlYUV444 *lumaAndChroma=glYUV444_C_withChroma;
#ifdef ADM_CPU_X86
      if(1 && CpuCaps::hasMMX())
      {
            glYUV444_MMXInit();
            luma=glYUV444_MMX;
            lumaAndChroma=glYUV444_YUVMMX;
      }
#endif
#define admAlloca alloca
    const uchar **yy=(const uchar **)admAlloca(height*sizeof(uint8_t *)); // FIXME : Use alloca here
    for(int i=0;i<height;i++)
    {
        yy[i]=qimg.constScanLine(height-i-1);
        if(!yy[i])
        {
            ADM_error("Can t get pointer to openGl texture\n");
            yy=NULL;
            return false;
        }
    }
    // Do Y
    for(int y=0;y<height;y++)
    {
       const uchar *src=yy[y];
       lumaAndChroma(src,toY,toU,toV,width);
       toY+=strideY;
       toU+=strideU;
       toV+=strideV;
       // 2nd line
       y++;
       src=yy[y];
       luma(src,toY,width);
       toY+=strideY;        
    }
#ifdef ADM_CPU_X86
    __asm__( "emms\n"::  );
#endif
    yy=NULL;
    return true;
}
/**
    \fn downloadTexture
    \brief Download YUVA texture into a YV12 image
 TODO FIXME : Make same optimisation as Qt version
*/
bool ADM_coreVideoFilterQtGl::downloadTexturesDma(ADMImage *image,  QGLFramebufferObject *fbo)
{
    int width=image->GetWidth(PLANAR_Y);
    int height=image->GetHeight(PLANAR_Y);
    bool r=true;

    ADM_glExt::bindBuffer(GL_PIXEL_PACK_BUFFER_ARB,0);
    // that one might fail : checkGlError("BindARB-00");

    ADM_glExt::bindBuffer(GL_PIXEL_PACK_BUFFER_ARB,bufferARB);
    checkGlError("BindARB");
    ADM_glExt::bufferData(GL_PIXEL_PACK_BUFFER_ARB,info.width*info.height*sizeof(uint32_t),
                                NULL,GL_STREAM_READ_ARB);
    checkGlError("BufferDataRB");

    glReadBuffer(GL_COLOR_ATTACHMENT0_EXT); 
    checkGlError("ReadBuffer (fbo)");
    ADM_glExt::bindBuffer(GL_PIXEL_PACK_BUFFER_ARB,bufferARB);
    checkGlError("Bind Buffer (arb)");

    glReadPixels(0, 0, width, height, GL_BGRA, GL_UNSIGNED_BYTE, 0);
    checkGlError("glReadPixel");

    // DMA call done, we can do something else here
    ADM_usleep(1*1000);


    GLubyte* ptr = (GLubyte*)ADM_glExt::mapBuffer(GL_PIXEL_PACK_BUFFER_ARB, GL_READ_ONLY_ARB);
    checkGlError("MapBuffer");
    if(!ptr)
    {
        ADM_error("Cannot map output buffer!\n");
        r=false;
    }
    else
    {
        // Assume RGB32, read R or A
        int strideY=image->GetPitch(PLANAR_Y);
        uint8_t *toY=image->GetWritePtr(PLANAR_Y);
        uint8_t *toU=image->GetWritePtr(PLANAR_U);
        uint8_t *toV=image->GetWritePtr(PLANAR_V);
        int      strideU=image->GetPitch(PLANAR_U);
        int      strideV=image->GetPitch(PLANAR_V);

        int width=image->GetWidth(PLANAR_Y);
        int height=image->GetHeight(PLANAR_Y);
        typeGlYv444 *luma=glYUV444_C;
        typeGlYUV444 *lumaAndChroma=glYUV444_C_withChroma;
    #ifdef ADM_CPU_X86
          if(1 && CpuCaps::hasMMX())
          {
                glYUV444_MMXInit();
                luma=glYUV444_MMX;
                lumaAndChroma=glYUV444_YUVMMX;
          }
    #endif
        // Do Y
        const uchar *src=ptr;
        for(int y=0;y<height;y+=2)
        {
           luma(src,toY,width);
           toY+=strideY;
           src+=4*width;
           lumaAndChroma(src,toY,toU,toV,width);
           toY+=strideY;
           src+=4*width;
           toU+=strideU;
           toV+=strideV;
        }
    #ifdef ADM_CPU_X86
        __asm__( "emms\n"::  );
    #endif
        ADM_glExt::unmapBuffer(GL_PIXEL_PACK_BUFFER_ARB);
    }
    ADM_glExt::bindBuffer(GL_PIXEL_PACK_BUFFER_ARB,0);
    return r;
}

// EOF
