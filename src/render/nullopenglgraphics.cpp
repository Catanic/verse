/*
 *  The ManaVerse Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2020  The ManaPlus Developers
 *  Copyright (C) 2020-2025  The ManaVerse Developers
 *
 *  This file is part of The ManaVerse Client.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#if defined USE_OPENGL

#include "render/nullopenglgraphics.h"

#ifdef DEBUG_BIND_TEXTURE
#include "logger.h"
#endif  // DEBUG_BIND_TEXTURE

#include "render/vertexes/imagecollection.h"

#include "resources/imagerect.h"
#include "resources/openglimagehelper.h"

#include "resources/image/image.h"

#include "utils/performance.h"

#include "debug.h"

GLuint NullOpenGLGraphics::mTextureBinded = 0;
#ifdef DEBUG_DRAW_CALLS
unsigned int NullOpenGLGraphics::mDrawCalls = 0;
unsigned int NullOpenGLGraphics::mLastDrawCalls = 0;
#endif  // DEBUG_DRAW_CALLS

NullOpenGLGraphics::NullOpenGLGraphics() :
    mFloatTexArray(nullptr),
    mIntTexArray(nullptr),
    mIntVertArray(nullptr),
    mTexture(false),
    mIsByteColor(false),
    mByteColor(),
    mFloatColor(1.0F),
    mMaxVertices(500),
    mColorAlpha(false),
#ifdef DEBUG_BIND_TEXTURE
    mOldTexture(),
    mOldTextureId(0),
#endif  // DEBUG_BIND_TEXTURE
    mFbo()
{
    mOpenGL = RENDER_NULL;
    mName = "null OpenGL";
}

NullOpenGLGraphics::~NullOpenGLGraphics()
{
    delete [] mFloatTexArray;
    delete [] mIntTexArray;
    delete [] mIntVertArray;
}

void NullOpenGLGraphics::initArrays(const int vertCount) restrict2
{
    mMaxVertices = vertCount;
    if (mMaxVertices < 500)
        mMaxVertices = 500;
    else if (mMaxVertices > 1024)
        mMaxVertices = 1024;

    // need alocate small size, after if limit reached reallocate to double size
    vertexBufSize = mMaxVertices;
    const size_t sz = mMaxVertices * 4 + 30;
    if (mFloatTexArray == nullptr)
        mFloatTexArray = new GLfloat[sz];
    if (mIntTexArray == nullptr)
        mIntTexArray = new GLint[sz];
    if (mIntVertArray == nullptr)
        mIntVertArray = new GLint[sz];
}

void NullOpenGLGraphics::deleteArrays() restrict2
{
}

bool NullOpenGLGraphics::setVideoMode(const int w, const int h,
                                      const int scale,
                                      const int bpp,
                                      const bool fs,
                                      const bool hwaccel,
                                      const bool resize,
                                      const bool noFrame,
                                      const bool allowHighDPI) restrict2
{
    setMainFlags(w, h,
        scale,
        bpp,
        fs,
        hwaccel,
        resize,
        noFrame,
        allowHighDPI);

    return setOpenGLMode();
}

static inline void drawQuad(const Image *restrict const image A_UNUSED,
                            const int srcX A_UNUSED, const int srcY A_UNUSED,
                            const int dstX A_UNUSED, const int dstY A_UNUSED,
                            const int width A_UNUSED,
                            const int height A_UNUSED) A_NONNULL(1);

static inline void drawQuad(const Image *restrict const image A_UNUSED,
                            const int srcX A_UNUSED, const int srcY A_UNUSED,
                            const int dstX A_UNUSED, const int dstY A_UNUSED,
                            const int width A_UNUSED,
                            const int height A_UNUSED)
{
    if (OpenGLImageHelper::mTextureType == GL_TEXTURE_2D)
    {
#ifdef DEBUG_DRAW_CALLS
        NullOpenGLGraphics::mDrawCalls ++;
#endif  // DEBUG_DRAW_CALLS
    }
    else
    {
#ifdef DEBUG_DRAW_CALLS
        NullOpenGLGraphics::mDrawCalls ++;
#endif  // DEBUG_DRAW_CALLS
    }
}

static inline void drawRescaledQuad(const Image *restrict const image A_UNUSED,
                                    const int srcX A_UNUSED,
                                    const int srcY A_UNUSED,
                                    const int dstX A_UNUSED,
                                    const int dstY A_UNUSED,
                                    const int width A_UNUSED,
                                    const int height A_UNUSED,
                                    const int desiredWidth A_UNUSED,
                                    const int desiredHeight A_UNUSED)
{
    if (OpenGLImageHelper::mTextureType == GL_TEXTURE_2D)
    {
#ifdef DEBUG_DRAW_CALLS
        NullOpenGLGraphics::mDrawCalls ++;
#endif  // DEBUG_DRAW_CALLS
    }
    else
    {
#ifdef DEBUG_DRAW_CALLS
        NullOpenGLGraphics::mDrawCalls ++;
#endif  // DEBUG_DRAW_CALLS
    }
}

void NullOpenGLGraphics::drawImage(const Image *restrict const image,
                                   int dstX, int dstY) restrict2
{
    drawImageInline(image, dstX, dstY);
}

void NullOpenGLGraphics::copyImage(const Image *restrict const image,
                                   int dstX, int dstY) restrict2
{
    drawImageInline(image, dstX, dstY);
}

void NullOpenGLGraphics::drawImageInline(const Image *restrict const image,
                                         int dstX, int dstY) restrict2
{
    FUNC_BLOCK("Graphics::drawImage", 1)
    if (image == nullptr)
        return;

    setColorAlpha(image->mAlpha);
#ifdef DEBUG_BIND_TEXTURE
    debugBindTexture(image);
#endif  // DEBUG_BIND_TEXTURE

    bindTexture(OpenGLImageHelper::mTextureType, image->mGLImage);
    enableTexturingAndBlending();

    const SDL_Rect &imageRect = image->mBounds;
    drawQuad(image, imageRect.x, imageRect.y, dstX, dstY,
        imageRect.w, imageRect.h);
}

void NullOpenGLGraphics::drawImageCached(const Image *restrict const image
                                         A_UNUSED,
                                         int x A_UNUSED,
                                         int y A_UNUSED) restrict2
{
}

void NullOpenGLGraphics::drawPatternCached(const Image *restrict const image
                                           A_UNUSED,
                                           const int x A_UNUSED,
                                           const int y A_UNUSED,
                                           const int w A_UNUSED,
                                           const int h A_UNUSED) restrict2
{
}

void NullOpenGLGraphics::completeCache() restrict2
{
}

void NullOpenGLGraphics::drawRescaledImage(const Image *restrict const image,
                                           int dstX, int dstY,
                                           const int desiredWidth,
                                           const int desiredHeight) restrict2
{
    FUNC_BLOCK("Graphics::drawRescaledImage", 1)
    if (image == nullptr)
        return;

    const SDL_Rect &imageRect = image->mBounds;

    // Just draw the image normally when no resizing is necessary,
    if (imageRect.w == desiredWidth && imageRect.h == desiredHeight)
    {
        drawImageInline(image, dstX, dstY);
        return;
    }

    setColorAlpha(image->mAlpha);
#ifdef DEBUG_BIND_TEXTURE
    debugBindTexture(image);
#endif  // DEBUG_BIND_TEXTURE

    bindTexture(OpenGLImageHelper::mTextureType, image->mGLImage);
    enableTexturingAndBlending();

    // Draw a textured quad.
    drawRescaledQuad(image, imageRect.x, imageRect.y, dstX, dstY,
        imageRect.w, imageRect.h, desiredWidth, desiredHeight);
}

void NullOpenGLGraphics::drawPattern(const Image *restrict const image,
                                     const int x, const int y,
                                     const int w, const int h) restrict2
{
    drawPatternInline(image, x, y, w, h);
}

void NullOpenGLGraphics::drawPatternInline(const Image *restrict const image,
                                           const int x, const int y,
                                           const int w, const int h) restrict2
{
    FUNC_BLOCK("Graphics::drawPattern", 1)
    if (image == nullptr)
        return;

    const SDL_Rect &imageRect = image->mBounds;
    const int srcX = imageRect.x;
    const int srcY = imageRect.y;
    const int iw = imageRect.w;
    const int ih = imageRect.h;

    if (iw == 0 || ih == 0)
        return;

    setColorAlpha(image->mAlpha);

#ifdef DEBUG_BIND_TEXTURE
    debugBindTexture(image);
#endif  // DEBUG_BIND_TEXTURE

    bindTexture(OpenGLImageHelper::mTextureType, image->mGLImage);

    enableTexturingAndBlending();

    unsigned int vp = 0;
    const unsigned int vLimit = mMaxVertices * 4;
    // Draw a set of textured rectangles
    if (OpenGLImageHelper::mTextureType == GL_TEXTURE_2D)
    {
        const float tw = static_cast<float>(image->mTexWidth);
        const float th = static_cast<float>(image->mTexHeight);

        const float texX1 = static_cast<float>(srcX) / tw;
        const float texY1 = static_cast<float>(srcY) / th;

        for (int py = 0; py < h; py += ih)
        {
            const int height = (py + ih >= h) ? h - py : ih;
            const int dstY = y + py;
            const float texY2 = static_cast<float>(srcY + height) / th;
            for (int px = 0; px < w; px += iw)
            {
                const int width = (px + iw >= w) ? w - px : iw;
                const int dstX = x + px;

                const float texX2 = static_cast<float>(srcX + width) / tw;

                mFloatTexArray[vp + 0] = texX1;
                mFloatTexArray[vp + 1] = texY1;

                mFloatTexArray[vp + 2] = texX2;
                mFloatTexArray[vp + 3] = texY1;

                mFloatTexArray[vp + 4] = texX2;
                mFloatTexArray[vp + 5] = texY2;

                mFloatTexArray[vp + 6] = texX1;
                mFloatTexArray[vp + 7] = texY2;

                mIntVertArray[vp + 0] = dstX;
                mIntVertArray[vp + 1] = dstY;

                mIntVertArray[vp + 2] = dstX + width;
                mIntVertArray[vp + 3] = dstY;

                mIntVertArray[vp + 4] = dstX + width;
                mIntVertArray[vp + 5] = dstY + height;

                mIntVertArray[vp + 6] = dstX;
                mIntVertArray[vp + 7] = dstY + height;

                vp += 8;
                if (vp >= vLimit)
                {
                    drawQuadArrayfi(vp);
                    vp = 0;
                }
            }
        }
        if (vp > 0)
            drawQuadArrayfi(vp);
    }
    else
    {
        for (int py = 0; py < h; py += ih)
        {
            const int height = (py + ih >= h) ? h - py : ih;
            const int dstY = y + py;
            for (int px = 0; px < w; px += iw)
            {
                const int width = (px + iw >= w) ? w - px : iw;
                const int dstX = x + px;

                mIntTexArray[vp + 0] = srcX;
                mIntTexArray[vp + 1] = srcY;

                mIntTexArray[vp + 2] = srcX + width;
                mIntTexArray[vp + 3] = srcY;

                mIntTexArray[vp + 4] = srcX + width;
                mIntTexArray[vp + 5] = srcY + height;

                mIntTexArray[vp + 6] = srcX;
                mIntTexArray[vp + 7] = srcY + height;

                mIntVertArray[vp + 0] = dstX;
                mIntVertArray[vp + 1] = dstY;

                mIntVertArray[vp + 2] = dstX + width;
                mIntVertArray[vp + 3] = dstY;

                mIntVertArray[vp + 4] = dstX + width;
                mIntVertArray[vp + 5] = dstY + height;

                mIntVertArray[vp + 6] = dstX;
                mIntVertArray[vp + 7] = dstY + height;

                vp += 8;
                if (vp >= vLimit)
                {
                    drawQuadArrayii(vp);
                    vp = 0;
                }
            }
        }
        if (vp > 0)
            drawQuadArrayii(vp);
    }
}

void NullOpenGLGraphics::drawRescaledPattern(const Image *restrict const image,
                                             const int x, const int y,
                                             const int w, const int h,
                                             const int scaledWidth,
                                             const int scaledHeight) restrict2
{
    if (image == nullptr)
        return;

    if (scaledWidth == 0 || scaledHeight == 0)
        return;

    const SDL_Rect &imageRect = image->mBounds;
    const int iw = imageRect.w;
    const int ih = imageRect.h;
    if (iw == 0 || ih == 0)
        return;

    const int srcX = imageRect.x;
    const int srcY = imageRect.y;

    setColorAlpha(image->mAlpha);

#ifdef DEBUG_BIND_TEXTURE
    debugBindTexture(image);
#endif  // DEBUG_BIND_TEXTURE

    bindTexture(OpenGLImageHelper::mTextureType, image->mGLImage);

    enableTexturingAndBlending();

    unsigned int vp = 0;
    const unsigned int vLimit = mMaxVertices * 4;

    // Draw a set of textured rectangles
    if (OpenGLImageHelper::mTextureType == GL_TEXTURE_2D)
    {
        const float tw = static_cast<float>(image->mTexWidth);
        const float th = static_cast<float>(image->mTexHeight);

        const float texX1 = static_cast<float>(srcX) / tw;
        const float texY1 = static_cast<float>(srcY) / th;

        const float tFractionW = iw / tw;
        const float tFractionH = ih / th;

        for (int py = 0; py < h; py += scaledHeight)
        {
            const int height = (py + scaledHeight >= h)
                ? h - py : scaledHeight;
            const int dstY = y + py;
            const float visibleFractionH = static_cast<float>(height)
                / scaledHeight;
            const float texY2 = texY1 + tFractionH * visibleFractionH;
            for (int px = 0; px < w; px += scaledWidth)
            {
                const int width = (px + scaledWidth >= w)
                    ? w - px : scaledWidth;
                const int dstX = x + px;
                const float visibleFractionW = static_cast<float>(width)
                    / scaledWidth;
                const float texX2 = texX1 + tFractionW * visibleFractionW;

                mFloatTexArray[vp + 0] = texX1;
                mFloatTexArray[vp + 1] = texY1;

                mFloatTexArray[vp + 2] = texX2;
                mFloatTexArray[vp + 3] = texY1;

                mFloatTexArray[vp + 4] = texX2;
                mFloatTexArray[vp + 5] = texY2;

                mFloatTexArray[vp + 6] = texX1;
                mFloatTexArray[vp + 7] = texY2;

                mIntVertArray[vp + 0] = dstX;
                mIntVertArray[vp + 1] = dstY;

                mIntVertArray[vp + 2] = dstX + width;
                mIntVertArray[vp + 3] = dstY;

                mIntVertArray[vp + 4] = dstX + width;
                mIntVertArray[vp + 5] = dstY + height;

                mIntVertArray[vp + 6] = dstX;
                mIntVertArray[vp + 7] = dstY + height;

                vp += 8;
                if (vp >= vLimit)
                {
                    drawQuadArrayfi(vp);
                    vp = 0;
                }
            }
        }
        if (vp > 0)
            drawQuadArrayfi(vp);
    }
    else
    {
        const float scaleFactorW = static_cast<float>(scaledWidth) / iw;
        const float scaleFactorH = static_cast<float>(scaledHeight) / ih;

        for (int py = 0; py < h; py += scaledHeight)
        {
            const int height = (py + scaledHeight >= h)
                ? h - py : scaledHeight;
            const int dstY = y + py;
            const int scaledY = srcY + height / scaleFactorH;
            for (int px = 0; px < w; px += scaledWidth)
            {
                const int width = (px + scaledWidth >= w)
                    ? w - px : scaledWidth;
                const int dstX = x + px;
                const int scaledX = srcX + width / scaleFactorW;

                mIntTexArray[vp + 0] = srcX;
                mIntTexArray[vp + 1] = srcY;

                mIntTexArray[vp + 2] = scaledX;
                mIntTexArray[vp + 3] = srcY;

                mIntTexArray[vp + 4] = scaledX;
                mIntTexArray[vp + 5] = scaledY;

                mIntTexArray[vp + 6] = srcX;
                mIntTexArray[vp + 7] = scaledY;

                mIntVertArray[vp + 0] = dstX;
                mIntVertArray[vp + 1] = dstY;

                mIntVertArray[vp + 2] = dstX + width;
                mIntVertArray[vp + 3] = dstY;

                mIntVertArray[vp + 4] = dstX + width;
                mIntVertArray[vp + 5] = dstY + height;

                mIntVertArray[vp + 6] = dstX;
                mIntVertArray[vp + 7] = dstY + height;

                vp += 8;
                if (vp >= vLimit)
                {
                    drawQuadArrayii(vp);
                    vp = 0;
                }
            }
        }
        if (vp > 0)
            drawQuadArrayii(vp);
    }
}

inline void NullOpenGLGraphics::drawVertexes(const
                                             OpenGLGraphicsVertexes
                                             &restrict ogl) restrict2
{
    const STD_VECTOR<GLint*> &intVertPool = ogl.mIntVertPool;
    STD_VECTOR<GLint*>::const_iterator iv;
    const STD_VECTOR<GLint*>::const_iterator iv_end = intVertPool.end();
    const STD_VECTOR<int> &vp = ogl.mVp;
    STD_VECTOR<int>::const_iterator ivp;
    const STD_VECTOR<int>::const_iterator ivp_end = vp.end();

    // Draw a set of textured rectangles
    if (OpenGLImageHelper::mTextureType == GL_TEXTURE_2D)
    {
        const STD_VECTOR<GLfloat*> &floatTexPool = ogl.mFloatTexPool;
        STD_VECTOR<GLfloat*>::const_iterator ft;
        const STD_VECTOR<GLfloat*>::const_iterator
            ft_end = floatTexPool.end();

        for (iv = intVertPool.begin(), ft = floatTexPool.begin(),
             ivp = vp.begin();
             iv != iv_end && ft != ft_end && ivp != ivp_end;
             ++ iv, ++ ft, ++ ivp)
        {
            drawQuadArrayfi(*iv, *ft, *ivp);
        }
    }
    else
    {
        const STD_VECTOR<GLint*> &intTexPool = ogl.mIntTexPool;
        STD_VECTOR<GLint*>::const_iterator it;
        const STD_VECTOR<GLint*>::const_iterator it_end = intTexPool.end();

        for (iv = intVertPool.begin(), it = intTexPool.begin(),
             ivp = vp.begin();
             iv != iv_end && it != it_end && ivp != ivp_end;
             ++ iv, ++ it, ++ ivp)
        {
            drawQuadArrayii(*iv, *it, *ivp);
        }
    }
}

void NullOpenGLGraphics::calcPattern(ImageVertexes *restrict const vert,
                                     const Image *restrict const image,
                                     const int x, const int y,
                                     const int w, const int h) const restrict2
{
    calcPatternInline(vert, image, x, y, w, h);
}

void NullOpenGLGraphics::calcPatternInline(ImageVertexes *restrict const vert,
                                           const Image *restrict const image,
                                           const int x,
                                           const int y,
                                           const int w,
                                           const int h) const restrict2
{
    if (image == nullptr || vert == nullptr)
        return;

    const SDL_Rect &imageRect = image->mBounds;
    const int iw = imageRect.w;
    const int ih = imageRect.h;

    if (iw == 0 || ih == 0)
        return;

    const int srcX = imageRect.x;
    const int srcY = imageRect.y;
    const unsigned int vLimit = mMaxVertices * 4;

    OpenGLGraphicsVertexes &ogl = vert->ogl;
    unsigned int vp = ogl.continueVp();

    // Draw a set of textured rectangles
    if (OpenGLImageHelper::mTextureType == GL_TEXTURE_2D)
    {
        const float tw = static_cast<float>(image->mTexWidth);
        const float th = static_cast<float>(image->mTexHeight);

        const float texX1 = static_cast<float>(srcX) / tw;
        const float texY1 = static_cast<float>(srcY) / th;

        GLfloat *floatTexArray = ogl.continueFloatTexArray();
        GLint *intVertArray = ogl.continueIntVertArray();

        for (int py = 0; py < h; py += ih)
        {
            const int height = (py + ih >= h) ? h - py : ih;
            const int dstY = y + py;
            const float texY2 = static_cast<float>(srcY + height) / th;
            for (int px = 0; px < w; px += iw)
            {
                const int width = (px + iw >= w) ? w - px : iw;
                const int dstX = x + px;
                const float texX2 = static_cast<float>(srcX + width) / tw;

                floatTexArray[vp + 0] = texX1;
                floatTexArray[vp + 1] = texY1;

                floatTexArray[vp + 2] = texX2;
                floatTexArray[vp + 3] = texY1;

                floatTexArray[vp + 4] = texX2;
                floatTexArray[vp + 5] = texY2;

                floatTexArray[vp + 6] = texX1;
                floatTexArray[vp + 7] = texY2;

                intVertArray[vp + 0] = dstX;
                intVertArray[vp + 1] = dstY;

                intVertArray[vp + 2] = dstX + width;
                intVertArray[vp + 3] = dstY;

                intVertArray[vp + 4] = dstX + width;
                intVertArray[vp + 5] = dstY + height;

                intVertArray[vp + 6] = dstX;
                intVertArray[vp + 7] = dstY + height;

                vp += 8;
                if (vp >= vLimit)
                {
                    floatTexArray = ogl.switchFloatTexArray();
                    intVertArray = ogl.switchIntVertArray();
                    ogl.switchVp(vp);
                    vp = 0;
                }
            }
        }
    }
    else
    {
        GLint *intTexArray = ogl.continueIntTexArray();
        GLint *intVertArray = ogl.continueIntVertArray();

        for (int py = 0; py < h; py += ih)
        {
            const int height = (py + ih >= h) ? h - py : ih;
            const int dstY = y + py;
            for (int px = 0; px < w; px += iw)
            {
                const int width = (px + iw >= w) ? w - px : iw;
                const int dstX = x + px;

                intTexArray[vp + 0] = srcX;
                intTexArray[vp + 1] = srcY;

                intTexArray[vp + 2] = srcX + width;
                intTexArray[vp + 3] = srcY;

                intTexArray[vp + 4] = srcX + width;
                intTexArray[vp + 5] = srcY + height;

                intTexArray[vp + 6] = srcX;
                intTexArray[vp + 7] = srcY + height;

                intVertArray[vp + 0] = dstX;
                intVertArray[vp + 1] = dstY;

                intVertArray[vp + 2] = dstX + width;
                intVertArray[vp + 3] = dstY;

                intVertArray[vp + 4] = dstX + width;
                intVertArray[vp + 5] = dstY + height;

                intVertArray[vp + 6] = dstX;
                intVertArray[vp + 7] = dstY + height;

                vp += 8;
                if (vp >= vLimit)
                {
                    intTexArray = ogl.switchIntTexArray();
                    intVertArray = ogl.switchIntVertArray();
                    ogl.switchVp(vp);
                    vp = 0;
                }
            }
        }
    }
    ogl.switchVp(vp);
}

void NullOpenGLGraphics::calcTileCollection(ImageCollection *
                                            restrict const vertCol,
                                            const Image *restrict const image,
                                            int x, int y) restrict2
{
    if (vertCol == nullptr || image == nullptr)
        return;
    if (vertCol->currentGLImage != image->mGLImage)
    {
        ImageVertexes *const vert = new ImageVertexes;
        vertCol->currentGLImage = image->mGLImage;
        vertCol->currentVert = vert;
        vert->image = image;
        vertCol->draws.push_back(vert);
        calcTileVertexesInline(vert, image, x, y);
    }
    else
    {
        calcTileVertexesInline(vertCol->currentVert, image, x, y);
    }
}

void NullOpenGLGraphics::drawTileCollection(const ImageCollection
                                            *restrict const vertCol) restrict2
{
    const ImageVertexesVector &draws = vertCol->draws;
    const ImageCollectionCIter it_end = draws.end();
    for (ImageCollectionCIter it = draws.begin(); it != it_end; ++ it)
    {
        const ImageVertexes *const vert = *it;
        const Image *const image = vert->image;

        setColorAlpha(image->mAlpha);
#ifdef DEBUG_BIND_TEXTURE
        debugBindTexture(image);
#endif  // DEBUG_BIND_TEXTURE

        bindTexture(OpenGLImageHelper::mTextureType, image->mGLImage);
        enableTexturingAndBlending();
        drawVertexes(vert->ogl);
    }
}

void NullOpenGLGraphics::calcPattern(ImageCollection *restrict  const vertCol,
                                     const Image *restrict const image,
                                     const int x, const int y,
                                     const int w, const int h) const restrict2
{
    if (vertCol == nullptr || image == nullptr)
        return;
    ImageVertexes *vert = nullptr;
    if (vertCol->currentGLImage != image->mGLImage)
    {
        vert = new ImageVertexes;
        vertCol->currentGLImage = image->mGLImage;
        vertCol->currentVert = vert;
        vert->image = image;
        vertCol->draws.push_back(vert);
    }
    else
    {
        vert = vertCol->currentVert;
    }

    calcPatternInline(vert, image, x, y, w, h);
}

void NullOpenGLGraphics::calcTileVertexes(ImageVertexes *restrict const vert,
                                          const Image *restrict const image,
                                          int dstX, int dstY) const restrict2
{
    calcTileVertexesInline(vert, image, dstX, dstY);
}

void NullOpenGLGraphics::calcTileVertexesInline(ImageVertexes *
                                                restrict const vert,
                                                const Image *
                                                restrict const image,
                                                int dstX,
                                                int dstY) const restrict2
{
    const SDL_Rect &imageRect = image->mBounds;
    const int w = imageRect.w;
    const int h = imageRect.h;

    if (w == 0 || h == 0)
        return;

    const int srcX = imageRect.x;
    const int srcY = imageRect.y;

    const unsigned int vLimit = mMaxVertices * 4;

    OpenGLGraphicsVertexes &ogl = vert->ogl;

//    STD_VECTOR<int> *vps = ogl.getVp();
    unsigned int vp = ogl.continueVp();

    // Draw a set of textured rectangles
    if (OpenGLImageHelper::mTextureType == GL_TEXTURE_2D)
    {
        const float tw = static_cast<float>(image->mTexWidth);
        const float th = static_cast<float>(image->mTexHeight);

        const float texX1 = static_cast<float>(srcX) / tw;
        const float texY1 = static_cast<float>(srcY) / th;

        const float texX2 = static_cast<float>(srcX + w) / tw;
        const float texY2 = static_cast<float>(srcY + h) / th;

        GLfloat *const floatTexArray = ogl.continueFloatTexArray();
        GLint *const intVertArray = ogl.continueIntVertArray();

        floatTexArray[vp + 0] = texX1;
        floatTexArray[vp + 1] = texY1;

        floatTexArray[vp + 2] = texX2;
        floatTexArray[vp + 3] = texY1;

        floatTexArray[vp + 4] = texX2;
        floatTexArray[vp + 5] = texY2;

        floatTexArray[vp + 6] = texX1;
        floatTexArray[vp + 7] = texY2;

        intVertArray[vp + 0] = dstX;
        intVertArray[vp + 1] = dstY;

        intVertArray[vp + 2] = dstX + w;
        intVertArray[vp + 3] = dstY;

        intVertArray[vp + 4] = dstX + w;
        intVertArray[vp + 5] = dstY + h;

        intVertArray[vp + 6] = dstX;
        intVertArray[vp + 7] = dstY + h;

        vp += 8;
        if (vp >= vLimit)
        {
            ogl.switchFloatTexArray();
            ogl.switchIntVertArray();
            ogl.switchVp(vp);
            vp = 0;
        }
    }
    else
    {
        GLint *const intTexArray = ogl.continueIntTexArray();
        GLint *const intVertArray = ogl.continueIntVertArray();

        intTexArray[vp + 0] = srcX;
        intTexArray[vp + 1] = srcY;

        intTexArray[vp + 2] = srcX + w;
        intTexArray[vp + 3] = srcY;

        intTexArray[vp + 4] = srcX + w;
        intTexArray[vp + 5] = srcY + h;

        intTexArray[vp + 6] = srcX;
        intTexArray[vp + 7] = srcY + h;

        intVertArray[vp + 0] = dstX;
        intVertArray[vp + 1] = dstY;

        intVertArray[vp + 2] = dstX + w;
        intVertArray[vp + 3] = dstY;

        intVertArray[vp + 4] = dstX + w;
        intVertArray[vp + 5] = dstY + h;

        intVertArray[vp + 6] = dstX;
        intVertArray[vp + 7] = dstY + h;

        vp += 8;
        if (vp >= vLimit)
        {
            ogl.switchIntTexArray();
            ogl.switchIntVertArray();
            ogl.switchVp(vp);
            vp = 0;
        }
    }
    ogl.switchVp(vp);
}

void NullOpenGLGraphics::drawTileVertexes(const ImageVertexes *
                                          restrict const vert) restrict2
{
    if (vert == nullptr)
        return;
    const Image *const image = vert->image;

    setColorAlpha(image->mAlpha);
#ifdef DEBUG_BIND_TEXTURE
    debugBindTexture(image);
#endif  // DEBUG_BIND_TEXTURE

    bindTexture(OpenGLImageHelper::mTextureType, image->mGLImage);
    enableTexturingAndBlending();
    drawVertexes(vert->ogl);
}

void NullOpenGLGraphics::calcWindow(ImageCollection *restrict const vertCol,
                                    const int x, const int y,
                                    const int w, const int h,
                                    const ImageRect &restrict imgRect)
                                    restrict2
{
    ImageVertexes *vert = nullptr;
    Image *const image = imgRect.grid[4];
    if (image == nullptr)
        return;
    if (vertCol->currentGLImage != image->mGLImage)
    {
        vert = new ImageVertexes;
        vertCol->currentGLImage = image->mGLImage;
        vertCol->currentVert = vert;
        vert->image = image;
        vertCol->draws.push_back(vert);
    }
    else
    {
        vert = vertCol->currentVert;
    }
    calcImageRect(vert, x, y, w, h, imgRect);
}

void NullOpenGLGraphics::updateScreen() restrict2
{
    BLOCK_START("Graphics::updateScreen")
#ifdef DEBUG_DRAW_CALLS
    mLastDrawCalls = mDrawCalls;
    mDrawCalls = 0;
#endif  // DEBUG_DRAW_CALLS

    BLOCK_END("Graphics::updateScreen")
}

void NullOpenGLGraphics::beginDraw() restrict2
{
    pushClipArea(Rect(0, 0, 640, 480));
}

void NullOpenGLGraphics::endDraw() restrict2
{
    popClipArea();
}

void NullOpenGLGraphics::pushClipArea(const Rect &restrict area) restrict2
{
    int transX = 0;
    int transY = 0;

    if (!mClipStack.empty())
    {
        const ClipRect &clipArea = mClipStack.top();
        transX = -clipArea.xOffset;
        transY = -clipArea.yOffset;
    }

    Graphics::pushClipArea(area);

    const ClipRect &clipArea = mClipStack.top();
    transX += clipArea.xOffset;
    transY += clipArea.yOffset;
}

void NullOpenGLGraphics::popClipArea() restrict2
{
    Graphics::popClipArea();

    if (mClipStack.empty())
        return;
}

void NullOpenGLGraphics::drawPoint(int x A_UNUSED, int y A_UNUSED) restrict2
{
    disableTexturingAndBlending();
    restoreColor();
}

void NullOpenGLGraphics::drawLine(int x1, int y1,
                                  int x2, int y2) restrict2
{
    disableTexturingAndBlending();
    restoreColor();

    mFloatTexArray[0] = static_cast<float>(x1) + 0.5F;
    mFloatTexArray[1] = static_cast<float>(y1) + 0.5F;
    mFloatTexArray[2] = static_cast<float>(x2) + 0.5F;
    mFloatTexArray[3] = static_cast<float>(y2) + 0.5F;

    drawLineArrayf(4);
}

void NullOpenGLGraphics::drawRectangle(const Rect &restrict rect) restrict2
{
    drawRectangle(rect, false);
}

void NullOpenGLGraphics::fillRectangle(const Rect &restrict rect) restrict2
{
    drawRectangle(rect, true);
}

void NullOpenGLGraphics::enableTexturingAndBlending() restrict2
{
    if (!mTexture)
        mTexture = true;

    if (!mAlpha)
        mAlpha = true;
}

void NullOpenGLGraphics::disableTexturingAndBlending() restrict2
{
    mTextureBinded = 0;
    if (mAlpha && !mColorAlpha)
        mAlpha = false;
    else if (!mAlpha && mColorAlpha)
        mAlpha = true;

    if (mTexture)
        mTexture = false;
}

void NullOpenGLGraphics::drawRectangle(const Rect &restrict rect A_UNUSED,
                                       const bool filled A_UNUSED) restrict2
{
    BLOCK_START("Graphics::drawRectangle")
    disableTexturingAndBlending();
    restoreColor();

#ifdef DEBUG_DRAW_CALLS
        mDrawCalls ++;
#endif  // DEBUG_DRAW_CALLS

    BLOCK_END("Graphics::drawRectangle")
}

void NullOpenGLGraphics::drawNet(const int x1, const int y1,
                                 const int x2, const int y2,
                                 const int width, const int height) restrict2
{
    unsigned int vp = 0;
    const unsigned int vLimit = mMaxVertices * 4;

    disableTexturingAndBlending();
    restoreColor();

    const float xf1 = static_cast<float>(x1);
    const float xf2 = static_cast<float>(x2);
    const float yf1 = static_cast<float>(y1);
    const float yf2 = static_cast<float>(y2);

    for (int y = y1; y < y2; y += height)
    {
        mFloatTexArray[vp + 0] = xf1;
        mFloatTexArray[vp + 1] = static_cast<float>(y);

        mFloatTexArray[vp + 2] = xf2;
        mFloatTexArray[vp + 3] = static_cast<float>(y);

        vp += 4;
        if (vp >= vLimit)
        {
            drawLineArrayf(vp);
            vp = 0;
        }
    }

    for (int x = x1; x < x2; x += width)
    {
        mFloatTexArray[vp + 0] = static_cast<float>(x);
        mFloatTexArray[vp + 1] = yf1;

        mFloatTexArray[vp + 2] = static_cast<float>(x);
        mFloatTexArray[vp + 3] = yf2;

        vp += 4;
        if (vp >= vLimit)
        {
            drawLineArrayf(vp);
            vp = 0;
        }
    }

    if (vp > 0)
        drawLineArrayf(vp);
}

void NullOpenGLGraphics::bindTexture(const GLenum target A_UNUSED,
                                     const GLuint texture)
{
    if (mTextureBinded != texture)
        mTextureBinded = texture;
}

inline void NullOpenGLGraphics::drawQuadArrayfi(const int size A_UNUSED)
                                                restrict2
{
#ifdef DEBUG_DRAW_CALLS
    mDrawCalls ++;
#endif  // DEBUG_DRAW_CALLS
}

inline void NullOpenGLGraphics::drawQuadArrayfi(const GLint *restrict const
                                                intVertArray A_UNUSED,
                                                const GLfloat *restrict const
                                                floatTexArray A_UNUSED,
                                                const int size A_UNUSED)
                                                restrict2
{
#ifdef DEBUG_DRAW_CALLS
    mDrawCalls ++;
#endif  // DEBUG_DRAW_CALLS
}

inline void NullOpenGLGraphics::drawQuadArrayii(const int size A_UNUSED)
                                                restrict2
{
#ifdef DEBUG_DRAW_CALLS
    mDrawCalls ++;
#endif  // DEBUG_DRAW_CALLS
}

inline void NullOpenGLGraphics::drawQuadArrayii(const GLint *restrict const
                                                intVertArray A_UNUSED,
                                                const GLint *restrict const
                                                intTexArray A_UNUSED,
                                                const int size A_UNUSED)
                                                restrict2
{
#ifdef DEBUG_DRAW_CALLS
    mDrawCalls ++;
#endif  // DEBUG_DRAW_CALLS
}

inline void NullOpenGLGraphics::drawLineArrayi(const int size A_UNUSED)
                                               restrict2
{
#ifdef DEBUG_DRAW_CALLS
    mDrawCalls ++;
#endif  // DEBUG_DRAW_CALLS
}

inline void NullOpenGLGraphics::drawLineArrayf(const int size A_UNUSED)
                                               restrict2
{
#ifdef DEBUG_DRAW_CALLS
    mDrawCalls ++;
#endif  // DEBUG_DRAW_CALLS
}

void NullOpenGLGraphics::dumpSettings()
{
}

void NullOpenGLGraphics::setColorAlpha(const float alpha) restrict2
{
    if (!mIsByteColor && mFloatColor == alpha)
        return;

    mIsByteColor = false;
    mFloatColor = alpha;
}

void NullOpenGLGraphics::restoreColor() restrict2
{
    if (mIsByteColor && mByteColor == mColor)
        return;

    mIsByteColor = true;
    mByteColor = mColor;
}

void NullOpenGLGraphics::drawImageRect(const int x, const int y,
                                       const int w, const int h,
                                       const ImageRect &restrict imgRect)
                                       restrict2
{
    #include "render/graphics_drawImageRect.hpp"
}

void NullOpenGLGraphics::calcImageRect(ImageVertexes *restrict const vert,
                                       const int x, const int y,
                                       const int w, const int h,
                                       const ImageRect &restrict imgRect)
                                       restrict2
{
    #include "render/graphics_calcImageRect.hpp"
}

void NullOpenGLGraphics::clearScreen() const restrict2
{
}

#ifdef DEBUG_BIND_TEXTURE
void NullOpenGLGraphics::debugBindTexture(const Image *restrict const image)
                                          restrict2
{
    const std::string texture = image->mIdPath;
    if (mOldTexture != texture)
    {
        if ((!mOldTexture.empty() || !texture.empty())
            && mOldTextureId != image->mGLImage)
        {
            logger->log("bind: %s (%d) to %s (%d)", mOldTexture.c_str(),
                mOldTextureId, texture.c_str(), image->mGLImage);
        }
        mOldTextureId = image->mGLImage;
        mOldTexture = texture;
    }
}
#else  // DEBUG_BIND_TEXTURE

void NullOpenGLGraphics::debugBindTexture(const Image *restrict const
                                          image A_UNUSED) restrict2
{
}
#endif  // DEBUG_BIND_TEXTURE
#endif  // USE_OPENGL
