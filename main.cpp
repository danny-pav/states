//
//  main.cpp
//  states
//
//  Created by Daniel Pav on 4/27/20.
//  Copyright © 2020 Daniel Pav. All rights reserved.
//

#include "event.hpp"
#include "link.hpp"
#include "machine.hpp"
#include "noop.hpp"
#include "process.hpp"
#include "state.hpp"
#include "typenum.hpp"
#include "umlvisitor.hpp"

#include <iostream>

static const char sStart[] = "Start";
static const char sDigit1[] = "Digit1";
static const char sDecimal[] = "Decimal";
static const char sDigit2[] = "Digit2";
static const char sEnd[] = "End";

static const char eDigit[] = "Digit";
static const char eDot[] = "Dot";
static const char eDone[] = "Done";

struct Data
{
    const std::string in_;
    int npos_{0};
    std::string out_{};
};

struct Consume
{
    void operator()(Data& d)
    {
        char c = d.in_[d.npos_];
        ++d.npos_;
        d.out_ += c;
    }
};

typedef states::State<sStart> Start;
typedef states::State<sDigit1, Consume> Digit1;
typedef states::State<sDecimal, Consume> Decimal;
typedef states::State<sDigit2, Consume> Digit2;
typedef states::State<sEnd> End;

typedef states::Event<eDigit> Digit;
typedef states::Event<eDot> Dot;
typedef states::Event<eDone> Done;

typedef states::Link<Start, Digit, Digit1> L1;
typedef states::Link<Digit1, Digit, Digit1> L2;
typedef states::Link<Digit1, Dot, Decimal> L3;
typedef states::Link<Digit1, Done, End> L4;
typedef states::Link<Decimal, Digit, Digit2> L5;
typedef states::Link<Decimal, Done, End> L6;
typedef states::Link<Digit2, Digit, Digit2> L7;
typedef states::Link<Digit2, Done, End> L8;

typedef states::Machine<L1, L2, L3, L4, L5, L6, L7, L8> SM;

typedef states::Process<SM, Start, End, Data> Parser;

bool processEvent(Parser& p, Data& d)
{
    if (d.npos_ == d.in_.length())
        return p.next<Done>();
    char c = d.in_[d.npos_];
    if (c == '.')
        return p.next<Dot>();
    if (c >= '0' && c <= '9')
        return p.next<Digit>();
    return false;
}

Parser::TEventNum processEvent(Data& d)
{
    Parser::TEventNum ret;
    if (d.npos_ == d.in_.length())
        ret.set<Done>();
    else
    {
        char c = d.in_[d.npos_];
        if (c == '.')
            ret.set<Dot>();
        else if (c >= '0' && c <= '9')
            ret.set<Digit>();
    }
    return ret;
}

void testPass1(const std::string& s)
{
    std::cout << ">>>>" << std::endl;
    std::cout << "in:" << s << std::endl;
    Data d{s};
    Parser p(d);
    p.start();
    while (!p.done())
    {
        if (!processEvent(p, d))
        {
            std::cout << "ERROR" << std::endl;
            break;
        }
    }
    std::cout << "out:" << d.out_ << std::endl;
    std::cout << "<<<<" << std::endl;
}

void testPass2(const std::string& s)
{
    std::cout << ">>>>" << std::endl;
    std::cout << "in:" << s << std::endl;
    Data d{s};
    Parser p(d);
    p.start();
    while (!p.done())
    {
        Parser::TEventNum ev = processEvent(d);
        if (!p.next(ev))
        {
            std::cout << "ERROR" << std::endl;
            break;
        }
    }
    std::cout << "out:" << d.out_ << std::endl;
    std::cout << "<<<<" << std::endl;
}

int main(int argc, const char* argv[])
{
    testPass1("32");
    testPass1("101.57");
    testPass1("32,000"); // SHOULD BE ERROR
    testPass2("32");
    testPass2("101.57");
    testPass2("32,000"); // SHOULD BE ERROR
    
    // dump the plant uml of the parser
    states::UmlVisitor v(std::cout);
    Parser::visit(v);
    return 0;
}
