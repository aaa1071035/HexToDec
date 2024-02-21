#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

static uint8_t m_bBuffer[16];

typedef union
{
   uint8_t bData;
   uint16_t uData;
   uint32_t ulData;
   uint64_t ullData;
   uint8_t bDatas[8];
} Data_t;

#define HEX_TO_DEC_ERROR_NULL_POINTER           (0xFFFF)
#define HEX_TO_DEC_ERROR_NOT_HEX_STRING         (0xFFFE)

uint8_t CharToInt3(char cData)
{
    uint8_t bValue;
    bValue=((uint8_t)(cData-'0'));
    if(9<bValue)
    {
        bValue=(((bValue+'0') & (~('a'^'A'))-'A'));
        if(5<bValue)
        {
            bValue=HEX_TO_DEC_ERROR_NOT_HEX_STRING;
        }
        else
        {
            bValue+=10;
        }
    }
    return bValue;
}

uint8_t CharToInt(char bText)
{
   uint8_t uValue;
   uValue=HEX_TO_DEC_ERROR_NOT_HEX_STRING;
   if(('0'<=bText) && ('9'>=bText))
   {
       uValue=(bText-'0');
   }
   else
   {
       bText&=(~('a'^'A'));
       if(('A'<=bText) && ('F'>=bText))
       {
           uValue=(bText-'A'+10);
       }
   }
   return uValue;
}


uint8_t CharToInt2(char bText)
{
   uint8_t uNum=bText;
   if(uNum>='A' && uNum<='F')
   {
       uNum=uNum-55;
   }
   else if(uNum>='0' && uNum<='9')
   {
       uNum=uNum-48;
   }
   else if(uNum=='\0')
   {
       uNum=16;
   }
   else
   {
       uNum=17;
   }
   return uNum;
}

void mymemset(void* p, uint8_t bData, uint16_t uLength)
{
   memset(p, bData, uLength);
}

void reverse(char* pszDec, uint8_t uDecLength)
{
    char bTemp;
    uint8_t ArrayMax;
    ArrayMax=uDecLength-1;
    for(int i=0;i<=ArrayMax/2;i++)
    {
        bTemp=pszDec[i]+'0';
        pszDec[i]=pszDec[ArrayMax-i]+'0';
        pszDec[ArrayMax-i]=bTemp;
    }
}

uint8_t uQuotient(uint8_t uDividend, uint8_t uDivisor)
{
    uint8_t uCount;
    uCount=0;
    for(int i=1;uDividend>=10;i++)
    {
        uDividend-=uDivisor;
        uCount=i;
    }
    return uCount;
}
uint8_t uRemainder(uint8_t uDividend, uint8_t uDivisor)
{
    uint8_t uCount;
    uint8_t uRemainder;
    uRemainder=uDividend;
    uCount=uQuotient(uDividend, uDivisor);
    for(int i=0;i<uCount;i++)
    {
        uRemainder-=uDivisor;
    }
    return uRemainder;
}

uint8_t HexToDec_V1(char* pszDec, uint16_t uMaxDecLength, const char* pszHex, uint16_t uHexLength)
{
    uint8_t uDecLength;
    uint16_t uValue;
    uDecLength=1;
    memset(pszDec, '0', sizeof(pszDec));
    if(pszHex!=HEX_TO_DEC_ERROR_NULL_POINTER)
    {
        for(int i=0;i<uHexLength;i++)
        {
            uValue=CharToInt(pszHex[i]);
            if (uValue>=16)
            {
                break;
            }
            for(int j=0;j<uDecLength;j++)
            {
                uValue+=((pszDec[j]-'0')<<4);
                pszDec[j]='0'+(uValue%10);
                uValue=uValue/10;
            }
            while(0<uValue)
            {
                pszDec[uDecLength++]='0'+uValue%10;
                uValue=uValue/10;
            }
        }
        reverse(pszDec,uDecLength);
        if(uValue==0)
        {
            for(int i=0; i<uDecLength; i++)
            {
                printf("%c", pszDec[i]);
            }
        }
        else
        {
            printf("ERROR");
        }

    }
    printf("\n");
    return uDecLength;
}

uint8_t HexToDec_V2(char* pszDec, uint16_t uMaxDecLength, const char* pszHex, uint16_t uHexLength)
{
	int8_t BitCount;
    uint16_t uBit;
    uint8_t resultMax;
	uint8_t uDecLength;
	uint16_t uValue;
    BitCount=0;
    resultMax=0;
	uDecLength=1;
	memset(pszDec, 0, sizeof(pszDec));
	if(pszHex!=NULL)
	{
		for(int i=0;i<uHexLength;i++)
		{
			uValue=CharToInt(pszHex[i]);
            if (uValue>=16)
            {
                break;
            }
			for(int bit=3;bit>=0;bit--)
			{
				for(int j=0;j<=uDecLength-1;j++)
				{
					if((pszDec[j])>=5)
					{
						pszDec[j]=((pszDec[j])+3);
					}
				}
				if(((pszDec[uDecLength-1]) & (1<<3))!=0)
				{
					uDecLength++;
				}
				for(int k=uDecLength-1;k>=0;k--)
				{

					if(k>0)
					{
					    pszDec[k-1]=((pszDec[k-1]) & ((1<<4)-1));
						uBit=(pszDec[k-1]) & (1<<3);
					}
					else
					{
						uBit=(uValue) & (1<<bit);
					}
					if(uBit!=0)
					{
						pszDec[k]=((((pszDec[k])<<1)) | 1);
					}
					else
					{
						pszDec[k]=((((pszDec[k])<<1)) & (~1));
					}
					pszDec[k]=((pszDec[k]) & ((1<<4)-1));
				}
			}
		}
		reverse(pszDec,uDecLength);
        for(int i=0; i<uDecLength; i++)
        {
            printf("%c", (pszDec[i]));
        }
    }
    printf("\n");
    return uDecLength;
}

