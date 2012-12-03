// Copyright (c) 2012 Bg Porter.

// !!! License

#ifndef h_ScumblerDefs
#define h_ScumblerDefs

// Later, we can set this to conditionally compile in only on debug 
// builds if we like.
#define qUnitTests


// A macro to make it easier to calling a member function using a 
// pointer to member function.

#define mCallMemberFn(object, ptrToMember)   ((object).*(ptrToMember))

#endif // this must be the last line in this file.