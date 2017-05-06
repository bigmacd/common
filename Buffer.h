#ifndef __BUFFER_H__
#define __BUFFER_H__

class Buffer
{

  private:

    char*           mInternalBuffer;
    unsigned int    mCurrentSize;
    unsigned int    mLeft;

  protected:


  public:

    Buffer(int size = 4096);
    Buffer(Buffer& buffer);
    ~Buffer();

    void            Append(const char* data, int size);

    const char*     GetBuffer();
    void            Clear();

    
    unsigned int    CurrentSize() { return mCurrentSize; }

    unsigned int    Left() { return mLeft; } 


};

#endif
