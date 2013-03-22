#include "bridgesupportparser.h"

#include "__xattr__.h"

/* These values must match those in bridgesupport.rb */
#define CONTENT			"__9_OxQk__4__c_0_n_T_3_n_t_"
#define CONTENTEND		"__9_OxQk__4__3_0_F_"
#define CONTENTMETHOD		CONTENT "_m_3_T_h_0_d_"
#define CONTENTTYPE		CONTENT "_t_Y_p_3_"

#define MACROPREFIX		"__9_OxQk__4__M_a_c_r_0_"
#define RB_FINALIZER(func)	((void (*)(...))func)

VALUE klass_AnEnum;
VALUE klass_AFunctionDecl;
VALUE klass_AFunctionType;
VALUE klass_AMacroFunctionAlias;
VALUE klass_AMacroNumber;
VALUE klass_AMacroNumberFuncCall;
VALUE klass_AMacroString;
VALUE klass_AnObjCInterface;
VALUE klass_AnObjCCategory;
VALUE klass_AnObjCProtocol;
VALUE klass_AnObjCMethod;
VALUE klass_AStruct;
VALUE klass_ATypedef;
VALUE klass_AVar;

static const char *mmintrin_builtins[] = {
    // __m64 => long long
    "__builtin_ia32_paddb(x,y)=((__m64)0LL)",
    "__builtin_ia32_paddd(x,y)=((__m64)0LL)",
    "__builtin_ia32_paddq(x,y)=((__m64)0LL)",
    "__builtin_ia32_paddw(x,y)=((__m64)0LL)",
    "__builtin_ia32_pand(x,y)=((__m64)0LL)",
    "__builtin_ia32_pandn(x,y)=((__m64)0LL)",
    "__builtin_ia32_pmullw(x,y)=((__m64)0LL)",
    "__builtin_ia32_por(x,y)=((__m64)0LL)",
    "__builtin_ia32_psubb(x,y)=((__m64)0LL)",
    "__builtin_ia32_psubd(x,y)=((__m64)0LL)",
    "__builtin_ia32_psubq(x,y)=((__m64)0LL)",
    "__builtin_ia32_psubw(x,y)=((__m64)0LL)",
    "__builtin_ia32_pxor(x,y)=((__m64)0LL)",
    "__builtin_ia32_vec_init_v2si(x,y)=((__m64)0LL)",
    "__builtin_ia32_vec_init_v4hi(a,b,c,d)=((__m64)0LL)",
    "__builtin_ia32_vec_init_v8qi(a,b,c,d,e,f,g,h)=((__m64)0LL)",

    // __m128 => float
    // __v4sf => float
    "__builtin_ia32_addps(x,y)=(x)",
    "__builtin_ia32_addss(x,y)=(x)",
    "__builtin_ia32_andnps(x,y)=(x)",
    "__builtin_ia32_andps(x,y)=(x)",
    "__builtin_ia32_cmpeqps(x,y)=(x)",
    "__builtin_ia32_cmpeqss(x,y)=(x)",
    "__builtin_ia32_cmpgeps(x,y)=(x)",
    "__builtin_ia32_cmpgtps(x,y)=(x)",
    "__builtin_ia32_cmpleps(x,y)=(x)",
    "__builtin_ia32_cmpless(x,y)=(x)",
    "__builtin_ia32_cmpltps(x,y)=(x)",
    "__builtin_ia32_cmpltss(x,y)=(x)",
    "__builtin_ia32_cmpneqps(x,y)=(x)",
    "__builtin_ia32_cmpneqss(x,y)=(x)",
    "__builtin_ia32_cmpngeps(x,y)=(x)",
    "__builtin_ia32_cmpngtps(x,y)=(x)",
    "__builtin_ia32_cmpnleps(x,y)=(x)",
    "__builtin_ia32_cmpnless(x,y)=(x)",
    "__builtin_ia32_cmpnltps(x,y)=(x)",
    "__builtin_ia32_cmpnltss(x,y)=(x)",
    "__builtin_ia32_cmpordps(x,y)=(x)",
    "__builtin_ia32_cmpordss(x,y)=(x)",
    "__builtin_ia32_cmpunordps(x,y)=(x)",
    "__builtin_ia32_cmpunordss(x,y)=(x)",
    "__builtin_ia32_cvtsi2ss(x,y)=(x)",
    "__builtin_ia32_cvtsi642ss(x,y)=(x)",
    "__builtin_ia32_divps(x,y)=(x)",
    "__builtin_ia32_divss(x,y)=(x)",
    "__builtin_ia32_loadhps(x,y)=(x)",
    "__builtin_ia32_loadlps(x,y)=(x)",
    "__builtin_ia32_movhlps(x,y)=(x)",
    "__builtin_ia32_movlhps(x,y)=(x)",
    "__builtin_ia32_movss(x,y)=(x)",
    "__builtin_ia32_mulps(x,y)=(x)",
    "__builtin_ia32_mulss(x,y)=(x)",
    "__builtin_ia32_orps(x,y)=(x)",
    "__builtin_ia32_shufps(x,y,z)=(x)",
    "__builtin_ia32_subps(x,y)=(x)",
    "__builtin_ia32_subss(x,y)=(x)",
    "__builtin_ia32_unpckhps(x,y)=(x)",
    "__builtin_ia32_unpcklps(x,y)=(x)",
    "__builtin_ia32_vec_ext_v2di(x,y)=(0LL)",
    "__builtin_ia32_xorps(x,y)=(x)",

    // __m128d => double
    "__builtin_ia32_addpd(x,y)=(x)",
    "__builtin_ia32_addsd(x,y)=(x)",
    "__builtin_ia32_andnpd(x,y)=(x)",
    "__builtin_ia32_andpd(x,y)=(x)",
    "__builtin_ia32_cmpeqpd(x,y)=(x)",
    "__builtin_ia32_cmpeqsd(x,y)=(x)",
    "__builtin_ia32_cmpgepd(x,y)=(x)",
    "__builtin_ia32_cmpgtpd(x,y)=(x)",
    "__builtin_ia32_cmplepd(x,y)=(x)",
    "__builtin_ia32_cmplesd(x,y)=(x)",
    "__builtin_ia32_cmpltpd(x,y)=(x)",
    "__builtin_ia32_cmpltsd(x,y)=(x)",
    "__builtin_ia32_cmpneqpd(x,y)=(x)",
    "__builtin_ia32_cmpneqsd(x,y)=(x)",
    "__builtin_ia32_cmpngepd(x,y)=(x)",
    "__builtin_ia32_cmpngtpd(x,y)=(x)",
    "__builtin_ia32_cmpnlepd(x,y)=(x)",
    "__builtin_ia32_cmpnlesd(x,y)=(x)",
    "__builtin_ia32_cmpnltpd(x,y)=(x)",
    "__builtin_ia32_cmpnltsd(x,y)=(x)",
    "__builtin_ia32_cmpordpd(x,y)=(x)",
    "__builtin_ia32_cmpordsd(x,y)=(x)",
    "__builtin_ia32_cmpunordpd(x,y)=(x)",
    "__builtin_ia32_cmpunordsd(x,y)=(x)",
    "__builtin_ia32_divpd(x,y)=(x)",
    "__builtin_ia32_divsd(x,y)=(x)",
    "__builtin_ia32_movsd(x,y)=(x)",
    "__builtin_ia32_mulpd(x,y)=(x)",
    "__builtin_ia32_mulsd(x,y)=(x)",
    "__builtin_ia32_orpd(x,y)=(x)",
    "__builtin_ia32_shufpd(x,y,z)=(x)",
    "__builtin_ia32_subpd(x,y)=(x)",
    "__builtin_ia32_subsd(x,y)=(x)",
    "__builtin_ia32_xorpd(x,y)=(x)",

    // __m128i => double
    "__builtin_ia32_cvtsd2ss(x,y)=(x)",
    "__builtin_ia32_cvtsi2sd(x,y)=(x)",
    "__builtin_ia32_cvtsi642sd(x,y)=(x)",
    "__builtin_ia32_cvtss2sd(x,y)=(x)",
    "__builtin_ia32_loadhpd(x,y)=(x)",
    "__builtin_ia32_loadlpd(x,y)=(x)",
    "__builtin_ia32_loadlv4si(x)=(*(__m128i *)0L)",
    "__builtin_ia32_movqv4si(x)=(x)",
    "__builtin_ia32_paddb128(x,y)=(x)",
    "__builtin_ia32_paddd128(x,y)=(x)",
    "__builtin_ia32_paddq128(x,y)=(x)",
    "__builtin_ia32_paddw128(x,y)=(x)",
    "__builtin_ia32_pand128(x,y)=(x)",
    "__builtin_ia32_pandn128(x,y)=(x)",
    "__builtin_ia32_por128(x,y)=(x)",
    "__builtin_ia32_pshufd(x,y)=(x)",
    "__builtin_ia32_psubb128(x,y)=(x)",
    "__builtin_ia32_psubd128(x,y)=(x)",
    "__builtin_ia32_psubq128(x,y)=(x)",
    "__builtin_ia32_psubw128(x,y)=(x)",
    "__builtin_ia32_punpckhbw128(x,y)=(x)",
    "__builtin_ia32_punpckhdq128(x,y)=(x)",
    "__builtin_ia32_punpckhqdq128(x,y)=(x)",
    "__builtin_ia32_punpckhwd128(x,y)=(x)",
    "__builtin_ia32_punpcklbw128(x,y)=(x)",
    "__builtin_ia32_punpckldq128(x,y)=(x)",
    "__builtin_ia32_punpcklqdq128(x,y)=(x)",
    "__builtin_ia32_punpcklwd128(x,y)=(x)",
    "__builtin_ia32_pxor128(x,y)=(x)",
    "__builtin_ia32_unpckhpd(x,y)=(x)",
    "__builtin_ia32_unpcklpd(x,y)=(x)",

    NULL
};

