#ifndef __BUFFER_H__
#include "Buffer.h"
#endif

#include "compat.h"
#include <string.h>

Buffer::Buffer(int size)
       :mInternalBuffer(NULL),
	mCurrentSize(0),
	mLeft(size)
{
  mInternalBuffer = new char[size];
  memset(mInternalBuffer, 0, size);
}

Buffer::Buffer(Buffer& right)
{
  mCurrentSize = right.CurrentSize();
  mLeft = right.Left();
  mInternalBuffer = new char [mCurrentSize + mLeft];
  memcpy(mInternalBuffer, right.GetBuffer(), mCurrentSize);
}

Buffer::~Buffer()
{
  delete [] mInternalBuffer;
}

void
Buffer::Append(const char* data, int size)
{
  if ((unsigned int)size > mLeft)
  {
    mLeft = mCurrentSize + mLeft + size;
    char* temp = new char[mLeft];
    memset(temp, 0, mLeft);

    memcpy(temp, mInternalBuffer, mCurrentSize);
    mLeft -= mCurrentSize;
    char* deleteMe = mInternalBuffer;

    mInternalBuffer = temp;
    delete [] deleteMe;
  }
  memcpy(&mInternalBuffer[mCurrentSize], data, size);
  mCurrentSize += size;
  mLeft -= size;
}

const char* 
Buffer::GetBuffer()
{
  return (const char*)mInternalBuffer;
}

void
Buffer::Clear()
{
  memset(mInternalBuffer, 0, mLeft + mCurrentSize);
  mLeft += mCurrentSize;
  mCurrentSize = 0;
}
