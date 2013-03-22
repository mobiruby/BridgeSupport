#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <map>
#include <iostream>
#include <string>
#include <vector>

#ifdef SWIG
namespace clang {
class EnumDecl;
class FieldDecl;
class FunctionDecl;
class FunctionType;
class FunctionTypeLoc;
class ObjCCategoryDecl;
class ObjCInterfaceDecl;
class ObjCMethodDecl;
class ObjCProtocolDecl;
class RecordDecl;
class TypedefDecl;
class VarDecl;
};
#else /* SWIG */
#include "clang/AST/ASTContext.h"
#include "clang/AST/Decl.h"
#include "clang/AST/DeclGroup.h"
#include "clang/AST/DeclObjC.h"
#include "clang/AST/Expr.h"
#include "clang/AST/ExprObjC.h"
#include "clang/AST/Stmt.h"
#include "clang/AST/Type.h"
#include "clang/AST/TypeLoc.h"
#include "clang/Basic/Diagnostic.h"
#include "clang/Basic/FileManager.h"
#include "clang/Basic/IdentifierTable.h"
#include "clang/Basic/SourceLocation.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Basic/TargetInfo.h"
#include "clang/Basic/TargetOptions.h"
#include "clang/Frontend/DiagnosticOptions.h"
#include "clang/Frontend/FrontendOptions.h"
#include "clang/Frontend/HeaderSearchOptions.h"
#include "clang/Frontend/PreprocessorOptions.h"
#include "clang/Frontend/TextDiagnosticPrinter.h"
#include "clang/Frontend/Utils.h"
#include "clang/Lex/HeaderSearch.h"
#include "clang/Lex/LexDiagnostic.h"
#include "clang/Lex/MacroInfo.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/Lex/Token.h"
#include "clang/Parse/Action.h"
#include "clang/Parse/Ownership.h"
#include "clang/Parse/Parser.h"
#include "clang/Sema/ExternalSemaSource.h"
#include "clang/Sema/ParseAST.h"
#include "clang/Sema/Sema.h"
#include "clang/Sema/SemaConsumer.h"
#include "llvm/ADT/OwningPtr.h"
#include "llvm/Config/config.h"
#include "llvm/Support/MemoryBuffer.h"
#include "llvm/Support/raw_ostream.h"

// Quiet compiler
#undef PACKAGE_NAME
#undef PACKAGE_TARNAME
#undef PACKAGE_VERSION
#undef PACKAGE_STRING
#undef PACKAGE_BUGREPORT
#undef NORETURN
#undef TYPE

#include "ruby.h"

class MyDiagnosticOptions: public clang::DiagnosticOptions {
public:
    MyDiagnosticOptions()
    {
	// This avoids an assert when builtins aren't recognized
	// (when they should)
	ShowCarets = 0;
    }
};

class MyObjCLangOptions: public clang::LangOptions {
public:
    MyObjCLangOptions()
    {
	BCPLComment = Blocks = Bool = C99 = GNUMode = HexFloats = 1;
	ObjC1 = ObjC2 = ObjCNonFragileABI = 1;
    }
};

class MyPass2Consumer;

class MyTargetOptions: public clang::TargetOptions {
public:
    MyTargetOptions(const std::string& triple = LLVM_HOSTTRIPLE)
    {
	Triple = triple;
	CPU = "core2";
    }
};
#endif /* SWIG */
