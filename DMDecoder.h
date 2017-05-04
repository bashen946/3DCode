#include "ZXingDeCoder.h"
#include "zxing/datamatrix/DataMatrixReader.h"
using namespace zxing::datamatrix;

/**
* @class DMDecoder
* @brief DM������
*/
class DMDecoder : public ZXingDecoder<DataMatrixReader>
{
public:
	DMDecoder(DecodeHints hint = DecodeHints::DATA_MATRIX_HINT) 
		: ZXingDecoder(hint)
	{

	}
	~DMDecoder() { }
};