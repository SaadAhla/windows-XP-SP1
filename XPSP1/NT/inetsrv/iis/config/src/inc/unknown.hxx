#ifndef _UNKNOWN_HXX
#define _UNKNOWN_HXX

//===========================================================================
// This template implements the IUnknown portion of a given COM interface.

template <class I> class _unknown : public I
{
protected:    long _refcount;

public:        
        _unknown() 
        { 
            _refcount = 1; // BUGBUG(marcelv) is this actually correct? Shouldn't this be zero?
        }

        virtual ~_unknown()
        {
        }

        virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void ** ppvObject)
        {
            if (riid == IID_IUnknown)
            {
                *ppvObject = static_cast<IUnknown*>(this);
            }
            else if (riid == __uuidof(I))
            {
                *ppvObject = static_cast<I*>(this);
            }
            else
            {
                *ppvObject = NULL;
                return E_NOINTERFACE;
            }
            reinterpret_cast<IUnknown*>(*ppvObject)->AddRef();
            return S_OK;
        }
    
        virtual ULONG STDMETHODCALLTYPE AddRef( void)
        {
            return InterlockedIncrement(&_refcount);
        }
    
        virtual ULONG STDMETHODCALLTYPE Release( void)
        {
            if (InterlockedDecrement(&_refcount) == 0)
            {
                delete this;
                return 0;
            }
            return _refcount;
        }
};    

#endif _UNKNOWN_HXX


