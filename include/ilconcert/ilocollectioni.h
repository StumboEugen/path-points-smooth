// -------------------------------------------------------------- -*- C++ -*-
// File: ./include/ilconcert/ilocollectioni.h
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
#ifndef __CONCERT_ilocollectioniH
#define __CONCERT_ilocollectioniH

#ifdef _WIN32
#pragma pack(push, 8)
#endif

#include <ilconcert/ilocollection.h>
#include <ilconcert/ilosys.h>
#include <ilconcert/ilohash.h>
#include <ilconcert/iloany.h>
#include <ilconcert/ilolinkedlist.h>

class IloInt2IndexHashTable;
class IloNum2IndexHashTable;
class IloAny2IndexHashTable;

class IloObjectBase;

class IloDataCollectionI;
class IloContinuousDataI;
class IloNumRangeI;
class IloDiscreteDataCollectionI;
class IloIntCollectionI;
class IloIntAbstractSetI;
class IloIntSetI;
class IloIntRangeI;
class IloNumCollectionI;
class IloNumAbstractSetI;
class IloNumSetI;
class IloAnyCollectionI;
class IloAnyAbstractSetI;
class IloAnySetI;

class IloDataCollection;
class IloContinuousData;
class IloNumRange;
class IloDiscreteDataCollection;
class IloIntCollection;
class IloIntAbstractSet;
class IloIntSet;
class IloIntRange;
class IloNumCollection;
class IloNumAbstractSet;
class IloNumSet;
class IloAnyCollection;
class IloAnyAbstractSet;
class IloAnySet;

class IloDataIterator;


class IloNumVarDomainI;
class IloIntVarDomainI;



class IloDataException : public IloException {
public:
	IloDataException(const char * m) : IloException(m) { }
};

////////////////////////////////////////////////////////////////////////////
//
// IloData
//
////////////////////////////////////////////////////////////////////////////

class IloIntArray;
class IloNumArray;
class IloDataIterator;


class IloDataCollectionI : public IloOplBaseObjectI {
	ILORTTIDECL
		ILOEXTENSIONI
private:
	IloInt _lock;

public:
	virtual IloBool isSymbol() const { return IloFalse; }

	virtual void setLockable(IloBool flag){
		if( flag ) {
			_lock += 1;
		} else {
			_lock -= 1;
		}
		IloAssert( _lock >= 0, "lock cannot be negative" );
	}

	IloBool isLocked() {
		return _lock>0;
	}

	static void sortAsc(IloIntArray& array, IloInt min, IloInt max, IloIntArray* oldPosition = 0);
	static void sortAsc(IloIntArray& array);
	static void sortAsc(IloNumArray& array, IloInt min, IloInt max, IloIntArray* oldPosition = 0);
	static void sortAsc(IloNumArray& array);
	static void sortDesc(IloIntArray& array, IloInt min, IloInt max, IloIntArray* oldPosition = 0);
	static void sortDesc(IloIntArray& array);
	static void sortDesc(IloNumArray& array, IloInt min, IloInt max, IloIntArray* oldPosition = 0);
	static void sortDesc(IloNumArray& array);

public:

	IloDataCollectionI(IloEnv env);

	virtual ~IloDataCollectionI();
	virtual IloDataCollectionI* copy() const = 0;
	virtual IloDataCollection::IloDataType getDataType() const = 0;
	virtual IloBool isIntRange() const;
	virtual IloBool isNumRange() const;
	virtual IloBool isIntSet() const;
	virtual IloBool isNumSet() const;
	virtual IloBool isSymbolSet() const;
	virtual IloBool isAnySet() const;
	virtual IloBool isIntDataColumn() const;
	virtual IloBool isNumDataColumn() const;
	virtual IloBool isAnyDataColumn() const;
	virtual IloBool isTupleCollection() const;
	virtual IloBool isTupleSet() const;
	virtual IloBool isTupleRefColumn() const;
	virtual IloBool isIntCollectionColumn() const;
	virtual IloBool isNumCollectionColumn() const;
	virtual IloBool isAnyCollectionColumn() const;
	virtual IloBool isIntCollection() const{ return IloFalse; }
	virtual IloBool isNumCollection() const{ return IloFalse; }
	virtual IloBool isAnyCollection() const{ return IloFalse; }
	virtual IloBool isMapAsCollection() const { return IloFalse; }
	virtual void display(ILOSTD(ostream)& os) const=0;
	virtual IloDataCollectionI* makeClone(IloEnvI* env) const = 0;
	virtual IloRttiEnvObjectI* makeOplClone(IloEnvI* env) const{
		return (IloRttiEnvObjectI*)makeClone(env);
	}
};