std::string *BridgeSupportParser::defaultIncludePath = NULL;

using namespace clang;

static struct TypeLocClassName {
    TypeLoc::TypeLocClass key;
    const char *name;
} TypeLocClassName[] = {
    {TypeLoc::BlockPointer, "BlockPointer"},
    {TypeLoc::Builtin, "Builtin"},
    {TypeLoc::Complex, "Complex"},
    {TypeLoc::ConstantArray, "ConstantArray"},
    {TypeLoc::Decltype, "Decltype"},
    {TypeLoc::DependentSizedArray, "DependentSizedArray"},
    {TypeLoc::DependentSizedExtVector, "DependentSizedExtVector"},
    {TypeLoc::Elaborated, "Elaborated"},
    {TypeLoc::Enum, "Enum"},
    {TypeLoc::ExtVector, "ExtVector"},
    {TypeLoc::FunctionNoProto, "FunctionNoProto"},
    {TypeLoc::FunctionProto, "FunctionProto"},
    {TypeLoc::IncompleteArray, "IncompleteArray"},
    {TypeLoc::LValueReference, "LValueReference"},
    {TypeLoc::MemberPointer, "MemberPointer"},
    {TypeLoc::ObjCInterface, "ObjCInterface"},
    {TypeLoc::ObjCObjectPointer, "ObjCObjectPointer"},
    {TypeLoc::Pointer, "Pointer"},
    {TypeLoc::Qualified, "Qualified"},
    {TypeLoc::RValueReference, "RValueReference"},
    {TypeLoc::Record, "Record"},
    {TypeLoc::SubstTemplateTypeParm, "SubstTemplateTypeParm"},
    {TypeLoc::TemplateSpecialization, "TemplateSpecialization"},
    {TypeLoc::TemplateTypeParm, "TemplateTypeParm"},
    {TypeLoc::TypeOf, "TypeOf"},
    {TypeLoc::TypeOfExpr, "TypeOfExpr"},
    {TypeLoc::Typedef, "Typedef"},
    {TypeLoc::UnresolvedUsing, "UnresolvedUsing"},
    {TypeLoc::VariableArray, "VariableArray"},
    {TypeLoc::Vector, "Vector"},
};

#define NTypeLocName (sizeof(TypeLocClassName) / sizeof(struct TypeLocClassName))

static int TLNcompar(const void *a, const void *b);

static VALUE
declAttributes(const Decl *D, VALUE rhash = Qnil)
{
    if(!D->hasAttrs()) return rhash;
    std::map<std::string,std::string> m;
    const Attr *attr = D->getAttrs();
    do {
	switch(attr->getKind()) {
	  case attr::Annotate: {
	    llvm::StringRef str((static_cast<const AnnotateAttr *>(attr))->getAnnotation());
	    if(str.startswith(__XATTR_PREFIX)) {
		std::string s;
		{
		    std::string s0(str.substr(sizeof(__XATTR_PREFIX) - 1).str());
		    if(s0[0] == '(') s0 = s0.substr(1, s0.rfind(')') - 1);
		    size_t white = s0.find_first_of(" \t");
		    if(white == std::string::npos) {
			s = s0;
		    } else {
			size_t nonwhite = 0;
			for(;;) {
			    s.append(s0.substr(nonwhite, white - nonwhite));
			    nonwhite = s0.find_first_not_of(" \t", white + 1);
			    if(nonwhite == std::string::npos) break;
			    white = s0.find_first_of(" \t", nonwhite + 1);
			    if(white == std::string::npos) white = s0.size();
			}
		    }
		}
		size_t semi = s.find(';');
		if(semi == std::string::npos) semi = s.size();
		size_t nonsemi = 0;
		for(;;) {
		    size_t equal = s.find('=', nonsemi);
		    if(equal == std::string::npos || equal > semi) {
			m[s.substr(nonsemi, semi - nonsemi)] = "";
		    } else {
			m[s.substr(nonsemi, equal - nonsemi)] = s.substr(equal + 1, semi - (equal + 1));
		    }
		    nonsemi = s.find_first_not_of(';', semi + 1);
		    if(nonsemi == std::string::npos) break;
		    semi = s.find(';', nonsemi + 1);
		    if(semi == std::string::npos) semi = s.size();
		}
	    }
	    break;
	  }
	  case attr::Format: {
	    const FormatAttr *f = static_cast<const FormatAttr *>(attr);
	    std::string s(f->getType());
	    s.push_back(',');
	    char buf[16];
	    sprintf(buf, "%d", f->getFormatIdx());
	    s.append(buf);
	    s.push_back(',');
	    sprintf(buf, "%d", f->getFirstArg());
	    s.append(buf);
	    m["format"] = s;
	    break;
	  }
	  case attr::NonNull: {
	    const NonNullAttr *n = static_cast<const NonNullAttr *>(attr);
	    if(n->size() > 0) {
		std::string s;
		const char *sep = "";
		char buf[16];
		for(NonNullAttr::iterator i = n->begin(), e = n->end(); i != e; i++) {
		    s.append(sep);
		    sprintf(buf, "%u", *i);
		    s.append(buf);
		    sep = ",";
		}
		m["nonnull"] = s;
	    }
	    break;
	  }
	  case attr::Sentinel: {
	    const SentinelAttr *s = static_cast<const SentinelAttr *>(attr);
	    char buf[16];
	    sprintf(buf, "%d", s->getSentinel());
	    m["sentinel"] = buf;
	    break;
	  }
	}
    } while((attr = attr->getNext()));
    if(m.empty()) return rhash;
    m["_override"] = "true";
    if(rhash == Qnil) rhash = rb_hash_new();
    for(std::map<std::string,std::string>::iterator i = m.begin(), e = m.end(); i != e; i++) {
	rb_hash_aset(rhash, ID2SYM(rb_intern(i->first.c_str())), (i->second.empty() ? Qtrue : rb_str_new2(i->second.c_str())));
    }
    return rhash;
}

static bool
isDeclUnavailable(const Decl *decl)
{
    const Attr *attr = decl->getAttrs();
    while(attr) {
	if(attr->getKind() == attr::Unavailable) return true;
	attr = attr->getNext();
    }
    return false;
}

static const char *
locgetpath(SourceManager *sm, SourceLocation sl)
{
    const FileEntry *fe = NULL;
    const char *path;
    FileID fid = sm->getFileID(sl);
    if(fid == sm->getMainFileID()) return CONTENT;
    fe = sm->getFileEntryForID(fid);
    if(!fe || !(path = fe->getName())) path = "/dev/null";
    return path;
}

static const char *
lookupTypeLocClass(TypeLoc::TypeLocClass tl)
{
    static int inited = 0;
    if(!inited) {
	qsort(TypeLocClassName, NTypeLocName, sizeof(struct TypeLocClassName), TLNcompar);
	inited++;
    }
    struct TypeLocClassName key = {tl, NULL};
    struct TypeLocClassName *found = (struct TypeLocClassName *)bsearch(&key, TypeLocClassName, NTypeLocName, sizeof(struct TypeLocClassName), TLNcompar);
    return found ? found->name : "Unknown";
}

static int
TLNcompar(const void *a, const void *b)
{
    return ((struct TypeLocClassName *)a)->key - ((struct TypeLocClassName *)b)->key;
}

static VALUE
typedefAttributes(const Type *T, VALUE rhash = Qnil)
{
    //if(T->isPointerType()) T = T->getPointeeType().getTypePtr();
    if(T->getTypeClass() == Type::Typedef) {
	rhash = declAttributes((static_cast<const TypedefType *>(T))->getDecl(), rhash);
    }
    return rhash;
}

enum ExprKind {
    ExprUnknown,
    ExprInt,
    ExprFloat,
    ExprString,
    ExprObjCString,
    ExprFuncAlias,
    ExprCallCFSTR,
    ExprCallNumber,
};

struct ExprData {
    ExprKind kind;
    std::string str;
};

