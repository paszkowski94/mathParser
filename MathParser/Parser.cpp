#include "stdafx.h"
#include "Parser.h"

Token::Token(const Type pType, std::string pTextValue) : type_(pType),
textValue_(std::move(pTextValue)),
numberValue_(0)
{
}

Token::Token(const Type pType, const double pNumberValue) : type_(pType),
textValue_(""),
numberValue_(pNumberValue)
{
}

Token::operator std::basic_string<char>() const
{
	switch (type_)
	{
	case Type::name:	return "name";
	case Type::end:		return "end";
	case Type::plus:	return "+";
	case Type::minus:	return "-";
	case Type::mul:		return "*";
	case Type::div:		return "/";
	case Type::assign:	return "-";
	case Type::lp:		return "(";
	case Type::rp:		return ")";
	case Type::pow:		return "^";
	case Type::sqrt:	return "sqrt";
	case Type::sin:		return "sin";

	default: break;
	}

	return "";
}

TokenStream::TokenStream(std::string& pExpression)
{
	std::istringstream iss(pExpression);
	char character{};

	while (!iss.eof())
	{
		iss >> character;
		switch (character)
		{
		case ' ':
			break;
		case '+':
			tokenStream_.emplace_back(Token::Type::plus, "+");
			break;
		case '-':
			tokenStream_.emplace_back(Token::Type::plus, "+");
			tokenStream_.emplace_back(Token::Type::minus, "-");
			break;
		case '*':
			tokenStream_.emplace_back(Token::Type::mul, "*");
			break;
		case '/':
			tokenStream_.emplace_back(Token::Type::div, "/");
			break;
		case '(':
			tokenStream_.emplace_back(Token::Type::lp, "(");
			break;
		case ')':
			tokenStream_.emplace_back(Token::Type::rp, ")");

			break;
		case '^':
			tokenStream_.emplace_back(Token::Type::pow, "^");
			break;
		case 's':
			char buffer[3];
			iss >> buffer[0];
			iss >> buffer[1];

			if (buffer[0] == 'q' && buffer[1] == 'r')
			{
				iss >> character;
				tokenStream_.emplace_back(Token::Type::sqrt, "sqrt");
			}
			else if (buffer[0] == 'i' && buffer[1] == 'n')
			{
				tokenStream_.emplace_back(Token::Type::sin, "sin");
			}
			break;

		case 'r':
			iss >> character;
			iss >> character;
			iss >> character;
			iss >> character;

			tokenStream_.emplace_back(Token::Type::root, "root");
			tokenStream_.emplace_back(Token::Type::lp, "(");
			substream(iss);
			tokenStream_.emplace_back(Token::Type::lp, "(");
			substream(iss);


			iss >> character;

			break;
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
			iss.putback(character);
			double val;
			iss >> val;
			tokenStream_.emplace_back(Token::Type::number, val);
			break;
		default:
			break;
		}
	}
	tokenStream_.emplace_back(Token::Type::end, ';');
}

void TokenStream::substream(std::istringstream& iss)
{
	char character{};
	while (!iss.eof())
	{
		iss >> character;
		switch (character)
		{
		case ' ':
			break;
		case '+':
			tokenStream_.emplace_back(Token::Type::plus, "+");
			break;
		case '-':
			tokenStream_.emplace_back(Token::Type::plus, "+");
			tokenStream_.emplace_back(Token::Type::minus, "-");
			break;
		case '*':
			tokenStream_.emplace_back(Token::Type::mul, "*");
			break;
		case '/':
			tokenStream_.emplace_back(Token::Type::div, "/");
			break;
		case '(':
			tokenStream_.emplace_back(Token::Type::lp, "(");
			substream(iss);
			break;

		case ',':
		case ')':
			tokenStream_.emplace_back(Token::Type::rp, ")");
			return;

		case '^':
			tokenStream_.emplace_back(Token::Type::pow, "^");
			break;
		case 's':
			char buffer[3];
			iss >> buffer[0];
			iss >> buffer[1];

			if (buffer[0] == 'q' && buffer[1] == 'r')
			{
				iss >> character;
				tokenStream_.emplace_back(Token::Type::sqrt, "sqrt");
			}
			else if (buffer[0] == 'i' && buffer[1] == 'n')
			{
				tokenStream_.emplace_back(Token::Type::sin, "sin");
			}
			break;

		case 'r':
			iss >> character;
			iss >> character;
			iss >> character;
			iss >> character;

			tokenStream_.emplace_back(Token::Type::root, "root");
			tokenStream_.emplace_back(Token::Type::lp, "(");
			substream(iss);
			tokenStream_.emplace_back(Token::Type::lp, "(");
			//tokenStream_.emplace_back(Token::Type::lp, "(");
			substream(iss);
			//tokenStream_.emplace_back(Token::Type::lp, ")");


			iss >> character;

			break;
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
			iss.putback(character);
			double val;
			iss >> val;
			tokenStream_.emplace_back(Token::Type::number, val);
			break;
		default:
			break;
		}
	}
	tokenStream_.emplace_back(Token::Type::end, ';');
}

