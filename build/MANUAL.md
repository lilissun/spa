MANUAL for TAuth v0.0.7 (Alpha)
This manual is currently in alpha version. 

Author: 
Li Li: lilissun@gmail.com

-----------------------------------------------------------------------------
Introduction
In TAuth, services provided by the system is represented by a set of logic rules. We use these rules to describe the capabilities and the knowledge of the adversary. In this manual, we illustrate how to construct such services, queries and the ceiling. In addition, we explain some disciplines the models should follow. 

The syntax and grammar checking for TAuth is still experimental. Only very limited errors can be identified during the parsing. Thus please check the model carefully to make sure it is correct. 

-----------------------------------------------------------------------------
Services
A service consists of some input facts and an output fact constrained by a set of timing constraints. A fact is either a timed knowledge <m, t> or an event e(m_1, m_2, ..., m_n). m is a message which can be either a constant, a variable, a nonce, a timestamp or a function. They have a hierarchy structure as follows. 

1. Service : 
@rule e1, e2, ..., en -> e : #ti - #tj <: 2, #ts - #tk <= 4;

2. Event e* : when (m1, m2) is its id
@decl @event event(*, *, ..., -) @group e;
event(m1, m2, ..., mn)

3. Message m* : 
@decl @func f/l
constant c[]
nonce [n]
variable v
timestamp #t
function f(m1, m2, ..., ml)

* Whenever a variable appears in the output, it should appear in an input. 
* Whenever a nonce appears in a service, it should be initiated in an event. 
* IDs used in the model should be consistent. Function names and constant names should the same in a model if and only if they represent the same concept. 
* Nonces appeared in the events should be consistent because it means that the nonces are initiated in those events. 
-----------------------------------------------------------------------------
Queries
Each model supports only one query at present. Each query have an accept event and an init event constrained by by a set of timing constraints. 

1. Query : 
@query accept(m1, m2, ..., mn) <- init(m1, m2, ..., mn) 
: #ti - #tj <: 2, #ts - #tk <= 4;

* Messages in the query should only be variables or timestamps. 
-----------------------------------------------------------------------------
Ceiling
Currently, ceiling is manually set by using the syntax "@ceiling 7;" where 7 is the ceiling. 

* When a ceiling is set, the presences of constraints like #ti - #tj ~ d where d < 0 and ~ is <: or <= can make false positives 
-----------------------------------------------------------------------------
Last Modified Date
2013-10-23