class MySema : public Sema {
public:
    MySema(BridgeSupportParser *_BSP, ASTConsumer &consumer, bool CompleteTranslationUnit=true, CodeCompleteConsumer *CompletionConsumer=0) : Sema(_BSP->pp, _BSP->astctxt, consumer, CompleteTranslationUnit, CompletionConsumer), BSP(_BSP), customActOn(0) {}

#if 0
    virtual void ActOnEndOfTranslationUnit() {
	llvm::errs() << "***ActOnEndOfTranslationUnit\n";
	Sema::ActOnEndOfTranslationUnit();
    }
#endif

    virtual OwningStmtResult ActOnExprStmt(FullExprArg expr) {
	if(!customActOn) return Sema::ActOnExprStmt(expr);
	Expr *E = expr->takeAs<Expr>();
	CallExpr *C;
	QualType rettype;
	if(E) {
	    E = E->IgnoreParens();
	    Expr::EvalResult er;
	    if(E->Evaluate(er, Context)) {
		if(er.Val.isInt()) {
		    customActOn->str.append(er.Val.getInt().toString(10));
		    customActOn->kind = ExprInt;
		} else if(er.Val.isFloat()) {
		    //llvm::errs() << ">>isFloat: class=" << E->getStmtClassName() << "\n";
#if 0
		    /*
		     * In the case of a FloatingLiteral, we should be able to
		     * get the literal itself, but the "end" value of the
		     * SourceRange is equal to the "begin" value.  We could
		     * try to figure out where the literal ends ourselves, but
		     * it is easier to wait and deal with Tokens.
		     */
		    if(E->getStmtClass() == Stmt::FloatingLiteralClass) {
			FloatingLiteral *F = static_cast<FloatingLiteral *>(E);
			SourceRange r = F->getSourceRange();
			bool invalid0, invalid1;
			const char *begin = BSP->sm.getCharacterData(r.getBegin(), &invalid0);
			const char *end = BSP->sm.getCharacterData(r.getEnd(), &invalid1);
			const char *loc = BSP->sm.getCharacterData(F->getLocation());
			if(!invalid0 && !invalid1) {
			    std::string str(begin, end - begin);
			    fprintf(stderr, ">>FloatingLiteral: begin=%p end=%p loc=%p getValueAsApproximateDouble=%.18g %.10s\n", begin, end, loc, F->getValueAsApproximateDouble(), loc);
			    //llvm::errs() << ">>FloatingLiteral: " << str << "\n";
			}

		    }
#endif
		    llvm::SmallVector<char, 100> Buffer;
		    er.Val.getFloat().toString(Buffer);
		    customActOn->str.append(Buffer.data(), Buffer.size());
		    customActOn->kind = ExprFloat;
		} else if(E->getStmtClass() == Stmt::ObjCStringLiteralClass) {
		    goto doObjCString;
		} else if(E->getStmtClass() == Stmt::CStyleCastExprClass) {
		    CStyleCastExpr *CC = static_cast<CStyleCastExpr *>(E);
		    //llvm::errs() << ">>CStyleCastExprClass: castKind=" << CC->getCastKindName() << " type=" << CC->getType().getAsString() << " subExpr=" << CC->getSubExpr()->getStmtClassName() << " getSubExpr=" << CC->getSubExpr()->getStmtClassName() << "\n";
		    rettype = CC->getType();
		    if(rettype.getAsString() == "CFStringRef" && CC->getSubExpr()->getStmtClass() == Stmt::CallExprClass) {
			C = static_cast<CallExpr *>(CC->getSubExpr());
			goto doCFSTR;
		    }
		    //customActOn->str.append("(not int or float)");
#if 0
		} else {
		    llvm::errs() << ">>CallExpr: classname=" << E->getStmtClassName() << "\n";
		    customActOn->str.append("(not int or float)");
#endif
		}
	    } else {
		switch(E->getStmtClass()) {
		case Stmt::CallExprClass: {
		    C = static_cast<CallExpr *>(E);
#if 0
		    FunctionDecl *F = C->getDirectCallee();
		    if(F && C->getNumArgs() == 1 && (F->getName().equals("__CFStringMakeConstantString") || F->getName().equals("__builtin___CFStringMakeConstantString"))) {
			//llvm::errs() << ">>CallExpr: declName=" << F->getName() << "\n";
			Expr *A = C->getArg(0);
			//llvm::errs() << ">>CallExpr: arg0=" << A->getStmtClassName() << "\n";
			if(A->getStmtClass() == Stmt::ImplicitCastExprClass) {
			    ImplicitCastExpr *I = static_cast<ImplicitCastExpr *>(A);
			    //llvm::errs() << ">>CallExpr: castKind=" << I->getCastKindName() << " subExpr=" << I->getSubExpr()->getStmtClassName() << " subExprAsWritten=" << I->getSubExprAsWritten()->getStmtClassName() << "\n";
			    if(I->getSubExprAsWritten()->getStmtClass() == Stmt::StringLiteralClass) {
				StringLiteral *S = static_cast<StringLiteral *>(I->getSubExprAsWritten());
				customActOn->str.append(S->getStrData(), S->getByteLength());
				customActOn->kind = ExprObjCString;
				break;
			    }
			}
		    }
#endif
		    //llvm::errs() << ">>CallExpr: callee=" << C->getCallee()->getStmtClassName() << " getCallReturnType=" << C->getCallReturnType().getAsString() << "\n";
		    rettype = C->getCallReturnType();

		    if(rettype->isVectorType() || C->getNumArgs() > 1) break;
		    if(rettype->isIntegralType(BSP->astctxt) || rettype->isRealFloatingType()) {
			if(C->getNumArgs() == 1 && !C->getArg(0)->getType()->isIntegralType(BSP->astctxt)) break;
			customActOn->kind = ExprCallNumber;
			customActOn->str.append("(CallExprNumber)");
		    } else if(rettype.getAsString() == "CFStringRef") {
doCFSTR:
			if(C->getNumArgs() != 1) break;
			StringLiteral *S;
			if(C->getArg(0)->getStmtClass() == Stmt::ImplicitCastExprClass) {
			    ImplicitCastExpr *I = static_cast<ImplicitCastExpr *>(C->getArg(0));
			    //llvm::errs() << ">>CallExpr: ImplicitCastExpr: getSubExpr=" << I->getSubExprAsWritten()->getStmtClassName() << "\n";
			    if(I->getSubExprAsWritten()->getStmtClass() != Stmt::StringLiteralClass) break;
			    S = static_cast<StringLiteral *>(I->getSubExprAsWritten());
			} else if(C->getArg(0)->getStmtClass() == Stmt::StringLiteralClass) {
			    S = static_cast<StringLiteral *>(C->getArg(0));
			} else break;
			//llvm::errs() << ">>CallExpr:StringLiteral: " << str << "\n";
			/* possible CFSTR(...) */
			customActOn->kind = ExprCallCFSTR;
			customActOn->str.append(S->getStrData(), S->getByteLength());
		    }
		    break;
		}
		case Stmt::DeclRefExprClass: {
		    DeclRefExpr *D = static_cast<DeclRefExpr *>(E);
		    ValueDecl *V = D->getDecl();
		    if(V->getKind() == Decl::Function) {
			customActOn->str.append(V->getNameAsString());
			customActOn->kind = ExprFuncAlias;
		    }
		    break;
		}
		case Stmt::ObjCStringLiteralClass: {
doObjCString:
		    ObjCStringLiteral *O = static_cast<ObjCStringLiteral *>(E);
		    StringLiteral *S = O->getString();
		    customActOn->str.append(S->getStrData(), S->getByteLength());
		    customActOn->kind = ExprObjCString;
		    break;
		}
		case Stmt::StringLiteralClass: {
		    StringLiteral *S = static_cast<StringLiteral *>(E);
		    customActOn->str.append(S->getStrData(), S->getByteLength());
		    customActOn->kind = ExprString;
		    break;
		}
		default:
		    customActOn->str.append("(Evaluate failed)");
		    break;
		}
	    }
	} else {
	    customActOn->str.append("NULL");
	}
	return Owned(static_cast<Stmt*>(E));
    }

    void setCustomActOn(ExprData *e) { customActOn = e; }

private:
    BridgeSupportParser *BSP;
    ExprData *customActOn;
};

static bool
ignoreMacro(const MacroInfo *MI) {
    for (unsigned i = 0, e = MI->getNumTokens(); i != e; ++i) {
	switch(MI->getReplacementToken(i).getKind()) {
	case tok::l_brace:
	case tok::r_brace:
	case tok::semi:
	    return true;
	}
    }
    return false;
}

