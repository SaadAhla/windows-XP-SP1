OPSYS=NT

TARGET=dcomsec.dll

NO_OPTIM=1

RELEASE=CORE\$(RELDIR)

DEFFILE=dcomsec.def

STATIC=TRUE

CDEFS+= /D_WIN32_DCOM /D_WIN32_WINNT=0x0402

CPPFILES=\
    iac.cpp \

LIBS=\
    $(CONLIBS) \
    .\win32sdk\"$(PROCESSOR_ARCHITECTURE)"\uuid.lib \
