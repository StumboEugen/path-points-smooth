// -------------------------------------------------------------- -*- C++ -*-
// File: ./include/ilconcert/iloparami.h
// --------------------------------------------------------------------------
// Licensed Materials - Property of IBM
//
// 5725-A06 5725-A29 5724-Y47 5724-Y48 5724-Y49 5724-Y54 5724-Y55 5655-Y21
// Copyright IBM Corp. 2000, 2013
//
// US Government Users Restricted Rights - Use, duplication or
// disclosure restricted by GSA ADP Schedule Contract with
// IBM Corp.
// ---------------------------------------------------------------------------

#ifndef __CONCERT_IloParamIH
#define __CONCERT_IloParamIH

#ifdef _WIN32
#pragma pack(push, 8)
#endif

#include <ilconcert/ilobasic.h>
#include <ilconcert/iloextractable.h>

// ---------------------- INTERNAL DOCUMENTATION ------------------------------
// -- SEE: iloparami.cpp
// ----------------------------------------------------------------------------

class IloParameterI;
class IloExtractableI;
class IloParameterManagerI;
class IloParameterExtractedObjectI;
class IloParameterNodeI;


// ----------------------------------------------------------------------------
// class IloParameterExtractableLinkI 
// ----------------------------------------------------------------------------

// LINK BETWEEN A PARAMETER AND AN EXTRACTABLE

class IloParameterExtractableLinkI {
	friend class IloParameterI;

private:
	IloParameterExtractableLinkI* _prev;
	IloParameterExtractableLinkI* _next;
	IloParameterI*                _param;
	IloExtractableI*  _extr;
	IloAny                        _port;

protected:
	IloParameterExtractableLinkI(IloExtractableI* extr,
		IloParameterI* param,
		IloAny port)
		: _prev  (0)
		, _next  (0)
		, _param (param)
		, _extr  (extr)
		, _port  (port) {}
	~IloParameterExtractableLinkI() {}

	void setPrev(IloParameterExtractableLinkI* prev) { _prev = prev; }
	void setNext(IloParameterExtractableLinkI* next) { _next = next; }

public:
	IloParameterI* getParameter() const { return _param; } 
	IloParameterExtractableLinkI* getPrev() const { return _prev; }
	IloParameterExtractableLinkI* getNext() const { return _next; }
	IloExtractableI* getExtractable() const { return _extr; }
	IloAny getPort() const { return _port; }

	void removeAndDestroyOrphan();
	IloParameterI* remove();
	inline IloAny getOwner() const;
	inline void setOwner(IloAny owner) const;
	inline IloBool isUserCreated() const;
};

// Backward compatibility:
#define IloParametrizedExtractableI IloExtractableI

// ----------------------------------------------------------------------------
// class IloParameterI 
// ----------------------------------------------------------------------------

class IloParameterI : public 
#ifdef ILO_SEGF_GENERIC_ALLOC
	IloExtensibleRttiMemoryManagerObjectI
#else
	IloExtensibleRttiEnvObjectI 