class MyPass1Consumer : public ASTConsumer {
public:
    virtual void HandleTranslationUnit (ASTContext &Ctx) {
	Preprocessor::macro_iterator M = BSP->pp.macro_begin(false), ME = BSP->pp.macro_end(false);
	if(M == ME) return;

	for (int i = 0; M != ME; M++) {
	    const IdentifierInfo *I = M->first;
	    const MacroInfo *V = M->second;
	    if(!V->isEnabled() || V->isBuiltinMacro() || !BSP->inDir(BSP->sm.getFileID(V->getDefinitionLoc())) || V->isFunctionLike()) continue;
	    if(ignoreMacro(V)) continue;
	    //llvm::errs() << I->getName() << " "; BSP->pp.DumpMacro(*V); //DEBUG
	    std::string data = I->getName().str();
	    data.insert(0, "(void) {\n{");
	    data.insert(0, I->getName().str());
	    data.insert(0, "void " MACROPREFIX);
	    data.append(";}\n}\n");
	    //llvm::errs() << data;
	    macros.append(data);
	}
    }

    std::string *getMacros(void) {
	//llvm::errs() << "===== getMacros =====\n" << macros << "===============\n"; //DEBUG
	return new std::string(macros);
    }

    void setup(BridgeSupportParser *_BSP) {
	BSP = _BSP;
    }
private:
    std::string macros;
    BridgeSupportParser *BSP;
};

static
void tokIgnoreParens(const MacroInfo *M, unsigned &start, unsigned &len)
{
    start = 0;
    len = M->getNumTokens();
    //llvm::errs() << "tokIgnoreParens: len=" << len << "\n";
    while(len > 2) {
	if(M->getReplacementToken(start).getKind() == tok::l_paren
	&& M->getReplacementToken(start + len - 1).getKind() == tok::r_paren) {
	    start++;
	    len -= 2;
	    //llvm::errs() << "tokIgnoreParens: start=" << start << " len=" << len << "\n";
	} else
	    return;
    }
}

class MyPass2Consumer : public SemaConsumer {
public:
    virtual void InitializeMySema(MySema &S) {
	sema = &S;
    }

    virtual void HandleTranslationUnit (ASTContext &Ctx) {
	SourceLocation sl;

	for (DeclContext::decl_iterator i = Ctx.getTranslationUnitDecl()->decls_begin(), e = Ctx.getTranslationUnitDecl()->decls_end(); i != e; i++) {
	    const NamedDecl *ND = dyn_cast<NamedDecl>(*i);
	    if(!ND || isDeclUnavailable(ND)) continue;

	    sl = ND->getLocation();
	    if(!sl.isValid() || !sl.isFileID()) continue;
	    const char *path = locgetpath(&BSP->sm, sl);

	    switch(ND->getKind()) {
	      case Decl::Enum: {
		const EnumDecl *E = static_cast<const EnumDecl *>(ND);
		if(E->isDefinition()) {
		    AnEnum e(BSP, path, E);
		    rb_yield(Data_Wrap_Struct(klass_AnEnum, 0, 0, &e));
		}
		break;
	      }
	      case Decl::Function: {
		const FunctionDecl *F = static_cast<const FunctionDecl *>(ND);
		//if(F->hasWrittenPrototype()) {
		    AFunctionDecl f(BSP, path, F);
		    rb_yield(Data_Wrap_Struct(klass_AFunctionDecl, 0, 0, &f));
		//}
		break;
	      }
	      case Decl::ObjCCategory: {
		const ObjCCategoryDecl *CD = static_cast<const ObjCCategoryDecl *>(ND);
		AnObjCCategory c(BSP, path, CD);
		rb_yield(Data_Wrap_Struct(klass_AnObjCCategory, 0, 0, &c));
		break;
	      }
	      case Decl::ObjCInterface: {
		const ObjCInterfaceDecl *ID = static_cast<const ObjCInterfaceDecl *>(ND);
		AnObjCInterface i(BSP, path, ID);
		rb_yield(Data_Wrap_Struct(klass_AnObjCInterface, 0, 0, &i));
		break;
	      }
	      case Decl::ObjCProtocol: {
		AnObjCProtocol p(BSP, path, static_cast<const ObjCProtocolDecl *>(ND));
		rb_yield(Data_Wrap_Struct(klass_AnObjCProtocol, 0, 0, &p));
		break;
	      }
	      case Decl::Record: {
		const RecordDecl *R = static_cast<const RecordDecl *>(ND);
		if(R->isDefinition() && R->isStruct()) {
		    AStruct s(BSP, path, R);
		    rb_yield(Data_Wrap_Struct(klass_AStruct, 0, 0, &s));
		}
		break;
	      }
	      case Decl::Typedef: {
		const TypedefDecl *TD = static_cast<const TypedefDecl *>(ND);
		ATypedef t(BSP, path, TD);
		rb_yield(Data_Wrap_Struct(klass_ATypedef, 0, 0, &t));
		break;
	      }
	      case Decl::Var: {
		const VarDecl *V = static_cast<const VarDecl *>(ND);
		AVar v(BSP, path, V);
		rb_yield(Data_Wrap_Struct(klass_AVar, 0, 0, &v));
		break;
	      }
	    }
	}
    }

    virtual void HandleMacros (Parser &P) {
	//llvm::errs() << "=== Macros ===\n";

	Parser::DeclGroupPtrTy ADecl;
	ExprData expr;
	const char *path;

	expr.kind = ExprUnknown;
	sema->setCustomActOn(&expr);
	while (!P.ParseTopLevelDecl(ADecl)) {
	    if(ADecl) {
		DeclGroupRef D = ADecl.getAsVal<DeclGroupRef>();
		for(DeclGroupRef::const_iterator I = D.begin(), E = D.end(); I != E; I++) {
		    const NamedDecl *ND = dyn_cast<NamedDecl>(*I);
		    if(!ND || isDeclUnavailable(ND)) continue;
		    //llvm::errs() << "ParseTopLevelDecl: " << ND->getDeclKindName() << " " << ND->getName() << "\n";
		    if(ND->getKind() == Decl::Function && ND->getName().startswith(MACROPREFIX)) {
			if(expr.kind == ExprUnknown) continue;

			llvm::StringRef name(ND->getName().substr(sizeof(MACROPREFIX) - 1));
			IdentifierInfo *II = BSP->pp.getIdentifierInfo(name);
			if(!II) rb_raise(rb_eRuntimeError, "Can't lookup identifier %s", name.data());
			const MacroInfo *M = BSP->pp.getMacroInfo(II);
			if(!M) rb_raise(rb_eRuntimeError, "Can't find macro info for %s", name.data());
			SourceLocation sl = M->getDefinitionLoc();
			if(!sl.isValid() || !sl.isFileID()) continue;
			path = locgetpath(&BSP->sm, sl);
#define tokN(n)		(M->getReplacementToken(start + (n)))
#define spellN(n)	(BSP->pp.getSpelling(tokN(n)))
#define tokNIs(n,k)	(tokN(n).getKind() == k)
			switch(expr.kind) {
			case ExprCallCFSTR: {
			    /* look for CFSTR(x) */
			    unsigned start, len;
			    tokIgnoreParens(M, start, len);
			    if(len >= 4
			    && tokNIs(0, tok::identifier)
			    && spellN(0).compare("CFSTR") == 0
			    && tokNIs(1, tok::l_paren)
			    && tokNIs(len - 1, tok::r_paren)) {
				AMacroString s(path, name.data(), expr.str.c_str(), true);
				rb_yield(Data_Wrap_Struct(klass_AMacroString, 0, 0, &s));
			    }
			    //llvm::errs() << "ExprCallCFSTR: " << name << " start=" << start << " len=" << len << " found=" << found << " str=" << str << "\n";
			    break;
			}
			case ExprCallNumber: {
			    AMacroNumberFuncCall n(path, name.data());
			    rb_yield(Data_Wrap_Struct(klass_AMacroNumberFuncCall, 0, 0, &n));
			    break;
			}
			case ExprFloat: {
			    /* get the original literal, if recognizable */
			    std::string str;
			    bool found = false;
			    unsigned start, len;
			    tokIgnoreParens(M, start, len);
			    switch(len) {
			    case 1: /* x */
				if(tokNIs(0, tok::numeric_constant)) {
				    str = spellN(0);
				    found = true;
				}
				break;
			    case 2: /* -x */
				if(tokNIs(0, tok::minus)
				&& tokNIs(1, tok::numeric_constant)) {
				    str = spellN(1);
				    str.insert(0, "-");
				    found = true;
				}
				break;
			    }
			    if(found) {
#if 0
				/* truncate trailing [FLfl] */
				size_t i = str.find_last_of("FLfl");
				if(i != std::string::npos)
				    str = str.substr(0, i);
#endif
				//llvm::errs() << name << ": (float)" << str << "\n";
				AMacroNumber n(path, name.data(), str.c_str());
				rb_yield(Data_Wrap_Struct(klass_AMacroNumber, 0, 0, &n));
				continue;
			    }
			    //llvm::errs() << name << ": (float)" << expr.str << "\n";
			    AMacroNumber n(path, name.data(), expr.str.c_str());
			    rb_yield(Data_Wrap_Struct(klass_AMacroNumber, 0, 0, &n));
			    break;
			}
			case ExprFuncAlias: {
			    //llvm::errs() << name << ": (funcalias)" << expr.str << "\n";
			    AMacroFunctionAlias f(path, name.data(), expr.str.c_str());
			    rb_yield(Data_Wrap_Struct(klass_AMacroFunctionAlias, 0, 0, &f));
			    break;
			}
			case ExprInt: {
			    //llvm::errs() << name << ": (int)" << expr.str << "\n";
			    AMacroNumber n(path, name.data(), expr.str.c_str());
			    rb_yield(Data_Wrap_Struct(klass_AMacroNumber, 0, 0, &n));
			    break;
			}
			case ExprObjCString: {
			    //llvm::errs() << name << ": (objcstring)" << expr.str << "\n";
			    AMacroString s(path, name.data(), expr.str.c_str(), true);
			    rb_yield(Data_Wrap_Struct(klass_AMacroString, 0, 0, &s));
			    break;
			}
			case ExprString: {
			    //llvm::errs() << name << ": (string)" << expr.str << "\n";
			    AMacroString s(path, name.data(), expr.str.c_str(), false);
			    rb_yield(Data_Wrap_Struct(klass_AMacroString, 0, 0, &s));
			    break;
			}
#if 0
			default:
			    llvm::errs() << ">>>Unknown ExprKind: " << name << ": " << expr.str << "\n";
			    continue;
#endif
			}
		    }
		}
	    }
	    expr.kind = ExprUnknown;
	    expr.str.clear();
	}
    }

