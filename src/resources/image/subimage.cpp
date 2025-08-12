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

#include "resources/image/subimage.h"

#include "utils/cast.h"

#ifdef DEBUG_IMAGES
#include "logger.h"
#endif  // DEBUG_IMAGES

#include "debug.h"

#ifdef USE_SDL2
SubImage::SubImage(Image *const parent,
                   SDL_Texture *const image,
                   const int x, const int y,
                   const int width, const int height) :
    Image(image, width, height),
    mInternalBounds(),
    mParent(parent)
{
    if (mParent)
    {
        mParent->incRef();
        mParent->SDLTerminateAlphaCache();
        mHasAlphaChannel = mParent->hasAlphaChannel();
        mIsAlphaVisible = mHasAlphaChannel;
        mAlphaChannel = mParent->SDLgetAlphaChannel();
        mSource = parent->mIdPath;
#ifdef DEBUG_IMAGES
        logger->log("set name2 %p, %s",
            static_cast<void*>(this),
            mSource.c_str());
#endif  // DEBUG_IMAGES
#ifdef DEBUG_BIND_TEXTURE
        mIdPath = parent->mIdPath;
#endif  // DEBUG_BIND_TEXTURE
    }
    else
    {
        mHasAlphaChannel = false;
        mIsAlphaVisible = false;
        mAlphaChannel = nullptr;
    }

    // Set up the rectangle.
    mBounds.x = CAST_S16(x);
    mBounds.y = CAST_S16(y);
    mBounds.w = CAST_U16(width);
    mBounds.h = CAST_U16(height);
    if (mParent)
    {
        mInternalBounds.x = mParent->mBounds.x;
        mInternalBounds.y = mParent->mBounds.y;
        mInternalBounds.w = mParent->mBounds.w;
        mInternalBounds.h = mParent->mBounds.h;
    }
    else
    {
        mInternalBounds.x = 0;
        mInternalBounds.y = 0;
        mInternalBounds.w = 1;
        mInternalBounds.h = 1;
    }
    mUseAlphaCache = false;
}
#endif  // USE_SDL2

SubImage::SubImage(Image *const parent,
                   SDL_Surface *const image,
                   const int x, const int y,
                   const int width, const int height) :
    Image(image, false, nullptr),
    mInternalBounds(),
    mParent(parent)
{
    if (mParent != nullptr)
    {
        mParent->incRef();
        mParent->SDLTerminateAlphaCache();
        mHasAlphaChannel = mParent->hasAlphaChannel();
        mIsAlphaVisible = mHasAlphaChannel;
        mAlphaChannel = mParent->SDLgetAlphaChannel();
        mSource = parent->mIdPath;
#ifdef DEBUG_IMAGES
        logger->log("set name2 %p, %s", static_cast<void*>(this),
            mSource.c_str());
#endif  // DEBUG_IMAGES
#ifdef DEBUG_BIND_TEXTURE
        mIdPath = parent->mIdPath;
#endif  // DEBUG_BIND_TEXTURE
    }
    else
    {
        mHasAlphaChannel = false;
        mIsAlphaVisible = false;
        mAlphaChannel = nullptr;
    }

    // Set up the rectangle.
    mBounds.x = CAST_S16(x);
    mBounds.y = CAST_S16(y);
    mBounds.w = CAST_U16(width);
    mBounds.h = CAST_U16(height);
    if (mParent != nullptr)
    {
        mInternalBounds.x = mParent->mBounds.x;
        mInternalBounds.y = mParent->mBounds.y;
        mInternalBounds.w = mParent->mBounds.w;
        mInternalBounds.h = mParent->mBounds.h;
    }
    else
    {
        mInternalBounds.x = 0;
        mInternalBounds.y = 0;
        mInternalBounds.w = 1;
        mInternalBounds.h = 1;
    }
    mUseAlphaCache = false;
}

#ifdef USE_OPENGL
SubImage::SubImage(Image *const parent,
                   const GLuint image,
                   const int x, const int y,
                   const int width, const int height,
                   const int texWidth, const int texHeight) :
    Image(image, width, height, texWidth, texHeight),
    mInternalBounds(),
    mParent(parent)
{
    if (mParent != nullptr)
        mParent->incRef();

    // Set up the rectangle.
    mBounds.x = CAST_S16(x);
    mBounds.y = CAST_S16(y);
    mBounds.w = CAST_U16(width);
    mBounds.h = CAST_U16(height);
    if (mParent != nullptr)
    {
        mInternalBounds.x = mParent->mBounds.x;
        mInternalBounds.y = mParent->mBounds.y;
        mInternalBounds.w = mParent->mBounds.w;
        mInternalBounds.h = mParent->mBounds.h;
        mSource = parent->mIdPath;
#ifdef DEBUG_IMAGES
        logger->log("set name2 %p, %s", static_cast<void*>(this),
            mSource.c_str());
#endif  // DEBUG_IMAGES
#ifdef DEBUG_BIND_TEXTURE
        mIdPath = parent->mIdPath;
#endif  // DEBUG_BIND_TEXTURE
    }
    else
    {
        mInternalBounds.x = 0;
        mInternalBounds.y = 0;
        mInternalBounds.w = 1;
        mInternalBounds.h = 1;
    }
    mIsAlphaVisible = mHasAlphaChannel;
}
#endif  // USE_OPENGL

SubImage::~SubImage()
{
#ifdef DEBUG_IMAGES
    logger->log("delete subimage: %p", static_cast<void*>(this));
    logger->log("  %s, %s", mIdPath.c_str(), mSource.c_str());
#endif  // DEBUG_IMAGES
    // Avoid destruction of the image
    mSDLSurface = nullptr;
    // Avoid possible destruction of its alpha channel
    mAlphaChannel = nullptr;
#ifdef USE_SDL2
    // Avoid destruction of texture
    mTexture = nullptr;
#endif  // USE_SDL2
#ifdef USE_OPENGL
    mGLImage = 0;
#endif  // USE_OPENGL
    if (mParent != nullptr)
    {
#ifdef DEBUG_IMAGES
        logger->log("decref from subminage: %p, parent: %p",
            static_cast<void*>(this), static_cast<void*>(mParent));
#endif  // DEBUG_IMAGES

        mParent->decRef();
        mParent = nullptr;
    }
}

Image *SubImage::getSubImage(const int x, const int y,
                             const int w, const int h)
{
    if (mParent != nullptr)
        return mParent->getSubImage(mBounds.x + x, mBounds.y + y, w, h);
    return nullptr;
}

#ifdef USE_OPENGL
void SubImage::decRef()
{
    Resource::decRef();
}
#endif  // USE_OPENGL

int SubImage::calcMemoryLocal() const
{
    return static_cast<int>(sizeof(SubImage) +
        sizeof(std::map<float, SDL_Surface*>)) +
        Image::calcMemoryLocal();
}