//-------------------------------------------------------------------------

inline ILOSTD(ostream)& operator<<(ILOSTD(ostream)& out,IloDataCollectionI* dom) {
	if (dom){
		dom->display(out);
	}
	return (out);
}

class IloDiscreteDataCollectionI : public IloDataCollectionI {
	ILORTTIDECL
public:
	virtual void processBeforeFill() {}
	virtual void processAfterFill( IloBool  = IloFalse ){}
	virtual const IloIntArray getOldIndexPositions() { return 0; }
	virtual void endOldIndexes() {}
	virtual void clearAllCaches(){}
	virtual void empty(){}
	virtual IloBool hasReference() const { return IloFalse; }
	virtual IloInt getSize() const = 0;

	virtual IloObjectBase getMapItem(IloInt idx) const = 0;
	IloDiscreteDataCollectionI(IloEnv env): IloDataCollectionI(env) {}
	virtual IloDataIterator* iterator(IloGenAlloc* heap) const;
	virtual IloInt getMapItemIndex(IloObjectBase value) const;
	virtual IloInt getIndex(IloInt value) const;
	virtual IloInt getIndex(IloNum value) const;
	virtual IloInt getIndex(IloAny value) const;
	virtual IloInt getIndex(IloSymbol value) const;
	virtual IloInt getIntValue(IloInt idx) const;
	virtual IloNum getNumValue(IloInt idx) const;
	virtual IloAny getAnyValue(IloInt idx) const;
	IloSymbol getSymbolValue(IloInt idx) const;
	const char* getStringValue(IloInt idx) const;
	virtual IloBool supportDuplicates() const;
	virtual IloBool isSortedAsc() const{ return IloFalse; }
	virtual IloBool isSortedDesc() const{ return IloFalse; }
	virtual void sort(IloBool  = IloTrue){}
	virtual IloBool isOplRefCounted() const{ return IloTrue; }
	IloDataCollection::SortSense getSortSense() const;
	IloBool hasSortingProp() const{ return isSortedAsc() || isSortedDesc(); }
	IloBool isSortedSameSense( const IloDiscreteDataCollectionI& set ) const {
		return hasSortingProp() && getSortSense() == set.getSortSense();
	}
	IloBool isReversed() const{ return isSortedDesc(); }
	IloBool isSorted() const{ return isSortedAsc(); }
	IloBool isOrdered() const{ return !isSorted() && !isReversed(); }
};

class IloContinuousDataI : public IloDataCollectionI {
	ILORTTIDECL
public:
	IloContinuousDataI(IloEnv env): IloDataCollectionI(env) {}
};


class IloNumRangeI : public IloContinuousDataI {
	ILORTTIDECL
protected:
	IloNum _min;
	IloNum _max;
public:
	IloVarDomainI* getSharedDomain() const;
	IloNumRangeI(IloEnv env, IloNum min, IloNum max);
	IloNumRangeI(IloNumRangeI* obj);