    void setup(BridgeSupportParser *_BSP) {
	BSP = _BSP;
    }
private:
    class BridgeSupportParser *BSP;
    MySema *sema;
};

BridgeSupportParser::BridgeSupportParser(const char **headers, const std::string& triple, const char **defines, const char **incdirs, const std::string& sysroot, bool verbose)
	: diagClient(verbose ? llvm::errs() : llvm::nulls(), diagOpts),
	  diags(&diagClient),
	  targOpts(triple),
	  target(TargetInfo::CreateTargetInfo(diags, targOpts)),
	  hs(fm),
	  sm(diags),
	  pp(diags, opts, *target, sm, hs),
	  astctxt(opts, sm, *target, pp.getIdentifierTable(), pp.getSelectorTable(), pp.getBuiltinInfo(), 0),
	  verbose(verbose)
{
    diags.setSuppressSystemWarnings(true);
    diags.setDiagnosticMapping(diag::ext_multichar_character_literal, diag::MAP_IGNORE);
    pp.getBuiltinInfo().InitializeBuiltins(pp.getIdentifierTable());

    // Add header search directories
    HeaderSearchOptions hso(sysroot);
    hso.AddPath(*defaultIncludePath, frontend::After, false, false);

    /*
     * Add include directories and frameworks:
     * Each string is composed of 4 parts:
     *  group (1 character): 'A', 'Q' or 'S' for Angled, Quoted or System
     *  isUserSupplied (1 character): 'T', 'F' for True or False
     *  isFramework (1 character): 'T', 'F' for True or False
     *  path (remaining characters)
     */
    if(incdirs) {
	frontend::IncludeDirGroup g;
	bool u, f;
	for(const char **d = incdirs; *d; d++) {
	    switch((*d)[0]) {
	    case 'Q':
		g = frontend::Quoted;
		break;
	    case 'S':
		g = frontend::System;
		break;
	    default:
		g = frontend::Angled;
		break;
	    }
	    u = ((*d)[1] == 'T');
	    f = ((*d)[2] == 'T');
	    hso.AddPath(*d + 3, g, u, f);
	}
    }

    PreprocessorOptions ppo;
    const char **p;

    // Add the list of header files to parse
    for(p = headers; *p; p++)
	ppo.Includes.push_back(*p);

    // Add some predefined macros
    for(p = mmintrin_builtins; *p; p++)
	ppo.addMacroDef(*p);

    // Add macros from passed-in argument
    if(defines) {
	for(const char **d = defines; *d; d++)
	    ppo.addMacroDef(*d);
    }
    FrontendOptions feo;
    InitializePreprocessor(pp, ppo, hso, feo);

    // create a dummy FieldDecl for getObjCEncodingForType()
    dummyFD = FieldDecl::Create(astctxt, NULL, SourceLocation(), NULL, QualType(), NULL, NULL, false);
}

BridgeSupportParser::~BridgeSupportParser()
{
    delete target;
}

void
BridgeSupportParser::addFile(const char *file) {
    const DirectoryLookup *CurDir;
    const FileEntry *fe = pp.getHeaderSearchInfo().LookupFile(file, false, NULL, CurDir, NULL);
    if(!fe)
	rb_raise(rb_eRuntimeError, "addFile: Couldn't lookup file: %s", file);
    char path[PATH_MAX];
    if(realpath(fe->getName(), path) == NULL) {
	int saveerrno = errno;
	snprintf(path, sizeof(path), "addFile: realpath: %s", fe->getName());
	errno = saveerrno;
	rb_sys_fail(path);
    }
    char *p = strrchr(path, '/');
    if(!p)
	rb_raise(rb_eRuntimeError, "addFile: No slash after realpath: %s", path);
    *p = 0; // root (/) will result in empty string
    dirs[path] = strlen(path);
}

bool
BridgeSupportParser::inDir(FileID file) {
    // if addFile() was never called, then always return true
    if(dirs.size() == 0) return true;
    std::map<clang::FileID,bool>::iterator it = inDirCache.find(file);
    std::map<clang::FileID,bool>::iterator et = inDirCache.end();
    if(it != et) return it->second;

    const clang::FileEntry *fe = sm.getFileEntryForID(file);
    char path[PATH_MAX];
    if(fe) {
	if(realpath(fe->getName(), path) == NULL) {
	    int saveerrno = errno;
	    snprintf(path, sizeof(path), "inDir: realpath: %s", fe->getName());
	    errno = saveerrno;
	    rb_sys_fail(path);
	}
    } else {
	strcpy(path, "/non-existent");
    }
    std::map<std::string,size_t>::iterator id;
    std::map<std::string,size_t>::iterator ed;
    for(id = dirs.begin(), ed = dirs.end(); id != ed; id++) {
	if(id->first.compare(0, id->second, path, id->second) == 0 && path[id->second] == '/') {
	    return (inDirCache[file] = true);
	}
    }
    return (inDirCache[file] = false);
}

void
BridgeSupportParser::getObjCEncodingForType(QualType t, std::string &S, const FieldDecl *Field)
{
    astctxt.getObjCEncodingForType(t, S, Field);
    /*
     * Remove any leading 'r', which can't be removed with getUnqualifiedType()
     * or others.
     */
    if(S[0] == 'r') S.erase(0, 1);
}

// User must free() c-array
static const char **
rubyarr2c(VALUE arr)
{
    Check_Type(arr, T_ARRAY);
    /* Get the length of the array */
    int size = RARRAY(arr)->len; 
    const char **carr = (const char **) malloc((size+1)*sizeof(const char *));
    if(!carr) rb_raise(rb_eRuntimeError, "Out of memory");
    /* Get the first element in memory */
    VALUE *ptr = RARRAY(arr)->ptr; 
    const char **p;
    for (p = carr; size > 0; size--, ptr++)
    /* Convert Ruby Object String to char* */
	*p++ = STR2CSTR(*ptr); 
    *p = NULL; /* End of list */
    return carr;
}

// MyParseAST is based on ParseAST.cpp
void MyParseAST(BridgeSupportParser *BSP, MyPass2Consumer *Consumer) {
    MySema S(BSP, *Consumer);
    Parser P(BSP->pp, S);
    BSP->pp.EnterMainSourceFile();

    // Initialize the parser.
    P.Initialize();

    Consumer->Initialize(BSP->astctxt);
    Consumer->InitializeMySema(S);

    if (ExternalASTSource *External = BSP->astctxt.getExternalSource()) {
	if (ExternalSemaSource *ExternalSema =
	    dyn_cast<ExternalSemaSource>(External))
	    ExternalSema->InitializeSema(S);

	External->StartTranslationUnit(Consumer);
    }

    Parser::DeclGroupPtrTy ADecl;
    bool found = false;

    while (!P.ParseTopLevelDecl(ADecl)) {    // Not end of file.
	// If we got a null return and something *was* parsed, ignore it.    This
	// is due to a top-level semicolon, an action override, or a parse error
	// skipping something.
	if (!ADecl) continue;
	DeclGroupRef D = ADecl.getAsVal<DeclGroupRef>();
	for(DeclGroupRef::const_iterator I = D.begin(), E = D.end(); I != E; I++) {
	    if((*I)->getKind() == Decl::ObjCInterface) {
		const ObjCInterfaceDecl *ID = static_cast<const ObjCInterfaceDecl *>(*I);
		//llvm::errs() << "ObjCInterface: " << ID->getNameAsCString() << "\n"; //DEBUG
		if(ID->getName().equals(CONTENTEND)) {
		    //llvm::errs() << "Found " CONTENTEND "\n";
		    found = true;
		}
	    }
	    break; // Only check the first
	}
	if (found) break;
	Consumer->HandleTopLevelDecl(D);
    }
    if(!found) rb_raise(rb_eRuntimeError, "%s not found", CONTENTEND);

    // Check for any pending objective-c implementation decl.
    while ((ADecl = P.FinishPendingObjCActions()))
	Consumer->HandleTopLevelDecl(ADecl.getAsVal<DeclGroupRef>());

    // Process any TopLevelDecls generated by #pragma weak.
    for (llvm::SmallVector<Decl*,2>::iterator
	I = S.WeakTopLevelDecls().begin(),
	E = S.WeakTopLevelDecls().end(); I != E; ++I)
	Consumer->HandleTopLevelDecl(DeclGroupRef(*I));

    Consumer->HandleTranslationUnit(BSP->astctxt);
    Consumer->HandleMacros(P);

    if (ExternalSemaSource *ESS =
	dyn_cast_or_null<ExternalSemaSource>(BSP->astctxt.getExternalSource()))
	ESS->ForgetSema();

    Consumer->ForgetSema();
}

