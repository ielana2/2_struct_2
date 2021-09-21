#include "pch.h"
#include "CppUnitTest.h"
#include "../main/Huffman.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace lab2test
{
	TEST_CLASS(lab2test)
	{
	public:
		bool* code;
		char* decode;

		TEST_METHOD(test_)
		{
			char str[] = { 'N','e','w',' ','s','t','r','i','n','g','\n' };
			Huffman ForCode(str);
			int codeSize;
			int decodeSize;
			code = ForCode.Code(codeSize);
			decode = ForCode.Decode(decodeSize);
			for (size_t i = 0; i < decodeSize; i++)
			{
				Assert::AreEqual(str[i], decode[i]);
			}
		}
		TEST_METHOD(test_empty_line)
		{
			char* str = NULL;
			int codeSize;
			int decodeSize;
			try
			{
				Huffman ForCode(str);
				code = ForCode.Code(codeSize);
				decode = ForCode.Decode(decodeSize);
			}
			catch (std::exception e)
			{
				Assert::AreEqual("null argument", e.what());
			}
		}
	};
}