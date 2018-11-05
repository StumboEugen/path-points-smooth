// -------------------------------------------------------------- -*- C++ -*-
// File: ./include/ilconcert/iloiterator.h
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

#ifndef __CONCERT_iloiteratorH
#define __CONCERT_iloiteratorH

#ifdef _WIN32
#pragma pack(push, 8)
#endif

#include <ilconcert/iloextractable.h>

class IloBaseIterator {
private:
	IloEnvI*                     _env;
	class IloTypedList*          _list;
	class IloTypedList*          _current;

	IloBool                      _withSubClasses;
public:
	IloBaseIterator(const IloEnv env, IloBool withSubClasses = IloTrue)
		: _env(env.getImpl())
		, _list(0)
		, _current(0)
		, _withSubClasses(withSubClasses)
	{}
	void goToNextNonEmptyList(IloLinkedList<IloExtractableI>::Iterator& _iter);
	void init(IloRtti::TypeInfo typeInfo, IloLinkedList<IloExtractableI>::Iterator &_iter);
};

template <class E>

class IloIterator : public IloBaseIterator {
private:
	IloExtractableList::Iterator _iter;

#if defined(ILO_HP_aCC) || defined(ILO_LINUX) || defined(ILO_APPLE)
	typedef typename E::ImplClass EImplClass;
#endif

	void init() {
#if defined(ILO_HP_aCC) || defined(ILO_LINUX) || defined(ILO_APPLE)
		IloBaseIterator::init(EImplClass::GetTypeInfo(), _iter);
#else
		IloBaseIterator::init(E::ImplClass::GetTypeInfo(), _iter);
#endif
	}

public:
	
	IloIterator(const IloEnv env, IloBool withSubClasses = IloTrue)
		: IloBaseIterator(env,withSubClasses),
		_iter(IloExtractableList())
	{
		init();
	}

	
	IloBool ok() { return _iter.ok(); }
	
	void operator++() {
		++_iter;
		if (!_iter.ok()) { IloBaseIterator::goToNextNonEmptyList(_iter); }
	}

	E operator*() {
#if defined(ILO_HP_aCC) || defined(ILO_LINUX) || defined(ILO_APPLE)
		EImplClass* ext = (EImplClass*)*_iter;
		IloAssert(ext->isType(EImplClass::GetTypeInfo()),
			"Bad extractable type in IloKindOfIterator::operator*");
		return (EImplClass*)ext;
#elif defined(ILO_RS6000)
		typename E::ImplClass *ext = (typename E::ImplClass*)*_iter;
		IloAssert(ext->isType(E::ImplClass::GetTypeInfo()),
			"Bad extractable type in IloKindOfIterator::operator*");
		return (typename E::ImplClass*)ext;
#else
		E::ImplClass* ext = (E::ImplClass*)*_iter;
		IloAssert(ext->isType(E::ImplClass::GetTypeInfo()),
			"Bad extractable type in IloKindOfIterator::operator*");
		return (E::ImplClass*)ext;
#endif
	}
};

#ifdef _WIN32
#pragma pack(pop)
#endif

#endif