void
BridgeSupportParser::parse(VALUE rubyheaders, const char *content, const std::string& triple, VALUE rubydefines, VALUE rubyincdirs, const std::string& sysroot, bool verbose)
{
    if (!rb_block_given_p()) rb_raise(rb_eRuntimeError, "BridgeSupportParser::parse must be called with a block");
    if (rubyheaders == Qnil) rb_raise(rb_eRuntimeError, "BridgeSupportParser::parse: No headers specified");

    // get the (default) gcc/llvm include path
    if(!defaultIncludePath) {
	FILE *p;
	std::string cc;
	char buf[PATH_MAX + 16];
	char *nl;
	cc = "xcrun -find cc -sdk ";
	cc += sysroot;
	*buf = 0;
	if((p = popen(cc.c_str(), "r")) != NULL) {
	    if(fgets(buf, sizeof(buf), p)) {
		nl = strchr(buf, '\n');
		if(nl) *nl = 0;
	    }
	    pclose(p);
	}
	cc = (*buf ? buf : "cc");
	if(verbose) llvm::errs() << "cc=" << cc << "\n";
	cc += " -print-search-dirs";
	defaultIncludePath = new std::string();
	if((p = popen(cc.c_str(), "r")) != NULL) {
	    while(fgets(buf, sizeof(buf), p)) {
		if(strncmp(buf, "install: ", 9) == 0) {
		    nl = strchr(buf, '\n');
		    if(nl) *nl = 0;
		    defaultIncludePath->append(buf + 9);
		    defaultIncludePath->append("include");
		    while(fgets(buf, sizeof(buf), p)) {} // drain the source
		    break;
		}
	    }
	    pclose(p);
	}
	if(defaultIncludePath->empty()) defaultIncludePath->append("/usr/lib/gcc/i686-apple-darwin11/4.2.1/include");
	if(verbose) llvm::errs() << "defaultIncludePath=" << *defaultIncludePath << "\n";
    }

    const char **headers = rubyarr2c(rubyheaders);
    const char **defines = (rubydefines != Qnil) ? rubyarr2c(rubydefines) : NULL;
    const char **incdirs = (rubyincdirs != Qnil) ? rubyarr2c(rubyincdirs) : NULL;

    std::string *macros = pass1(headers, triple, defines, incdirs, sysroot, verbose);
    pass2(headers, content, triple, defines, incdirs, sysroot, verbose, macros);

    delete macros;
    free(incdirs);
    free(defines);
    free(headers);
}

std::string *
BridgeSupportParser::pass1(const char **headers, const std::string& triple, const char **defines, const char **incdirs, const std::string& sysroot, bool verbose) {
    BridgeSupportParser bs(headers, triple, defines, incdirs, sysroot, verbose);
    // Limit pass 1 output to just files in the same directories (or
    // subdirectories) as the files in "headers".
    for(const char **p = headers; *p; p++)
	bs.addFile(*p);

    const char *empty = "";
    llvm::MemoryBuffer *membuf = llvm::MemoryBuffer::getMemBuffer(empty, empty);
    bs.sm.createMainFileIDForMemBuffer(membuf); // ownership of membuf passes to sm
    MyPass1Consumer c;
    c.setup(&bs);
    ParseAST(bs.pp, &c, bs.astctxt);    // calls EnterMainSourceFile() for us
    return c.getMacros();
}

void
BridgeSupportParser::pass2(const char **headers, const char *content, const std::string& triple, const char **defines, const char **incdirs, const std::string& sysroot, bool verbose, std::string *macros) {
    BridgeSupportParser bs(headers, triple, defines, incdirs, sysroot, verbose);
    std::string src;
    if(content) src.append(content);
    src.append("@interface " CONTENTEND "\n@end\n");
    src.append(*macros);
    //llvm::errs() << "-----------\n" << src << "-----------\n"; //DEBUG
    llvm::MemoryBuffer *membuf = llvm::MemoryBuffer::getMemBuffer(src.c_str(), src.c_str() + src.length());
    bs.sm.createMainFileIDForMemBuffer(membuf); // ownership of membuf passes to sm
    MyPass2Consumer c;
    c.setup(&bs);
    MyParseAST(&bs, &c);    // calls EnterMainSourceFile() for us
}

#define RB_STR_NEW2(x) ((x) ? rb_str_new2(x) : Qnil)

#if 0
static inline VALUE
arrayOf1String1Bool(const char *str0, bool bool0)
{
    return rb_ary_new3(2, RB_STR_NEW2(str0), bool0 ? Qtrue : Qfalse);
}
#endif

static inline VALUE
arrayOf2Strings(const char *str0, const char *str1)
{
    return rb_ary_new3(2, RB_STR_NEW2(str0), RB_STR_NEW2(str1));
}

static inline VALUE
arrayOf2Strings1Bool(const char *str0, const char *str1, bool bool0)
{
    return rb_ary_new3(3, RB_STR_NEW2(str0), RB_STR_NEW2(str1), bool0 ? Qtrue : Qfalse);
}

static inline VALUE
arrayOf2Strings1VALUE(const char *str0, const char *str1, VALUE v0)
{
    return rb_ary_new3(3, RB_STR_NEW2(str0), RB_STR_NEW2(str1), v0);
}

static inline VALUE
arrayOf2Strings2VALUEs(const char *str0, const char *str1, VALUE v0, VALUE v1)
{
    return rb_ary_new3(4, RB_STR_NEW2(str0), RB_STR_NEW2(str1), v0, v1);
}

static inline VALUE
arrayOf3Strings(const char *str0, const char *str1, const char *str2)
{
    return rb_ary_new3(3, RB_STR_NEW2(str0), RB_STR_NEW2(str1), RB_STR_NEW2(str2));
}

#if 0
static inline VALUE
arrayOf3Strings1Bool(const char *str0, const char *str1, const char *str2, bool bool0)
{
    return rb_ary_new3(4, RB_STR_NEW2(str0), RB_STR_NEW2(str1), RB_STR_NEW2(str2), bool0 ? Qtrue : Qfalse);
}

static inline VALUE
arrayOf3Strings1VALUE(const char *str0, const char *str1, const char *str2, VALUE v0)
{
    return rb_ary_new3(4, RB_STR_NEW2(str0), RB_STR_NEW2(str1), RB_STR_NEW2(str2), v0);
}

static inline VALUE
arrayOf3Strings1VALUE1Bool(const char *str0, const char *str1, const char *str2, VALUE v0, bool bool0)
{
    return rb_ary_new3(5, RB_STR_NEW2(str0), RB_STR_NEW2(str1), RB_STR_NEW2(str2), v0, bool0 ? Qtrue : Qfalse);
}

static inline VALUE
arrayOf3Strings2Bools(const char *str0, const char *str1, const char *str2, bool bool0, bool bool1)
{
    return rb_ary_new3(5, RB_STR_NEW2(str0), RB_STR_NEW2(str1), RB_STR_NEW2(str2), bool0 ? Qtrue : Qfalse, bool1 ? Qtrue : Qfalse);
}

static inline VALUE
arrayOf3Strings2Bools2VALUEs(const char *str0, const char *str1, const char *str2, bool bool0, bool bool1, VALUE v0, VALUE v1)
{
    return rb_ary_new3(7, RB_STR_NEW2(str0), RB_STR_NEW2(str1), RB_STR_NEW2(str2), bool0 ? Qtrue : Qfalse, bool1 ? Qtrue : Qfalse, v0, v1);
}
#endif

static inline VALUE
arrayOf3Strings2VALUEs(const char *str0, const char *str1, const char *str2, VALUE v0, VALUE v1)
{
    return rb_ary_new3(5, RB_STR_NEW2(str0), RB_STR_NEW2(str1), RB_STR_NEW2(str2), v0, v1);
}

