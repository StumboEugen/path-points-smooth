// -------------------------------------------------------------- -*- C++ -*-
// File: ./include/ilconcert/ilothread.h
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

#ifndef __CONCERT_ilothreadH
#define __CONCERT_ilothreadH

#ifdef _WIN32
#pragma pack(push, 8)
#endif

#include <ilconcert/ilosys.h>

#if defined(ILO_MSVC)
#include <windows.h>
#endif

class IloThread;
class IloAgent;
class IloFastMutex;
class IloSemaphore;
class IloCondition;
class IloBarrier;

class IloAgentError : public IloException {
public:
	IloAgentError(const char* msg): IloException(msg) {}
};

class IloAgentFailure : public IloAgentError {
public:
	IloAgentFailure() : IloAgentError("Could not create the IloAgent object") {}
};


class IloMutexProblem : public IloAgentError {
public:
	
	IloMutexProblem(const char* msg) : IloAgentError(msg)  {}
};



class IloMutexDeadlock : public IloMutexProblem {
public:
	IloMutexDeadlock() : IloMutexProblem("Mutex already locked") {}
};



class IloMutexNotOwner : public IloMutexProblem {
public:
	IloMutexNotOwner() : IloMutexProblem("Not owner of the mutex") {}
};


class IloFastMutex {
#if defined(ILO_MSVC)
	CRITICAL_SECTION _holderMutex;
	CRITICAL_SECTION _realMutex;
	DWORD _holder;
#elif defined(ILOPTHREAD)
	pthread_mutex_t _realMutex;
	pthread_mutex_t _holderMutex;
	pthread_t _holder;
#endif
	int        _holding;
	friend class IloCondition;
	IloBool isLockedUnsafe() const;
	inline void  lockHolder();
	inline void unlockHolder();
private:
	IloFastMutex( const IloFastMutex& other ){}
	IloFastMutex& operator=( const IloFastMutex& rhs ){
		throw IloWrongUsage("bad constructor");
		return (IloFastMutex&)rhs;
	}
public:
	
	IloFastMutex();
	
	~IloFastMutex();
	
	int isLocked();
	
	void lock();
	
	void unlock();
};



class IloSafeMutexLock
{
private:
	IloFastMutex* _mutex;
public:
	IloSafeMutexLock(IloFastMutex* mutex) : _mutex(mutex)
	{
		if (_mutex!=NULL)
		{
			// only lock if not already locked by same thread:
			if (!_mutex->isLocked())
			{
				_mutex->lock();
			}
			else
			{
				// so wont unlock
				_mutex=NULL;
			}
		}
	}

	~IloSafeMutexLock()
	{
		if (_mutex!=NULL) _mutex->unlock();
	}
};




class IloCondition {
#if defined(ILO_MSVC)
	IloThread*     _head;
	IloThread*     _tail;
	IloFastMutex   _lock;
	long           _waiting;
#elif defined(ILOPTHREAD)
	pthread_cond_t _realCond;
#endif
public:
	
	IloCondition();
	
	~IloCondition();
	
	void broadcast();
	
	void notify();
	
	void wait(IloFastMutex *m);
};


class IloSemaphore {
#if defined(ILO_MSVC)
	IloFastMutex    _lock;
	HANDLE       _realSem;
	long         _count;
#elif defined(ILOPTHREAD)
	IloFastMutex _lock;
	IloCondition _go;
	long         _count;
#endif
	friend class IloCondition;
	void post(int x);
public:
	
	IloSemaphore(int value = 0);
	
	~IloSemaphore();
	
	void post();
	
	int tryWait();
	
	void wait();
};


class IloBarrier {
	int       _number;
	int       _count;
	int       _ocount;
	int       _generation;
	IloFastMutex _mut;
	IloCondition _cv;
public:
	
	IloBarrier(int count);
	
	int wait();
};

#if defined(ILO_MSVC)
#define PRI_FIFO_MAX THREAD_PRIORITY_ABOVE_NORMAL
#define PRI_FIFO_MIN THREAD_PRIORITY_BELOW_NORMAL
#define PRI_RR_MAX THREAD_PRIORITY_ABOVE_NORMAL
#define PRI_RR_MIN THREAD_PRIORITY_BELOW_NORMAL
#define PRI_OTHER_MAX THREAD_PRIORITY_ABOVE_NORMAL
#define PRI_OTHER_MIN THREAD_PRIORITY_BELOW_NORMAL
#define PRI_NORMAL    THREAD_PRIORITY_NORMAL
#elif defined(ILOPTHREAD)
#define PRI_FIFO_MAX 0
#define PRI_FIFO_MIN 0
#define PRI_RR_MAX 0
#define PRI_RR_MIN 0
#define PRI_OTHER_MAX 0
#define PRI_OTHER_MIN 0
#define PRI_NORMAL    0
#endif

class IloThread {
	friend class IloCondition;
public:
	enum ThreadType {detached, joinable};
	enum ThreadPolicy {fifo,rr,other};
	static void initialize();
	static void end();
private:
	static int _alreadyInitted;
#if defined(ILO_MSVC)
	IloFastMutex  _mon;      // monitor for accessing/setting data items.
	IloSemaphore  _go;       // used for the makeRunnable
#elif defined(ILOPTHREAD)
	pthread_mutex_t _runLock;
	pthread_mutex_t _mon;
	pthread_cond_t  _go;
	pthread_attr_t _attr;
	int          _runnable;
#endif
	int           _active;   // flag is set to 1 in the constructor, set back to 0 when the thread object is going away.
	int           _ready;    // flag is set to 1 after the thread is made runnable. (never changes after that)
	ThreadType    _type;     // thread type: detached or joinable
#if defined(ILO_MSVC)
	HANDLE        _thdl;     // actual thread handle
	HANDLE        _event;    // used to signal a thread waiting on a condition
	IloThread*     _link;     // used to uplink all the threads currently waiting on conditions.
	IloCondition* _waitingOn;// which condition a thread is waiting on (meaningful iff the thread is actually waiting).
	unsigned long _tid;      // The (other) thread identifier
#elif defined(ILOPTHREAD)
	pthread_t     _tid;
	static pthread_key_t moi;
	static void cleanup(IloThread*);
#endif
	static void realStartup(IloThread *);
protected:
	void monLock();
	void monUnlock();
#if defined(ILO_MSVC)
	IloThread(long);
#elif defined(ILOPTHREAD)
	IloThread(pthread_t);
#endif
	virtual void startup() = 0;
	virtual ~IloThread();
public:
	IloThread(ThreadType T = detached, size_t stackSize = 0,ThreadPolicy policy = other,int pr = PRI_NORMAL);
	void setPriority(ThreadPolicy policy = other,int pr = PRI_NORMAL);
	void makeRunnable();
	void join();
	int  running() const { return _ready;}
#if defined(ILO_MSVC)
	HANDLE getThreadHandle() { return _thdl; }
#endif
	static IloThread* current();
	static void exit();
#ifndef ILO_ZOS
	static unsigned long getAgentId() { return (unsigned long)current()->_tid; }
#else
	static pthread_t getAgentId() { return current()->_tid; }
#endif
};


#ifdef _WIN32
#pragma pack(pop)
#endif

#endif

