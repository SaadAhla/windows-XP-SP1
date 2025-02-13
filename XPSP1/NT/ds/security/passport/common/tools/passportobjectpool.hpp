#ifndef PASSPORTOBJECTPOOL_HPP
#define PASSPORTOBJECTPOOL_HPP

/**
* A class representing a pool of objects. Objects
* can be checked out and returned to the
* the pool. If there are no objects available
* when a client tries to check one out. The client
* will block until one is returned. The pool can 
* be configured to always
* have a minimum number of objects, to have
* a maximum number of objects, and to remove objects
* that have been inactive. Client are required
* to supply an Object Constructot that is used by the 
* pool to construct new objects on demand. Client
* can also modify all the existing objects in the
* pool be supplying a Modifier object.
*
* @author Jesse Hull
*/
#include <list>
#include <deque>

#include "PassportDefaultConstructor.hpp"
#include "PassportWaitableLock.hpp"

template<class PoolT, class ObjectConstructor = PassportDefaultConstructor<PoolT> >
class PassportObjectPool
{
public:
	static int smDefaultMinObjects;
	static int smDefaultMaxObjects; 
	static long smDefaultMaxSecondsInActive; 
	
	/**
	* Constructor 
	* 
	* @param  minObjects the minimum number of objects in the pool. 
	*         Negative is assumed to be zero
	* @param  maxObjects  the maximum objects in the pool.
	*         Negative is assumed to unlimited number of objects
	*         If maxObjects is less then minObjects then the maximum
	*         number of objects will be equal to minObjects
	* @param  maxSecondsInActive  the maximum number of seconds that
	*         an object that is not checked out is kept around.
	*         Negative value assumes that objects once created will
	*         never be released. (note: the number of objects will
	*         never fall below minObjects even if they are not being used)
	*/
    PassportObjectPool(int minObjects = smDefaultMinObjects,
		int maxObjects = smDefaultMaxObjects,
		int maxSecondsInActive = smDefaultMaxSecondsInActive);
	
		/**
		* Constructor with object constructor
		* 
		* @param  objectConstructor is an instance of a class that has
		*         a newObject object member function that returns a pointer to
		*         an object of type PoolT
		* @param  minObjects the minimum number of objects in the pool. 
		*         Negative is assumed to be zero
		* @param  maxObjects  the maximum objects in the pool.
		*         Negative is assumed to unlimited number of objects
		*         If maxObjects is less then minObjects then the maximum
		*         number of objects will be equal to minObjects
		* @param  maxSecondsInActive  the maximum number of seconds that
		*         an object that is not checked out is kept around.
		*         Negative value assumes that objects once created will
		*         never be released. (note: the number of objects will
		*         never fall below minObjects even if they are not being used)
	*/
    PassportObjectPool(ObjectConstructor& objectConstructor,
		int minObjects = smDefaultMinObjects,
		int maxObjects = smDefaultMaxObjects,
		int maxSecondsInActive = smDefaultMaxSecondsInActive);
	
		/**
		* Gets the min objects in the pool
	*/
	int getMinObjects();
	
	/**
	* Sets the min objects in the pool
	* @param  minObjects the minimum number of objects in the pool. 
	*         Negative is assumed to be zero. The minimum is greater
	*         then the current maximum then the current maximum is 
	*         increased.
	*/
	void setMinObjects(int value);
	
	/**
	* Gets the max objects in the pool. Negative means
	* there is no maximum.
	*/
	int getMaxObjects();
	
	
	/**
	* Sets the max objects in the pool. Note because some objects
	* may be checked out this may not take effect immediately
	* @param  maxObjects  the maximum objects in the pool.
	*         Negative is assumed to unlimited number of objects
	*         If maxObjects is less then minObjects then the maximum
	*         number of objects will be equal to minObjects
	*/
	void setMaxObjects(int value);
	
	/**
	* Gets the maximum no activity time for the pool. Negtive
	* means there is no maximum.
	*/
	int getMaxSecondsInActive();
	
	/**
	* Sets the maximum no activity time for the pool.
	* @param  maxSecondsInActive  the maximum number of seconds that
	*         an object that is not checked out is kept around.
	*         Negative value assumes that objects once created will
	*         never be released. (note: the number of objects will
	*         never fall below minObjects even if they are not being used)
	*/
	void setMaxSecondsInActive(int value);
	
	/**
	*  The object number of objects this pool is managing
	*/
	int totalObjects();
	
	/**
	* The total number of objects available to be checked 
	* out
	*/
	int availableObjects();
	
	/**
	* checks the object out of the pool
	*/
	PoolT* checkout();
	
	/**
	* checks the object back into the pool. The 
	* object must be one that was retrieved using
	* checkout.and has not yet been checked in.
	* returns throws an assert if the object in being returned
	* was not generated by the pool or if the object
	* has already been checked in. 
	*/
	void checkin(PoolT* object);
	
	/**
	* This functions calls the modify
	* functions on the modifier for
	* every function in the pool.
	* NOTE: that the objects in the pool
	* are responsible for locking themselves
	* because this method could get call while
	* another thread is using a checked out 
	* pooled object.
	*/
	//template <class modifier>
	//void modifyPooledObjects()
	
	/*
	* NOTE: all objects better be returned before this is called
	*/
	~PassportObjectPool();
	
private:
	
	bool roomToGrow();
	PoolT* getObjectFromTop();
	
	bool contains(const std::list< PoolT* > & vec, PoolT* obj);
	bool contains(const std::deque<PoolT*>& vec, PoolT* obj);
	
	void returnObjectToTop(PoolT* obj);
	void growPoolToMinimum();
	void growPool();
	void shrinkPoolToMaximum();
	void removeInActiveObjects();
	bool oldestObjectExpired();
	virtual void removeOldestObject();
	
	void removeElement(std::list<PoolT*>& vec, PoolT* obj);
	
private:
    // --------------------
    // whether to print
    // debugging stuff to stdout
    static bool smDebug;
	
    // -------------------------
    // min number of objects to
    // have at all times
    int mMinObjects;
	
    // ---------------------------
    // maximum number of objects
    // negative indicates no max
    int mMaxObjects; // no maximum
	
    // --------------------------------------------
    // time to keep in active objects around before
    // removing them
    int mMaxSecondsInActive; // 5 minutes
	
    // ----------------------------------
    // the last time we checked for
    // in active objects
    long mLastInActivityCheck;
	
    // ---------------------------------------
    // the vector of all objects being managed
    // by the pool.
	std::list<PoolT*> mAllObjects;
	
    // --------------------------------------
    // the vector of pooled objects currently
    // not out
	std::deque<PoolT*> mPooledObjects;
	
    // ------------------------------------
    // a vector of the last time each object 
    // was used.
	std::deque<long> mInActiveTimes;
	
	// -----------------------------------
	// the object constructor for the pool
	ObjectConstructor& mObjectConstructor;
	
	// -------------------------------------
	// whether we own the object constructor
	bool mOwnObjectConstructor;
	
	// -------------------
	// lock to guard data
	PassportWaitableLock mLock;
};

#include "PassportObjectPool.cpp"

#endif // !PASSPORTOBJECTPOOL_HPP

