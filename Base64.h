
class CBase64  
{
    // Internal bucket class.
    class TempBucket
    {
      public:
	unsigned char		nData[4];
	unsigned char		nSize;
	void		Clear() { ::ZeroMemory(nData, 4); nSize = 0; };
    };

    unsigned char*					m_pDBuffer;
    unsigned char*					m_pEBuffer;
    unsigned long					m_nDBufLen;
    unsigned long					m_nEBufLen;
    unsigned long					m_nDDataLen;
    unsigned long					m_nEDataLen;

  public:
    CBase64();
    virtual ~CBase64();

  public:
    virtual void		Encode(const unsigned char*, unsigned long);
    virtual void		Decode(const unsigned char*, unsigned long);
    virtual void		Encode(const char* sMessage);
    virtual void		Decode(const char* sMessage);

    virtual const char*	DecodedMessage() const;
    virtual const char*	EncodedMessage() const;

    virtual void		AllocEncode(unsigned long);
    virtual void		AllocDecode(unsigned long);
    virtual void		SetEncodeBuffer(const unsigned char* pBuffer, unsigned long nBufLen);
    virtual void		SetDecodeBuffer(const unsigned char* pBuffer, unsigned long nBufLen);

  protected:
    virtual void		_EncodeToBuffer(const TempBucket &Decode, unsigned char* pBuffer);
    virtual unsigned long		_DecodeToBuffer(const TempBucket &Decode, unsigned char* pBuffer);
    virtual void		_EncodeRaw(TempBucket &, const TempBucket &);
    virtual void		_DecodeRaw(TempBucket &, const TempBucket &);
    virtual BOOL		_IsBadMimeChar(unsigned char);

    static  char		m_DecodeTable[256];
    static  BOOL		m_Init;
    void					_Init();
};


