//
//  umlvisitor.hpp
//  states
//
//  Created by Daniel Pav on 4/29/21.
//  Copyright © 2021 Daniel Pav. All rights reserved.
//

#pragma once

#include <ostream>

namespace states
{
/* a visitor to dump the uml of the process it visits */
class UmlVisitor
{
public:
    UmlVisitor(std::ostream& os) : os_(os) {}

public:
    void preLink() {}
    void inLink1() { os_ << " -> "; }
    void inLink2() { os_ << " : "; }
    void postLink() { os_ << std::endl; }
    void preProcess() { os_ << "@startuml" << std::endl; }
    void postProcess() { os_ << "@enduml" << std::endl; }
    void visitEvent(const char* name) { os_ << name; }
    void visitState(const char* name) { os_ << name; }
    void visitBegin(const char* name);
    void visitEnd(const char* name);

private:
    std::ostream& os_;
};

} // namespace states