	virtual ~IloNumRangeI();
	virtual IloDataCollectionI* copy() const;
	virtual IloDataCollectionI* makeClone(IloEnvI* env) const;
	virtual IloDataCollection::IloDataType getDataType() const;
	virtual IloBool isNumRange() const;
	void setUB(IloNum val){
		_max = val;
	}
	void setLB(IloNum val){
		_min = val;
	}
	virtual void display(ILOSTD(ostream)& os) const{
		os << "[";
		os << _min << " ," << _max;
		os << "]";
	}
	IloNum getLB() const {
		return _min;
	}
	IloNum getUB() const {
		return _max;
	}
	virtual IloBool isOplRefCounted() const{ return IloTrue; }
};


class IloIntCollectionI : public IloDiscreteDataCollectionI {
	ILORTTIDECL
public:
	IloIntCollectionI(IloEnv env): IloDiscreteDataCollectionI(env) {}
	virtual IloInt getIndex(IloSymbol s) const;
	virtual IloInt getIndex(IloInt value) const;
	virtual IloInt getIndex(IloNum value) const;
	virtual IloInt getIndex(IloAny value) const;
	virtual IloInt getIntValue(IloInt idx) const;
	virtual IloNum getNumValue(IloInt idx) const;
	virtual IloAny getAnyValue(IloInt idx) const;
	virtual IloIntArray getArray() const = 0;
	virtual IloBool contains(IloInt e) const=0;
	virtual IloInt getValue(IloInt index) const=0;
	virtual IloBool isIntCollection() const { return IloTrue; }
};

class IloIntAbstractSetI : public IloIntCollectionI {
	ILORTTIDECL
public:
	IloIntAbstractSetI(IloEnv env): IloIntCollectionI(env) {}
	virtual IloInt getNext(IloInt value, IloInt n=1) const = 0;
	virtual IloInt getPrevious(IloInt value, IloInt n=1) const = 0;
	virtual IloInt getNextC(IloInt value, IloInt n=1) const = 0;
	virtual IloInt getPreviousC(IloInt value, IloInt n=1) const = 0;
	virtual IloInt getFirst() const = 0;
	virtual IloInt getLast() const = 0;
	virtual IloBool supportDuplicates() const;
	virtual IloInt getLB() const;
	virtual IloInt getUB() const;
};


class IloIntAbstractDataColumnI : public IloIntCollectionI {
	ILORTTIDECL
public:
	IloIntAbstractDataColumnI(IloEnv env): IloIntCollectionI(env) {}
	virtual IloIntArray makeIndexArray(IloInt value) const = 0;
};


class IloNumCollectionI : public IloDiscreteDataCollectionI {
	ILORTTIDECL
public:
	IloNumCollectionI(IloEnv env): IloDiscreteDataCollectionI(env) {}
	virtual IloNumArray getArray() const = 0;
	virtual IloInt getIndex(IloSymbol s) const;
	virtual IloInt getIndex(IloInt value) const;
	virtual IloInt getIndex(IloNum value) const;
	virtual IloInt getIndex(IloAny value) const;
	virtual IloInt getIntValue(IloInt idx) const;
	virtual IloNum getNumValue(IloInt idx) const;
	virtual IloAny getAnyValue(IloInt idx) const;
	virtual IloBool contains(IloNum e) const=0;
	virtual IloNum getValue(IloInt index) const=0;
	virtual IloBool isNumCollection() const { return IloTrue; }
};


class IloNumAbstractSetI : public IloNumCollectionI {
	ILORTTIDECL
public:
	IloNumAbstractSetI(IloEnv env): IloNumCollectionI(env) {}

	IloInt getIndex(IloSymbol s) const;
	IloInt getIndex(IloAny s) const;
	IloInt getIndex(IloNum s) const;

	IloInt getIndex(IloInt value) const;

	virtual IloNum getNext(IloNum value, IloInt n=1) const = 0;
	virtual IloNum getPrevious(IloNum value, IloInt n=1) const = 0;
	virtual IloNum getNextC(IloNum value, IloInt n=1) const = 0;
	virtual IloNum getPreviousC(IloNum value, IloInt n=1) const = 0;
	virtual IloNum getFirst() const = 0;
	virtual IloNum getLast() const = 0;
	virtual IloBool supportDuplicates() const;
	virtual IloNum getLB() const;
	virtual IloNum getUB() const;
};

