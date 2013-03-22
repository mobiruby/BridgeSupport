#include "classes.h"

class BridgeSupportParser {
public:
    static void parse(VALUE headers, const char* content, const std::string& triple, VALUE defines = Qnil, VALUE incdirs = Qnil, const std::string& sysroot = "/", bool verbose = false);

private:
    // Takes ownership of client.
    BridgeSupportParser(const char **headers, const std::string& triple, const char **defines, const char **incdirs, const std::string& sysroot, bool verbose);
    ~BridgeSupportParser();
#ifndef SWIG
    void addFile(const char *file);
    bool inDir(clang::FileID file);
    void getObjCEncodingForType(clang::QualType t, std::string &S, const clang::FieldDecl *Field=0);

    static std::string *pass1(const char **headers, const std::string& triple, const char **defines, const char **incdirs, const std::string& sysroot, bool verbose);
    static void pass2(const char **headers, const char* content, const std::string& triple, const char **defines, const char **incdirs, const std::string& sysroot, bool verbose, std::string *macros);

    static std::string *defaultIncludePath;

    MyDiagnosticOptions diagOpts;
    clang::TextDiagnosticPrinter diagClient;
    clang::Diagnostic diags;
    MyTargetOptions targOpts;
    clang::TargetInfo* target;
    clang::FileManager fm;
    clang::HeaderSearch hs;
    MyObjCLangOptions opts;
    clang::SourceManager sm;
    clang::Preprocessor pp;
    clang::ASTContext astctxt;
    bool verbose;
    clang::FieldDecl* dummyFD;
    std::map<std::string,size_t> dirs;
    std::map<clang::FileID,bool> inDirCache;

    friend class AnEnum;
    friend class AFunctionDecl;
#if 0
    friend class AFunctionLoc;
#endif
    friend class AFunctionType;
    friend class AnObjCMethodProtocolIter;
    friend class AnObjCMethod;
    friend class AStruct;
    friend class ATypedef;
    friend class AVar;
    friend class MyPass1Consumer;
    friend class MyPass2Consumer;
    friend class MySema;
    friend void MyParseAST(BridgeSupportParser *BSP, MyPass2Consumer *Consumer);
#endif /* SWIG */
};

class ABase {
public:
    virtual VALUE path() {return Path ? rb_str_new2(Path) : Qnil;}
protected:
    ABase(const char *path) : Path(path) {}
    ~ABase() {}

    const char *Path;
};

class TopLevel : public ABase {
protected:
    virtual VALUE info() = 0;
    TopLevel(BridgeSupportParser *bsp, const char *path) : ABase(path), BSP(bsp) {}
    ~TopLevel() {}

    BridgeSupportParser* BSP;
};

class AnEnum : public TopLevel {
public:
    virtual void each_value();
    virtual VALUE info();
protected:
    AnEnum(BridgeSupportParser *bsp, const char *path, const clang::EnumDecl* ed) : TopLevel(bsp, path), ED(ed) {}
    ~AnEnum() {}

    const clang::EnumDecl* ED;
    friend class MyPass1Consumer;
    friend class MyPass2Consumer;
};

class AFunction : public TopLevel {
protected:
    AFunction(BridgeSupportParser *bsp, const char *path) : TopLevel(bsp, path) {}
    ~AFunction() {}
};

class AFunctionDecl : public AFunction {
public:
    virtual void each_argument();
    virtual VALUE info();
protected:
    AFunctionDecl(BridgeSupportParser *bsp, const char *path, const clang::FunctionDecl* fd) : AFunction(bsp, path), FD(fd) {}
    ~AFunctionDecl() {}

    const clang::FunctionDecl* FD;
    friend class MyPass1Consumer;
    friend class MyPass2Consumer;
};

class AFunctionType : public AFunction {
public:
    virtual void each_argument();
    virtual VALUE info();
protected:
    AFunctionType(BridgeSupportParser *bsp, const char *path, const clang::FunctionType* ft) : AFunction(bsp, path), FT(ft) {}
    ~AFunctionType() {}

    const clang::FunctionType* FT;
    friend class AFunctionDecl;
    friend class AnObjCMethod;
    friend class AStruct;
    friend class AVar;
    friend void delete_AFunctionType(AFunctionType *);
};

class AMacroBase : public ABase {
protected:
    AMacroBase(const char *path, const char *name) : ABase(path), Name(name) {}
    ~AMacroBase() {}

    const char *Name;
};

class AMacroValueBase : public AMacroBase {
public:
    virtual VALUE info();
protected:
    AMacroValueBase(const char *path, const char *name, const char *value) : AMacroBase(path, name), Value(value) {}
    ~AMacroValueBase() {}

    const char *Value;
};

class AMacroFunctionAlias : public AMacroValueBase {
protected:
    AMacroFunctionAlias(const char *path, const char *name, const char *value) : AMacroValueBase(path, name, value) {}
    ~AMacroFunctionAlias() {}

    friend class MyPass1Consumer;
    friend class MyPass2Consumer;
};

class AMacroNumber : public AMacroValueBase {
protected:
    AMacroNumber(const char *path, const char *name, const char *value) : AMacroValueBase(path, name, value) {}
    ~AMacroNumber() {}

    friend class MyPass1Consumer;
    friend class MyPass2Consumer;
};

class AMacroNumberFuncCall : public AMacroBase {
public:
    virtual VALUE info();
protected:
    AMacroNumberFuncCall(const char *path, const char *name) : AMacroBase(path, name) {}
    ~AMacroNumberFuncCall() {}

    friend class MyPass1Consumer;
    friend class MyPass2Consumer;
};