NumberExpression::NumberExpression(const double i) : number_(i)
{
}

double NumberExpression::interpret()
{
	return number_;
}

void NumberExpression::print(std::ostringstream& oss, std::string space)
{
	oss << space << number_ << std::endl;
}

AddExpression::
AddExpression(std::shared_ptr<Expression> pLeft, std::shared_ptr<Expression> pRight) :
	leftExpression_(std::move(pLeft)), rightExpression_(std::move(pRight))
{
}

double AddExpression::interpret()
{
	if (leftExpression_ && rightExpression_)
		return leftExpression_->interpret() + rightExpression_->interpret();
	else if (leftExpression_)
		return leftExpression_->interpret();
	else rightExpression_->interpret();

}

void AddExpression::setLeftExpression(std::shared_ptr<Expression>& pLeftExpression)
{
	leftExpression_ = pLeftExpression;
}

void AddExpression::setRightExpression(std::shared_ptr<Expression>& pRightExpression)
{
	rightExpression_ = pRightExpression;
}

void AddExpression::print(std::ostringstream& oss, std::string space)
{
	oss << space << "+" << std::endl;
	leftExpression_->print(oss, space + "   ");
	rightExpression_->print(oss, space + "   ");
}

DifExpression::
DifExpression(std::shared_ptr<Expression> pLeft, std::shared_ptr<Expression> pRight) : leftExpression_(std::move(pLeft)),
rightExpression_(std::move(pRight))
{
}

double DifExpression::interpret()
{
	return leftExpression_->interpret() - rightExpression_->interpret();
}

void DifExpression::setLeftExpression(std::shared_ptr<Expression>& pLeftExpression)
{
	leftExpression_ = pLeftExpression;
}

void DifExpression::setRightExpression(std::shared_ptr<Expression>& pRightExpression)
{
	rightExpression_ = pRightExpression;
}

void DifExpression::print(std::ostringstream& oss, std::string space)
{
	oss << space << "-" << std::endl;
	leftExpression_->print(oss, space + "   ");
	rightExpression_->print(oss, space + "   ");
}

MulExpression::
MulExpression(std::shared_ptr<Expression> pLeft, std::shared_ptr<Expression> pRight) : leftExpression_(std::move(pLeft)),
rightExpression_(std::move(pRight))
{
}

double MulExpression::interpret()
{
	return leftExpression_->interpret() * rightExpression_->interpret();
}

void MulExpression::setLeftExpression(std::shared_ptr<Expression>& pLeftExpression)
{
	leftExpression_ = pLeftExpression;
}

void MulExpression::setRightExpression(std::shared_ptr<Expression>& pRightExpression)
{
	rightExpression_ = pRightExpression;
}

void MulExpression::print(std::ostringstream& oss, std::string space)
{
	oss << space << "*" << std::endl;
	leftExpression_->print(oss, space + "   ");
	rightExpression_->print(oss, space + "   ");
}

DivExpression::
DivExpression(std::shared_ptr<Expression> pLeft, std::shared_ptr<Expression> pRight) : leftExpression_(std::move(pLeft)),
rightExpression_(std::move(pRight))
{
}

double DivExpression::interpret()
{
	return leftExpression_->interpret() / rightExpression_->interpret();
}

void DivExpression::setLeftExpression(std::shared_ptr<Expression>& left_expression)
{
	leftExpression_ = left_expression;
}

void DivExpression::setRightExpression(std::shared_ptr<Expression>& right_expression)
{
	rightExpression_ = right_expression;
}

void DivExpression::print(std::ostringstream& oss, std::string space)
{
	oss << space << "/" << std::endl;
	leftExpression_->print(oss, space + "   ");
	rightExpression_->print(oss, space + "   ");
}

PowExpression::
PowExpression(std::shared_ptr<Expression> pLeft, std::shared_ptr<Expression> pRight) : leftExpression_(std::move(pLeft)),
rightExpression_(std::move(pRight))
{
}

double PowExpression::interpret()
{
	return pow(leftExpression_->interpret(), rightExpression_->interpret());
}

void PowExpression::setLeftExpression(std::shared_ptr<Expression>& left_expression)
{
	leftExpression_ = left_expression;
}

void PowExpression::setRightExpression(std::shared_ptr<Expression>& right_expression)
{
	rightExpression_ = right_expression;
}

void PowExpression::print(std::ostringstream& oss, std::string space)
{
	oss << space << "pow" << std::endl;
	leftExpression_->print(oss, space + "   ");
	rightExpression_->print(oss, space + "   ");
}


SqrtExpression::
SqrtExpression(std::shared_ptr<Expression> pLeft) : leftExpression_(std::move(pLeft))
{
}

double SqrtExpression::interpret()
{
	return pow(leftExpression_->interpret(), 0.5);
}