class IloNumAbstractDataColumnI : public IloNumCollectionI {
	ILORTTIDECL
public:
	IloNumAbstractDataColumnI(IloEnv env): IloNumCollectionI(env) {}
	virtual IloIntArray makeIndexArray(IloNum value) const = 0;
};

class IloAnyCollectionI : public IloDiscreteDataCollectionI {
	ILORTTIDECL
public:
	IloAnyCollectionI(IloEnv env): IloDiscreteDataCollectionI(env) {}
	virtual IloAnyArray getArray() const = 0;
	virtual IloInt getIndex(IloSymbol s) const;
	virtual IloInt getIndex(IloInt value) const;
	virtual IloInt getIndex(IloNum value) const;
	virtual IloInt getIndex(IloAny value) const;
	virtual IloInt getIntValue(IloInt idx) const;
	virtual IloNum getNumValue(IloInt idx) const;
	virtual IloAny getAnyValue(IloInt idx) const;
	virtual IloBool contains(IloAny e) const=0;
	virtual IloAny getValue(IloInt index) const=0;
	virtual IloBool isAnyCollection() const { return IloTrue; }
};

class IloAnyAbstractSetI : public IloAnyCollectionI {
	ILORTTIDECL
		IloDataCollection::IloDataType _type;
public:
	IloDataCollection::IloDataType getType() const { return _type;}
	IloAnyAbstractSetI(IloEnv env): IloAnyCollectionI(env), _type(IloDataCollection::AnySet) {}
	void setType(IloDataCollection::IloDataType type){ _type = type; }
	virtual IloAny getNext(IloAny value, IloInt n=1) const = 0;
	virtual IloAny getPrevious(IloAny value, IloInt n=1) const = 0;
	virtual IloAny getNextC(IloAny value, IloInt n=1) const = 0;
	virtual IloAny getPreviousC(IloAny value, IloInt n=1) const = 0;
	virtual IloAny getFirst() const = 0;
	virtual IloAny getLast() const = 0;
	virtual IloBool supportDuplicates() const;
};

class IloAnyAbstractDataColumnI : public IloAnyCollectionI {
	ILORTTIDECL
public:
	IloAnyAbstractDataColumnI(IloEnv env): IloAnyCollectionI(env) {}
	virtual IloIntArray makeIndexArray(IloAny value) const = 0;
};



class IloIntRangeI : public IloIntAbstractSetI {
	ILORTTIDECL
protected:
	IloInt _min;
	IloInt _max;
public:
	IloVarDomainI* getSharedDomain() const;
	virtual IloDataCollection::IloDataType getDataType() const;
	virtual IloDataCollectionI* copy() const;
	virtual IloDataCollectionI* makeClone(IloEnvI* env) const;
	virtual IloInt getNext(IloInt value, IloInt n=1) const;
	virtual IloInt getPrevious(IloInt value, IloInt n=1) const;
	virtual IloInt getNextC(IloInt value, IloInt n=1) const;
	virtual IloInt getPreviousC(IloInt value, IloInt n=1) const;
	virtual IloInt getFirst() const;
	virtual IloInt getLast() const;
	virtual IloInt getMapItemIndex(IloObjectBase value) const;
	IloInt getIndex(IloInt value) const;
	IloInt getIndex(IloAny value) const;
	IloInt getIndex(IloNum value) const;
	IloInt getIndex(IloSymbol value) const;
	IloInt getIntValue(IloInt idx) const;
	IloIntArray getArray() const;
	virtual IloDataIterator* iterator(IloGenAlloc* heap) const;
	IloIntRangeI(IloEnv env);
	IloIntRangeI(IloEnv env, IloInt min, IloInt max);
	IloIntRangeI(IloIntRangeI* obj);
	virtual ~IloIntRangeI();
	virtual IloBool isIntRange() const;
	virtual void display(ILOSTD(ostream)& os) const;
	virtual IloInt getSize() const;
	virtual IloObjectBase getMapItem(IloInt idx) const;
	virtual IloInt getLB() const {
		return _min;
	}
	virtual IloInt getUB() const {
		return _max;
	}
	virtual void setLB(IloInt min);
	virtual void setUB(IloInt max);

