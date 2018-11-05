// -------------------------------------------------------------- -*- C++ -*-
// File: ./include/ilconcert/ilodomaini.h
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

#ifndef __CONCERT_ilodomainiH
#define __CONCERT_ilodomainiH

#ifdef _WIN32
#pragma pack(push, 8)
#endif

#include <ilconcert/iloexpression.h>

class IloVarDomainI {
public:
	IloVarDomainI() {}
	virtual ~IloVarDomainI() {}
	virtual IloNum getLB() const =0;
	virtual void setLB(IloNum LB)=0;
	virtual IloNum getUB() const =0;
	virtual void setUB(IloNum UB)=0;
	virtual IloNumVar::Type getType() const =0;
	virtual IloInt getMin() const =0;
	virtual IloInt getMax() const =0;
	virtual IloBool isEnumerated() const =0;
	virtual IloBool isAny() const = 0;
	virtual IloNumArray getNumValues() const =0;
	virtual IloIntArray getIntValues() const =0;
	virtual void setNumValues(IloNumArray ar) =0;
	virtual void setIntValues(IloIntArray ar) =0;
	virtual void suicide(IloEnvI*, void* ptr) =0;
	virtual void display(ILOSTD(ostream)& out) = 0;
	virtual IloVarDomainI* makeClone(IloEnvI* env) = 0;
	virtual IloBool isShared() const{ return IloFalse; }
	virtual IloBool isIntDomain() const{ return IloFalse; }
	virtual IloBool isNumDomain() const{ return IloFalse; }
	virtual IloBool isBoolDomain() const{ return IloFalse; }
};



class IloNumVarDomainI : public IloVarDomainI {
	IloNum _lb;
	IloNum _ub;
protected:
	IloNumArray _array;
public:
	virtual ~IloNumVarDomainI(){}
	IloNumVarDomainI(const IloNumRangeI* coll);
	IloNumVarDomainI(IloNum lb, IloNum ub);
	IloNumVarDomainI(IloNumArray ar);
	virtual IloNum getLB() const;
	virtual void setLB(IloNum LB);
	virtual IloNum getUB() const;
	virtual void setUB(IloNum UB);
	virtual IloNumVar::Type getType() const;
	virtual IloInt getMin() const;
	virtual IloInt getMax() const;
	virtual IloBool isEnumerated() const;
	virtual IloBool isAny() const;
	virtual IloNumArray getNumValues() const;
	virtual IloIntArray getIntValues() const;
	virtual void setNumValues(IloNumArray ar);
	virtual void setIntValues(IloIntArray);
	virtual void suicide(IloEnvI* env, void* ptr);
	virtual void display(ILOSTD(ostream)& out);
	virtual IloVarDomainI* makeClone(IloEnvI* env);
	virtual IloBool isNumDomain() const{ return IloTrue; }
};


class IloIntVarDomainI : public IloVarDomainI {
	IloInt      _lb;
	IloInt      _ub;
protected:
	IloIntArray _array;
private:
	IloBool     _any;
public:
	virtual ~IloIntVarDomainI(){}
	IloIntVarDomainI(const IloIntRangeI* coll);
	IloIntVarDomainI(IloInt lb, IloInt ub, IloBool any = IloFalse);
	IloIntVarDomainI(IloIntArray ar, IloBool any = IloFalse);
	void cleanArray(IloInt lb, IloInt ub);
	virtual IloNum getLB() const;
	virtual void setLB(IloNum LB);
	virtual IloNum getUB() const;
	virtual void setUB(IloNum UB);
	virtual IloNumVar::Type getType() const;
	virtual IloInt getMin() const;
	virtual IloInt getMax() const;
	virtual IloBool isEnumerated() const;
	virtual IloBool isAny() const;
	virtual IloNumArray getNumValues() const;
	virtual IloIntArray getIntValues() const;
	virtual void setNumValues(IloNumArray);
	virtual void setIntValues(IloIntArray ar);
	virtual void suicide(IloEnvI* env, void* ptr);
	virtual void display(ILOSTD(ostream)& out);
	virtual IloVarDomainI* makeClone(IloEnvI* env);
	virtual IloBool isIntDomain() const{ return IloTrue; }
	virtual IloBool isBoolDomain() const;
};
//
class IloBoolVarDomainI : public IloVarDomainI {
	IloInt      _lb;
	IloInt      _ub;
public:
	virtual ~IloBoolVarDomainI(){}
	IloBoolVarDomainI(IloInt lb=0, IloInt ub=1);
	virtual IloNum getLB() const;
	virtual void setLB(IloNum LB);
	virtual IloNum getUB() const;
	virtual void setUB(IloNum UB);
	virtual IloNumVar::Type getType() const;
	virtual IloInt getMin() const;
	virtual IloInt getMax() const;
	virtual IloBool isEnumerated() const;
	virtual IloBool isAny() const;
	virtual IloNumArray getNumValues() const;
	virtual IloIntArray getIntValues() const;
	virtual void setNumValues(IloNumArray);
	virtual void setIntValues(IloIntArray);
	virtual void suicide(IloEnvI* env, void* ptr);
	virtual void display(ILOSTD(ostream)& out);
	virtual IloVarDomainI* makeClone(IloEnvI* env);
	virtual IloBool isBoolDomain() const{ return IloTrue; }
};

#ifdef _WIN32
#pragma pack(pop)
#endif

#endif