void SqrtExpression::setLeftExpression(std::shared_ptr<Expression>& left_expression)
{
	leftExpression_ = left_expression;
}

void SqrtExpression::print(std::ostringstream& oss, std::string space)
{
	oss << space << "sqrt" << std::endl;
	leftExpression_->print(oss, space + "   ");
}


Parser::Parser(TokenStream pTokenStream) : tokenStream_(pTokenStream)
{
}


RootExpression::
RootExpression(std::shared_ptr<Expression> pLeft, std::shared_ptr<Expression> pRight) : leftExpression_(pLeft),
rightExpression_(pRight)
{
}

double RootExpression::interpret()
{
	return pow(rightExpression_->interpret(), 1 / leftExpression_->interpret());
}

void RootExpression::setLeftExpression(std::shared_ptr<Expression>& left_expression)
{
	leftExpression_ = left_expression;
}

void RootExpression::setRightExpression(std::shared_ptr<Expression>& right_expression)
{
	rightExpression_ = right_expression;
}

void RootExpression::print(std::ostringstream& oss, std::string space)
{
	oss << space << "root" << std::endl;
	leftExpression_->print(oss, space + "   ");
	rightExpression_->print(oss, space + "   ");
}

SinExpression::
SinExpression(std::shared_ptr<Expression> pLeft) : leftExpression_(std::move(pLeft))
{
}

double SinExpression::interpret()
{
	return sin(leftExpression_->interpret());
}

void SinExpression::setLeftExpression(std::shared_ptr<Expression>& left_expression)
{
	leftExpression_ = left_expression;
}

void SinExpression::print(std::ostringstream& oss, std::string space)
{
	oss << space << "sin" << std::endl;
	leftExpression_->print(oss, space + "   ");
}

std::shared_ptr<Expression> Parser::expr(const bool pGet)
{
	auto left_ = term(pGet);

	for (;;)
	{
		switch (tokenStream_.current().getType())
		{
		case Token::Type::plus:
			left_ = std::make_shared<AddExpression>(AddExpression(left_, term(true)));
			break;
		case Token::Type::minus:
			left_ = std::make_shared<DifExpression>(DifExpression(left_, term(true)));
			break;
		default:
			return left_;
		}
	}
}

std::shared_ptr<Expression> Parser::term(const bool pGet)
{
	auto left_ = oneMinus(pGet);

	for (;;)
	{
		switch (tokenStream_.current().getType())
		{
		case Token::Type::mul:
			left_ = std::make_shared<MulExpression>(MulExpression(left_, oneMinus(true)));
			break;
		case Token::Type::div:
			left_ = std::make_shared<DivExpression>(DivExpression(left_, oneMinus(true)));
			break;
		default:
			return left_;
		}
	}
}

std::shared_ptr<Expression> Parser::oneMinus(const bool pGet)
{
	auto left_ = powering(pGet);

	for (;;)
	{
		switch (tokenStream_.current().getType())
		{
		case Token::Type::minus:
			left_ = std::make_shared<MulExpression>(MulExpression(std::make_shared<NumberExpression>(NumberExpression(-1)), powering(true)));
			break;
		default:
			return left_;
		}
	}
}

std::shared_ptr<Expression> Parser::powering(const bool pGet)
{
	auto left_ = prim(pGet);

	for (;;)
	{
		switch (tokenStream_.current().getType())
		{
		case Token::Type::pow:
			left_ = std::make_shared<PowExpression>(PowExpression(left_, prim(true)));
			break;
		case Token::Type::sqrt:
			left_ = std::make_shared<SqrtExpression>(SqrtExpression(prim(true)));
			break;
		case Token::Type::root:
		{
			auto param1 = prim(true);
			auto param2 = prim(false);
			left_ = std::make_shared<RootExpression>(param1, param2);
			break;
		}
		case Token::Type::sin:
			left_ = std::make_shared<SinExpression>(SinExpression(prim(true)));
			break;
		default:
			return left_;
		}
	}
}

std::shared_ptr<Expression> Parser::prim(const bool pGet)
{
	if (pGet) tokenStream_.get();
	auto x = tokenStream_.current().getType();
	switch (x)
	{
	case Token::Type::number:
	{
		auto v_ = std::make_shared<NumberExpression>(NumberExpression(tokenStream_.current().getNumberValue()));
		tokenStream_.get();
		return v_;
	}

	case Token::Type::lp:
	{
		auto e = expr(true);
		auto x = tokenStream_.get();
		return e;
	}
	default:
		break;
	}
	return {};
}

std::ostream& operator<<(std::ostream& pOs, const Token& pObj)
{
	if (!pObj.textValue_.empty())
		return pOs << "`" << pObj.textValue_ << "`";
	return pOs << "`" << pObj.numberValue_ << "`";
}

std::ostream& operator<<(std::ostream& pOs, const TokenStream& pObj)
{
	for (const auto& x_ : pObj.tokenStream_)
	{
		pOs << x_ << "    ";
	}
	return pOs;
}