	virtual IloBool contains(IloInt e) const;
	virtual IloInt getValue(IloInt index) const;

	virtual IloBool isSortedAsc() const{ return IloTrue; }
};


class IloIntSetI : public IloIntAbstractSetI {
	ILORTTIDECL
protected:
	IloBool _hasIndex;
	IloIntArray _array;
	IloInt2IndexHashTable* _hashForGetIndex;
public:
	void createHashTable();
	IloInt2IndexHashTable* getOrMakeHashTable();
	virtual void removeIndexes();
	void updateIndexes();
	virtual void clearAllCaches();
protected:
	void removeFromArray(IloInt elt);

public:
	void addWithoutCheck(IloInt elt);

public:
	virtual IloDataCollectionI* copy() const;
	virtual IloDataCollectionI* makeClone(IloEnvI* env) const;

	virtual IloInt getNext(IloInt value, IloInt n=1) const;
	virtual IloInt getPrevious(IloInt value, IloInt n=1) const;
	virtual IloInt getNextC(IloInt value, IloInt n=1) const;
	virtual IloInt getPreviousC(IloInt value, IloInt n=1) const;
	virtual IloInt getFirst() const;
	virtual IloInt getLast() const;
	virtual IloInt getMapItemIndex(IloObjectBase value) const;
	IloInt getIndex(IloInt value) const;
	IloInt getIndex(IloSymbol value) const;
	IloInt getIndex(IloNum value) const;
	IloInt getIndex(IloAny value) const;
	IloInt getIntValue(IloInt idx) const;
	virtual IloDataIterator* iterator(IloGenAlloc* heap) const;
	IloIntSetI(IloEnvI* env, const IloIntArray array, IloBool withIndex = IloFalse);
	IloIntSetI(IloEnvI* env, IloBool withIndex = IloFalse);
	IloIntSetI(IloEnvI* env,  IloIntSetI* set);
	virtual ~IloIntSetI();
	IloBool hasIndex() const {
		return  _hasIndex;
	}
	IloIntArray getArray() const {return _array;}
	IloInt getSize() const;
	virtual IloObjectBase getMapItem(IloInt idx) const;
	virtual void add(IloInt elt);
	virtual void addWithoutSort(IloInt elt){ add(elt); }
	virtual void add(IloIntSetI* set);
	void remove(IloInt elt);
	void remove(IloIntSetI* set);
	void setIntersection(IloInt elt);
	virtual void setIntersection(IloIntSetI* set);
	virtual void empty();
	virtual IloBool contains(IloInt elt) const;
	virtual IloInt getValue(IloInt index) const;
	IloBool contains(IloIntSetI* set) const;
	virtual IloBool intersects(IloIntSetI* set) const;
	IloNumSetI * toNumSet() const;
	IloAnySetI * toAnySet() const;
	IloIntArray toArray() const { return _array.copy(); }
	IloDataCollection::IloDataType getDataType() const;
	IloBool isIntSet() const;
	void display(ILOSTD(ostream)& os) const;
	virtual IloIntSetI* makeDiff(IloIntSetI* set1, IloIntSetI* set2);
	void setHasIndex(IloBool flag) { _hasIndex = flag; }
};

class IloNumSetI : public IloNumAbstractSetI {
	ILORTTIDECL
protected:
	IloBool _hasIndex;
	IloNumArray _array;
	IloNum2IndexHashTable* _hashForGetIndex;
public:
	void createHashTable();
	IloNum2IndexHashTable* getOrMakeHashTable();

