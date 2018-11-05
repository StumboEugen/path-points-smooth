// -------------------------------------------------------------- -*- C++ -*-
// File: ./include/ilconcert/iloanyexpri.h
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

#ifndef __CONCERT_iloanyexpriH
#define __CONCERT_iloanyexpriH

#ifdef _WIN32
#pragma pack(push, 8)
#endif

#include <ilconcert/iloextractable.h>
#include <ilconcert/iloexpression.h>
#include <ilconcert/iloexpressioni.h>

class IloAnySetI;


//---------------------------------------------------------------------------
// IloAnyExpr
//---------------------------------------------------------------------------
class IloAnyExprI : public IloExtractableI {
	ILOEXTRDECL
protected:
	IloInt _exprUsed;
public:
	IloAnyExprI(IloEnvI* env, const char* name=0)
		:IloExtractableI(env, name), _exprUsed(0) {}
	virtual ~IloAnyExprI();
	
	IloAnyExprI* lockExpr() { _exprUsed++;; return this; }
	
	IloAnyExprI* unlockExpr() {_exprUsed--; return this; }
	
	virtual void end();
	
	virtual void release();
	

	virtual IloInt getExprUsed() const { return _exprUsed; }

	virtual IloBool isRefCountedExpr() const;

	virtual IloBool isGround() const{ return IloFalse; }
	virtual IloBool isDecisionExpr() const{ return IloFalse; }
	virtual IloBool isAnyExpr() {
		return IloTrue;
	}
};


//---------------------------------------------------------------------------
class IloAnyEqI : public IloConstraintI {
	ILOEXTRDECL
private:
	IloAnyExprI* _expr1;
	IloAnyExprI* _expr2;
public:
	IloAnyEqI(IloEnvI* env, IloAnyExprI* expr1, IloAnyExprI* expr2, const char* name=0);
	virtual ~IloAnyEqI();
	virtual void visitSubExtractables(IloExtractableVisitor* v);
	IloAnyExprI* getExpr1() const { return _expr1; } 
	IloAnyExprI* getExpr2() const { return _expr2; }
	IloNum eval(const IloAlgorithm alg) const;
	ILOEXTROTHERDECL
};

class IloAnyNeqI : public IloConstraintI {
	ILOEXTRDECL
private:
	IloAnyExprI* _expr1;
	IloAnyExprI* _expr2;
public:
	IloAnyNeqI(IloEnvI* env, IloAnyExprI* expr1, IloAnyExprI* expr2, const char* name=0);
	virtual ~IloAnyNeqI();
	virtual void visitSubExtractables(IloExtractableVisitor* v);
	IloAnyExprI* getExpr1() const { return _expr1; } 
	IloAnyExprI* getExpr2() const { return _expr2; }
	IloNum eval(const IloAlgorithm alg) const;
	ILOEXTROTHERDECL
};

class IloAnyEqCstI : public IloConstraintI {
	ILOEXTRDECL
private:
	IloAnyExprI* _expr;
	IloAny _value;
public:
	IloAnyEqCstI(IloEnvI* env, IloAnyExprI* expr, IloAny value, const char* name=0);
	virtual ~IloAnyEqCstI();
	virtual void visitSubExtractables(IloExtractableVisitor* v);
	IloAnyExprI* getExpr() const { return _expr; } 
	IloAny getConstant() const { return _value; }
	IloNum eval(const IloAlgorithm alg) const;
	ILOEXTROTHERDECL
};

class IloAnyNeqCstI : public IloConstraintI {
	ILOEXTRDECL
private:
	IloAnyExprI* _expr;
	IloAny _value;
public:
	IloAnyNeqCstI(IloEnvI* env, IloAnyExprI* expr, IloAny value, const char* name=0);
	virtual ~IloAnyNeqCstI();
	virtual void visitSubExtractables(IloExtractableVisitor* v);
	IloAnyExprI* getExpr() const { return _expr; } 
	IloAny getConstant() const { return _value; }
	IloNum eval(const IloAlgorithm alg) const;
	ILOEXTROTHERDECL
};


#ifdef _WIN32
#pragma pack(pop)
#endif

#endif