#endif
{
	ILORTTIDECL
		friend class IloParameterManagerI;
	friend class IloParameterExtractableLinkI;
	friend class IloParametrizedExtractableIterator;

#ifdef ILODELETEOPERATOR
	void operator delete(void *, const IloEnvI *);
	void operator delete(void *, const IloEnv&);
#endif
	void operator delete(void * obj, size_t size);

private:
	IloParameterExtractableLinkI* _links;
	IloParameterExtractedObjectI* _objects;
	IloAny                        _owner;     // =0 means "Created by User"

protected:
	// Removes an extractable from the list of extractable parametrized
	// by the parameter. Argument 'link' is freed and invalid afterward.
	void unlinkExtractable(IloParameterExtractableLinkI* link);


	// Calls the virtual function changeParameter(this) on each
	// parametrized extractable. This function must be called after each
	// modification of the parameter
	void notifyChange();

public:

	// GENERIC PARAMETER EXCEPTION
	class Exception : public IloException {
	public:
		Exception() : IloException("Generic parameter exception") {}
		Exception(const char* msg) : IloException(msg) {}
		virtual ~Exception() {}
	};

	IloParameterExtractableLinkI* getLinks() const {
		return _links; }

	// Cursor on parametrized extractables
	class ParameterExtractableLinkIterator {
	protected:
		IloParameterExtractableLinkI* _cursor;
	public:
		ParameterExtractableLinkIterator(IloParameterI* parameter)
			: _cursor(parameter->getLinks()) {} 
		//
		IloBool ok() const { 
			return _cursor != 0; 
		}
		IloParameterExtractableLinkI* operator*() { 
			return _cursor; 
		}
		IloExtractableI* getExtractable() { 
			return _cursor->getExtractable(); 
		}
		IloAny getPort() const { 
			return _cursor->getPort(); 
		}
		ParameterExtractableLinkIterator& operator++() {
			_cursor = _cursor->getNext();
			return (*this);
		}
	};

	IloParameterI(IloEnvI* env, const char* name = 0, IloAny owner = 0);
#ifdef ILO_SEGF_GENERIC_ALLOC
	IloParameterI(IloMemoryManagerI* env, const char* name = 0, IloAny owner = 0);
#endif  
	// 1. Calls the virtual function endParameter(this) on each
	// parametrized extractable
	// 2. Recycle the nodes and objects
	virtual ~IloParameterI();

	virtual IloParameterI* makeCopy() const = 0;

	virtual void display(ILOSTD(ostream)&) const;

	// Get the extracted object associated with the parameter, if any.
	// Otherwise, returns 0
	IloAny getExtractedObject(const IloParameterManagerI* mng) const;

	// Associate an extracted object from the parameter, identified
	// by a IloParameterManagerI pointer. Must be called only once.
	void setExtractedObject(IloParameterManagerI* mng, IloAny obj);

	// Called by the virtual function IloCPExtractorI::clear which is
	// called when a identifier manager is ended or when it is cleared in order
	// to re-extract. This function clear the links with objects
	// extracted in the algo.
	void unextract(IloParameterManagerI* mng);

	virtual void unsetExtractedObject(IloParameterManagerI*, IloAny) {}

	// LINKS WITH PARAMETRIZED EXTRACTABLES

	// Declare an extractable extr to be parametrized by the parameter;
	// returns the container that stores this extractable (useful for
	// quickly removing it later)
	IloParameterExtractableLinkI* 
		linkExtractable(IloExtractableI* extr, IloAny port =(IloAny)0);


	// Returns IloTrue if the parameter has a single parameterized
	// extractable
	IloBool isLocal() const { return !isUserCreated() && ((0 != _links) && (0 == _links->getNext())); };

	IloBool isOrphan() const { return (0 == _links) && !isUserCreated(); }

	IloAny getOwner() const { return _owner; }
	void setOwner(IloAny owner) { _owner = owner; }
	IloBool isUserCreated() const { return (getOwner()==0); }

	// useful queries
	IloInt getNbReference() const;
	IloBool isParameterOf(const IloExtractableI* extr, IloAny port) const;
};



inline IloAny IloParameterExtractableLinkI::getOwner() const {
	assert(_param!=0);
	return _param->getOwner();
}
inline void IloParameterExtractableLinkI::setOwner(IloAny owner) const {
	assert(_param!=0);
	_param->setOwner( owner );
}
inline IloBool IloParameterExtractableLinkI::isUserCreated() const { 
	assert(_param!=0);
	return _param->isUserCreated();
}


// ----------------------------------------------------------------------------
// class IloParameterManagerI 
// ----------------------------------------------------------------------------

// AN INSTANCE OF THIS CLASS WILL BE STORED IN THE SCHEDULER AND/OR
// DISPATCHER ENV AND PASSED TO THE MEMBER FUNCTIONS OF IloParameterI
class IloParameterManagerI
{
private:
	IloEnv             _env;
	IloParameterNodeI* _params;

public:
	IloParameterManagerI(IloEnv env);
	~IloParameterManagerI();
	void clear();

	// Add an extracted parameter to the managed list of parameter.
	// setExtractedObject() the 
	// computation object (if any) associated with an identifying manager.
	void manage(IloParameterI*, IloAny obj);

	// remove a parameter from the list of extracted (managed) objects
	void unManage(IloParameterI*);

	// Get the extracted object associated with the parameter and the
	// manager, if any. Otherwise, returns 0
	IloAny getExtractedObject(const IloParameterI*) const;
};

// ----------------------------------------------------------------------------

#ifdef _WIN32
#pragma pack(pop)
#endif

#endif  