static inline VALUE
arrayOf3Strings3VALUEs2Bools(const char *str0, const char *str1, const char *str2, VALUE v0, VALUE v1, VALUE v2, bool bool0, bool bool1)
{
    return rb_ary_new3(8, RB_STR_NEW2(str0), RB_STR_NEW2(str1), RB_STR_NEW2(str2), v0, v1, v2, bool0 ? Qtrue : Qfalse, bool1 ? Qtrue : Qfalse);
}

static inline VALUE
arrayOf4Strings3VALUEs2Bools(const char *str0, const char *str1, const char *str2, const char *str3, VALUE v0, VALUE v1, VALUE v2, bool bool0, bool bool1)
{
    return rb_ary_new3(9, RB_STR_NEW2(str0), RB_STR_NEW2(str1), RB_STR_NEW2(str2), RB_STR_NEW2(str3), v0, v1, v2, bool0 ? Qtrue : Qfalse, bool1 ? Qtrue : Qfalse);
}

static inline VALUE
arrayOf4Strings2VALUEs(const char *str0, const char *str1, const char *str2, const char *str3, VALUE v0, VALUE v1)
{
    return rb_ary_new3(6, RB_STR_NEW2(str0), RB_STR_NEW2(str1), RB_STR_NEW2(str2), RB_STR_NEW2(str3), v0, v1);
}

void
delete_AFunctionType(AFunctionType *obj)
{
    delete obj;
}

void
AnEnum::each_value()
{
    for(EnumDecl::enumerator_iterator E = ED->enumerator_begin(), EE = ED->enumerator_end(); E != EE; E++) {
	rb_yield(arrayOf2Strings((*E)->getNameAsCString(), (*E)->getInitVal().toString(10).c_str()));
    }
}

VALUE
AnEnum::info()
{
    return rb_str_new2(ED->getNameAsCString());
}

void
AFunctionDecl::each_argument()
{
    std::string enc;
    for(FunctionDecl::param_const_iterator P = FD->param_begin(), PE = FD->param_end(); P != PE; P++) {
	enc.clear();
	BSP->getObjCEncodingForType((*P)->getType(), enc);
	/*
	 * We treat function pointer and block pointer as just the single
	 * AFunctionType class, since gen_bridge_metadata makes no distinction
	 * between the two (other than the Objective C encoding type).  If
	 * we ever need to make the distinction, we could create a new
	 * ABlockType class for blocks.
	 */
	if((*P)->getType()->isFunctionPointerType() || (*P)->getType()->isBlockPointerType()) {
	    AFunctionType f(BSP, Path, (*P)->getType()->getPointeeType()->getAs<FunctionType>());
	    rb_yield(arrayOf3Strings2VALUEs(
		(*P)->getNameAsCString(),
		(*P)->getType().getAsString().c_str(),
		enc.c_str(),
		typedefAttributes(
		    (*P)->getType().getTypePtr(),
		    declAttributes(*P)
		),
		Data_Wrap_Struct(klass_AFunctionType, 0, 0, &f)
	    ));
	} else {
	    rb_yield(arrayOf3Strings2VALUEs(
		(*P)->getNameAsCString(),
		(*P)->getType().getAsString().c_str(),
		enc.c_str(),
		typedefAttributes(
		    (*P)->getType().getTypePtr(),
		    declAttributes(*P)
		),
		Qnil
	    ));
	}
    }
}

VALUE
AFunctionDecl::info()
{
    std::string retenc;
    QualType rettype = FD->getResultType();
    BSP->getObjCEncodingForType(rettype, retenc);
    const FunctionProtoType *FP = FD->getType()->getAs<FunctionProtoType>();
    if(rettype->isFunctionPointerType() || rettype->isBlockPointerType()) {
	AFunctionType *f = new AFunctionType(BSP, Path, rettype->getPointeeType()->getAs<FunctionType>());
	return arrayOf3Strings3VALUEs2Bools(
	    FD->getNameAsCString(),
	    FD->getResultType().getAsString().c_str(),
	    retenc.c_str(),
	    typedefAttributes(rettype.getTypePtr()),
	    Data_Wrap_Struct(klass_AFunctionType, 0, RB_FINALIZER(delete_AFunctionType), f),
	    declAttributes(FD),
	    (FP && FP->isVariadic()),
	    FD->isInlined()
	);
    } else {
	return arrayOf3Strings3VALUEs2Bools(
	    FD->getNameAsCString(),
	    FD->getResultType().getAsString().c_str(),
	    retenc.c_str(),
	    typedefAttributes(rettype.getTypePtr()),
	    Qnil,
	    declAttributes(FD),
	    (FP && FP->isVariadic()),
	    FD->isInlined()
	);
    }
}
#if 0
void
AFunctionLoc::each_argument()
{
    std::string enc;
    for(unsigned n = FTL->getNumArgs(), i = 0; i < n; i++) {
	ParmVarDecl *P = FTL->getArg(i);
	enc.clear();
	BSP->getObjCEncodingForType(P->getType(), enc);
	rb_yield(arrayOf3Strings(P->getNameAsCString(), P->getType().getAsString().c_str(), enc.c_str()));
    }
}

VALUE
AFunctionLoc::info()
{
    std::string enc;
    QualType rettype = FTL->getResultLoc().getType();
    BSP->getObjCEncodingForType(rettype, enc);
    const FunctionProtoType *FP = FTL->getType()->getAs<FunctionProtoType>();
    return arrayOf3Strings2Bools("", rettype.getAsString().c_str(), enc.c_str(), (FP && FP->isVariadic()), false);
}
#endif
void
AFunctionType::each_argument()
{
    const FunctionProtoType *FPT = FT->getAs<FunctionProtoType>();
    if(!FPT) return;
    std::string enc;
    for(FunctionProtoType::arg_type_iterator T = FPT->arg_type_begin(), TE = FPT->arg_type_end(); T != TE; T++) {
	enc.clear();
	BSP->getObjCEncodingForType(*T, enc);
	if((*T)->isFunctionPointerType() || (*T)->isBlockPointerType()) {
	    AFunctionType f(BSP, Path, (*T)->getPointeeType()->getAs<FunctionType>());
	    rb_yield(arrayOf3Strings2VALUEs(
		"",
		(*T).getAsString().c_str(),
		enc.c_str(),
		typedefAttributes((*T).getTypePtr()),
		Data_Wrap_Struct(klass_AFunctionType, 0, 0, &f)
	    ));
	} else {
	    rb_yield(arrayOf3Strings2VALUEs(
		"",
		(*T).getAsString().c_str(),
		enc.c_str(),
		typedefAttributes((*T).getTypePtr()),
		Qnil
	    ));
	}
    }
}

VALUE
AFunctionType::info()
{
    std::string retenc;
    QualType rettype = FT->getResultType();
    BSP->getObjCEncodingForType(rettype, retenc);
    const FunctionProtoType *FP = rettype->getAs<FunctionProtoType>();
    if(rettype->isFunctionPointerType() || rettype->isBlockPointerType()) {
	AFunctionType *f = new AFunctionType(BSP, Path, rettype->getPointeeType()->getAs<FunctionType>());
	return arrayOf3Strings3VALUEs2Bools(
	    "",
	    rettype.getAsString().c_str(),
	    retenc.c_str(),
	    typedefAttributes(rettype.getTypePtr()),
	    Data_Wrap_Struct(klass_AFunctionType, 0, RB_FINALIZER(delete_AFunctionType), f),
	    Qnil,
	    (FP && FP->isVariadic()),
	    false
	);
    } else {
	return arrayOf3Strings3VALUEs2Bools(
	    "",
	    rettype.getAsString().c_str(),
	    retenc.c_str(),
	    typedefAttributes(rettype.getTypePtr()),
	    Qnil,
	    Qnil,
	    (FP && FP->isVariadic()),
	    false
	);
    }
}

VALUE
AMacroValueBase::info()
{
    //fprintf(stderr, "AMacroValueBase::info: %p %p(%s) %p(%s)\n", this, Name, Name, Value, Value); //DEBUG
    return arrayOf2Strings(Name, Value);
}

VALUE
AMacroNumberFuncCall::info()
{
    return RB_STR_NEW2(Name);
}

VALUE
AMacroString::info()
{
    return arrayOf2Strings1Bool(Name, Value, ObjCString);
}

void
AnObjCMethodProtocolIter::each_method()
{
    for(ObjCContainerDecl::method_iterator M = this->meth_begin(), ME = this->meth_end(); M != ME; M++) {
	if(isDeclUnavailable(*M)) continue;
	AnObjCMethod m(BSP, *M);
	rb_yield(Data_Wrap_Struct(klass_AnObjCMethod, 0, 0, &m));
    }
}

void
AnObjCMethodProtocolIter::each_protocol()
{
    for(ObjCInterfaceDecl::protocol_iterator P = this->protocol_begin(), PE = this->protocol_end(); P != PE; P++) {
	rb_yield(rb_str_new2((*P)->getNameAsCString()));
    }
}

VALUE
AnObjCCategory::info()
{
    return arrayOf2Strings(CD->getClassInterface()->getNameAsCString(), CD->getNameAsCString());
}

