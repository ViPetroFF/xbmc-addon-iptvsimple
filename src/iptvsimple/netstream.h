/*
 *      Copyright (C) 2013 Viktor PetroFF
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with XBMC; see the file COPYING.  If not, write to
 *  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 *  http://www.gnu.org/copyleft/gpl.html
 *
 */
#pragma once

#include "client.h"

#ifndef IN_MULTICAST
#define IN_MULTICAST(a) ((((uint32_t)(a)) & 0xf0000000) == 0xe0000000)
#endif
#ifndef IN6_IS_ADDR_MULTICAST
#define IN6_IS_ADDR_MULTICAST(a) (((uint8_t *) (a))[0] == 0xff)
#endif

namespace LibNetStream
{

//typedef long HRESULT;
//static const long E_NOTIMPL = 0x80004001L;

class ISequentialStream
{
public:
	virtual ULONG AddRef( void) = 0;
	virtual ULONG Release( void) = 0;

    virtual HRESULT Read(void *pv, ULONG cb, ULONG *pcbRead) = 0;
    virtual HRESULT Write(const void *pv, ULONG cb, ULONG *pcbWritten) = 0;
        
};

class IStream : public ISequentialStream
{
public:
    virtual HRESULT Seek( LARGE_INTEGER dlibMove, DWORD dwOrigin, ULARGE_INTEGER *plibNewPosition) = 0;
    virtual HRESULT SetSize(ULARGE_INTEGER libNewSize) = 0;
    virtual HRESULT CopyTo( 
        IStream *pstm,
        ULARGE_INTEGER cb,
        ULARGE_INTEGER *pcbRead,
        ULARGE_INTEGER *pcbWritten) = 0;
    virtual HRESULT Commit(DWORD grfCommitFlags) = 0;
    virtual HRESULT Revert( void) = 0;
    virtual HRESULT LockRegion( 
        ULARGE_INTEGER libOffset,
        ULARGE_INTEGER cb,
        DWORD dwLockType) = 0;
    virtual HRESULT UnlockRegion( 
        ULARGE_INTEGER libOffset,
        ULARGE_INTEGER cb,
        DWORD dwLockType) = 0;
        
    virtual HRESULT Clone(IStream **ppstm) = 0;
};

class INetStreamFactory
{
public:
	static IStream* NewStream(const std::string& strUrl);
	static void SetMCastIf(ULONG ulMCastIf);
};

class ComImplStream : public IStream
{
protected:
	ComImplStream()
	{
        _refcount = 1;
	}

	virtual ~ComImplStream() {}

public:

    virtual ULONG AddRef(void) 
    { 
        return (ULONG)InterlockedIncrement(&_refcount);
		//return ++_refcount;
    }

    virtual ULONG Release(void) 
    {
        ULONG res = (ULONG) InterlockedDecrement(&_refcount);
		//ULONG res = _refcount--;
        if (res == 0) 
            delete this;
        return res;
    }

    // ISequentialStream Interface
public:
    virtual HRESULT Read(void* pv, ULONG cb, ULONG* pcbRead)
    {
        return E_NOTIMPL;
	}

    virtual HRESULT Write(void const* pv, ULONG cb, ULONG* pcbWritten)
    {
        return E_NOTIMPL;
	}

    // IStream Interface
public:
    virtual HRESULT SetSize(ULARGE_INTEGER)
    { 
        return E_NOTIMPL;   
    }
    
    virtual HRESULT CopyTo(IStream*, ULARGE_INTEGER, ULARGE_INTEGER*,
        ULARGE_INTEGER*) 
    { 
        return E_NOTIMPL;   
    }
    
    virtual HRESULT Commit(DWORD)                                      
    { 
        return E_NOTIMPL;   
    }
    
    virtual HRESULT Revert(void)                                       
    { 
        return E_NOTIMPL;   
    }
    
    virtual HRESULT LockRegion(ULARGE_INTEGER, ULARGE_INTEGER, DWORD)              
    { 
        return E_NOTIMPL;   
    }
    
    virtual HRESULT UnlockRegion(ULARGE_INTEGER, ULARGE_INTEGER, DWORD)            
    { 
        return E_NOTIMPL;   
    }
    
    virtual HRESULT Clone(IStream **)                                  
    { 
        return E_NOTIMPL;   
    }

    virtual HRESULT Seek(LARGE_INTEGER liDistanceToMove, DWORD dwOrigin,
        ULARGE_INTEGER* lpNewFilePointer)
    { 
        return E_NOTIMPL;
	}

    virtual HRESULT Stat(STATSTG* pStatstg, DWORD grfStatFlag) 
    {
        return E_NOTIMPL;
	}

protected:

private:
	LONG _refcount;
};

} //namespace LibHttpStream