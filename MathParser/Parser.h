#pragma once
#include "stdafx.h"


class __declspec(dllexport) Token
{
public:
	enum class Type : char
	{
		name,
		number,
		end = ';',
		plus = '+',
		minus = '-',
		mul = '*',
		div = '/',
		assign = '=',
		lp = '(',
		rp = ')',
		pow = '^',
		root = 'r',
		sqrt = 's',
		sin
	};

	Token(const Type pType, std::string pTextValue);
	Token(const Type pType, const double pNumberValue);
	explicit operator std::string() const;
	friend std::ostream& operator<<(std::ostream& pOs, const Token& pObj);

	Type getType() const { return type_; }
	double getNumberValue() const { return numberValue_; }
	std::string getTextValue() const { return textValue_; }

private:
	Type type_;
	std::string textValue_;
	double numberValue_;
};

class __declspec(dllexport) TokenStream
{
public:
	explicit TokenStream(std::string& pExpression);
	Token get() { return tokenStream_[index_++]; }
	Token current() const { return tokenStream_[index_]; }
	void substream(std::istringstream& pIss);
	friend std::ostream& operator<<(std::ostream& pOs, const TokenStream& pObj);

private:
	std::vector<Token> tokenStream_{};
	int index_ = 0;
};

class __declspec(dllexport) Expression
{
public:
	Expression() = default;
	virtual ~Expression() = default;
	virtual double interpret() = 0;
	virtual void print(std::ostringstream& pOss, std::string pSpace = "") = 0;
};

class NumberExpression : public Expression
{
public:
	explicit NumberExpression(const double i);
	virtual double interpret() override;
	virtual void print(std::ostringstream& pOss, std::string pSpace = "") override;

private:
	double number_;
};

class AddExpression : public Expression
{
public:
	AddExpression(const std::shared_ptr<Expression> pLeft, std::shared_ptr<Expression> pRight);
	virtual double interpret() override;
	void setLeftExpression(std::shared_ptr<Expression>& pLeftExpression);
	void setRightExpression(std::shared_ptr<Expression>& pRightExpression);

	void print(std::ostringstream& pOss, std::string pSpace = "") override;
private:
	std::shared_ptr<Expression> leftExpression_;
	std::shared_ptr<Expression> rightExpression_;
};

class DifExpression : public Expression
{
public:
	DifExpression(std::shared_ptr<Expression> pLeft, std::shared_ptr<Expression> pRight);
	virtual double interpret() override;
	void setLeftExpression(std::shared_ptr<Expression>& pLeftExpression);
	void setRightExpression(std::shared_ptr<Expression>& pRightExpression);
	void print(std::ostringstream& pOss, std::string pSpace) override;
private:
	std::shared_ptr<Expression> leftExpression_;
	std::shared_ptr<Expression> rightExpression_;
};

class MulExpression : public Expression
{
public:
	MulExpression(std::shared_ptr<Expression> pLeft, std::shared_ptr<Expression> pRight);
	virtual double interpret() override;
	void setLeftExpression(std::shared_ptr<Expression>& pLeftExpression);
	void setRightExpression(std::shared_ptr<Expression>& pRightExpression);
	void print(std::ostringstream& pOss, std::string pSpace) override;
private:
	std::shared_ptr<Expression> leftExpression_;
	std::shared_ptr<Expression> rightExpression_;
};

class DivExpression : public Expression
{
public:
	DivExpression(std::shared_ptr<Expression> pLeft, std::shared_ptr<Expression> pRight);
	virtual double interpret() override;
	void setLeftExpression(std::shared_ptr<Expression>& pLeftExpression);
	void setRightExpression(std::shared_ptr<Expression>& pRightExpression);
	virtual void print(std::ostringstream& pOss, std::string pSpace) override;
private:
	std::shared_ptr<Expression> leftExpression_;
	std::shared_ptr<Expression> rightExpression_;
};

class PowExpression : public Expression
{
public:
	PowExpression(std::shared_ptr<Expression> pLeft, std::shared_ptr<Expression> pRight);
	virtual double interpret() override;
	void setLeftExpression(std::shared_ptr<Expression>& pLeftExpression);
	void setRightExpression(std::shared_ptr<Expression>& pRightExpression);
	void print(std::ostringstream& pOss, std::string pSpace) override;
private:
	std::shared_ptr<Expression> leftExpression_;
	std::shared_ptr<Expression> rightExpression_;
};

class SqrtExpression : public Expression
{
public:
	explicit SqrtExpression(std::shared_ptr<Expression> pLeft);
	virtual double interpret() override;
	void setLeftExpression(std::shared_ptr<Expression>& pLeftExpression);
	virtual void print(std::ostringstream& pOss, std::string pSpace) override;
private:
	std::shared_ptr<Expression> leftExpression_;
};

class RootExpression : public Expression
{
public:
	RootExpression(std::shared_ptr<Expression> pLeft, std::shared_ptr<Expression> pRight);
	virtual double interpret() override;
	void setLeftExpression(std::shared_ptr<Expression>& pLeftExpression);
	void setRightExpression(std::shared_ptr<Expression>& pRightExpression);
	void print(std::ostringstream& pOss, std::string pSpace) override;
private:
	std::shared_ptr<Expression> leftExpression_;
	std::shared_ptr<Expression> rightExpression_;
};

class SinExpression : public Expression
{
public:
	explicit SinExpression(std::shared_ptr<Expression> pLeft);
	virtual double interpret() override;
	void setLeftExpression(std::shared_ptr<Expression>& pLeftExpression);
	void print(std::ostringstream& pOss, std::string pSpace) override;
private:
	std::shared_ptr<Expression> leftExpression_;
};

class __declspec(dllexport) Parser
{
public:
	explicit Parser(TokenStream pTokenStream);

	std::shared_ptr<Expression> expr(const bool pGet);
	std::shared_ptr<Expression> term(const bool pGet);
	std::shared_ptr<Expression> prim(const bool pGet);
	std::shared_ptr<Expression> powering(const bool pGet);
	std::shared_ptr<Expression> oneMinus(const bool pGet);

private:
	TokenStream tokenStream_;
};
