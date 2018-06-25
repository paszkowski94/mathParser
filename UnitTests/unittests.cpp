#include "stdafx.h"
#include "CppUnitTest.h"
#include "../MathParser/Parser.h"
#include <cmath>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{		
	TEST_CLASS(UnitTests)
	{
	public:
		TEST_METHOD(TestMethod1)
		{
			std::string expression_{ "root((2+2)^2 , (3+3)^4)" };
			const auto tokenStream_ = TokenStream(expression_);
			Parser parser_(tokenStream_);
			auto expr_ = parser_.expr(false);
			expr_->interpret();

			const auto result_ = pow(pow(3.0 + 3.0, 4.0), 1.0 / pow(2.0 + 2.0, 2.0));
			Assert::AreEqual(expr_->interpret(), result_);
		}

		TEST_METHOD(TestMethod2)
		{
			std::string expression_{ "2+2*2" };
			const auto tokenStream_ = TokenStream(expression_);
			Parser parser_(tokenStream_);
			auto expr_ = parser_.expr(false);
			expr_->interpret();

			const auto result_ = 6.0;
			Assert::AreEqual(expr_->interpret(), result_);
		}

		TEST_METHOD(TestMethod3)
		{
			std::string expression_{ "root(2+2, sqrt(5)+sin(2)^10 / 20 + 2*2)" };
			const auto tokenStream_ = TokenStream(expression_);
			Parser parser_(tokenStream_);
			auto expr_ = parser_.expr(false);
			expr_->interpret();
			const auto result_ = pow(sqrt(5.0) + pow(sin(2.0), 10.0) / 20.0 + 2.0*2.0, 1.0 / 4.0);

			Assert::AreEqual(expr_->interpret(), result_);
		}
	};
}