VALUE
AnObjCInterface::info()
{
    return rb_str_new2(ID->getNameAsCString());
}

void
AnObjCMethod::each_argument()
{
    std::string type, enc;
    const char *type_modifier;
    for(ObjCMethodDecl::param_iterator P = MD->param_begin(), PE = MD->param_end(); P != PE; P++) {
	type = "";
	type_modifier = NULL;
	switch((*P)->getObjCDeclQualifier()) {
	  case Decl::OBJC_TQ_In:
	    type = "in ";
	    type_modifier = "n";
	    break;
	  case Decl::OBJC_TQ_Inout:
	    type = "inout ";
	    type_modifier = "N";
	    break;
	  case Decl::OBJC_TQ_Out:
	    type = "out ";
	    type_modifier = "o";
	    break;
	  case Decl::OBJC_TQ_Bycopy:
	    type = "bycopy ";
	    break;
	  case Decl::OBJC_TQ_Byref:
	    type = "byref ";
	    break;
	  case Decl::OBJC_TQ_Oneway:
	    type = "oneway ";
	    break;
	}
	type.append((*P)->getType().getAsString());
	enc.clear();
	BSP->getObjCEncodingForType((*P)->getType(), enc);
	if((*P)->getType()->isFunctionPointerType() || (*P)->getType()->isBlockPointerType()) {
	    AFunctionType f(BSP, Path, (*P)->getType()->getPointeeType()->getAs<FunctionType>());
	    rb_yield(arrayOf4Strings2VALUEs(
		(*P)->getNameAsCString(),
		type.c_str(),
		enc.c_str(),
		type_modifier,
		typedefAttributes(
		    (*P)->getType().getTypePtr(),
		    declAttributes(*P)
		),
		Data_Wrap_Struct(klass_AFunctionType, 0, 0, &f)
	    ));
	} else {
	    rb_yield(arrayOf4Strings2VALUEs(
		(*P)->getNameAsCString(),
		type.c_str(),
		enc.c_str(),
		type_modifier,
		typedefAttributes(
		    (*P)->getType().getTypePtr(),
		    declAttributes(*P)
		),
		Qnil
	    ));
	}
    }
}

VALUE
AnObjCMethod::info()
{
    //std::string type = MD->getResultType().isNull() ? "id" : MD->getResultType().getAsString();
    std::string menc, retenc;
    BSP->astctxt.getObjCEncodingForMethodDecl(MD, menc);
    QualType rettype = MD->getResultType();
    BSP->getObjCEncodingForType(rettype, retenc);
    if(rettype->isFunctionPointerType() || rettype->isBlockPointerType()) {
	AFunctionType *f = new AFunctionType(BSP, Path, rettype->getPointeeType()->getAs<FunctionType>());
	return arrayOf4Strings3VALUEs2Bools(
	    MD->getSelector().getAsString().c_str(),
	    menc.c_str(),
	    rettype.getAsString().c_str(),
	    retenc.c_str(),
	    typedefAttributes(rettype.getTypePtr()),
	    Data_Wrap_Struct(klass_AFunctionType, 0, RB_FINALIZER(delete_AFunctionType), f),
	    declAttributes(MD),
	    MD->isClassMethod(),
	    MD->isVariadic()
	);
    } else {
	return arrayOf4Strings3VALUEs2Bools(
	    MD->getSelector().getAsString().c_str(),
	    menc.c_str(),
	    rettype.getAsString().c_str(),
	    retenc.c_str(),
	    typedefAttributes(rettype.getTypePtr()),
	    Qnil,
	    declAttributes(MD),
	    MD->isClassMethod(),
	    MD->isVariadic()
	);
    }
}

VALUE
AnObjCProtocol::info()
{
    return rb_str_new2(PD->getNameAsCString());
}

void
AStruct::each_field()
{
    std::string enc;
    for(RecordDecl::field_iterator F = RD->field_begin(), FE = RD->field_end(); F != FE; F++) {
	QualType type = (*F)->getType();
	enc.clear();
	BSP->getObjCEncodingForType(type, enc, *F);
	if(type->isFunctionPointerType() || type->isBlockPointerType()) {
	    AFunctionType f(BSP, Path, type->getPointeeType()->getAs<FunctionType>());
	    rb_yield(arrayOf3Strings2VALUEs(
		(*F)->getNameAsCString(),
		type.getAsString().c_str(),
		enc.c_str(),
		typedefAttributes(
		    type.getTypePtr(),
		    declAttributes(*F)
		),
		Data_Wrap_Struct(klass_AFunctionType, 0, 0, &f)
	    ));
	} else {
	    rb_yield(arrayOf3Strings2VALUEs(
		(*F)->getNameAsCString(),
		type.getAsString().c_str(),
		enc.c_str(),
		typedefAttributes(
		    type.getTypePtr(),
		    declAttributes(*F)
		),
		Qnil
	    ));
	}
    }
}

VALUE
AStruct::info()
{
    std::string enc;
    BSP->getObjCEncodingForType(RD->getTypeForDecl()->getCanonicalTypeInternal(), enc, BSP->dummyFD);
    return arrayOf2Strings(RD->getNameAsCString(), enc.c_str());
}

VALUE
ATypedef::encoding()
{
    std::string enc;
    BSP->getObjCEncodingForType(TD->getUnderlyingType(), enc);
    return RB_STR_NEW2(enc.c_str());
}

VALUE
ATypedef::info()
{
    return arrayOf2Strings1VALUE(TD->getNameAsCString(), TD->getUnderlyingType().getAsString().c_str(), declAttributes(TD));
}

void
ATypedef::walk_types()
{
    TypeSourceInfo *tsi = TD->getTypeSourceInfo();
    if(tsi) {
again:
	TypeLoc tl = tsi->getTypeLoc();
	do {
redo:
	    const char *name = NULL;
	    const char *type = lookupTypeLocClass(tl.getTypeLocClass());
	    // special cases
	    switch(tl.getTypeLocClass()) {
#if 0
	      case TypeLoc::FunctionProto: {
		FunctionTypeLoc *fl = static_cast<FunctionTypeLoc *>(&tl);
		AFunctionLoc f(BSP, Path, fl);
		rb_yield(arrayOf2Strings1VALUE(name, type, Data_Wrap_Struct(klass_AFunctionLoc, 0, 0, &f)));
		continue;
	      }
#endif
	      case TypeLoc::Qualified: {
		QualifiedTypeLoc *ql = static_cast<QualifiedTypeLoc *>(&tl);
		tl = ql->getUnqualifiedLoc();
		goto redo;
		break;
	      }
	      case TypeLoc::Record: {
		RecordTypeLoc *rl = static_cast<RecordTypeLoc *>(&tl);
		RecordDecl *R = rl->getDecl();
		name = R->getNameAsCString();
		if(!R->isStruct()) {
		    /*
		     * No need to test for R->isEnum(), since there is a
		     * separate EnumTypeLoc class
		     */
		    if(R->isUnion()) type = "Union";
		    else if (R->isClass()) type = "Class";
		    break; /* do default code at bottom */
		}
		type = "Struct";
		if(R->isDefinition()) {
		    AStruct s(BSP, Path, R);
		    rb_yield(arrayOf2Strings2VALUEs(name, type, Data_Wrap_Struct(klass_AStruct, 0, 0, &s), declAttributes(R)));
		    continue;
		}
		break;
	      }
	      case TypeLoc::Typedef: {
		TypedefTypeLoc *td = static_cast<TypedefTypeLoc *>(&tl);
		TypedefDecl *T = td->getTypedefDecl();
		rb_yield(arrayOf2Strings2VALUEs(T->getNameAsCString(), type, Qnil, declAttributes(T)));
		tsi = T->getTypeSourceInfo();
		goto again;
	      }
	    }
	    // default
	    rb_yield(arrayOf2Strings2VALUEs(name, type, Qnil, Qnil));
	} while((tl = tl.getNextTypeLoc()));
    }
}

VALUE
AVar::info()
{
    QualType type = VD->getType();
    std::string enc;
    BSP->getObjCEncodingForType(type, enc);
    if(type->isFunctionPointerType() || type->isBlockPointerType()) {
	AFunctionType *f = new AFunctionType(BSP, Path, type->getPointeeType()->getAs<FunctionType>());
	return arrayOf3Strings2VALUEs(
	    VD->getNameAsCString(),
	    type.getAsString().c_str(),
	    enc.c_str(),
	    typedefAttributes(
		type.getTypePtr(),
		declAttributes(VD)
	    ),
	    Data_Wrap_Struct(klass_AFunctionType, 0, RB_FINALIZER(delete_AFunctionType), f)
	);
    } else {
	return arrayOf3Strings2VALUEs(
	    VD->getNameAsCString(),
	    type.getAsString().c_str(),
	    enc.c_str(),
	    typedefAttributes(
		type.getTypePtr(),
		declAttributes(VD)
	    ),
	    Qnil
	);
    }
}