uint16_t HexToDec_V3(char* pszDec, uint16_t uMaxDecLength, const char* pszHex, uint16_t uHexLength)
{
	uint8_t uDecLength;
	uint8_t uValue;
	uint8_t uHex;
	uHex=0;
	uValue=0;
	uDecLength=1;
	memset(pszDec, 0, sizeof(pszDec));
	if(NULL!=pszHex)
	{
//	    --------------------1
		for(int i=0; i<uHexLength; i++)
        {
            uHex=CharToInt(pszHex[i]);
            if(16<=uHex)
            {
                break;
            }
            for(uint8_t bBitMask=(1<<3); bBitMask!=0; bBitMask>>=1)
            {
                uValue=0;
                if(uHex & bBitMask)
                {
                    uValue=1;
                }
                for(int i=0; i<uDecLength; i++)
                {
                    uValue|=(pszDec[i]<<1);
                    if(10<=uValue)
                    {
                        uValue+=((-10)+16);
                    }
                    pszDec[i]=(uValue & 0x0F);
                    uValue>>=4;
                }
                if(0!=uValue)
                {
                    pszDec[uDecLength]=((uint8_t)uValue);
                    uDecLength++;
                }
            }
//            ----------------2
//            for(int bit=3; bit>=0; bit--)
//            {
//                uValue=(1 & (uHex>>bit));
//                for(int i=0; i<uDecLength; i++)
//                {
//                    uValue|=(pszDec[i]<<1);
//                    if(10<=uValue)
//                    {
//                        uValue+=((-10)+16);
//                    }
//                    pszDec[i]=(uValue & 0x0F);
//                    uValue>>=4;
//                }
//                if(0!=uValue)
//                {
//                    pszDec[uDecLength]=((uint8_t)uValue);
//                    uDecLength++;
//                }
//            }
//            }
//-----------------------------------------------------------------
//            for(int bit=3; bit>=0; bit--)
//            {
//                uValue=uHex & (1<<bit);
//                for(int i=0; i<uDecLength; i++)
//                {
//                    pszDec[i]<<=1;
//                    if(0!=uValue)
//                    {
//                        pszDec[i] |=1;
//                    }
//                    uValue=pszDec[i];
//                    if(10<=uValue)
//                    {
//                        uValue=(uValue-10)+16;
//                        pszDec[i]=uValue & 0x0F;
//                    }
//                    uValue>>=4;
//                }
//                if(0!=uValue)
//                {
//                    pszDec[uDecLength] |=1;
//                    uDecLength++;
//                }
//            }
        }
		reverse(pszDec,uDecLength);
        for(int i=0; i<uDecLength; i++)
        {
            printf("%c", (pszDec[i]));
        }
    }
    printf("\n");
    return uDecLength;
}


const char* m_pszHexList[]=
{
   "F",                        // 15
   "FF",                       // 255
   "FFF",                      // 4095
   "FFFF",                     // 65535
   "FFFFF",                    // 1048575
   "FFFFFF",                   // 16777215*
   "123",                      // 291
   "345",                      // 837
   "234",                      // 564
   "234234",                   // 2310708*
   "2342342",                  // 36971330
   "FFFFFFFFF",                // 68719476735
   "FFFFFFFFFF",               // -1
   "100000001",                // 4294967297
   "101010101",                // 4311810305
   "E8D4A696A1",               // 1000000100001
   "FFFFFFFFFFF",
};
#define HEX_LIST_MAX_COUNT          (sizeof(m_pszHexList)/sizeof(const char*))

int main(void)
{
   const char* pszHexData;
   uint16_t uDecLength;
   uint16_t uHexLen;
   uint16_t i;
   char szDec[256];
   for(i=0; i<HEX_LIST_MAX_COUNT; i++)
   {
       pszHexData=m_pszHexList[i];
       uHexLen=strlen(pszHexData);
       memset(szDec, 0, sizeof(szDec));
       if(uHexLen==0)
       {
           pszHexData=HEX_TO_DEC_ERROR_NULL_POINTER;
       }
       uDecLength=HexToDec_V3(szDec, sizeof(szDec), pszHexData, uHexLen);
       printf("%03u: Hex[%03u]: %s, Dec[%03u]: %.*s\r\n", i, uHexLen, pszHexData, uDecLength, uDecLength, szDec);
   }
   return 0;
}