class AMacroString : public AMacroValueBase {
public:
    virtual VALUE info();
protected:
    AMacroString(const char *path, const char *name, const char *value, bool objcstring) : AMacroValueBase(path, name, value), ObjCString(objcstring) {}
    ~AMacroString() {}

    bool ObjCString;
    friend class MyPass1Consumer;
    friend class MyPass2Consumer;
};

class AnObjCMethodProtocolIter : public TopLevel {
public:
    virtual void each_method();
    virtual void each_protocol();
protected:
    AnObjCMethodProtocolIter(BridgeSupportParser *bsp, const char *path) : TopLevel(bsp, path) {}
    ~AnObjCMethodProtocolIter() {}
#ifndef SWIG
    virtual clang::ObjCContainerDecl::method_iterator meth_begin() = 0;
    virtual clang::ObjCContainerDecl::method_iterator meth_end() = 0;
    virtual clang::ObjCInterfaceDecl::protocol_iterator protocol_begin() = 0;
    virtual clang::ObjCInterfaceDecl::protocol_iterator protocol_end() = 0;
#endif /* SWIG */
};

class AnObjCCategory : public AnObjCMethodProtocolIter {
public:
    virtual VALUE info();
protected:
    AnObjCCategory(BridgeSupportParser *bsp, const char *path, const clang::ObjCCategoryDecl* cd) : AnObjCMethodProtocolIter(bsp, path), CD(cd) {}
    ~AnObjCCategory() {}
#ifndef SWIG
    virtual clang::ObjCContainerDecl::method_iterator meth_begin() {return CD->meth_begin();}
    virtual clang::ObjCContainerDecl::method_iterator meth_end() {return CD->meth_end();}
    virtual clang::ObjCInterfaceDecl::protocol_iterator protocol_begin() {return CD->protocol_begin();}
    virtual clang::ObjCInterfaceDecl::protocol_iterator protocol_end() {return CD->protocol_end();}
#endif /* SWIG */

    const clang::ObjCCategoryDecl* CD;
    friend class MyPass1Consumer;
    friend class MyPass2Consumer;
};

class AnObjCInterface : public AnObjCMethodProtocolIter {
public:
    virtual VALUE info();
protected:
    AnObjCInterface(BridgeSupportParser *bsp, const char *path, const clang::ObjCInterfaceDecl* id) : AnObjCMethodProtocolIter(bsp, path), ID(id) {}
    ~AnObjCInterface() {}
#ifndef SWIG
    virtual clang::ObjCContainerDecl::method_iterator meth_begin() {return ID->meth_begin();}
    virtual clang::ObjCContainerDecl::method_iterator meth_end() {return ID->meth_end();}
    virtual clang::ObjCInterfaceDecl::protocol_iterator protocol_begin() {return ID->protocol_begin();}
    virtual clang::ObjCInterfaceDecl::protocol_iterator protocol_end() {return ID->protocol_end();}
#endif /* SWIG */

    const clang::ObjCInterfaceDecl* ID;
    friend class MyPass1Consumer;
    friend class MyPass2Consumer;
};

class AnObjCMethod : public TopLevel {
public:
    virtual void each_argument();
    virtual VALUE info();
protected:
    AnObjCMethod(BridgeSupportParser *bsp, const clang::ObjCMethodDecl* md) : TopLevel(bsp, NULL/*unused*/), MD(md) {}
    ~AnObjCMethod() {}

    const clang::ObjCMethodDecl* MD;
    friend class AnObjCMethodProtocolIter;
};

class AnObjCProtocol : public AnObjCMethodProtocolIter {
public:
    virtual VALUE info();
protected:
    AnObjCProtocol(BridgeSupportParser *bsp, const char *path, const clang::ObjCProtocolDecl* pd) : AnObjCMethodProtocolIter(bsp, path), PD(pd) {}
    ~AnObjCProtocol() {}
#ifndef SWIG
    virtual clang::ObjCContainerDecl::method_iterator meth_begin() {return PD->meth_begin();}
    virtual clang::ObjCContainerDecl::method_iterator meth_end() {return PD->meth_end();}
    virtual clang::ObjCInterfaceDecl::protocol_iterator protocol_begin() {return PD->protocol_begin();}
    virtual clang::ObjCInterfaceDecl::protocol_iterator protocol_end() {return PD->protocol_end();}
#endif /* SWIG */

    const clang::ObjCProtocolDecl* PD;
    friend class MyPass1Consumer;
    friend class MyPass2Consumer;
};

class AStruct : public TopLevel {
public:
    virtual void each_field();
    virtual VALUE info();
protected:
    AStruct(BridgeSupportParser *bsp, const char *path, const clang::RecordDecl* rd) : TopLevel(bsp, path), RD(rd) {}
    ~AStruct() {}

    const clang::RecordDecl* RD;
    friend class ATypedef;
    friend class MyPass1Consumer;
    friend class MyPass2Consumer;
};

class ATypedef : public TopLevel {
public:
    virtual VALUE encoding();	// only use on pointers or fully declared types
    virtual VALUE info();
    virtual void walk_types();
protected:
    ATypedef(BridgeSupportParser *bsp, const char *path, const clang::TypedefDecl* td) : TopLevel(bsp, path), TD(td) {}
    ~ATypedef() {}

    const clang::TypedefDecl* TD;
    friend class MyPass1Consumer;
    friend class MyPass2Consumer;
};

class AVar : public TopLevel {
public:
    virtual VALUE info();
protected:
    AVar(BridgeSupportParser *bsp, const char *path, const clang::VarDecl* vd) : TopLevel(bsp, path), VD(vd) {}
    ~AVar() {}

    const clang::VarDecl* VD;
    friend class MyPass1Consumer;
    friend class MyPass2Consumer;
};