	virtual void removeIndexes();
	void updateIndexes();
	virtual void clearAllCaches();
protected:
	void removeFromArray(IloNum elt);

public:
	void addWithoutCheck(IloNum elt);

public:
	virtual IloDataCollectionI* copy() const;
	virtual IloDataCollectionI* makeClone(IloEnvI* env) const;
	virtual IloNum  getNext(IloNum value, IloInt n=1) const;
	virtual IloNum  getPrevious(IloNum value, IloInt n=1) const;
	virtual IloNum  getNextC(IloNum value, IloInt n=1) const;
	virtual IloNum  getPreviousC(IloNum value, IloInt n=1) const;
	virtual IloNum getFirst() const;
	virtual IloNum getLast() const;
	virtual IloInt getMapItemIndex(IloObjectBase value) const;
	IloInt getIndex(IloNum value) const;
	IloInt getIndex(IloInt value) const;
	IloInt getIndex(IloAny value) const;
	IloInt getIndex(IloSymbol value) const;
	IloNum getNumValue(IloInt idx) const;
	virtual IloDataIterator* iterator(IloGenAlloc* heap) const;
	IloNumSetI(IloEnvI* env, const IloNumArray array, IloBool withIndex = IloFalse);
	IloNumSetI(IloEnvI* env, IloBool withIndex = IloFalse);
	IloNumSetI(IloEnvI* env,  IloNumSetI* set);
	virtual ~IloNumSetI();
	IloBool hasIndex() const {
		return  _hasIndex;
	}
	IloNumArray getArray() const {return _array;}
	IloInt getSize() const;
	virtual IloObjectBase getMapItem(IloInt idx) const;
	virtual void add(IloNum elt);
	virtual void addWithoutSort(IloNum elt){ add(elt); }
	virtual void add(IloNumSetI* set);
	void add(IloIntSetI* set);
	void remove(IloNum elt);
	void remove(IloNumSetI* set);
	void setIntersection(IloNum elt);
	virtual void setIntersection(IloNumSetI* set);
	virtual void empty();
	virtual IloBool contains(IloNum elt) const;
	virtual IloNum getValue(IloInt index) const;
	virtual IloBool contains(IloNumSetI* set) const;
	virtual IloBool intersects(IloNumSetI* set) const;
	IloIntSetI * toIntSet() const;
	IloNumArray toArray() const { return _array.copy(); }
	IloDataCollection::IloDataType getDataType() const;
	IloBool isNumSet() const;
	void display(ILOSTD(ostream)& os) const;
	virtual IloNumSetI* makeDiff(IloNumSetI* set1, IloNumSetI* set2);
	void setHasIndex(IloBool flag) { _hasIndex = flag; }
};


