//
//  umlvisitor.cpp
//  states
//
//  Created by Daniel Pav on 4/29/21.
//  Copyright © 2021 Daniel Pav. All rights reserved.
//

#include "umlvisitor.hpp"

namespace states
{
void UmlVisitor::visitBegin(const char* name)
{
    preLink();
    visitState("[*]");
    inLink1();
    visitState(name);
    postLink();
}
void UmlVisitor::visitEnd(const char* name)
{
    preLink();
    visitState(name);
    inLink1();
    visitState("terminate");
    postLink();
}

} // namespace states