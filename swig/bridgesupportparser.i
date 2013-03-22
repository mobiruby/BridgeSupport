%{
#include "bridgesupportparser.h"

extern VALUE klass_AnEnum;
extern VALUE klass_AFunctionDecl;
extern VALUE klass_AFunctionType;
extern VALUE klass_AMacroFunctionAlias;
extern VALUE klass_AMacroNumber;
extern VALUE klass_AMacroNumberFuncCall;
extern VALUE klass_AMacroString;
extern VALUE klass_AnObjCInterface;
extern VALUE klass_AnObjCCategory;
extern VALUE klass_AnObjCProtocol;
extern VALUE klass_AnObjCMethod;
extern VALUE klass_AStruct;
extern VALUE klass_ATypedef;
extern VALUE klass_AVar;
%}

%module bridgesupportparser

%include std_string.i
%include bridgesupportparser.h

%init %{
    klass_AnEnum = SwigClassAnEnum.klass;
    klass_AFunctionDecl = SwigClassAFunctionDecl.klass;
    klass_AFunctionType = SwigClassAFunctionType.klass;
    klass_AMacroFunctionAlias = SwigClassAMacroFunctionAlias.klass;
    klass_AMacroNumber = SwigClassAMacroNumber.klass;
    klass_AMacroNumberFuncCall = SwigClassAMacroNumberFuncCall.klass;
    klass_AMacroString = SwigClassAMacroString.klass;
    klass_AnObjCInterface = SwigClassAnObjCInterface.klass;
    klass_AnObjCCategory = SwigClassAnObjCCategory.klass;
    klass_AnObjCProtocol = SwigClassAnObjCProtocol.klass;
    klass_AnObjCMethod = SwigClassAnObjCMethod.klass;
    klass_AStruct = SwigClassAStruct.klass;
    klass_ATypedef = SwigClassATypedef.klass;
    klass_AVar = SwigClassAVar.klass;
%}
