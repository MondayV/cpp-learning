/*
 * This file contains code from "C++ Primer, Fifth Edition", by Stanley B.
 * Lippman, Josee Lajoie, and Barbara E. Moo, and is covered under the
 * copyright and warranty notices given in that book:
 * 
 * "Copyright (c) 2013 by Objectwrite, Inc., Josee Lajoie, and Barbara E. Moo."
 * 
 * 
 * "The authors and publisher have taken care in the preparation of this book,
 * but make no expressed or implied warranty of any kind and assume no
 * responsibility for errors or omissions. No liability is assumed for
 * incidental or consequential damages in connection with or arising out of the
 * use of the information or programs contained herein."
 * 
 * Permission is granted for this code to be used for educational purposes in
 * association with the book, given proper citation if and when posted or
 * reproduced. Any commercial use of this code requires the explicit written
 * permission of the publisher, Addison-Wesley Professional, a division of
 * Pearson Education, Inc. Send your request for permission, stating clearly
 * what code you would like to use, and in what specific way, to the following
 * address: 
 * 
 * 	Pearson Education, Inc.
 * 	Rights and Permissions Department
 * 	One Lake Street
 * 	Upper Saddle River, NJ  07458
 * 	Fax: (201) 236-3290
*/

#ifndef STRBLOB_H
#define STRBLOB_H
#include <vector>
#include <string>
#include <initializer_list>
#include <memory>
#include <stdexcept>

using namespace std;
// forward declaration needed for friend declaration in StrBlob
class StrBlobPtr;

class StrBlob {
	friend class StrBlobPtr;
public:
    typedef vector<string>::size_type size_type;
    StrBlob();
    StrBlob(initializer_list<string> il);
    StrBlob(vector<string> *p);

    size_type size() const { return data->size(); }
    bool empty() const { return data->empty(); }

    // add and remove elements
    void push_back(const string &t) { data->push_back(t); }
    void pop_back();

    // element access
    string& front();
    const string& front() const;
    string& back();
    const string& back() const;

	// interface to StrBlobPtr
	StrBlobPtr begin();  // can't be defined until StrBlobPtr is
    StrBlobPtr end();
    //const level
    StrBlobPtr cbegin() const;
    StrBlobPtr cend() const;
private:
    shared_ptr<std::vector<std::string>> data; 
    // throws msg if data[i] isn't valid
    void check(size_type i, const std::string &msg) const;
};

// constructor
inline StrBlob::StrBlob(): data(make_shared<vector<string>>()) {  }
inline StrBlob::StrBlob(initializer_list<string> il):
        data(make_shared<vector<string>>(il))  {  }
inline StrBlob::StrBlob(vector<string> *p): data(p)  {  }
// StrBlobPtr throws an exception on attempts to access a nonexistent element 
inline void StrBlob::check(size_type i, const string &msg) const
{
    if (i >= data->size())
        throw out_of_range(msg);
}

inline string& StrBlob::front()
{
    check(0, "front on empty StrBlob");
    return data->front();
}

inline const string& StrBlob::front() const
{
    check(0, "front on empty StrBlob");
    return data->front();
}
    
inline string& StrBlob::back()
{
    check(0, "back on empty StrBlob");
    return data->back();
}

inline const string& StrBlob::back() const
{
    check(0, "back on empty StrBlob");
    return data->back();
}

inline void StrBlob::pop_back()
{
    check(0, "pop_back on empty StrBlob");
    data->pop_back();
}

class StrBlobPtr {
    friend bool eq(const StrBlobPtr&, const StrBlobPtr&);
public:
    StrBlobPtr(): curr(0)   {  }
    StrBlobPtr(StrBlob &a, size_t sz = 0): wptr(a.data), curr(sz)  {  }
    StrBlobPtr(const StrBlob &a,size_t sz = 0): wptr(a.data), curr(sz)  {  }

    string& deref() const;
    string& deref(int off) const;
    StrBlobPtr& incr();
    StrBlobPtr& decr();
private:
    shared_ptr<vector<string>>
        check(size_t i, const string&) const;

    weak_ptr<vector<string>> wptr;
    size_t curr;
};

inline
shared_ptr<vector<string>>
StrBlobPtr::check(size_t i, const string &msg) const
{
    auto ret = wptr.lock();  // is the vector still around?
    if (!ret)
        throw runtime_error("unbound StrBlobPtr");
    if (i >= ret->size())
        throw out_of_range(msg);
    return ret;
}

inline string& StrBlobPtr::deref() const
{
    auto p = check(curr, "dereference past end");
    return (*p)[curr];
}

inline string& StrBlobPtr::deref(int off) const
{
    auto p = check(curr + off, "dereference past end");
    return (*p)[curr + off];
}

inline StrBlobPtr& StrBlobPtr::incr()
{
    check(curr, "increment past end of StrBlobPtr");
    ++curr;
    return *this;
}

inline StrBlobPtr& StrBlobPtr::decr()
{
    --curr;
    check(curr, "decrement past begin of StrBlobPtr");
    return *this;
}

inline StrBlobPtr StrBlob::begin()
{
    return StrBlobPtr(*this);
}

inline StrBlobPtr StrBlob::end()
{
    auto ret = StrBlobPtr(*this, data->size());
    return ret;
}

inline StrBlobPtr StrBlob::cbegin() const
{
    return StrBlobPtr(*this);
}

inline StrBlobPtr StrBlob::cend() const
{
    auto ret = StrBlobPtr(*this, data->size());
    return ret;
}

inline
bool eq(const StrBlobPtr &lhs, const StrBlobPtr &rhs)
{
   auto l = lhs.wptr.lock(), r = rhs.wptr.lock();
    if (l == r)
        return (!r || lhs.curr == rhs.curr);
    else
        return false; // one or both are unbound
}

inline
bool neq(const StrBlobPtr &lhs, const StrBlobPtr &rhs)
{
    return !eq(lhs, rhs);
}
#endif