class IloAnySetI : public IloAnyAbstractSetI {
	ILORTTIDECL
protected:
	IloBool _hasIndex;
	IloAnyArray _array;
	IloAny2IndexHashTable* _hashForGetIndex;
public:
	void createHashTable();
	IloAny2IndexHashTable* getOrMakeHashTable();
	virtual void removeIndexes();
	void updateIndexes();
	virtual void clearAllCaches();
protected:
	void removeFromArray(IloAny elt);
public:
	void addWithoutCheck(IloAny elt);
public:
	virtual IloDataCollectionI* copy() const;
	virtual IloDataCollectionI* makeClone(IloEnvI* env) const;
	virtual IloAny getNext(IloAny value, IloInt n=1) const;
	virtual IloAny getPrevious(IloAny value, IloInt n=1) const;
	virtual IloAny getNextC(IloAny value, IloInt n=1) const;
	virtual IloAny getPreviousC(IloAny value, IloInt n=1) const;
	virtual IloAny getFirst() const;
	virtual IloAny getLast() const;
	IloInt getIndex(IloNum value) const;
	virtual IloInt getMapItemIndex(IloObjectBase value) const;
	IloInt getIndex(IloAny value) const;
	IloInt getIndex(IloSymbol value) const;
	IloInt getIndex(IloInt value) const;
	IloAny getAnyValue(IloInt idx) const;
	virtual IloDataIterator* iterator(IloGenAlloc* heap) const;
	IloAnySetI(IloEnvI* env, const IloAnyArray array, IloBool withIndex = IloFalse);
	IloAnySetI(IloEnvI* env, IloBool withIndex = IloFalse);
	IloAnySetI(IloEnvI* env,  IloAnySetI* set);
	virtual ~IloAnySetI();
	IloBool hasIndex() const {
		return  _hasIndex;
	}
	IloAnyArray getArray() const {return _array;}
	IloInt getSize() const;
	virtual IloObjectBase getMapItem(IloInt idx) const;
	virtual void add(IloAny elt);
	virtual void add(IloAnySetI* set);
	void remove(IloAny elt);
	void remove(IloAnySetI* set);
	void setIntersection(IloAny elt);
	void setIntersection(IloAnySetI* set);
	virtual void empty();
	virtual IloBool contains(IloAny elt) const;
	virtual IloAny getValue(IloInt index) const;
	IloBool contains(IloAnySetI* set) const;
	IloBool intersects(IloAnySetI* set) const;
	IloNumSetI * toNumSet() const;
	IloIntSetI * toIntSet() const;
	IloAnyArray toArray() const { return _array.copy(); }
	IloDataCollection::IloDataType getDataType() const;
	IloBool isAnySet() const;
	IloBool isSymbolSet() const;
	virtual void display(ILOSTD(ostream)& os) const;
	virtual IloAnySetI* makeDiff(IloAnySetI* set1, IloAnySetI* set2);
	virtual void addWithoutSort(IloAny elt){ add(elt); }
	void setHasIndex(IloBool flag) { _hasIndex = flag; }
};


class IloIntArrayAsCollectionI : public IloIntCollectionI {
	ILORTTIDECL
private:
	IloIntArray _array;
public:
	virtual IloDataCollectionI* copy() const;
	virtual IloDataCollectionI* makeClone(IloEnvI* env) const;
	IloIntArrayAsCollectionI(IloEnvI* env, const IloIntArray array);
	virtual ~IloIntArrayAsCollectionI();
	virtual IloDataCollection::IloDataType getDataType() const;
	virtual IloInt getSize() const;
	virtual IloObjectBase getMapItem(IloInt idx) const;
	virtual IloIntArray getArray() const;
	virtual IloBool contains(IloInt e) const;
	virtual IloInt getIndex(IloNum value) const;
	virtual IloInt getIndex(IloAny value) const;
	virtual IloInt getIndex(IloSymbol value) const;
	virtual IloInt getValue(IloInt index) const;
	virtual IloInt getIndex(IloInt value) const;
	virtual IloInt getIntValue(IloInt idx) const;
	virtual IloDataIterator* iterator(IloGenAlloc* heap) const;
	virtual void display(ILOSTD(ostream)& os) const;
};

class IloNumArrayAsCollectionI : public IloNumCollectionI {
	ILORTTIDECL
private:
	IloNumArray _array;
public:
	virtual IloDataCollectionI* copy() const;
	virtual IloDataCollectionI* makeClone(IloEnvI* env) const;
	IloNumArrayAsCollectionI(IloEnvI* env, const IloNumArray array);
	virtual ~IloNumArrayAsCollectionI();
	virtual IloDataCollection::IloDataType getDataType() const;
	virtual IloInt getSize() const;
	virtual IloObjectBase getMapItem(IloInt idx) const;
	virtual IloNumArray getArray() const;
	virtual IloBool contains(IloNum e) const;
	virtual IloNum getValue(IloInt index) const;
	virtual IloInt getIndex(IloInt value) const;
	virtual IloInt getIndex(IloSymbol value) const;
	virtual IloInt getIndex(IloAny value) const;
	virtual IloInt getIndex(IloNum value) const;
	virtual IloNum getNumValue(IloInt idx) const;
	virtual IloDataIterator* iterator(IloGenAlloc* heap) const;
	virtual void display(ILOSTD(ostream)& os) const;
};

class IloAnyArrayAsCollectionI : public IloAnyCollectionI {
	ILORTTIDECL
private:
	IloAnyArray _array;
public:
	virtual IloDataCollectionI* copy() const;
	virtual IloDataCollectionI* makeClone(IloEnvI* env) const;
	IloAnyArrayAsCollectionI(IloEnvI* env, const IloAnyArray array);
	virtual ~IloAnyArrayAsCollectionI();
	virtual IloDataCollection::IloDataType getDataType() const;
	virtual IloInt getSize() const;
	virtual IloObjectBase getMapItem(IloInt idx) const;
	virtual IloAnyArray getArray() const;
	virtual IloBool contains(IloAny e) const;
	virtual IloAny getValue(IloInt index) const;
	virtual IloInt getIndex(IloInt value) const;
	virtual IloInt getIndex(IloNum value) const;
	virtual IloInt getIndex(IloSymbol value) const;
	virtual IloInt getIndex(IloAny value) const;
	virtual IloAny getAnyValue(IloInt idx) const;
	virtual IloDataIterator* iterator(IloGenAlloc* heap) const;
	virtual void display(ILOSTD(ostream)& os) const;
};


class IloDummyIntDataIterator : public IloIntDataIterator {
public:
	IloDummyIntDataIterator(IloGenAlloc* heap);
	virtual ~IloDummyIntDataIterator();
	virtual IloBool next();
};
class IloDummyNumDataIterator : public IloNumDataIterator {
public:
	IloDummyNumDataIterator(IloGenAlloc* heap);
	virtual ~IloDummyNumDataIterator();
	virtual IloBool next();
};

class IloDummyAnyDataIterator : public IloAnyDataIterator {
public:
	IloDummyAnyDataIterator(IloGenAlloc* heap);
	virtual ~IloDummyAnyDataIterator();
	virtual IloBool next();
};

class IloDataChangeListenerI : public IloRttiEnvObjectI {
	ILORTTIDECL
		friend class IloDataChangeNotifierI;
protected :
	virtual void change() const = 0;
public:
	IloDataChangeListenerI(IloEnvI* env) : IloRttiEnvObjectI(env) {}
	virtual ~IloDataChangeListenerI() {};
};

class IloDataChangeNotifierI : public IloRttiEnvObjectI {
	ILORTTIDECL
protected:
	class Listener {
	public:
		Listener(IloDataChangeListenerI* listener, Listener* next) : _listener(listener), _next(next) {}
		IloDataChangeListenerI* _listener;
		Listener* _next;
	};
	Listener* _listeners;
protected:
	virtual void change() const;
public:
	IloDataChangeNotifierI(IloEnvI*);
	void registerListener(IloDataChangeListenerI*);
	void unregisterListener(IloDataChangeListenerI*);
};


class IloSortSetPropertyI : public IloNamedPropertyI {
	ILORTTIDECL
		static const char* SortSet;

public:
	IloDataCollection::SortSense _sense;

	IloSortSetPropertyI(IloMemoryManager env, IloDataCollection::SortSense sense)
		: IloNamedPropertyI(env), _sense(sense){};
	~IloSortSetPropertyI(){}
	IloBool isAsc() const{
		return _sense == IloDataCollection::ASC;
	}
	IloBool isDesc() const{
		return _sense == IloDataCollection::DESC;
	}
	IloNamedPropertyI * makeClone(IloMemoryManager) const{
		throw IloWrongUsage("can not clone this property");
		ILOUNREACHABLE(return 0;)
	}
	const char * getName() const { return IloSortSetPropertyI::SortSet; }
};


#ifdef _WIN32
#pragma pack(pop)
#endif

#endif


